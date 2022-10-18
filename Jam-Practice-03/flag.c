#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Usage is flag <number> <position> <bit>\n");
    exit(1);
  }
  int orig_number= atoi(argv[1]);
  int new_number;

  int position= atoi(argv[2]);
  int bit;
  if (!strcmp("1", argv[3])) {
    bit= 0x1;
    bit<<= position;
    new_number= orig_number | bit;
  } else {
    int mask= 0x1;
    mask<<= position;
    bit= orig_number & mask;
    bit= ~bit;
    new_number= orig_number & bit;
  }
  printf("This is your original number: %x\n", orig_number);
  printf("This is your new number: %x\n", new_number);

  return 0;


}
