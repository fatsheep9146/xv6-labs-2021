#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{

		int p[2];
    int pid; 
    char buf[1024];
    pipe(p);
    if(fork() == 0) {
      pid = getpid();
      read(p[0], buf, sizeof buf);
      fprintf(1, "%d: received ping\n", pid);
      write(p[1], "pong\n", 12);
      close(p[0]);
      close(p[1]);
    } else {
      pid = getpid();
      write(p[1], "ping\n", 12);
      read(p[0], buf, sizeof buf);
      fprintf(1, "%d: received pong\n", pid);
      close(p[0]);
      close(p[1]);
		}

    exit(0);
}
