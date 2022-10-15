#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <time.h>


void shift_glitch(struct ppm_pixel* image, int w, int h) {
  for (int i= 0; i < w*h; i++) {
    image[i].red= image[i].red << (rand() % 2);
    image[i].green= image[i].green << (rand() % 2);
    image[i].blue= image[i].blue << (rand() % 2);
  }
}

void invert_glitch(struct ppm_pixel* image, int w, int h) {
  unsigned char inverter= 255;
  for (int i= 0; i < w*h; i++) {
    image[i].red= inverter - image[i].red;
    image[i].green= inverter- image[i].green;
    image[i].blue= inverter - image[i].blue;
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: glitch <file.ppm>\n");
    return 0;
  }
  srand(time(0));

  char input[64];
  strcpy(input, argv[1]);
  char output[64];
  int input_len= strlen(input)-4;

  strncpy(output, input, input_len); // removes extension
  output[input_len]= '\0';
  
  char suffix[32];
  strcpy(suffix, "-glitch.ppm");
  
  strcat(output, suffix); // appends suffix
  
  int w;
  int h;


  struct ppm_pixel* image= read_ppm(input, &w, &h);
  printf("Reading %s with width %d and height %d\n", input, w, h);
  
  invert_glitch(image, w, h);
  shift_glitch(image, w, h);
  printf("Writing file %s\n", output);
  write_ppm(output, image, w, h);

  free(image);

  return 0;
}

