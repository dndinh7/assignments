#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// function to assign correct color value to the right pixel in image
void assign_pixel(struct ppm_pixel* image, int idx, char c) {

  // every 3 bytes constitutes a pixel, so we can represent
  // 0 as red, 1 as green, and 2 as blue
  switch(idx%3) {
    case 0:
      image[idx/3].red= c;
      break;
    case 1:
      image[idx/3].green= c;
      break;
    case 2:
      image[idx/3].blue= c;
      break; 
  }
}

// checks if the character is less than the max_value
int c_over_max(int c, int max_color, FILE* file, struct ppm_pixel* image) {
  if (c > max_color) {
    printf("The color value %d is over max color value %d\n", c, max_color);
    free(image);
    fclose(file);
    return 1;
  }
  return 0;
}

// checks if the index is over the amount of colors (one pixel == 3 colors)
int idx_over_color(int idx, int num_colors, FILE* input, struct ppm_pixel* image) {
  if (idx >= num_colors) {
    printf("This is the idx %d and it's going over the number of colors %d.\n", idx, num_colors);
    free(image);
    fclose(input);
    return 1;
  }
  return 0;
}

// Choose *one* to implement
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE* input= fopen(filename, "r");
  if (!input) {
    printf("Input file %s could not be opened\n", filename);
    return NULL;
  }

  // delimiter
  char *whitespace= " \t\f\r\v\n";

  
  struct ppm_pixel* image;
  int max_color= 0;
  int row= 0;
  char magic_num= '0';

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
      magic_num= token[1];      
      if (token[0] != 'P' && (token[1] != '3' || token[1] != '6')) {
        printf("Did not get a valid magic number.\n");
        fclose(input);
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
        fclose(input);
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
        fclose(input);
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
    if (max_color == 0 || max_color > 255) {
      printf("Not a valid max color value.\n");
      fclose(input);
      return NULL;
    }
    // after we get the max color value, there is only a single white space
    token= strtok(NULL, whitespace);
    break;
  }

  image= malloc(sizeof(struct ppm_pixel) * *w * *h);
  if (image == NULL) {
    printf("Not enough space to allocate the image.\n");
    fclose(input);
    return NULL;
  }

  int idx= 0;
  int num_colors= *w * *h * 3;
  int c= 0;
  // this is binary representation of pixels
  if (magic_num == '6') {
    // this is the case where the whitespace isnt a new line
    if (token != NULL) {
      int n= strlen(token);
      while (idx < n) {
        c= token[idx];
        // makes sure the index doesn't go over the number of pixels (seg fault)
        if (idx_over_color(idx, num_colors, input, image)) return NULL;
        // makes sure the character value is below the max value color
        if (c_over_max(c, max_color, input, image)) return NULL;
        assign_pixel(image, idx, c);
        idx++;
      }
    }
    
    // after the token string is iterated over, we just go onto the next character
    // in the file pointer
    int c;
    while ((c= fgetc(input)) != EOF) {
      if (idx_over_color(idx, num_colors, input, image)) return NULL;
      if (c_over_max(c, max_color, input, image)) return NULL;
      assign_pixel(image, idx, c);
      idx++;
    }
  } else if (magic_num == '3') { // decimal form for pixels
    while (token != NULL) {
      c= atoi(token);
      if (idx_over_color(idx, num_colors, input, image)) return NULL;
      if (c_over_max(c, max_color, input, image)) return NULL;
      assign_pixel(image, idx, c);
      token= strtok(NULL, whitespace);       
      idx++;
    }

    while (fgets(line, 80, input) != NULL) {
      token= strtok(line, whitespace);
      while (token != NULL) {
        c= atoi(token);
        if (idx_over_color(idx, num_colors, input, image)) return NULL;
        if (c_over_max(c, max_color, input, image)) return NULL;
        assign_pixel(image, idx, c);
        token= strtok(NULL, whitespace);       
        idx++;
      }
    }
  }

  if (idx != num_colors) {
    printf("The number of colors %d go over the header defined colors %d\n", idx, num_colors);
    free(image);
    fclose(input);
    return NULL;
  }

  fclose(input);

  return image;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
