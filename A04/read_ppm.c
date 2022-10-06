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
  int max_color= 0;
  int row= 0;

  // no line should have more than 70 chars
  char line[81];
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
    // after we get the max color value, there is only a single white space
    token= strtok(NULL, whitespace);
    break;
  }

  image= malloc(sizeof(struct ppm_pixel) * *w * *h);
  if (image == NULL) {
    printf("Not enough space to allocate the image.\n");
    return NULL;
  }

  enum color{red, green, blue};

  // this is the case where the whitespace isnt a new line
  int idx= 0;
  if (token != NULL) {
    int n= strlen(token);
    while (idx < n) {
      switch(idx%3) {
        case red:
          image[idx/3].red= token[idx];
          break;
        case green:
          image[idx/3].green= token[idx];
          break;
        case blue:
          image[idx/3].blue= token[idx];
          break;
      }
      idx++;
    }
  }
  
  // after the token string is iterated over, we just go onto the next character
  // in the file pointer
  int c;
  int num_pixels= *w * *h * 3;
  while ((c= fgetc(input)) != EOF) {
    // checks if the indices don't go past the number of pixels
    if (idx >= num_pixels) {
      printf("The amount of read pixels do not match up to the header pixels.\n");
      return NULL;
    }

    switch(idx%3) {
      case red:
        image[idx/3].red= c;
        break;
      case green:
        image[idx/3].green= c;
        break;
      case blue:
        image[idx/3].blue= c;
        break; 
    }
    idx++;
  }

  fclose(input);

  return image;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
