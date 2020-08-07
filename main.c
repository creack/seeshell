#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "seeshell.h"

/*
 * fork_exec forks the process and executes the given command in the child.
 */
void  fork_exec(char* const buf) {
  /* Exec input. */
  int pid;
  char* cmdArgs[] = {NULL, NULL};
  char* const cmdEnv[] = {NULL};

  if ((pid = fork()) == -1) {
    perror("fork:");
    exit(1);
  }

  if (pid == 0) {
    cmdArgs[0] = buf;
    if (execve(cmdArgs[0], cmdArgs, cmdEnv) == -1) {
      perror("execve:");
      exit(1);
    }
  }

  if (wait(&pid) == -1) {
    perror("wait:");
    exit(1);
  }
}

/*
 * read_loop init the memory and start the read loop to exec commands.
 */
void    read_loop(int fd) {
  char* buf;
  int   n;

  buf = xmalloc(sizeof(char) * (1024 + 1));
  while (1) {
    print_prompt();

    /* Read from STDIN. */
    if (((n = read(fd, buf, 1024)) < 0)) {
      perror("read:");
      exit(1);
    }
    if (n == 0) {
      break;
    }
    /* Always strip the trialing char (expected to be \n). */
    buf[n - 1] = '\0';

    fork_exec(buf);
  }
}

int       main() {
  char**  list;

  printf("Hello!\n");
  /* read_loop(STDIN_FILENO); */
  list = my_split("Hello World, how is it going?");
  while (list && *list) {
    printf("Element from the list: '%s'.\n", *list++);
  }

  return (0);
}
