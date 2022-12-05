#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include "string.h"
#include <sys/time.h>

struct thread_args {
  struct ppm_pixel* image;
  struct ppm_pixel* palette;
  int r_start;
  int r_end;
  int c_start;
  int c_end;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int id;
  pthread_mutex_t* mutex;
};



void *mandelbrot(void* args) { // delineates the parameters defined in main
  struct thread_args* tdata= (struct thread_args*) args;
  printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n", tdata->id, tdata->c_start, tdata->c_end, tdata->r_start, tdata->r_end);


  for (int r= tdata->r_start; r < tdata->r_end; r++) {
    for (int c= tdata->c_start; c < tdata->c_end; c++) {
      float xfrac = (float)c/(float)tdata->size; // this is the x-coordinate based on the columns
      float yfrac = (float)r/(float)tdata->size; // this is the y-coordinate based on the rows
      float x0= tdata->xmin + xfrac * (tdata->xmax - tdata->xmin);
      float y0= tdata->ymin + yfrac * (tdata->ymax - tdata->ymin);

      float x= 0;
      float y= 0;
      int iter= 0;
      while (iter < tdata->maxIterations && x*x + y*y < 2*2) {
        float xtmp= x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      struct ppm_pixel color;
      if (iter < tdata->maxIterations) {
        color = tdata->palette[iter];
      } else {
        color = tdata->palette[tdata->maxIterations];
      }
      pthread_mutex_lock(tdata->mutex);
      tdata->image[r * tdata->size + c]= color;
      pthread_mutex_unlock(tdata->mutex);
    }
  }

  printf("Thread %d) finished\n", tdata->id);
  return (void *)0;
}

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
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

  pthread_mutex_t mutex;


  struct ppm_pixel* image= malloc(sizeof(struct ppm_pixel) * size * size);

  pthread_t* threads= malloc(sizeof(pthread_t)*numProcesses);
  struct thread_args* arg= malloc(sizeof(struct thread_args)*numProcesses);

  pthread_mutex_init(&mutex, NULL);

  struct timeval tstart, tend;
  // timing the function
  gettimeofday(&tstart, NULL);
  for (int i= 0; i < numProcesses; i++) { // this has to be four for subdivision to work
    arg[i].image= image;
    arg[i].palette= palette;
    arg[i].r_start= (size/2) * (i/2);
    arg[i].r_end= size/(2 - i/2);
    arg[i].c_start= (size/2) * (i%2);
    arg[i].c_end= size/(2 - (i%2));
    arg[i].size= size;
    arg[i].xmin= xmin;
    arg[i].xmax= xmax;
    arg[i].ymin= ymin;
    arg[i].ymax= ymax;
    arg[i].size= size;
    arg[i].maxIterations= maxIterations;
    arg[i].mutex= &mutex;
    arg[i].id= i;
    pthread_create(&threads[i], NULL, mandelbrot, &arg[i]);
  }

  for (int i= 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
    
  }
  gettimeofday(&tend, NULL);

  pthread_mutex_destroy(&mutex);


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


  free(image);
  free(palette);
  free(threads);
  free(arg);



}
