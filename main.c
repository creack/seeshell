#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "seeshell.h"


/*
in: "Hello World! How is it going?"

// strdup

str: "Hello World! How is it going?"

// First iteration: replace space with \0.

str: "Hello\0World!\0How\0is\0it\0going?".

ret = malloc(1 + 5 + 1);

*str == str[0] == 'H'.

"string" char* ---> "hello"
* char* -> char -> h

ret char** -> List of string. == list of list of char.

ret

[] char*| str
[] char*|  str
[] str
[] str
[]

 */

/*
 * my_split splits the given string on each space and returns a list of strings.
 */
char**    my_split(char* in) {
  char**  ret;
  char*   str;
  int     len;
  int     i;
  int     j;
  int     num_space;

  len = my_strlen(in);
  str = strdup((const char*)in); // Same things as xmalloc + strcpy.
  num_space = 0;
  i = 0;
  while (str && str[i]) {
    if (str[i] == ' ') {
      str[i] = '\0';
      num_space++;
      /* Continue until the last space in this block. */
      while (str[i] == ' ') str[i++] = '\0';
    }
    i++;
  }

  ret = xmalloc(sizeof(char*) * (1 + num_space + 1));
  i = 0;
  j = 0;
  while (i < len) {
    while (i < len && str[i] == '\0') i++;

    if (j >= (num_space + 1)) break;
    ret[j++] = str + i;
    while (str[i] != '\0' && i < len) i++;
  }
  ret[j] = NULL;
  return ret;
}

/*
 * fork_exec forks the process and executes the given command in the child.
 */
void  fork_exec(char* const buf) {
  /* Exec input. */
  int pid;

  if ((pid = fork()) == -1) {
    perror("fork:");
    exit(1);
  }

  if (pid == 0) {
    char* const cmdArgs[] = {buf, NULL};
    char* const cmdEnv[] = {NULL};
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

    // Read from STDIN.
    if (((n = read(fd, buf, 1024)) < 0)) {
      perror("read:");
      exit(1);
    }
    if (n == 0) {
      break;
    }
    // Always strip the trialing char (expected to be \n).
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
}
