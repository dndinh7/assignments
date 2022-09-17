#include <stdio.h>
#include <string.h>
#include <stdlib.h>




int main() {
  char buff[32];
  int pause;
  int size_slow;

  printf("Pause length: ");
  scanf("%d", &pause);

  printf("Text: ");
  scanf("%s", buff);

  // you add pause number of periods onto buff and add 1 for terminating char
  size_slow= (pause+1)*strlen(buff)+1;
  char* slow= malloc(size_slow);

  for (int i= 0; i < size_slow-1; i++) {
    if (i % (pause+1) == 0) {
      slow[i]= buff[i/(pause+1)];
    } else {
      slow[i]= '.';
    }
  }

  slow[size_slow-1]= '\0';

  printf("%s\n", slow);

  free(slow);
  slow= NULL;

  return 0;
}
