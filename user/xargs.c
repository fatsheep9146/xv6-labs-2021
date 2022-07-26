#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void
xargs()
{
  char buf[5000];
  int count = 0;
  char *p;
  while ((count = read(0, buf, sizeof(buf))) != 0){
    printf("read line %s, count %d\n", buf, count);
    for (p = buf;p < buf+count && ; p++) {
      if (*p != '\n')
    }
  }
  // for(p=path+strlen(path); p >= path && *p != '/'; p--)
  //   ;
  // p++;
}

int
main(int argc, char *argv[])
{
  int i;
  printf("argc %d\n", argc);

  for(i = 1; i < argc; i++){
    printf("argv [%d] is %s\n", i, argv[i]);
  }
  xargs();
  exit(0);
}
