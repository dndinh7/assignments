#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct restaurant {
  char name[32];
  int open_time;
  int close_time;
  float stars;
  int id;
};


void print_restaurants(struct restaurant* restaurant) {
  printf("%d) %-29s Opens: %2d:00    Closes: %2d:00    Rating: %.2f\n",
    restaurant->id, restaurant->name, restaurant->open_time,
    restaurant->close_time, restaurant->stars);

}


int main() {
  int num_restaurants;


  printf("Enter number of restaurants: ");
  scanf("%d%*c", &num_restaurants);

  struct restaurant* restaurants= malloc(sizeof(struct restaurant) * num_restaurants);
  
  for (int i= 0; i < num_restaurants; i++) {
    restaurants[i].id= i;
    printf("Enter a name: ");
    scanf("%[^\n]%*c", restaurants[i].name);
    printf("Open time (24 hours): ");
    scanf("%d%*c", &restaurants[i].open_time);
    printf("Closing time (24 hours): ");
    scanf("%d%*c", &restaurants[i].close_time);
    printf("Stars (0 to 5): ");
    scanf("%e%*c", &restaurants[i].stars);

  }

  for (int i= 0; i < num_restaurants; i++) {
    print_restaurants(&restaurants[i]);
  }


  free(restaurants);
  return 0;
}
