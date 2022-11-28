#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>


void mandelbrot(struct ppm_pixel* image, struct ppm_pixel* palette, int size, float xmin, float xmax, float ymin, float ymax, int maxIterations) {
  for (int r= 0; r < size; r++) {
    for (int c= 0; c < size; c++) {
      float xfrac = (float)c/(float)size; // this is the x-coordinate based on the columns
      float yfrac = (float)r/(float)size; // this is the y-coordinate based on the rows
      float x0= xmin + xfrac * (xmax - xmin);
      float y0= ymin + yfrac * (ymax - ymin);
    
      float x= 0;
      float y= 0;
      int iter= 0;
      while (iter < maxIterations && x*x + y*y < 2*2) {
        float xtmp= x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      struct ppm_pixel color;
      if (iter < maxIterations) {
        color = palette[iter];
      } else {
        color = palette[maxIterations];
      }
      image[r * size + c]= color;
    }
  }


}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  // generate pallet
  srand(time(0));
  struct ppm_pixel* palette= malloc(sizeof(struct ppm_pixel)*(maxIterations+1)); // this is just for ease, when it goes over max iterations I just set the out of range to be black
  for (int i= 0; i < maxIterations; i++) {
    palette[i].red= rand() % 255;
    palette[i].green= rand() % 255;
    palette[i].blue= rand() % 255;
  }
  palette[maxIterations].red= 0;
  palette[maxIterations].green= 0;
  palette[maxIterations].blue= 0;



  // compute image
  struct ppm_pixel* image= malloc(sizeof(struct ppm_pixel) * size * size);

  struct timeval tstart, tend;

  // timing the function
  gettimeofday(&tstart, NULL);
  mandelbrot(image, palette, size, xmin, xmax, ymin, ymax, maxIterations);
  gettimeofday(&tend, NULL);


  // this is to write the file name
  char output[128];
  time_t t= time(0);
  strcpy(output, "mandelbrot-");

  char size_s[64];
  sprintf(size_s, "%d", size);

  char time_s[64];
  sprintf(time_s, "%ld", t);

  strcat(output, size_s);
  strcat(output, "-");
  strcat(output, time_s);
  strcat(output, ".ppm");

  write_ppm(output, image, size, size);

  double timer= tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g seconds\n", size, size, timer);
  printf("Writing file: %s\n", output);

  free(palette);
  free(image);

}
