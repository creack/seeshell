#define _DEFAULT_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include "seeshell.h"


void      fork_exec(char* cmd, char** args, const char** env);

const char* my_getenv(const char** env, const char* var_name) {
  int       var_name_len;
  int       i;
  char*     full_var_name;

  var_name_len = my_strlen(var_name) + 1; /* +1 for the `=`. */
  full_var_name = xmalloc(sizeof(char) * (var_name_len + 1)); /* +1 for the NULL termination. */
  strcpy(full_var_name, var_name);
  full_var_name[var_name_len - 1] = '=';
  i = 0;
  while (env != NULL && env[i] != NULL) {
    if (strncmp(env[i], full_var_name, var_name_len) == 0) {
      free(full_var_name);
      return (env[i] + var_name_len);
    }
    i++;
  }
  free(full_var_name);
  return (NULL);
}

int           run_cmd(char* cmd, char** args, const char** env) {
  char*       cmd_path;
  char**      path_dirs;
  int         i;
  struct stat sb;

  path_dirs = my_split(my_getenv(env, "PATH"), ':');
  i = 0;
  while (path_dirs != NULL && path_dirs[i] != NULL) {
    /* Length of directory path + '/' + length of cmd + '\0'. */
    cmd_path = xmalloc(sizeof(char) * (my_strlen(path_dirs[i]) + 1 + my_strlen(cmd) + 1));
    strcat(cmd_path, path_dirs[i]);
    strcat(cmd_path, "/");
    strcat(cmd_path, cmd);

    if (lstat(cmd_path, &sb) == 0) {
      fork_exec(cmd_path, args, env);
      free(cmd_path);
      my_free_split(path_dirs);
      return (0);
    }

    free(cmd_path);
    i++;
  }
  my_free_split(path_dirs);
  return (-1);
}

/*
 * fork_exec forks the process and executes the given command in the child.
 */
void      fork_exec(char* cmd, char** args, const char** env) {
  int     pids[2];
  int     pipefd[2];
  char*   cargs[] = {NULL, NULL};

  /* Populate the array. */
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if ((pids[0] = fork()) == -1) {
    perror("fork");
    exit(1);
  }

  if (pids[0] == 0) {
    close(pipefd[1]);          /* Close unused write end */

    if (dup2(pipefd[0], 0) == -1) {
      perror("dup2");
      exit(1);
    }

    cargs[0] = "/usr/bin/wc"; /* (char*)argv[2]; */
    if (execve(cargs[0], cargs, (char*const*)env) == -1) {
      perror("execve");
      exit(1);
    }
  } else {            /* Parent writes argv[1] to pipe */
    close(pipefd[0]);          /* Close unused read end */

    if ((pids[1] = fork()) == -1) {
      perror("fork");
      exit(1);
    }

    if (pids[1] == 0) {
      if (dup2(pipefd[1], 1) == -1) {
        perror("dup2");
        exit(1);
      }

      if (execve(cmd, args, (char*const*)env) == -1) {
        perror("execve");
        exit(1);
      }
    } else {
      close(pipefd[1]);          /* Reader will see EOF */

      /* wait until the child process is dead (exited). */
      if (wait(&pids[1]) == -1) {
        perror("wait");
        exit(1);
      }
    }
  }

  /* wait until the child process is dead (exited). */
  if (wait(&pids[0]) == -1) {
    perror("wait");
    exit(1);
  }
}

/*
 * read_loop init the memory and start the read loop to exec commands.
 */
void      read_loop(int fd, const char** env) {
  char*   buf;
  int     n;
  char**  cmd_args;

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

    if (n == 1 && buf[0] == '\0') {
      continue;
    }

    /* Split line to get command and its arguments. */
    if ((cmd_args = my_split(buf, ' ')) == NULL) {
      continue;
    }
    if (run_cmd(cmd_args[0], cmd_args, env) == -1) {
      fprintf(stderr, "seeshell: command not found: %s\n", cmd_args[0]);
    }
    my_free_split(cmd_args);
  }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) const char* argv[], __attribute__((unused))  const char* env[]) {

  read_loop(STDIN_FILENO, env);
  return (0);
}
