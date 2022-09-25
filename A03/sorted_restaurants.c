#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct restaurant {
  char name[32];
  int open_time;
  int close_time;
  float rating;
};

struct node {
  struct restaurant r;
  struct node* next;
};

void print_restaurants(struct node* head) {
  int i= 0;
  struct restaurant restaurant;
  while (head) {
    restaurant= head->r;
    printf("%d) %-32s Opens: %02d:00    Closes: %02d:00    Rating: %.2f\n",
      i++, restaurant.name, restaurant.open_time,
      restaurant.close_time, restaurant.rating);
    head= head->next;
  }
}

struct node* insert_sorted(struct restaurant new_rest, struct node* head) {
  struct node* n= malloc(sizeof(struct node));
  if (n == NULL) {
    printf("Not enough space to allocate for a new node.\n");
    exit(1);
  }
  
  n->r= new_rest;
  n->next= NULL;

  // node is in the start of the list
  if (head == NULL || n->r.rating >= head->r.rating) {
    n->next= head;
    return n;
  }

  struct node* cur= head;
  // node is in the middle of the list
  while (cur->next) {
    if (n->r.rating >= cur->next->r.rating) {
       n->next= cur->next;
       cur->next= n;
       return head;
    }
    cur= cur->next;
  }

  // node is at the end of the list
  cur->next= n;
  return head;
}

void clear(struct node* head) {
  struct node* cur= head;
  while (cur) {
    struct node* next= cur->next;
    free(cur);
    cur= next;
  }
}




int main() {
  int num_restaurants;


  printf("Enter number of restaurants: ");
  scanf("%d%*c", &num_restaurants);
 
  struct node* head= NULL;
  struct restaurant restaurant;
  for (int i= 0; i < num_restaurants; i++) {
    printf("Enter a name: ");
    scanf("%[^\n]%*c", restaurant.name);
    printf("Open time (24 hours): ");
    scanf("%d%*c", &restaurant.open_time);
    printf("Closing time (24 hours): ");
    scanf("%d%*c", &restaurant.close_time);
    printf("Stars (0 to 5): ");
    scanf("%e%*c", &restaurant.rating);
    printf("\n");


    head= insert_sorted(restaurant, head);
  }

  printf("\n\nWelcome to Sorted Sashimi's Favorite List of Restaurants\n\n");

  print_restaurants(head);

  clear(head);
  
  return 0;
}
