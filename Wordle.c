#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_NUM_OF_WORDS 100

/**
 * This function will go through the user's guess and check for any matching letters. 
 * The first loop will check for any exact matches (words that are in the same position)
 * The second loop will check for matches taht are not in the same position
 * 
 * @param wordleAnswer the correct answer to the game
 * @param userGuess the users current guess
 * @return true if the users guess matches the answer
 * @return false if the users guess DOES NOT match the answer
 */
bool processGuess(const char* wordleAnswer, const char* userGuess){
    //Create an array of characters and an array of booleans
    //These two will help us indicate whether a letter has been guessed and if it's in the correct/incorrect position
    char clue[6] = {'-', '-', '-', '-', '-', '\0'};
    bool flags[5] = {false, false, false, false, false};

    //first loop, go through every letter (only 5 letters)
    for (int i = 0; i < 5; i++){
        //look for exact matches
        if (userGuess[i] == wordleAnswer[i]){
            clue[i] = 'C';
            flags[i] = true;
        }
    }

    //second pass, look for matches that are not in the same position
    for (int i = 0; i < 5; i++){
        //if there is no exact match (the flag is not set during the first pass)
        if (clue[i] == '-'){
            for (int j = 0; j < 5; j++){
                if (userGuess[i] == wordleAnswer[j] && !flags[j]){
                    //match, but they are not in the same position
                    clue[i] = 'I';
                    flags[j] = true;
                    break; //end the j loop so we don't get multiple clues from the same letter
                }
            }
        }
    }

    //string compare the clue with "GGGGG", if they are identical then we return true, else we'll print the clue and return false
    if (strcmp(clue, "CCCCC") == 0){
        return true;
    }
    else{
        printf("%s\n", clue);
        return false;
    }
}

/**
 * This is a brief user guide that will print a message to users telling them the basics of the game
 * Furthermore, the message will explain how the clues work
 */
void userGuide(){
    printf("Welcome to the text-based version of the popular game Wordle.\n You have 6 attempts to guess a 5 letter word.\n Each time you guess you will be given a clue. \n 'C' means you have guessed the letter and position correctly and 'I' means you have guessed a letter, but it is in the incorrect position. \n");
}

int main() {

    //Create our list of words
    char** listOfWords = calloc(MAX_NUM_OF_WORDS, sizeof(char*));
    int wordCount = 0;
    char* fiveLetterWord = malloc(6*sizeof(char));

    //open and read our text file
    FILE* wordsFile = fopen("dictionary.txt", "r");
    
    //while loop to go through and load our words
    while (fscanf(wordsFile, "%s", fiveLetterWord) != EOF && wordCount < MAX_NUM_OF_WORDS) {
        listOfWords[wordCount] = fiveLetterWord;
        wordCount++;
        //allocate another set of memory
        fiveLetterWord = malloc(6*sizeof(char));
    }
    //close our text file
    fclose(wordsFile);

    //starting the game -> select a random word
    srand(time(NULL));
    char* answer = listOfWords[rand()%wordCount];

    //game loop
    int num_of_guesses = 0;
    bool correctly_guessed = false;
    //memory allocate -> again since it is a 5 letter word we need a special character to store the end of the c string, so we use 6 characters in total
    char* userInput = malloc(6*sizeof(char));

    //Introduction to our game
    printf("Welcome to the text-based version of Wordle! Here are some helpful commands: \n");
    printf("\"Help\" - brief user guide\n");
    printf("\"Quit\" - exit the game\n\n");

    //while number of guesses is less than 6 and the user has not correctly guessed
    while(num_of_guesses < 6 && !correctly_guessed) {
        //prompt user for guess
        printf("Enter a 5-letter word: ");

        //the first parameter tells what format it should take in, so in this case it should take in a string
        //the second parameter will tell the scanner where to put the input -> in other words, it'll put the user input into the variable guess
        scanf("%s", userInput);
        //If the user inputs "Help", call the userGuide function
        if (strcmp(userInput, "Help") == 0){
            userGuide();
        }
        //if the user inputted "Quit", print a message and exit the loop
        else if (strcmp(userInput, "Quit") == 0){
            printf("Thanks for playing!\n");
            num_of_guesses = 6;
        }
        //if the user inputted a word that is not a 5 letter word, print a message
        else if(strlen(userInput) > 5 || strlen(userInput) < 5){
            printf("You have guessed a word that is not of length 5\n");
        }
        //else, increment the number of guesses and call the processGuess function to check if the guess is correct
        else {
            printf("You have guessed %s\n", userInput);
            num_of_guesses++;
            //processing the guess
            correctly_guessed = processGuess(answer, userInput);
        }

        
    }

    //display end game message
    if (correctly_guessed){
        printf("Congratulations! You guessed the correct in %d attempts!\n", num_of_guesses);
    }
    else {
        printf("Game over, the correct word is %s\n", answer);
    }


    //free all memory blocks that were previously allocated
    for (int i = 0; i < wordCount; i++){
        free(listOfWords[i]);
    }
    free(listOfWords);
    free(fiveLetterWord);
    free(userInput);

    return 0;
}