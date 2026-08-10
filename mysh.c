#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define MAXLINE 128
#define MAXARGS 128

void str_replace(char *str, int from, int to) {
  char *pos;
  if ((pos = strchr(str, from)))
    *pos = to;
}

void handler(int sig) {
  write(STDOUT_FILENO, "\n> ", 3);
  return;
}

// return 1 if built in, 0 if not
int built_in_cmd(char *argv[]) {
  if (!strcmp(argv[0], "exit"))
    exit(0);
  if (!strcmp(argv[0], "cd")) {
    if (argv[1] && chdir(argv[1])) {
      printf("%s: Invalid path.\n", argv[1]);
    } else if (!(argv[1])) {
      printf("Insufficient arguments.\n");
    }
    return 1;
  }
  return 0;
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

  strcpy(buf, cmdline);
  bg = parseline(buf, argv);

  if (argv[0] == NULL)
    return;

  if (!built_in_cmd(argv)) {
    if ((pid = fork()) == 0) {
      if (execvp(argv[0], argv) < 0) {
        printf("%s: Command not found.\n", argv[0]);
        exit(1);
      }
    } else {
      if (!bg) {
        waitpid(pid, NULL, 0);
      }
    }
  }

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
    if (signal(SIGINT, handler) == SIG_ERR) {
      perror("signal error");
      exit(1);
    }
    printf("> ");
    if(fgets(cmdline, MAXLINE, stdin)==NULL)
      exit(0);
    str_replace(cmdline, '\n', ' ');
    eval(cmdline);
  }
}
