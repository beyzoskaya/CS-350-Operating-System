#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// we have 5 items inside the menu
#define MAX_CHOICES 5

// Food has name and score based on the player
struct FoodChoice {
    char name[20];
    int score;
};

// All items inside the menu have 0 score at initialization point
// menu is also in a type of FoodChoice to access name and score for printing later
void initializeMenu(struct FoodChoice menu[]) {
    char *foodNames[MAX_CHOICES] = {"Kebab", "Burger", "Pasta", "Lahmacun", "Salad"};

    for (int i = 0; i < MAX_CHOICES; i++) {
        strcpy(menu[i].name, foodNames[i]);
        menu[i].score = 0;
    }
}

// PLaying the game as a first round
void makeChoices(struct FoodChoice menu[], int num_people) {
    for (int i = 0; i < num_people; i++) {
        printf("Player %d, enter your choices from the menu:\n", i + 1);
        printf("1-Kebab, 2-Burger, 3-Pasta, 4-Lahmacun, 5-Salad\n");

        // initialize array of choices with all setted with 0
        int choices[MAX_CHOICES];
        int num_choices = 0;

        // At most Max choices of iteration allowed, this is the number of items in the menu
        for (int j = 0; j < MAX_CHOICES; j++) {
            printf("Enter choice %d (or 0 to not select no more item): ", j + 1);
            scanf("%d", &choices[j]);
            if (choices[j] >= 1 && choices[j] <= 5) {
                num_choices++;
            // no more choice anymore (skip for another person to play)
            } else if (choices[j] == 0) {
                break;
            // invalid choice, we have only 5 items in the menu
            } else {
                printf("Select between 1 and 5.\n");
                j--;
            }
        }

        // Updating the scores for selected items based on player's choice
        for (int j = 0; j < MAX_CHOICES; j++) {
            if (choices[j] >= 1 && choices[j] <= 5) {
                // update the score based on choice array selection order
                int score_increment = MAX_CHOICES - j;
                menu[choices[j] - 1].score += score_increment;
            }
        }

        printf("Player %d, made %d choices.\n", i + 1, num_choices);
    }
}

void printShortlist(struct FoodChoice menu[], int threshold) {
    printf("Scores for each item:\n");
    for (int i = 0; i < MAX_CHOICES; i++) {
        printf("%s: %d\n", menu[i].name, menu[i].score);
    }

    
    printf("Shortlist of preferences above threshold %d:\n", threshold);
    bool no_choices_above_threshold = true; 
    
    // Printing the items above the threshold from first round as a short list
    for (int i = 0; i < MAX_CHOICES; i++) {
        if (menu[i].score > threshold) {
            printf("%s: %d\n", menu[i].name, menu[i].score);
            no_choices_above_threshold = false; 
        }
    }
    // If no item go above for the threshold in the first round, eat at homeee
    if (no_choices_above_threshold) { 
        printf("You are eating at home/dorm today!\n");
        exit(0);
    }
}

// Based on the first round result, there can be possibility to play second round
void playSecondRound(struct FoodChoice menu[], int threshold, int num_people) {
    printf("Second round: Choose from shortlist\n");

    int num_above_threshold = 0;
    int max_selected_index = 0; 

    // Look for the number of items above the threshold for selection later
    for (int i = 0; i < MAX_CHOICES; i++) {
        if (menu[i].score > threshold) {
            num_above_threshold++;
            max_selected_index = i; // To print out the selected item if there is no need for second round to play
        }
    }
    
    // If there are more than 1 item above the threshold after the first round, need to play the second round
    if (num_above_threshold > 1) {
        printf("Another round is needed.\n");

        int selected[MAX_CHOICES] = {0};
        // Ask everyone to play one by one for selection
        for (int i = 0; i < num_people; i++) {
            printf("Player %d, choose from the following items:\n", i + 1);
            // Select from the items in the menu (at most 5 items)
            for (int j = 0; j < MAX_CHOICES; j++) {
                // Print the item if it is above the threshold as a list
                if (menu[j].score > threshold) {
                    printf("%d. %s\n", j + 1, menu[j].name);
                }
            }

            int choice;
            do {
                printf("Enter your choice: ");
                scanf("%d", &choice);
            // while the players make a choice, if the choice is valid and more than threshold 
            } while (choice < 1 || choice > num_above_threshold);
            // increment the array of selected
            selected[choice - 1]++;
        }

        // Update the maximum scored selected item to find preferable item for second round 
        int max_selected = 0;
        for (int i = 0; i < MAX_CHOICES; i++) {
            // If the item has a score more than threshold and the item has score more than lastly max one, update the max selected item
            if (menu[i].score > threshold && selected[i] > max_selected) {
                max_selected = selected[i];
                max_selected_index = i;
            }
        }

        // Control for how many selected as a max scored item
        int selected_count = 0;
        for (int i = 0; i < MAX_CHOICES; i++) {
            if (menu[i].score > threshold && selected[i] == max_selected) {
                selected_count++;
            }
        }

        // Look for how many selected item as a max to end the game
        if (selected_count == 1) {
            printf("Item with the max selection in the second round: %s\n", menu[max_selected_index].name);
        // If there is no selection with max score after second round, find with random selection
        } else {
            printf("There is no maximum scored item so random selection is needed. Randomly selecting from the above threshold items.\n");
            int random_index = rand() % num_above_threshold;
            printf("Randomly selected item: %s\n", menu[random_index].name);
        }
    // After the first round, if there is one item above the threshold no need for second round
    } else {
        printf("Only one item above threshold. No need for another round: %s\n", menu[max_selected_index].name);
    }
}


int main() {
    struct FoodChoice menu[MAX_CHOICES];
    
    initializeMenu(menu);

    int num_people;
    int threshold;

    printf("Play What to eat today\n");
    printf("Enter the number of people to play: ");
    scanf("%d", &num_people);

    // Give some thresholds based on number of peoples to play (I am logically select the thresholds)
    if (num_people == 2) {
        threshold = 5;
    } else if (num_people == 3) {
        threshold = 10;
    } else if (num_people == 4) {
        threshold = 12;
    } else {
        threshold = 15;
    }

    makeChoices(menu, num_people);

    printShortlist(menu, threshold);

    playSecondRound(menu, threshold, num_people);

    return 0;
}
