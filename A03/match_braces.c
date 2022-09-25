#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// this bracket stack should always just be '{'
struct stack {
  int line;
  int col;
  struct stack* below;
};

struct stack* push(char bracket, int line, int col, struct stack* stack) {
  struct stack* s= malloc(sizeof(struct stack));
  s->line= line;
  s->col= col;
  s->below= stack;

  return s;
}

struct stack* pop(struct stack* stack) {
  struct stack* s= stack->below;
  free(stack);
  return s;
}

void clear(struct stack* stack) {
  struct stack* cur= stack;
  while (cur) {
    struct stack* next= cur->below;
    free(cur);
    cur= next;
  }
}

void print_stack(struct stack* stack) {
  struct stack* cur= stack;
  while (cur) {
    struct stack* next= cur->below;
    printf("This is bracket } at line number %d and column %d.\n",
        cur->line, cur->col);
    cur= next;
  }
}

int main(int argc, char** argv) {
  FILE *infile;

  if (argc != 2) {
    printf("usage: ./match_braces <file>\n");
    exit(1);
  }
  
  infile = fopen(argv[1], "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", argv[1]);
    exit(1);
  }
  int line= 1;
  int col= 1;
  int ch;
  struct stack* stack= NULL;

  ch= fgetc(infile);
  while (ch != EOF) {
    if (ch == '\n') {
      line++;
      col= 1;
      ch= fgetc(infile);
      continue;
    }

    if (ch == '}') {
      if (stack) {
        printf("Found matching braces: (%d, %d) -> (%d, %d)\n", stack->line,
            stack->col, line, col);
        stack= pop(stack);
      } else {
        printf("Unmatched brace '}' on Line %d, Column %d\n", line, col);
      }
    } else if (ch == '{') {
      stack= push('}', line, col, stack);
    }

    ch= fgetc(infile);
    col++;
  }

  while (stack) {
    printf("Unmatched brace '{' on Line %d, Column %d\n", stack->line, stack->col);
    stack= pop(stack);
  }
  
  fclose(infile);
  
  return 0;
}
