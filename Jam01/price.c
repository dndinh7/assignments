#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  srand(time(0));
  
  int dishwasher_price= 3000 + rand()%2000;

  int AI1= 3000 + rand()%2000;
  int AI2= 3000 + rand()%2000;

  int user_guess;
  
  printf("Welcome to the Price is Right!\n");


  printf("Enter a guess to win a luxurious dishwasher: $");
  scanf("%d", &user_guess);

  printf("AI contestant #1 guesses %d\n", AI1);
  printf("AI contestant #2 guesses %d\n", AI2);

  printf("The dishwasher costs %d\n", dishwasher_price);

  int diff[3];

  diff[0] = dishwasher_price - AI1;
  diff[1] = dishwasher_price - AI2;
  diff[2] = dishwasher_price - user_guess;

  int minimum= 10000;
  int idx= -1;
  for (int i= 0; i < 3; i++) {
    if (diff[i] > 0 && diff[i] < minimum) {
        idx= i;
        minimum= diff[i];
    
    }

  }

  if (idx == -1) {
    printf("Sorry. No one wins the dishwasher. I'm keeping it.\n");
  } else {
    if (idx == 0) {
      printf("AI Contestant #1 wins the dishwasher!\n");
    } else if (idx == 1) {
      printf("AI Contestant #2 wins the dishwasher!\n");
    } else {
      printf("Congratulations!! You win the dishwasher!\n");
    }
  }

  return 0;
  

}

