#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  FILE* input= fopen("word.txt", "r");
  if (!input) {
    printf("Cannot open file\n");
    exit(1);
  }

  int ch= 0;
  int size= 0;
  while (ch != EOF) {
    ch= fgetc(input);
    size++;
  }
  char* s= malloc(sizeof(char) * size);
  rewind(input);

  s[size-1] = '\0';

  ch= 0;
  int i= 0;
  char c= 0;
  while (ch != EOF) {
    ch= fgetc(input);
    c= ch;
    s[size-1-i]= c;
    i++;
  }
  // there is a \n character at the end of the word
  printf("Here is the word reversed: %s\n", s);
  fclose(input);  
  free(s);
  return 0;

}
