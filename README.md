# Seeshell


## Fork flow.

```
        a.out  pid: 5

      printf-- > Hello

        int pidA = fork();
           / \
          /   \
       Child  Parent
 getpid():6    getpid():5
     pidA:0    pidA:6
getppid():5    getppid():?
```

## Steps

1. Exec/fork (7-29)
2. Read line (7-29)
3. Split string (8-4)
4. Use split string for parsing command line and isolate PATH (8-6, 8-11)
5. Break out fork and introduce builtin (e.g. `exit`, `cd`, `echo`, `pwd`, `export`)

## Other topics

1. More advanced parsing
2. Pipelines (using `|`)
3. Job control
4. Terminal management
5. Line processing

## For next stream:

1. Write builtins.
