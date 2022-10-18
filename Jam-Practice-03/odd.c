#include <stdio.h>



int main() {
  int nums[9] = {4, 6, 8, 8, 6, 2, 3, 4, 3};
  int odd= 0;

  for (int i= 0; i < 9; i++) {
    odd^= nums[i];
  }
  printf("Here is the odd number out %d\n", odd);


  return 0;


}
