#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHOICES 5

struct FoodChoice {
    char name[20];
    int score;
};

void makeChoices(struct FoodChoice menu[], int num_people) {
    for (int i = 0; i < num_people; i++) {
        printf("Player %d, please enter your choices from the menu:\n", i + 1);
        printf("1-Kebab, 2-Burger, 3-Pasta, 4-Lahmacun, 5-Salad\n");

        int choices[MAX_CHOICES] = {0};
        int num_choices = 0;

        for (int j = 0; j < MAX_CHOICES; j++) {
            printf("Enter choice %d (or 0 to skip): ", j + 1);
            scanf("%d", &choices[j]);
            if (choices[j] >= 1 && choices[j] <= 5) {
                num_choices++;
            } else if (choices[j] == 0) {
                break;
            } else {
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
                j--; // Decrement j to re-enter the choice
            }
        }

        for (int j = 0; j < MAX_CHOICES; j++) {
            if (choices[j] >= 1 && choices[j] <= 5) {
                // Assign score based on order of preference
                menu[choices[j] - 1].score += (MAX_CHOICES - j);
            }
        }

        printf("Player %d, you have made %d choices.\n", i + 1, num_choices);
    }
}

// Function to print the shortlist of preferences above a threshold
void printShortlist(struct FoodChoice menu[], int threshold) {
    printf("Scores for each item:\n");
    for (int i = 0; i < MAX_CHOICES; i++) {
        printf("%s: %d\n", menu[i].name, menu[i].score);
    }

    printf("Shortlist of preferences above threshold %d:\n", threshold);
    int no_choices_above_threshold = 1;

    for (int i = 0; i < MAX_CHOICES; i++) {
        if (menu[i].score > threshold) {
            printf("%s: %d\n", menu[i].name, menu[i].score);
            no_choices_above_threshold = 0;
        }
    }

    if (no_choices_above_threshold) {
        printf("You are eating at home/dorm today!\n");
        exit(0);
    }
}

// Function to play the second round
void playSecondRound(struct FoodChoice menu[], int threshold, int num_people) {
    printf("Second round: Choose from shortlist\n");

    struct FoodChoice above_threshold[MAX_CHOICES];
    int num_above_threshold = 0;
    for (int i = 0; i < MAX_CHOICES; i++) {
        if (menu[i].score > threshold) {
            strcpy(above_threshold[num_above_threshold].name, menu[i].name);
            above_threshold[num_above_threshold].score = menu[i].score;
            num_above_threshold++;
        }
    }

    if (num_above_threshold > 1) {
        printf("Another round is needed.\n");

        // Each player selects from items above threshold
        int selected[MAX_CHOICES] = {0}; // Counter for each item
        for (int i = 0; i < num_people; i++) {
            printf("Player %d, choose from the following items:\n", i + 1);
            for (int j = 0; j < num_above_threshold; j++) {
                printf("%d. %s\n", j + 1, above_threshold[j].name);
            }
            int choice;
            do {
                printf("Enter your choice: ");
                scanf("%d", &choice);
            } while (choice < 1 || choice > num_above_threshold);
            selected[choice - 1]++;
        }

        // Find the most selected item
        int max_selected = 0;
        int max_selected_index = 0;
        for (int i = 0; i < num_above_threshold; i++) {
            if (selected[i] > max_selected) {
                max_selected = selected[i];
                max_selected_index = i;
            }
        }
        printf("Item with the max selection in the second round: %s\n", above_threshold[max_selected_index].name);
    } else {
        printf("Only one item above threshold. No need for another round.\n");
    }
}

int main() {
    struct FoodChoice menu[MAX_CHOICES] = {
        {"Kebab", 0},
        {"Burger", 0},
        {"Pasta", 0},
        {"Lahmacun", 0},
        {"Salad", 0}
    };

    int num_people;
    int threshold;

    printf("Welcome to What to eat today?\n");
    printf("Enter the number of people: ");
    scanf("%d", &num_people);

    // Set threshold based on the number of people
    if (num_people == 2) {
        threshold = 5;
    } else if (num_people == 3) {
        threshold = 10;
    } else if (num_people == 4) {
        threshold = 12;
    } else {
        threshold = 15; // Change threshold for other numbers of people
    }

    // First round: Make choices
    makeChoices(menu, num_people);

    // Print shortlist of preferences
    printShortlist(menu, threshold);

    // Second round: Choose from shortlist
    playSecondRound(menu, threshold, num_people);

    return 0;
}
