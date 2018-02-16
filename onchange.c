#include <stdio.h>

int usage (char *argv0)
{
  fprintf(stderr, "Usage: %s CMD PATH [PATH1 ...]", argv0);
  return 1;
}

int main (int argc, char **argv)
{
  if (argc < 3)
    return usage(argv[0]);

  return 0;
}
