#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 128
#define MAXARGS 128

void str_replace(char *str, int from, int to) {
  char *pos;
  if ((pos = strchr(str, from)))
    *pos = to;
}

// function to create argc
int parseline(char *buf, char *argv[]) {
  char *delim;
  int bg;
  int argc;

  while ((*buf && (*buf == ' ')))
    buf++;

  argc = 0;
  while ((delim = strchr(buf, ' '))) {
    argv[argc++] = buf;
    *delim = '\0'; // this would mark the eos
    buf = delim + 1;
    while (*buf && (*buf == ' '))
      buf++;
  }

  argv[argc]= NULL;

  if (argc == 0)
    return 1;

  if ((bg = (*argv[argc-1] == '&')) != 0)
    argv[--argc] = NULL;

  return bg;
}

void eval(char *cmdline) {
  char *argv[MAXARGS];
  char buf[MAXLINE];
  int bg;
  pid_t pid;

  if (!strcmp(cmdline, "exit ")) {
    exit(0);
  }

  strcpy(buf, cmdline);
  bg = parseline(buf, argv);

  // int i = 0;
  // while (argv[i]) {
  //   printf("%s\n", argv[i]);
  //   i++;
  // }
  //
}


int main() {
  char cmdline[MAXLINE];

  while (1) {
    printf("> ");
    fgets(cmdline, MAXLINE, stdin);
    str_replace(cmdline, '\n', ' ');
    eval(cmdline);
  }
}
