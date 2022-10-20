#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  unsigned long mask= 0x1ul;
  unsigned long shift_mask;
  for (int i= 63; i >= 0; i--) {
    shift_mask= mask << i;

    if (shift_mask & img) { // if bit is a 1 (black pixel)
      printf("@ ");
    } else { // if bit is a 0 (white pixel)
      printf("  "); 
    }

    if (i % 8 == 0) printf("\n");

  }
  printf("-----\n");
  return 0;
}
