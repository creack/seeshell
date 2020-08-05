#ifndef __SEESHELL_H__
# define __SEESHELL_H__

void print_prompt();

/* Utils. */
void xputs(void* str);
void *xmalloc(int size);
int  my_strlen(char* str);
int  count_char(char* str, char c);

#endif /* !__SEESHELL_H__ */
