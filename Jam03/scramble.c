#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  char message[64];
  printf("Please enter a phrase: ");
  scanf("%[^\n]%*c", message);

  unsigned char mask3= 0x1 << 2;
  unsigned char mask4= 0x1 << 3;

  for (int i= 0; i < strlen(message); i++) {
    unsigned char bit3= message[i] & mask3;
    unsigned char bit4= message[i] & mask4;
    if (bit3) {
      bit3 <<= 1;
      message[i]|= bit3;
    } else {
      bit3= 0x1 << 3;
      bit3= ~bit3;
      message[i]&= bit3;
    }

    if (bit4) {
      bit4 >>= 1;
      message[i]|= bit4;
    } else {
      bit4= 0x1 << 2;
      bit4= ~bit4;
      message[i]&= bit4;
    }
  }

  printf("scramble: %s\n", message);


  return 0;



}