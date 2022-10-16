#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <time.h>
#include <math.h>

// clamps it between min and max
int clamp(unsigned char a, unsigned char min, unsigned char max) {
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

// this will assign the color rgb
void rgb_color(unsigned char color[], unsigned char r, unsigned char g, unsigned char b) {
  color[0]= r;
  color[1]= g;
  color[2]= b;
}

// interpolate the colors
unsigned char interpolate(unsigned char a, unsigned char b, double alpha) {
  unsigned char hi= a*(1-alpha) + b*(alpha);
  return hi;
}

void col_glitch(struct ppm_pixel* image, int w, int h, double alpha) {
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
    int rand_rows= rand() % (h/2) + h/3;
    int rand_color= rand() % 12;
    for (int i= 0; i < rand_rows; i++) {
      image[i*w + j].red= interpolate(image[i*w + j].red, colors[rand_color][0], alpha);
      image[i*w + j].green= interpolate(image[i*w + j].green, colors[rand_color][1], alpha);
      image[i*w + j].blue= interpolate(image[i*w + j].blue, colors[rand_color][2], alpha);
    }
  }
}

// alpha is how much we want to darken the image
void darken_glitch(struct ppm_pixel* image, int w, int h, double alpha) {
  for (int i= 0; i < w*h; i++) {
    image[i].red= interpolate(image[i].red, 0, alpha);
    image[i].green= interpolate(image[i].green, 0, alpha);
    image[i].blue= interpolate(image[i].blue, 0, alpha);
  }
}

void jitter_glitch(struct ppm_pixel* image, int w, int h) {
  unsigned char red_jitter;
  unsigned char green_jitter;
  unsigned char blue_jitter;
  for (int i= 0; i < w*h; i++) {
    red_jitter= rand() % 35;
    green_jitter= rand() % 35;
    blue_jitter= rand() % 35;
    image[i].red= clamp(image[i].red + red_jitter, 0, 255);
    image[i].green= clamp(image[i].green + green_jitter, 0, 255);
    image[i].blue= clamp(image[i].blue + blue_jitter, 0, 255);
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
  

  
  
  //shift_glitch(image, w, h);
  //rotate_glitch(image, w, h); 
  jitter_glitch(image, w, h);
  col_glitch(image, w, h, 0.65); 
  invert_glitch(image, w, h);
  darken_glitch(image, w, h, 0.4);
  
  printf("Writing file %s\n", output);
  write_ppm(output, image, w, h);

  free(image);

  return 0;
}

