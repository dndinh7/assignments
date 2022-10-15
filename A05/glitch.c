#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <time.h>
#include <math.h>

// clamps it between min and max
int clamp(int a, int min, int max) {
  if (a < min) return min;
  else if (a > max) return max;
  return a;
}

// this will shift the byte to the left by 1 or 2 bits
void shift_glitch(struct ppm_pixel* image, int w, int h) {
  for (int i= 0; i < w*h; i++) {
    image[i].red= image[i].red << (rand() % 2);
    image[i].green= image[i].green << (rand() % 2);
    image[i].blue= image[i].blue << (rand() % 2);
  }
}

// this will complement the color
void invert_glitch(struct ppm_pixel* image, int w, int h) {
  unsigned char inverter= 255;
  for (int i= 0; i < w*h; i++) {
    image[i].red= inverter - image[i].red;
    image[i].green= inverter - image[i].green;
    image[i].blue= inverter - image[i].blue;
  }
}

// this will rotate the colors by the rgb "tuple"
void rotate_glitch(struct ppm_pixel* image, int w, int h) {
  // this will be the rotation factor for the pixels of rgb
  unsigned char red_rotate= 100;
  unsigned char green_rotate= 200;
  unsigned char blue_rotate= 55;

  for (int i= 0; i < w*h; i++) {
    image[i].red= (red_rotate + image[i].red)%255;
    image[i].green= (green_rotate + image[i].green)%255;
    image[i].blue= (blue_rotate + image[i].blue)%255;
  }
}

void rgb_color(unsigned char color[], unsigned char r, unsigned char g, unsigned char b) {
  color[0]= r;
  color[1]= g;
  color[2]= b;
}

void col_glitch(struct ppm_pixel* image, int w, int h) {
  enum {red, green, blue, yellow, cyan, magenta, orange, yellow_green, cyan_green, cyan_blue, blue_magenta, red_magenta};
  unsigned char colors[12][3];
  rgb_color(colors[red], 255, 0, 0);
  rgb_color(colors[green], 0, 255, 0);
  rgb_color(colors[blue], 0, 0, 255);
  rgb_color(colors[yellow], 255, 255, 0);
  rgb_color(colors[cyan], 0, 255, 255);
  rgb_color(colors[magenta], 255, 0, 255);
  rgb_color(colors[orange], 255, 125, 0);
  rgb_color(colors[yellow_green], 125, 255, 0);
  rgb_color(colors[cyan_green], 0, 255, 125);
  rgb_color(colors[cyan_blue], 0, 125, 255);
  rgb_color(colors[blue_magenta], 125, 0, 255);
  rgb_color(colors[red_magenta], 255, 0, 255);
  for (int j= 0; j < w; j++) {
    int rand_rows= rand() % (h/3) + h/2;
    for (int i= 0; i < h; i++) {

    }
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
  


  //invert_glitch(image, w, h);
  //shift_glitch(image, w, h);
  //rotate_glitch(image, w, h);  
  
  printf("Writing file %s\n", output);
  write_ppm(output, image, w, h);

  free(image);

  return 0;
}

