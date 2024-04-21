/** <Beyza Kaya, S021747>
 * This is the kitchen simulation code for OzuRest.
 * French chef and 3 students from gastronomy department are preparing delicious meals in here
 * You need to solve their problems.
**/
#include "meal.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define GLOVE_COUNT 3
#define POT_SIZE 3
#define APPRENTICE_COUNT 3
#define MEALS_TO_PREPARE 4
#define REQUIRED_INGREDIENTS 3

struct meal Menu[4] = {
    {"Menemen", {{"Tomato", 3}, {"Onion", 4}, {"Egg", 1}}, 10},
    {"Chicken Pasta", {{"Pasta", 2}, {"Chicken", 5}, {"Curry", 2}}, 8}, 
    {"Beef Steak", {{"Beef", 7}, {"Pepper", 3}, {"Garlic", 2}}, 13}, 
    {"Ali Nazik", {{"Eggplant", 4}, {"Lamb Meat", 4}, {"Yoghurt", 1}}, 10}
}; 

int meal_counter = 0;
int meal_ing_counter = 0;
// Define all required mutexes here
pthread_mutex_t glove_mutex[GLOVE_COUNT];
pthread_mutex_t pot_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pot_cond = PTHREAD_COND_INITIALIZER;

void put_gloves(int apprentice_id) {
    printf("Apprentice %d is picking gloves\n", apprentice_id);
    int i = 0;
    while (1) {
        if (pthread_mutex_lock(&glove_mutex[i]) == 0) {
            printf("Apprentice %d has picked gloves\n", apprentice_id);
            break;
        }
        i = (i + 1) % GLOVE_COUNT;
    }
}

void remove_gloves(int apprentice_id) {
    int i = 0;
    while (1) {
        if (pthread_mutex_unlock(&glove_mutex[i]) == 0) {
            printf("Apprentice %d has removed gloves\n", apprentice_id);
            break;
        }
        i = (i + 1) % GLOVE_COUNT;
    }
}

void pick_ingredient(int apprentice_id, int *meal_index, int *ing_index) {
    put_gloves(apprentice_id);

    printf("Apprentice %d is inside pick_ingredient\n", apprentice_id);

    if (pthread_mutex_lock(&glove_mutex[apprentice_id]) != 0) {
        perror("Failed to lock mutex");
        exit(EXIT_FAILURE);
    }

    printf("Apprentice %d has locked glove_mutex[%d]\n", apprentice_id, apprentice_id);

    *meal_index = meal_counter;
    *ing_index = meal_ing_counter;
    
    printf("Apprentice %d has taken ingredient %s\n", apprentice_id, Menu[*meal_index].ingredients[*ing_index].name);

    meal_counter++;
    meal_ing_counter++;

    if (pthread_mutex_unlock(&glove_mutex[apprentice_id]) != 0) {
        perror("Failed to unlock mutex");
        exit(EXIT_FAILURE);
    }

    printf("Apprentice %d has unlocked glove_mutex[%d]\n", apprentice_id, apprentice_id);

    remove_gloves(apprentice_id);
}




void prepare_ingredient(int apprentice_id, int meal_index, int ing_index) {
    printf("Apprentice %d is preparing: %s \n", apprentice_id, Menu[meal_index].ingredients[ing_index].name);
    sleep(Menu[meal_index].ingredients[ing_index].time_to_process);
    printf("Apprentice %d is done preparing %s \n", apprentice_id, Menu[meal_index].ingredients[ing_index].name);
}

void put_ingredient(int id, int meal_index, int ing_index) {
    while (1) {
        // Lock the pot mutex to access the pot
        if (pthread_mutex_lock(&pot_mutex) != 0) {
            perror("Failed to lock mutex");
            exit(EXIT_FAILURE);
        }

        // Wait until there are ingredients available to put into the pot
        while (meal_ing_counter <= ing_index) {
            printf("Apprentice %d is waiting for more ingredients.\n", id);
            pthread_cond_wait(&pot_cond, &pot_mutex);
        }

        // Put the ingredient into the pot
        printf("Apprentice %d is trying to put %s into pot\n", id, Menu[meal_index].ingredients[ing_index].name);
        printf("Apprentice %d has put %s into pot\n", id, Menu[meal_index].ingredients[ing_index].name);
        meal_index++;

        // If the pot is now full, signal the chef to cook
        if (meal_index == POT_SIZE) {
            printf("Pot is full. Signaling chef to cook.\n");
            pthread_cond_signal(&pot_cond);
        }

        // Unlock the pot mutex
        if (pthread_mutex_unlock(&pot_mutex) != 0) {
            perror("Failed to unlock mutex");
            exit(EXIT_FAILURE);
        }

        break;
    }
}

void help_chef(int apprentice_id) {
    int meal_index, meal_ingredient_index;
    pick_ingredient(apprentice_id, &meal_index, &meal_ingredient_index);
    prepare_ingredient(apprentice_id, meal_index, meal_ingredient_index);
    put_ingredient(apprentice_id, meal_index, meal_ingredient_index);
}

void *apprentice(int *apprentice_id) {
    printf("Im apprentice %d\n", *apprentice_id);
    while(1) {
        help_chef(*apprentice_id);
    }
    pthread_exit(NULL);
}

void *chef(void *arg) {
    while (1) {
        if (pthread_mutex_lock(&pot_mutex) != 0) {
            perror("Failed to lock mutex");
            exit(EXIT_FAILURE);
        }

        // Wait for the pot to be full
        while (meal_counter < POT_SIZE) {
            printf("Chef is waiting for the pot to be full.\n");
            pthread_cond_wait(&pot_cond, &pot_mutex);
        }

        // Chef can now cook
        printf("Chef is preparing meal %s\n", Menu[meal_counter - POT_SIZE].name);
        sleep(Menu[meal_counter - POT_SIZE].time_to_prepare);
        printf("Chef prepared the meal %s\n", Menu[meal_counter - POT_SIZE].name);

        meal_ing_counter = 0;
        meal_counter -= POT_SIZE; // Update meal_counter to reflect the cooked meals

        if (pthread_mutex_unlock(&pot_mutex) != 0) {
            perror("Failed to unlock mutex");
            exit(EXIT_FAILURE);
        }

        sleep(3); // Let the chef rest after cooking

        if (meal_counter == MEALS_TO_PREPARE) // If all meals are prepared, chef can go home
            break;
    }
    pthread_exit(NULL);
}



int main() {
    pthread_t apprentice_threads[APPRENTICE_COUNT];
    pthread_t chef_thread;

    int apprentice_ids[APPRENTICE_COUNT] = {0 ,1 ,2};
    
    // Initialize Glove mutexes here
    int glove_mutex_initialized[GLOVE_COUNT] = {0};

    for (int i = 0; i < GLOVE_COUNT; i++) {
        if (!glove_mutex_initialized[i]) {
            if (pthread_mutex_init(&glove_mutex[i], NULL) != 0) {
                printf("Mutex initialization failed.\n");
                exit(1);
            }
            glove_mutex_initialized[i] = 1;
        }
    }
    printf("All mutexes initialized successfully.\n");

    // Initialize threads here

    /*
    The threads for apprentice and the chefs with pthread_create
    pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg)
    */
    for (int i = 0; i < APPRENTICE_COUNT; i++) {
        if (pthread_create(&apprentice_threads[i], NULL, (void *)apprentice, &apprentice_ids[i]) != 0) {
            printf("Failed to create apprentice thread %d.\n", i);
            exit(1);
        }
    }
    printf("All apprentice threads created successfully.\n");

    if (pthread_create(&chef_thread, NULL, (void *)chef, NULL) != 0) {
        printf("Failed to create chef thread.\n");
        exit(1);
    }
    printf("All chef threads created successfully.\n");

    // Tell the main thread to wait for other threads to complete
    for (size_t i = 0; i < APPRENTICE_COUNT; i++)
        pthread_join(apprentice_threads[i],NULL);

    pthread_join(chef_thread, NULL);

    return 0;
}
