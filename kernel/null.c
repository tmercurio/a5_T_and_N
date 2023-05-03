/* 
    Created by Nikita Volkov (21393323)
    Team: T&N
*/

#include "types.h"
#include "fs.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "file.h"
#include "null.h"

int
null_read(int m, uint64 addr, int nbytes)
{
  return -1;
}

int
null_write(int m, uint64 addr, int nbytes)
{
  return nbytes;
}

void
null_init(void)
{
  devsw[NULL_DEV].write = null_write;
  devsw[NULL_DEV].read = null_read;
}
