#include <err.h>
#include <malloc.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct path {
  const char *path;
  int watch;
} s_path;

static char   *g_cmd = 0;
static int     g_inotify = -1;
static s_path *g_paths = 0;

int usage ()
{
  fprintf(stderr, "Usage: onchange CMD PATH [PATH1 ...]\n");
  return 1;
}

void setup_inotify ()
{
  uint32_t flags = IN_CREATE | IN_CLOSE_WRITE;
  s_path *p = g_paths;
  if ((g_inotify = inotify_init()) < 0)
    err(1, "inotify_init");
  while (p->path) {
    if ((p->watch = inotify_add_watch(g_inotify, p->path, flags)) < 0)
      err(1, p->path);
    p++;
  }
  p->watch = -1;
}

static int wait_child (pid_t pid)
{
  while (1) {
    int status = 0;
    pid_t w = waitpid(pid, &status, 0);
    if (w < 0)
      err(1, "waitpid");
    if (w > 0 && (WIFEXITED(status) || WIFSIGNALED(status)))
      return 0;
  }
}

int run (char *cmd)
{
  int pid = fork();
  if (pid < 0)
    err(1, "fork");
  else if (pid)
    return wait_child(pid);
  else {
    char *argv[] = { "/bin/sh", "-c", cmd, NULL };
    int i = 3;
    while (i < 1024)
      close(i++);
    execvp(argv[0], argv);
    err(1, argv[0]);
    return -1;
  }
}

int loop_inotify ()
{
  struct inotify_event ev[100];
  ssize_t r;
  while ((r = read(g_inotify, ev, sizeof(ev))) > 0) {
    puts(g_cmd);
    run(g_cmd);
  }
  if (r < 0)
    err(1, "read");
  return 1;
}

int main (int argc, char **argv)
{
  int a = 0;
  int i = 0;
  if (argc < 3)
    return usage();
  a++;
  g_cmd = argv[a++];
  g_paths = malloc((argc - a + 1) * sizeof(s_path));
  while (a < argc && argv[a])
    g_paths[i++].path = argv[a++];
  g_paths[i].path = 0;
  setup_inotify();
  return loop_inotify();
}
