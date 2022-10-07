#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include <string.h>

// returns a double just in case
char intensity(int red, int green, int blue, char symbols[]) {
  // will truncate because we should never be rounding up anyway
  int avg= (red + green + blue)/3.0;
  // clamping
  return avg/25 <= 9 ? symbols[avg/25] : symbols[9];

}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: ascii_image <file.ppm>\n");
    return 0;
  }
  int w= 0;
  int h= 0;
  char file[64];
  struct ppm_pixel* image;
  // easy way to assign the symbols
  char symbols[10];
  symbols[0]= '@';
  symbols[1]= '#';
  symbols[2]= '%';
  symbols[3]= '*';
  symbols[4]= 'o';
  symbols[5]= ';';
  symbols[6]= ':';
  symbols[7]= ',';
  symbols[8]= '.';
  symbols[9]= ' ';
  
  strcpy(file, argv[1]);
  image= read_ppm(file, &w, &h);
  if (image == NULL) {
    printf("Something went wrong with reading the file, such that it is NULL.\n");
    exit(1);
  }
  printf("Reading %s with width %d and height %d\n", file, w, h);
  
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  for (int i= 0; i < h; i++) {
    for (int j= 0; j < w; j++) {
      red= image[i*w + j].red;
      green= image[i*w + j].green;
      blue= image[i*w + j].blue;
      printf("%c", intensity(red, green, blue, symbols));
    }
    printf("\n");
  }


  free(image);
  return 0;
}

