NAME := seeshell
SRCS := main.c \
        prompt.c \
        utils.c
OBJS := ${SRCS:.c=.o}

CFLAGS  = -W -Wall -Werror -ansi -pedantic -std=c99
LDFLAGS =
LD      = gcc
CC      = gcc

${NAME}: ${OBJS}
	${LD} ${OBJS} -o $@ ${LDFLAGS}

.PHONY: clean
clean:
	${RM} ${OBJS}

.PHONY: dist-clean
dist-clean: clean
	${RM} ${NAME}

.PHONY: re
re: dist-clean ${NAME}
