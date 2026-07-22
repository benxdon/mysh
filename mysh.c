#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 128
#define MAXARGS 128

int eval(char *cmdline) {
  if (strcmp(cmdline, "exit\n") == 0) {
    return -1;
  }

  printf("%s", cmdline);
  return 0;
}


int main() {
  char cmdline[MAXLINE];

  while (1) {
    printf("> ");
    fgets(cmdline, MAXLINE, stdin);
    if (eval(cmdline)==-1)
      exit(0);
  }
}
