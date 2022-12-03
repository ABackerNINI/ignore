# IGNORE

Ignore is a simple tool to add pattern to a gitignore file through command line arguments. It works great with filename-auto-completion provided by your shell.
It will search for a .gitignore file in the current directory and all parent directories. If it finds one, it will add the patterns to that file, one per line. If one pattern is already present, it will not be added again.

## Usage:

```bash
ignore [files-or-patterns-to-ignore...]
```

## Examples:

```bash
ignore a.out
ignore main '*.o' '*.a'
```

## Note

Remember to quote your arguments if they contain wildcard characters such as `*` or `?`.
