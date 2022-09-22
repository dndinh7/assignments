#include <stdio.h>
#include <string.h>





// algorithm will take O(4^2) 
void maxShift(int* val, int* grid_val, int* max) {
  if (*grid_val == *val) {
    *max= *grid_val + *val;
  }
}


int main() {
  int state[4][4];
  int val;
  int max= 0;
  int max_no_shift= 0;
  int coord;


  for (int i= 0; i < 4; i++) {
    for (int j= 0; j < 4; j++) {
      scanf("%d", &state[i][j]);
    }
  }

  
  for (int i= 0; i < 4; i++) {
    for (int j= 0; j < 4; j++) {
      val= state[i][j];
      if (val < max) continue; // no need to calculate if less than max
      if (val > max_no_shift) max_no_shift = val; 
      coord= i;
      // shift up
      while (--coord >= 0 && state[coord][j] == 0);
      if (coord >= 0 ) maxShift(&val, &state[coord][j], &max);

      coord= i;
      // shift down
      while (++coord < 4 && state[coord][j] == 0);
      if (coord < 4) maxShift(&val, &state[coord][j], &max);
      

      coord= j;
      // shift left
      while (--coord >= 0 && state[i][coord] == 0);
      if (coord >= 0) maxShift(&val, &state[i][coord], &max);
      

      coord= j;
      // shift right
      while (++coord < 4 && state[i][coord] == 0);
      if (coord < 4) maxShift(&val, &state[i][coord], &max);
     

    }
  }
  
  if (max != 0) printf("The largest value that results from a shift is %d.\n", max);
  else printf("Shifts do not result in a new value. The current largest value on the grid is %d.\n", max_no_shift);


  return 0;
}
