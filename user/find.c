#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = '\0';
  return buf;
}

void
find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  char *filename;
  struct dirent de;
  struct stat st;

  // open the directory
  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  // fetch the stat
  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  // if this path is a file, then compare the name with the name of target file
  case T_FILE:
    // strcmp
    filename = fmtname(path);
    if (strcmp(target, filename) == 0) {
      printf("%s\n", path);
      return;
    }
    break;
  // if this path is a directory, iterate all file in this directory recursively
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (strcmp(de.name, ".") == 0) {
        continue;
      }
      if (strcmp(de.name, "..") == 0) {
        continue;
      }  
      find(buf, target);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{

  if(argc != 3){
    fprintf(2, "the number of input args should be two");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
