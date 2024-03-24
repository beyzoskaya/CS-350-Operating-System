#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/*
Example play case:

Play What to eat today
Enter the number of people to play: 2
Player 1, enter your choices from the menu:
1-Kebab, 2-Burger, 3-Pasta, 4-Lahmacun, 5-Salad
Enter choice 1 (or 0 to not select no more item): 1
Enter choice 2 (or 0 to not select no more item): 5
Enter choice 3 (or 0 to not select no more item): 4
Enter choice 4 (or 0 to not select no more item): 0
Player 1, made 3 choices.

Player 2, enter your choices from the menu:
1-Kebab, 2-Burger, 3-Pasta, 4-Lahmacun, 5-Salad
Enter choice 1 (or 0 to not select no more item): 2
Enter choice 2 (or 0 to not select no more item): 1
Enter choice 3 (or 0 to not select no more item): 3
Enter choice 4 (or 0 to not select no more item): 4
Enter choice 5 (or 0 to not select no more item): 5
Player 2, made 5 choices.

Scores for each item:
Kebab: 9
Burger: 5
Pasta: 3
Lahmacun: 5
Salad: 5

Shortlist of preferences above threshold 5:
Kebab: 9

Second round: Choose from shortlist
Only one item above threshold. No need for another round: Kebab (Score: 9)
Scores for each item after the second round:
Kebab: 9
Burger: 5
Pasta: 3
Lahmacun: 5
Salad: 5
*/


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

// Print function for see items score after second round
void printSecondRoundShortlist(struct FoodChoice menu[]) {
    printf("Scores for each item after the second round:\n");
    for (int i = 0; i < MAX_CHOICES; i++) {
        printf("%s: %d\n", menu[i].name, menu[i].score);
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
            max_selected_index = i; // Use later as a no need for second round playing, directly print the item
        }
    }

    // If there are more than 1 item above the threshold after the first round, need to play the second round
    // Otherwise one item above the threshold is order
    if (num_above_threshold > 1) {
        printf("Another round is needed.\n");

        // Shortlist options above the threshold
        printf("Shortlist of preferences above threshold %d:\n", threshold);
        for (int i = 0; i < MAX_CHOICES; i++) {
            if (menu[i].score > threshold) {
                printf("%d. %s (Score: %d)\n", i + 1, menu[i].name, menu[i].score);
            }
        }

        // Array of player choices initialization
        int choices[MAX_CHOICES][MAX_CHOICES] = {0}; // Keep which player select which item as a 2D array

        // Each player plays like choice from shorlit with their prefered order
        for (int i = 0; i < num_people; i++) {
            printf("Player %d, order your choices from the shortlist (Enter 0 to finish):\n", i + 1);

            int choice;
            int order = 1;
            do {
                printf("Enter your choice %d: ", order);
                scanf("%d", &choice);

                if (choice >= 1 && choice <= MAX_CHOICES) {
                    choices[i][choice - 1] = order;
                    order++;
                }
            } while (choice != 0 && order <= num_above_threshold);

            printf("\n");
        }

        // Score update based on second round choices
        for (int i = 0; i < MAX_CHOICES; i++) {
            for (int j = 0; j < num_people; j++) {
                if (choices[j][i] > 0) {
                    // Score update with rule of order line like in first round
                    menu[i].score += MAX_CHOICES - choices[j][i] + 1;
                }
            }
        }

        int max_score = 0;
        for (int i = 0; i < MAX_CHOICES; i++) {
            if (menu[i].score > max_score) {
                max_score = menu[i].score;
                max_selected_index = i;
            }
        }

        int winners = 0;
        for (int i = 0; i < MAX_CHOICES; i++) {
            if (menu[i].score == max_score) {
                winners++;
            }
        }

        if(winners == 1) {
            printf("Selected item with max score after second round: %s (Score: %d)\n", menu[max_selected_index].name, menu[max_selected_index].score);
        
        // If number of winners more than 1 than random selection is needed or we can directly print this:
        // printf("You are eating at home/dorm today!\n");
        /*
        If you don't want random choice, you can change the part with 

            printf("You are eating at home/dorm today!\n");
            exit(0);
        */

        } else {
            printf("There is no maximum scored item. Randomly selecting from the above threshold items.\n");
            int above_threshold_indices[MAX_CHOICES];
            int num_items_above_threshold = 0;
            for (int i = 0; i < MAX_CHOICES; i++) {
                if (menu[i].score > threshold) {
                    above_threshold_indices[num_items_above_threshold] = i;
                    num_items_above_threshold++;
            }
        }
            int random_index = rand() % num_items_above_threshold;
            printf("Randomly selected item: %s\n", menu[above_threshold_indices[random_index]].name);
            
        }
        

    // No need to play second round, only one item above threshold
    } else if (num_above_threshold == 1) {
        printf("Only one item above threshold. No need for another round: %s (Score: %d)\n", menu[max_selected_index].name, menu[max_selected_index].score);
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

    if(num_people < 2){
        printf("Play with more than 1 person");
        exit(0);
    }

    // Give some thresholds based on number of peoples to play (I am logically select the thresholds)
    // You can change the threshold values based on your necessities
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

    // Print each item's score after game is finish
    printSecondRoundShortlist(menu);

    return 0;
}
