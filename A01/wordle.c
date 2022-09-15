#include <stdio.h>
#include <string.h>
#include "words.h"
#include <time.h>
#include <stdlib.h>

// This function will print out the rules and instructions of the game
void instructions() {
  printf("Welcome to WORDLE!\nGuess the 5-letter word in 6 guesses!\nLetters guessed in the correct position will be shown.\nLetters in the word, but not in the right position will be shown as *.\nLetters not in the word will appear as #.\nYou lose after 6 wrong guesses.\n\n");
}

// Function for user to guess the word, and has to be a 5 letter word.
// Note that if the user puts a string that is more than 64 (63 plus temrinating),
// then the letters will overflow to the next array that is allocated right after
// guess
void GuessWord(char guess[]) {
  printf("Your Guess: ");  
  scanf("%s", guess);

  // has to guess a valid 5 letter word
  while (strlen(guess) != 5) {
    printf("Please enter a valid 5 letter word: ");
    scanf("%s", guess);
  }

}

// this function finds letters in the word, but not in the correct positions 
void find_chars(char word[], char guess[], char positions[], int letters_used[]) {
  for (int i= 0; i < 5; i++) {
    if (positions[i] == '#') {
      for (int j= 0; j < 5; j++) {
        if (!letters_used[j] && guess[i] == word[j]) {
          letters_used[j]= 1;
          positions[i]='*';
          break;
        }
      }
    }

  }
}


// Iterates over the 5 indices and notes which letters are in the correct position
// letters_used corresponds to characters in the actual WORD
void correct_positions(char positions[], char word[], char guess[], int letters_used[]) {
  for (int i= 0; i < strlen(guess); i++) {
    if (word[i] == guess[i]) {
      positions[i]= word[i];
      letters_used[i]= 1;
    }
  }
}

// this resets positions, guess, and letters_used array
void reset(char positions[], char guess[], int letters_used[]) {
  strcpy(guess, "");
  for (int i= 0; i < 5; i++) {
    positions[i]= '#';
    letters_used[i]= 0;
  }
}

// prints the grid and the word that they previously guessed
void print_grid(char grid[][6], char grid_guess[][6]) {
  for (int i= 0; i < 6; i++) {
    if (strcmp(grid[i], "")) {    
    printf("%c %c %c %c %c (%s)\n", grid[i][0], grid[i][1], grid[i][2],
        grid[i][3], grid[i][4], grid_guess[i]);

    }
  }
  printf("\n");
}

int main() {
  // set seed
  srand(time(0));

  int guesses_left= 6;
  char word[6];
  char guess[64];
  char positions[6]= {'#', '#', '#', '#', '#', '\0'}; // corresponds to the letters in the word
  int  letters_used[5]= {0, 0, 0, 0, 0}; // corresponds to the letters seen in guess

  // initialize the grid of guesses
  char grid[6][6];
  char grid_guess[6][6];

  strcpy(word, chooseWord());
  
  instructions();

  while (guesses_left > 0) {
        
    GuessWord(guess);
    if (strcmp(word, guess) == 0) {
      printf("You guessed it!! You Win!\n");
      return 0;
    }


    correct_positions(positions, word, guess, letters_used);
    find_chars(word, guess, positions, letters_used);
    
    strcpy(grid[6-guesses_left], positions);
    strcpy(grid_guess[6-guesses_left], guess);
    print_grid(grid, grid_guess);

    reset(positions, guess, letters_used);
    
    guesses_left--;

  }

  printf("\nYou lose!\nThe word was %s.\n", word);

}
