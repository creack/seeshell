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

