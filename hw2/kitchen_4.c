#include "meal.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define GLOVE_COUNT 3
#define POT_SIZE 3
#define APPRENTICE_COUNT 3
#define MEALS_TO_PREPARE 4
#define REQUIRED_INGREDIENTS 3

// Menu of meals
struct meal Menu[4] = {
    {"Menemen", {{"Tomato", 3}, {"Onion", 4}, {"Egg", 1}}, 10},
    {"Chicken Pasta", {{"Pasta", 2}, {"Chicken", 5}, {"Curry", 2}}, 8}, 
    {"Beef Steak", {{"Beef", 7}, {"Pepper", 3}, {"Garlic", 2}}, 13}, 
    {"Ali Nazik", {{"Eggplant", 4}, {"Lamb Meat", 4}, {"Yoghurt", 1}}, 10}
}; 

int meal_counter = 0;
int meal_ing_counter = 0;
int pot_count = 0;
int available_gloves = 3;

// Mutexes and condition variables
pthread_mutex_t gloves_mutex;
pthread_cond_t two_gloves_available;
pthread_mutex_t pot_mutex;
pthread_cond_t pot_cond;


void put_gloves(int apprentice_id) {
    pthread_mutex_lock(&gloves_mutex);
    while (available_gloves < 2) { // Ensure two gloves are available
        pthread_cond_wait(&two_gloves_available, &gloves_mutex);
    }
    available_gloves -= 2; // Apprentice takes two gloves
    printf("Apprentice %d has picked two gloves\n", apprentice_id);
    pthread_mutex_unlock(&gloves_mutex);
}

void remove_gloves(int apprentice_id) {
    pthread_mutex_lock(&gloves_mutex);
    available_gloves += 2; // Apprentice returns two gloves
    printf("Apprentice %d is putting back two gloves\n", apprentice_id);
    pthread_cond_signal(&two_gloves_available); // Signal that gloves are available
    pthread_mutex_unlock(&gloves_mutex);
}

void pick_ingredient(int apprentice_id, int* meal_index, int* ing_index) {
    put_gloves(apprentice_id);
    pthread_mutex_lock(&pot_mutex);
    if (meal_ing_counter >= REQUIRED_INGREDIENTS * MEALS_TO_PREPARE) {
        meal_counter = 0;
        meal_ing_counter = 0;
    }
    *meal_index = meal_counter % MEALS_TO_PREPARE;
    *ing_index = meal_ing_counter % REQUIRED_INGREDIENTS;
    meal_ing_counter++;
    printf("Apprentice %d has taken ingredient %s\n", apprentice_id, Menu[*meal_index].ingredients[*ing_index].name);
    pthread_mutex_unlock(&pot_mutex);
    remove_gloves(apprentice_id);
}

void put_ingredient(int id, int meal_index, int ing_index) {
    pthread_mutex_lock(&pot_mutex);
    while (pot_count >= POT_SIZE) {
        pthread_cond_wait(&pot_cond, &pot_mutex);
    }
    printf("Apprentice %d is trying to put %s into the pot\n", id, Menu[meal_index].ingredients[ing_index].name);
    pot_count++;
    printf("Apprentice %d has put %s into the pot\n", id, Menu[meal_index].ingredients[ing_index].name);
    if (pot_count == REQUIRED_INGREDIENTS) {
        pthread_cond_signal(&pot_cond); // Signal chef that the pot is ready
    }
    pthread_mutex_unlock(&pot_mutex);
}

void *apprentice(void *apprentice_id) {
    int id = *(int*)apprentice_id;
    printf("I'm apprentice %d\n", id);
    int meal_index, ing_index;
    while (1) {
        pick_ingredient(id, &meal_index, &ing_index);
        sleep(Menu[meal_index].ingredients[ing_index].time_to_process); // Simulate time to prepare ingredient
        put_ingredient(id, meal_index, ing_index);
    }
    pthread_exit(NULL);
}

void *chef() {
    while (1) {
        pthread_mutex_lock(&pot_mutex);
        while (pot_count < REQUIRED_INGREDIENTS) {
            pthread_cond_wait(&pot_cond, &pot_mutex); // Wait for the pot to be ready
        }
        if (meal_counter >= MEALS_TO_PREPARE) { // If all meals are prepared, chef can go home
            pthread_mutex_unlock(&pot_mutex);
            break;
        }
        printf("Chef is preparing meal %s\n", Menu[meal_counter % MEALS_TO_PREPARE].name);
        sleep(Menu[meal_counter % MEALS_TO_PREPARE].time_to_prepare);
        printf("Chef prepared the meal %s\n", Menu[meal_counter % MEALS_TO_PREPARE].name);
        pot_count = 0;
        meal_counter++;
        pthread_cond_broadcast(&pot_cond); // Notify all waiting threads that pot is reset
        pthread_mutex_unlock(&pot_mutex);
        sleep(3); // Chef rests
    }
    pthread_exit(NULL);
}


int main() {
    pthread_t apprentice_threads[APPRENTICE_COUNT];
    pthread_t chef_thread;
    int apprentice_ids[APPRENTICE_COUNT] = {0, 1, 2};

    pthread_mutex_init(&gloves_mutex, NULL);
    pthread_cond_init(&two_gloves_available, NULL);
    pthread_mutex_init(&pot_mutex, NULL);
    pthread_cond_init(&pot_cond, NULL);

    for (int i = 0; i < APPRENTICE_COUNT; i++) {
        pthread_create(&apprentice_threads[i], NULL, apprentice, &apprentice_ids[i]);
    }
    pthread_create(&chef_thread, NULL, chef, NULL);

    for (int i = 0; i < APPRENTICE_COUNT; i++) {
        pthread_join(apprentice_threads[i], NULL);
    }
    pthread_join(chef_thread, NULL);

    pthread_mutex_destroy(&gloves_mutex);
    pthread_cond_destroy(&two_gloves_available);
    pthread_mutex_destroy(&pot_mutex);
    pthread_cond_destroy(&pot_cond);

    return 0;
}
