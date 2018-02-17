onchange
========

onchange is a Linux command line tool to watch multiple files and
directories for change and run a command each time changes happen to
watched files and directories.

It allows for asynchronous and distributed implementation of most make
commands.

Usage
-----

```
    onchange CMD FILE [FILE1 ...]
```

Runs CMD using `/bin/sh -c` each time the given FILEs change according
to inotify under Linux. Other systems are currently unsupported however
portable patches are welcome.

Examples
--------

```
    onchange pdflatex *.tex
    onchange "cc -W -Wall -Werror -O0 -ggdb -o test test.c" test.c
```

I am a programmer, so I mostly work on programs in their source code
form but what I really want is the output of a program generated from
that source by another program. `onchange` allows me to re-generate
the target program every time the source code changes. It really works
great with me, you should try it. Really. Try it. Now. Oh come on what
is it now ?

Future
------

Support command substitution

Either with syntax :
 * `%n` file name without directory and extension
 * `%f` file path with directory and extension

```
    # watch and rebuild changed .c files
    onchange "cc -o %n.o %f" *.c
```

Or run command with environment variable set :
 * ONCHANGE_FILES

```
    onchange "make $ONCHANGE_FILES" *.c
```

Or both ?
