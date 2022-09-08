#include <stdio.h>
#include <string.h>

struct restaurant {
  char  name[64];
  int   opening_hour;
  int   closing_hour;
  float rating;
};


int main() {
  struct restaurant restaurant[4];
  int hour;
  int rest;

  strcpy(restaurant[0].name, "Gusteau's");
  restaurant[0].opening_hour= 10;
  restaurant[0].closing_hour= 18;
  restaurant[0].rating= 4.9;

  strcpy(restaurant[1].name, "Bonefish Grill");
  restaurant[1].opening_hour= 16;
  restaurant[1].closing_hour= 2;
  restaurant[1].rating= 3.5;

  strcpy(restaurant[2].name, "Saigon Social");
  restaurant[2].opening_hour= 6;
  restaurant[2].closing_hour= 17;
  restaurant[2].rating= 5.0;

  strcpy(restaurant[3].name, "Tada Korean Fried Chicken");
  restaurant[3].opening_hour= 3;
  restaurant[3].closing_hour= 20;
  restaurant[3].rating= 3.8;
  
  printf("Some local favorite restaurants.\n\n");

  printf("What hour is it (24 hr clock)? ");
  scanf("%d", &hour);
  printf("\n");
  
  if (hour >= 24 || hour < 0) {
    printf("Sorry, %d is not a valid time! Exiting program...\n", hour);
    return 0;
  }

  for (int i= 0; i < 4; i++) {
    printf("%d) %-29s Opens: %2d:00    Closes: %2d:00    Rating: %.2f\n",
        i, restaurant[i].name, restaurant[i].opening_hour,
        restaurant[i].closing_hour, restaurant[i].rating);

  }
  
  printf("\nWhich restaurant (0, 1, 2, or 3) do you want to go to? ");
  scanf("%d", &rest);
  printf("\n");

  if (restaurant[rest].closing_hour < restaurant[rest].opening_hour) {
    if ((hour < restaurant[rest].opening_hour && hour < restaurant[rest].closing_hour)
        || (hour >= restaurant[rest].opening_hour && hour > restaurant[rest].closing_hour)) {
      printf("%s is OPEN! Let's go!\n", restaurant[rest].name);
    } else {
      printf("%s is CLOSED at %d o'clock!\n", restaurant[rest].name, hour);
    }
  } else {
    if (hour < restaurant[rest].opening_hour || hour > restaurant[rest].closing_hour) {
      printf("%s is CLOSED at %d o'clock!\n", restaurant[rest].name, hour);
    } else {
      printf("%s is OPEN! Let's go!\n", restaurant[rest].name);
    }
  }

}
