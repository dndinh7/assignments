#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>

void mandelbrot(struct ppm_pixel* image, struct ppm_pixel* palette, 
  int r_start, int r_end, int c_start, int c_end, // delineates which square we are calculating
  int size, float xmin, float xmax, float ymin, float ymax, int maxIterations) { // delineates the parameters defined in main
  for (int r= r_start; r < r_end; r++) {
    for (int c= c_start; c < c_end; c++) {
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
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case 'p': numProcesses = atoi(optarg); break;
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

  // after we fork, this will stay in place for each process

  // allocate shared memory of image pixels
  int shmid;
  shmid= shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size * size, 0644 | IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  struct ppm_pixel* image= shmat(shmid, NULL, 0);
  if (image == (void*)-1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }



  // compute image

  // timing
  struct timeval tstart, tend;

  // create processes
  gettimeofday(&tstart, NULL);
  pid_t pid;
  int child_status;
  pid= fork();
  printf("Launched child process: %d\n", getpid());
  if (pid == 0) {
    pid= fork();
    if (pid == 0) {
      printf("Launched child process: %d\n", getpid());
      pid = fork();
      if (pid == 0) { // 4th child
        printf("Launched child process: %d\n", getpid());
        printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d)\n", getpid(), size/2, size, size/2, size);
        mandelbrot(image, palette, size/2, size, size/2, size, size, xmin, xmax, ymin, ymax, maxIterations);
        printf("Child process complete: %d\n", getpid());

      } else { // 3rd child
        printf("%d) Sub-image block: cols (0, %d) to rows (%d, %d)\n", getpid(), size/2, size/2, size);
        mandelbrot(image, palette, size/2, size, 0, size/2, size, xmin, xmax, ymin, ymax, maxIterations);
        printf("Child process complete: %d\n", getpid());
        pid= wait(&child_status);
      }

    } else { // 2nd child
      printf("%d) Sub-image block: cols (%d, %d) to rows (0, %d)\n", getpid(), size/2, size, size/2);
      mandelbrot(image, palette, 0, size/2, size/2, size, size, xmin, xmax, ymin, ymax, maxIterations);
      printf("Child process complete: %d\n", getpid());
      pid= wait(&child_status);
    }
    
  } else { // 1st child
    printf("%d) Sub-image block: cols (0, %d) to rows (0, %d)\n", getpid(), size/2, size/2);
    mandelbrot(image, palette, 0, size/2, 0, size/2, size, xmin, xmax, ymin, ymax, maxIterations);
    // this is the parent process, which will wait for the four children to end before writing
    pid= wait(&child_status);
    printf("Child process complete: %d\n", getpid());
    gettimeofday(&tend, NULL);

    // this is to write the file name
    char output[128];
    time_t t= time(0);
    strcpy(output, "multi-mandelbrot-");

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

    //clean up for shared image pixels
    if (shmdt(image) == -1) {
      perror("Error: cannot detach from shared memory\n");
      exit(1);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
      perror("Error: cannot remove shared memory\n");
      exit(1);
    }

    

    free(palette);


  }



}
