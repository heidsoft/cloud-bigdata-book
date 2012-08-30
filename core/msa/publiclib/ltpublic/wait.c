#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "wait.h"

int wait_pid(int *wstat,int pid)
{
  int r;

  do
    r = waitpid(pid,wstat,0);
  while ((r == -1) && (errno == -1));
  return r;
}

int wait_nohang(int *wstat)
{

  return waitpid(-1,wstat,WNOHANG);

}
