# myshell

Implementation of a basic shell

## How to run

To try the shell, run:

```bash
make
./mysh
```

## Capabilities

1. Basic command execution
2. Commands with pipes
3. Redirections ` < > >>`

3 modes of execution:
1. with option -c
```bash
./mysh -c 'COMMAND'
```
2. non-interactive mode via a file as the first parameter
```bash
./mysh script.sh
```

3. interactive mode via standard input
```bash
./mysh
mysh$ _
```
