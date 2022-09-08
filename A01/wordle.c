#include <stdio.h>
#include <string.h>
#include "words.h"
#include <time.h>
#include <stdlib.h>

// This function will print out the rules and instructions of the game
void instructions() {
  printf("Welcome to WORDLE!\nGuess the 5-letter word in 6 guesses!\nLetters guessed in the correct position will be shown.\nLetters guessed correctly, but not in the right position will be shown as *.\nLetters not in the word will appear as #.\nYou lose after 6 wrong guesses.\n\n");
}

// allows the user to guess the word
void GuessWord(char guess[]) {
  printf("Your Guess: ");  
  scanf("%s", guess);

  // has to guess a valid 5 letter word
  while (strlen(guess) != 5) {
    printf("Please enter a valid 5 letter word. \n");
    scanf("%s", guess);
  }

}

// will return the index of the char if it appears in word
// else it will return -1
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
void correct_positions(char positions[], char word[], char guess[], int letters_used[]) {
  for (int i= 0; i < strlen(guess); i++) {
    if (word[i] == guess[i]) {
      positions[i]= word[i];
      letters_used[i]= 1;
    }
  }
}

void reset(char positions[], char guess[], int letters_used[]) {
  strcpy(guess, "");
  for (int i= 0; i < 5; i++) {
    positions[i]= '#';
    letters_used[i]= 0;
  }
}

void print_grid(char grid[][6]) {
  for (int i= 0; i < 6; i++) {
    if (strcmp(grid[i], "")) {    
    printf("%c %c %c %c %c\n", grid[i][0], grid[i][1], grid[i][2],
        grid[i][3], grid[i][4]);

    }
  }
}
int main() {
  // set seed
  srand(time(0));

  int guesses_left= 6;
  char word[6];
  char guess[6];
  char positions[6]= {'#', '#', '#', '#', '#', '\0'};
  int  letters_used[5]= {0, 0, 0, 0, 0};
  
  // initialize the grid of guesses
  char grid[6][6];
  for (int i= 0; i < 6; i++) {
    strcpy(grid[i], "");
  }

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
    print_grid(grid);

    reset(positions, guess, letters_used);
    
    guesses_left--;

  }

  printf("\nYou lose!\nThe word was %s.\n", word);

}
