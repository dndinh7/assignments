#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE* input= fopen(filename, "r");
  if (!input) {
    printf("Input file %s could not be opened", filename);
    return NULL;
  }

  // delimiter
  char *whitespace= " \t\f\r\v\n";

  
  struct ppm_pixel* image;
  //char magic_num[16];
  int max_color= 0;
  int row= 0;

  // no line should have more than 70 chars
  char line[80];
  char *token;
  

  // hard code to get information about image (header)

  while (fgets(line, 80, input) != NULL && row < 3) {
    token= strtok(line, whitespace);
    if (token[0] == '#') { // this line is then a comment, so we skip
      continue;
    }
    if (row == 0) { // check magic number
      if (token[0] != 'P' && (token[1] != '3' || token[1] != '6')) {
        printf("Did not get a valid magic number.\n");
        return NULL;
      }
      row++;
      token= strtok(NULL, whitespace);
      if (token == NULL) {
        continue;
      }

    }
    if (row == 1) { // check grid sizes
      *w= atoi(token);
      if (*w == 0) {
        printf("Not a valid width.\n");
        return NULL;
      }

      token= strtok(NULL, whitespace);
      if (token == NULL) {
        fgets(line, 80, input);
        token= strtok(line, whitespace);
      }
      
      *h= atoi(token);
      if (*h == 0) {
        printf("Not a valid height.\n");
        return NULL;
      }

      row++;
      token= strtok(NULL, whitespace);
      if (token == NULL) {
        continue;
      }
    }
    // check max color value
    max_color= atoi(token);
    if (max_color == 0) {
      printf("Not a valid max color value.\n");
      return NULL;
    }
    row++;
    token= strtok(NULL, whitespace);
    break;
  }
  
  image= malloc(sizeof(struct ppm_pixel) * *w * *h);
  if (image == NULL) {
    printf("Not enough space to allocate the image.\n");
    return NULL;
  }
  
  // each character then is now a color, so we can put them in the struct
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  for (int i= 0; i < *h; i++) {
    for (int j= 0; j < *w; j++) {
      red= fgetc(input);
      green= fgetc(input);
      blue= fgetc(input);
      image[i* *w + j].red= red;
      image[i* *w + j].green= green;
      image[i* *w + j].blue= blue;
    }
  }



  fclose(input);

  return image;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
