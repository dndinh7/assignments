#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement one

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
  FILE* output;
  output= fopen(filename, "w");
  
  fprintf(output, "P6\n%d %d\n255\n", w, h);
  
  fwrite(pixels, 1, sizeof(struct ppm_pixel)*w*h, output);
  

  fclose(output);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
