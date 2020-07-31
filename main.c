#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>

/*


         a.out  pid: 5

          printf-- > Hello

        int pidA = fork();
           / \
          /   \
       Child  Parent
 getpid():6    getpid():5
     pidA:0    pidA:6
getppid():5    getppid():?

 */

int main() {
  char* buf;
  int n;

  // Read from STDIN.
  if ((buf = malloc(sizeof(char) * 1024)) == NULL) {
    perror("malloc:");
    exit(1);
  }

  while ((n = read(STDIN_FILENO, buf, 1024)) > 0) {
    if ((write(STDOUT_FILENO, buf, n)) < 0) {
      perror("write:");
      exit(1);
    }
  }
  if (n < 0) {
    perror("read:");
    exit(1);
  }
}

/* int main() { */
/*   int pid; */

/*   if ((pid = fork()) == -1) { */
/*     perror("fork:"); */
/*     exit(1); */
/*   } */

/*   if (pid == 0) { */
/*     char* const lsArgs[] = {"/usr/bin/ls", NULL}; */
/*     char* const lsEnv[] = {NULL}; */
/*     if (execve(lsArgs[0], lsArgs, lsEnv) == -1) { */
/*       perror("execve:"); */
/*       exit(1); */
/*     } */
/*   } */

/*   if (wait(&pid) == -1) { */
/*     perror("wait:"); */
/*     exit(1); */
/*   } */

/*   printf("Success!\n"); */
/* } */

/* int main2(int argc, char *argv[]) { */
/*   printf("Hello world! %s (%d)\n", argv[0], argc); */

/*   // We have become the process. */
/*   fork() */

/*   // Exec ls */
/*   char* const lsArgs[] = {"/usr/bin/ls", NULL}; */
/*   char* const lsEnv[] = {NULL}; */

/*   if (execve(lsArgs[0], lsArgs, lsEnv) == -1) { */
/*     perror("execve:"); */
/*     exit(1); */
/*   } */

/*   printf("Success!\n"); */

/*   // Display output to screen. */
/* } */
