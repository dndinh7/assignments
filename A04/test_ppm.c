#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include <string.h>

int main() {
  // load feep-raw and print out the grid of pixels
  int w= 0;
  int h= 0;
  char file[64];
  struct ppm_pixel* image;

  strcpy(file, "feep-raw.ppm");
  image= read_ppm(file, &w, &h);
  
  printf("Testing file feep-raw.ppm: width %d and height %d\n", w, h);

  for (int i= 0; i < h; i++) {
    for (int j= 0; j < w; j++) {
      printf("(%u, %u, %u) ", image[i*w + j].red, image[i*w +j].green,
          image[i*w + j].blue);

    }
    printf("\n");
  }

  free(image);
  return 0;
}
