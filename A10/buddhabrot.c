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
#include "math.h"


struct thread_args {
  struct ppm_pixel* image;
  char* membership;
  int* counts;
  int* maxCount;
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
  pthread_barrier_t* barrier;
};

void inMandelbrot(struct thread_args* data) { // delineates the parameters defined in main
  for (int r= data->r_start; r < data->r_end; r++) {
    for (int c= data->c_start; c < data->c_end; c++) {
      float xfrac = (float)c/(float)data->size; // this is the x-coordinate based on the columns
      float yfrac = (float)r/(float)data->size; // this is the y-coordinate based on the rows
      float x0= data->xmin + xfrac * (data->xmax - data->xmin);
      float y0= data->ymin + yfrac * (data->ymax - data->ymin);

      float x= 0;
      float y= 0;
      int iter= 0;
      while (iter < data->maxIterations && x*x + y*y < 2*2) {
        float xtmp= x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      pthread_mutex_lock(data->mutex);
      if (iter < data->maxIterations) {
        data->membership[r * data->size + c]= 0;
      } else {
        data->membership[r * data->size + c]= 1;
      }
      pthread_mutex_unlock(data->mutex);
    }
  }
}

void visitedCounts(struct thread_args* data) {
  
  for (int r= data->r_start; r < data->r_end; r++) {
    for (int c= data->c_start; c < data->c_end; c++) {
      if (data->membership[r * data->size + c]) continue;

      float xfrac= (float)c / (float)data->size;
      float yfrac= (float)r / (float)data->size;
      float x0= data->xmin + xfrac * (data->xmax - data->xmin);
      float y0= data->ymin + yfrac * (data->ymax - data->ymin);

      float x= 0;
      float y= 0;
      while (x*x + y*y < 2*2) {
        float xtmp= x*x - y*y + x0;
        y= 2*x*y + y0;
        x= xtmp;

        int yrow= round(data->size * (y - data->ymin)/(data->ymax - data->ymin));
        int xcol= round(data->size * (x - data->xmin)/(data->xmax - data->xmin));
        if (yrow < 0 || yrow >= data->size) continue; // out of range
        if (xcol < 0 || xcol >= data->size) continue; // out of range

        // updates the count and updates maxCount
        pthread_mutex_lock(data->mutex);
        *(data->maxCount)= ++data->counts[yrow * data->size + xcol] > *(data->maxCount) ? data->counts[yrow * data->size + xcol] : *(data->maxCount);
        pthread_mutex_unlock(data->mutex);
      }
    }
  }
}

void computeColor(struct thread_args* data) {
  float gamma= 0.681;
  float factor= 1.0/gamma;
  for (int r= data->r_start; r < data->r_end; r++) {
    for (int c= data->c_start; c < data->c_end; c++) {
      float value= 0;

      if (data->counts[r * data->size + c] > 0) {
        value = log(data->counts[r * data->size + c]) / log(*(data->maxCount));
        value = pow(value, factor);
      }

      unsigned char red= value * 255;
      unsigned char green= value * 255;
      unsigned char blue= value * 255;

      pthread_mutex_lock(data->mutex);
      data->image[r * data->size + c].red= red;
      data->image[r * data->size + c].green= green;
      data->image[r * data->size + c].blue= blue;
      pthread_mutex_unlock(data->mutex);
    }
  }
}

void *start(void* data) {
  struct thread_args* tdata= (struct thread_args*) data;
  printf("Thread %d) sub-image block: cols (%d, %d) to rows (%d, %d)\n", tdata->id, tdata->c_start, tdata->c_end, tdata->r_start, tdata->r_end);

  inMandelbrot(tdata);
  visitedCounts(tdata);
  
  pthread_barrier_wait(tdata->barrier);
  computeColor(data);

  printf("Thread %d) finished\n", tdata->id);
  return (void*)0;
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
  // compute image


  pthread_mutex_t mutex;


  struct ppm_pixel* image= malloc(sizeof(struct ppm_pixel) * size * size);

  pthread_t* threads= malloc(sizeof(pthread_t)*numProcesses);
  struct thread_args* arg= malloc(sizeof(struct thread_args)*numProcesses);
  char* membership= malloc(sizeof(char)*size*size);
  int* counts= malloc(sizeof(int)*size*size);
  memset(counts, 0, sizeof(int)*size*size);
  int maxCount= 0;

  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, numProcesses);

  struct timeval tstart, tend;
  // timing the function
  gettimeofday(&tstart, NULL);
  for (int i= 0; i < numProcesses; i++) { // this has to be four for subdivision to work
    arg[i].image= image;
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
    arg[i].membership= membership;
    arg[i].counts= counts;
    arg[i].maxCount= &maxCount;
    arg[i].barrier= &barrier;
    pthread_create(&threads[i], NULL, start, &arg[i]);
  }

  for (int i= 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
    
  }
  gettimeofday(&tend, NULL);

  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);


  // this is to write the file name
  char output[128];
  time_t t= time(0);
  strcpy(output, "buddhabrot-");

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
  printf("Computed buddhabrot set (%dx%d) in %g seconds\n", size, size, timer);
  printf("Writing file: %s\n", output);


  free(image);
  free(threads);
  free(arg);
  free(membership);
  free(counts);
}
