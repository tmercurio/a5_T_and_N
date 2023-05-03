/* 
    Created by Nikita Volkov (21393323)
    Team: T&N
*/

#ifndef NULL_H
#define NULL_H

int null_read(int m, uint64 addr, int nbytes);
int null_write(int m, uint64 addr, int nbytes);
void null_init(void);

#endif
