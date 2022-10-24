#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  char input[64];
  strcpy(input, argv[1]);

  char output[64];
  int input_len= strlen(input)-4;

  strncpy(output, input, input_len); // removes extension
  output[input_len]= '\0';

  char suffix[32];
  strcpy(suffix, "-encoded.ppm");

  strcat(output, suffix); // appends suffix

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
  char* message= malloc(sizeof(char)*maxChar+1);
  
  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", message);
  unsigned char mask= 0x1;

  // we also want to include the terminating character
  for (int i= 0; i < (strlen(message)+1)*8; i++) {
    unsigned char bit= message[i/8] & (mask << (7 - i%8));
    // well since we only want to change the least significant bit, shift back
    bit>>= (7 - i % 8);
    if (bit) {
      image[i]|= bit;
    } else {
      bit= 0x1; // this is to get the place to turn 0
      bit= ~bit;
      image[i]&= bit;
    }
  }

  printf("Writing file %s\n", output);
  write_ppm(output, (struct ppm_pixel*) image, w, h);
  

  free(image);
  free(message);
  return 0;
}

