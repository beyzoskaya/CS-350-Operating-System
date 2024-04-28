/** Beyza Kaya, S021747
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
int pot_counter = 0;
int temporary_counter = 0;
// Define all required mutexes here
pthread_mutex_t glove_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ingredientMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t potMutex = PTHREAD_MUTEX_INITIALIZER;

// Locking gloves with one mutexes because only one apprentice use at a time
// This function uses one mutex for several resources like in Dİning Phlisophers problem
void put_gloves(int apprentice_id) {
    printf("Apprentice %d is picking gloves \n", apprentice_id);
    // Implement a mutex lock mechanism for gloves here
    pthread_mutex_lock(&glove_mutex); 


    
    /*
    This mechanism is for try to schedule resource of gloves
    int first_glove = apprentice_id;
    int second_glove = (apprentice_id + 1) % APPRENTICE_COUNT;

    if (first_glove > second_glove) {
        int temp = first_glove;
        first_glove = second_glove;
        second_glove = temp;
    }

    pthread_mutex_lock(&gloveMutex[first_glove]);
    pthread_mutex_lock(&gloveMutex[second_glove]); 
    */
    

    printf("Apprentice %d has picked gloves\n", apprentice_id); // DOne with wearing gloves for one apprentice (locked by one apprentice)
}

// Remove gloves with unlock the same mutex
// This function release the resource with same mutex 
void remove_gloves(int apprentice_id) {
    // Implement a mutex unlock mechanism for gloves here

     pthread_mutex_unlock(&glove_mutex);


    /*
    This mechanism release the resources in a same schedule idea
    int first_glove = apprentice_id;
    int second_glove = (apprentice_id + 1) % APPRENTICE_COUNT;

    if (first_glove > second_glove) {
        int temp = first_glove;
        first_glove = second_glove;
        second_glove = temp;
    }

    // For prevent deadlock unlock the gloves in same order as locked them
    pthread_mutex_unlock(&gloveMutex[first_glove]); 
    pthread_mutex_unlock(&gloveMutex[second_glove]);
    */
    
    printf("Apprentice %d has removed gloves\n", apprentice_id);
}

// Function to keep track of number of ingredients and the meals that needs to be cook
// This function keeps track of both ingredients and meals to cook because each meal has ingredient to collect for cooking
void update_counters() {
    meal_ing_counter++;
    if (meal_ing_counter == REQUIRED_INGREDIENTS) {
        meal_ing_counter = 0;
        meal_counter++;
    }
}

// This function is for selection of ingredients from apprentices to prepare
void pick_ingredient(int apprentice_id, int* meal_index, int* ing_index) {
    put_gloves(apprentice_id);
    // Implement a control mechanism here using mutexes if needed
    pthread_mutex_lock(&ingredientMutex);

    *meal_index = meal_counter;
    *ing_index = meal_ing_counter;
    printf("Apprentice %d has taken ingredient %s\n", apprentice_id, Menu[*meal_index].ingredients[*ing_index].name);
    update_counters();
    // Possible Race condition here, take your precautions
    pthread_mutex_unlock(&ingredientMutex);
    // After picked the ingredient remove gloves for another apprentice
    remove_gloves(apprentice_id);
}

// This function is for preparing the selected ingredients with necessary time
void prepare_ingredient(int apprentice_id, int meal_index, int ing_index) {
    printf("Apprentice %d is preparing: %s \n", apprentice_id, Menu[meal_index].ingredients[ing_index].name);
    sleep(Menu[meal_index].ingredients[ing_index].time_to_process);
    printf("Apprentice %d is done preparing %s \n", apprentice_id, Menu[meal_index].ingredients[ing_index].name);
}

// This function controls the pot for not exploid the pot size but control the ingredients to cook correct meal
void put_ingredient(int id, int meal_index, int ing_index) {
    while(1) {
        if(pot_counter < POT_SIZE) { 
            if(meal_index == temporary_counter){
                printf("Apprentince %d is trying to put %s into pot\n", id, Menu[meal_index].ingredients[ing_index].name);
                pthread_mutex_lock(&potMutex); // lock the pot mutex while putting an ingredient
                printf("Apprentince %d has put %s into pot\n", id, Menu[meal_index].ingredients[ing_index].name);
                pot_counter++; // to keep track of constraint about 3 ingredient maximum at a time for pot
                pthread_mutex_unlock(&potMutex); // After putting release the resource
                break;
            }
        }
    }
}


// This function controls the apprentices to help chef for cooking
void help_chef(int apprentice_id) {
    int meal_index, meal_ingredient_index;
    pick_ingredient(apprentice_id, &meal_index, &meal_ingredient_index);
    prepare_ingredient(apprentice_id, meal_index, meal_ingredient_index);
    put_ingredient(apprentice_id, meal_index, meal_ingredient_index);
}

// This function controls the apprentices for continue until all the meals cooked so until there all the apprentices should call for function help_chef
void *apprentice(int *apprentice_id) {
    while(meal_counter < MEALS_TO_PREPARE){
        printf("Im apprentice %d\n", *apprentice_id);
        help_chef(*apprentice_id); // Apprentices help chef about pick-prepare and put ingredients
    }
    pthread_exit(NULL);
    exit(0);
}

// This function controls the cooking with chef
// First lock the pot as a resource because chef uses the ingredients inside the pot when they done for one meal
// Need to reset pot counter after cooked specific meal
void *chef() {
    while(1) {
        /** Chef works but you need to implement a control mechanism for him
         *  to prevent the second problem mentioned in HW file.
         *  As for now, he just cooks without checking the pot.
         */

        pthread_mutex_lock(&potMutex); // lock the pot while cooking to empty pot after cooking meal
        if(pot_counter == POT_SIZE){ // If pot counter is equal to pot size then chef can cook
            printf("Chef is preparing meal %s\n", Menu[temporary_counter].name);
            sleep(Menu[temporary_counter].time_to_prepare);
            printf("Chef prepared the meal %s\n",  Menu[temporary_counter].name);
            temporary_counter = temporary_counter + 1;
            //meal_counter = meal_counter + 1;
            sleep(3); // Let the chef rest after preparing the meal
            pot_counter = 0; // reset the pot counter after chef cook some meal
        }
        pthread_mutex_unlock(&potMutex);

        // Continue to control until all the meals prepare and the temporary_counter is a variable to keep track of meals 
        if(temporary_counter == MEALS_TO_PREPARE)
            //printf("All the meals cooked,go home");
            break;
        //if(meal_counter == MEALS_TO_PREPARE)
        //    break;
    }
    pthread_exit(NULL);

}

int main() {
    pthread_t apprentice_threads[APPRENTICE_COUNT];
    pthread_t chef_thread;

    int apprentice_ids[APPRENTICE_COUNT] = {0 ,1 ,2};

    // Initialize Glove mutexes here
    
    //for(int i = 0; i<GLOVE_COUNT; i++){
    //    pthread_mutex_init(& gloveMutex[i], NULL);
    //}

    // Only one mutex initialize for gloves
    pthread_mutex_init(&glove_mutex, NULL);
    // Initialize threads here
    pthread_create(& chef_thread, NULL, chef, NULL);

    for(int i = 0; i < APPRENTICE_COUNT; i++){
        pthread_create(& apprentice_threads[i], NULL, apprentice, &apprentice_ids[i]);
    }

    // Tell the main thread to wait for other threads to complete
    for (size_t i = 0; i < APPRENTICE_COUNT; i++)
        pthread_join(apprentice_threads[i],NULL);

    pthread_join(chef_thread, NULL);

    printf("All meals cooked by chef and apprentices!\n");

    return 0;
}