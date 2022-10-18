#include <stdio.h>



int main() {
  unsigned int num;
  

  printf("Enter an unsigned integer: ");
  scanf("%ud", &num);
  printf("Here is your number in binary %x\n", num);
  unsigned int mask= 0x10000000;
  unsigned char significant_bit= num & mask;

  num<<= 1;

  if (significant_bit) {
    num= num | 0x1;
  }

  printf("Here is the shifted number %x\n", num);
  return 0;




}
