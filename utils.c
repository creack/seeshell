#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void  xwrite(int fd, void* buf, int len) {
  int n;

  if ((n = write(fd, buf, len)) < 0) {
    perror("write:");
    exit(1);
  }
}

int     my_strlen(char *str) {
  /* Iterate through str bytes until we
     reach a null delimeter (\0). */
  int   charCount;

  charCount = 0;
  /* Iterate through string until we've reached the end. */
  while (str != NULL && *(str + charCount++) != '\0');

  return charCount;
}

void  xputs(void* str) {
  xwrite(STDOUT_FILENO, str, my_strlen(str));
}

/*
 * xmalloc allocates the set size in the heap and adds one for the null termination.
 * Initializes the memory with 0.
 * Exits the process in case of failure.
 */
void*   xmalloc(int size) {
  void* buf;

  if ((buf = malloc(size)) == NULL) {
    perror("malloc:");
    exit(1);
  }
  return memset(buf, 0, size);
}

/*
 * count_char counts the number of instances of c in str.
 */
int   count_char(char* str, char c) {
  int num_char;

  /* Iterate through string until we've reached the end. */
  num_char = 0;
  while (str != NULL && *str != '\0') {
    if (*str == c) {
      num_char++;
    }
    str++;
  }
  return num_char;
}
