#include <stdio.h>
#include <string.h>


int main() {
  char word[64];
  int shift;

  printf("Enter a word: ");
  scanf("%s", word);

  
  printf("Enter a shift: ");
  scanf("%d", &shift);
  
  for (int i= 0; i < strlen(word); i++) {
    word[i]= 'a' + ((word[i]+shift%26)-'a')%26;
  }

  printf("Your cypher is %s\n", word);

  return 0;
}


