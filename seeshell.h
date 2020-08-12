#ifndef __SEESHELL_H__
# define __SEESHELL_H__

void print_prompt();

/* Utils. */
void    xputs(void* str);
void*   xmalloc(int size);
int     my_strlen(const char* str);
char**  my_split(const char* str, char tok);
void    my_free_split(char** ptr);

#endif /* !__SEESHELL_H__ */
