#define _DEFAULT_SOURCE
#include <string.h>
#include <stdlib.h>
#include "seeshell.h"

/*
 * my_split splits the given string on each space and returns a list of strings.
 */
char**    my_split(const char* in, char tok) {
  char**  ret;
  char*   str;
  int     len;
  int     i;
  int     j;

  if (in == NULL) {
    return (NULL);
  }

  /* First, we allocate and copy the given string so we have
     full control over it. */
  str = strdup((const char*)in);  /* Same things as xmalloc + strcpy. */

  /* Iterate over the string and replace the token by '\0'
     to effectively create one string per token. */
  j = 0; /* Here, we use j to keep track of how many element we find in the string. */
  i = 0;
  while (str != NULL && str[i] != '\0') {
    if (str[i] == tok) {
      str[i] = '\0';
      j++;
      /* Continue until the last space in this block. */
      while (str[i] == tok) str[i++] = '\0';
    }
    i++;
  }

  /* 1 for the first segment, number of sections (j) + 1 for the null termination. */
  ret = xmalloc(sizeof(char*) * (1 + j + 1));
  i = 0;
  j = 0;
  len = my_strlen(in);
  while (i < len) {
    while (i < len && str[i] == '\0') i++;

    /* We are at the end of the string and '\0', move to the first element of the next string. */
    ret[j++] = str + i;
    while (i < len && str[i] != '\0') i++;
  }
  if (j > 1) {
    ret[j - 1] = NULL;
  }
  return (ret);
}

void my_free_split(char** ptr) {
  if (ptr == NULL) {
    return;
  }
  free(*ptr);
  free(ptr);
}
