#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// prints grid of characters
void print_grid(char* grid, int m, int n) {
  for (int i= 0; i < m; i++) {
    for (int j= 0; j < n; j++) {
      printf("%c ", grid[i*n + j]);
      
    }
    printf("\n");
  }
}

// inserts mines in 
void mineInCell(double p, char* cell, char* solved_cell) {
    int rand_num= rand() % 10000;
    p*= 10000;
    if (rand_num <= p) {
      *cell= 'X';
      *solved_cell= 'X';
    } else {
      *cell= '.';
    }
}

// numbers the grid
void number_grid(char* solved_grid, int m, int n) {
  for (int i= 0; i < m; i++) {
    for (int j= 0; j < n; j++) {
      if (solved_grid[i*n + j] == 'X') {
        // up
        if (i-1 >= 0 && solved_grid[(i-1)*n + j] != 'X') solved_grid[(i-1)*n + j]+= 1;
        // left
        if (j-1 >= 0 && solved_grid[i*n + j-1] != 'X') solved_grid[i*n + (j-1)]+= 1;
        // down
        if (i+1 < m && solved_grid[(i+1)*n + j] != 'X') solved_grid[(i+1)*n + j]+= 1;
        // right
        if (j+1 < n && solved_grid[i*n + j+1] != 'X') solved_grid[i*n + j+1]+= 1;
        // topright
        if (i-1 >= 0 && j+1 < n && solved_grid[(i-1)*n + j+1] != 'X') solved_grid[(i-1)*n + j+1]+= 1;
        // topleft
        if (i-1 >= 0 && j-1 >= 0 && solved_grid[(i-1)*n + j-1] != 'X') solved_grid[(i-1)*n + j-1]+= 1;
        // botright
        if (i+1 < m && j+1 < n && solved_grid[(i+1)*n + j+1] != 'X') solved_grid[(i+1)*n + j+1]+= 1;
        // botleft
        if (i+1 < m && j-1 >= 0 && solved_grid[(i+1)*n + j-1] != 'X') solved_grid[(i+1)*n + j-1]+= 1;
      }
    }
  }
}

int main(int argc, char** argv) {
  srand(time(0));

  if (argc != 4) {
    printf("usage: %s m n p\n", argv[0]);
    return 0;
  }
  
  int m= atoi(argv[1]);
  int n= atoi(argv[2]);
  double p= atof(argv[3]);

  char* grid= malloc(sizeof(char)*m*n);
  char* solved_grid= malloc(sizeof(char)*m*n);
  memset(solved_grid, '0',  sizeof(char)*m*n);
  for (int i= 0; i < m; i++) {
    for (int j= 0; j < n; j++) {
      mineInCell(p, &grid[i*n+j], &solved_grid[i*n+j]);
    }
  }
  number_grid(solved_grid, m, n);
  print_grid(grid, m, n);
  printf("\n\n");
  print_grid(solved_grid, m, n);

  free(grid);
  free(solved_grid);
  return 0;
}
