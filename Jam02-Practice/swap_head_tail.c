#include <stdio.h>
#include <stdlib.h>

struct node {
    int val;
      struct node* next;
};

struct node* insert_front(int val, struct node* head) {
    struct node* n = malloc(sizeof(struct node));
      if (n == NULL) {
            printf("ERROR: Out of space!\n");
                exit(1);
                  }
        n->val = val;
          n->next = head;
            return n;
}

void print(struct node* list) {
    for (struct node* n = list; n != NULL; n = n->next) {
      printf("Val: %d\n", n->val);
    }
}

struct node* swap_nodes(struct node* head) {
  struct node* cur= head;

  while  (cur->next->next) {
    cur= cur->next;
  }
  struct node* tail= cur->next;
  tail->next= head->next;
  cur->next= head;
  head->next= NULL;

  return tail;

}

int main() {
  struct node* n5 = insert_front(4, NULL);
  struct node* n4 = insert_front(3, n5);
  struct node* n3 = insert_front(2, n4);
  struct node* n2 = insert_front(1, n3);
  struct node* n1 = insert_front(0, n2);
  print(n1); 
  printf("\n");
  struct node* new_head= swap_nodes(n1);
  print(new_head);


  free(n1);
  free(n2);
  free(n3);
  free(n4);
  free(n5);
}
