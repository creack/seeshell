NAME := seeshell
SRCS := main.c \
        prompt.c \
        split.c \
        utils.c
# TODO: Add dependency to headers.
OBJS := ${SRCS:.c=.o}

CFLAGS  = -W -Wall -Werror -ansi -pedantic -std=c99
LDFLAGS =
LD      = gcc
CC      = gcc

.DEFAULT_GOAL = ${NAME}

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
