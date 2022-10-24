#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include <string.h>

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: ./decode <ppm file>\n");
    exit(1);
  }

  char input[64];
  strcpy(input, argv[1]);

  FILE* file= fopen(input, "r");
  if (!file) {
    printf("File %s cannot be read, exiting\n", argv[1]);
    exit(1);
  }

  int w;
  int h;

  unsigned char* image= (unsigned char*) read_ppm(input, &w, &h);
  printf("Reading %s with width %d and height %d\n", input, w, h);

  int maxChar= w*h*3/8;
  printf("Max number of characters in the image: %d\n", maxChar);
  unsigned char* message= malloc(sizeof(char)*maxChar+1);
  unsigned char mask= 0x1;


  for (int i= 0; i < w*h*3; i++) {
    //breaks when we find a terminating character (on the character last finished)
    if (i - 8 >= 0 && (i-8) % 8 == 0 && message[(i-8)/8] == '\0') break;
    unsigned char bit= image[i] & mask;
    if (bit) {
      bit= bit << (7 - i%8);
      message[i/8]|= bit;
    } else {
      bit= 0x1; // this is to get the place to turn 0
      bit= bit << (7 - i%8);
      bit= ~bit;
      message[i/8]&= bit;
    }
  }

  printf("%s", message);


  free(image);
  free(message);
  return 0;
}
