#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include "seeshell.h"

/*
 * fork_exec forks the process and executes the given command in the child.
 */
void      fork_exec(char* const buf) {
  int     pid;
  char**  cmdArgs;
  char*   cmdEnv[] = {NULL};

  if ((pid = fork()) == -1) {
    perror("fork");
    exit(1);
  }

  /* Split line to get command and its arguments. */
  cmdArgs = my_split(buf, ' ');
  if (pid == 0) {
    if (execve(cmdArgs[0], cmdArgs, cmdEnv) == -1) {
      perror("execve");
      exit(1);
    }
  }

  if (wait(&pid) == -1) {
    perror("wait");
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
      perror("read");
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

int           main(int argc, const char* argv[], const char* env[]) {
  int         i;
  char**      path_dirs;
  char*       dir;
  struct stat sb;

  if (argc == 0 || argv == NULL) return 0;
  i = 0;
  while (env != NULL && env[i] != NULL) {
    if (strncmp(env[i], "PATH=", 5) == 0) {
      path_dirs = my_split(env[i]+5, ':');

      free(*path_dirs);
      free(path_dirs);

      i = 0; /* We are going to break here, so we can reuse i safely. */
      while (path_dirs != NULL && path_dirs[i]) {
        /* Length of directory path + length of "/ls" + '\0'. */
        dir = xmalloc(sizeof(char) * (my_strlen(path_dirs[i]) + 3 + 1));
        strcat(dir, path_dirs[i]);
        strcat(dir, "/ls");

        if (lstat(dir, &sb) == 0) {
          printf("execing: %s\n", dir);
          fork_exec(dir);
          free(dir);
          break;
        }
        free(dir);

        i++;
      }
      break;
    }
    i++;
  }

  read_loop(STDIN_FILENO);

  return (0);
}
