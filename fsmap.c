#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BSIZE (1024)

int
main(int argc, char *argv[])
{
    int fd;
    unsigned char block[BSIZE];
    int res;
    int blockNo;
    int i;
    int j;
    char c;

    if (argc < 2)
    {
	printf("Usage: fsmap path-to-fs.img\n");
	exit(0);
    }

    fd = open(argv[1],O_RDONLY);
    if (fd == -1)
    {
	    fprintf(stderr,"Cannot open %s\n",argv[1]);
	    exit(-1);
    }

    printf("map of %s\n", argv[1]);

    blockNo = 1;  // Reading data from the first block

    // move file read position to BSIZE*blockNo from the start of the file
    lseek(fd,BSIZE*blockNo,SEEK_SET);

    //read the block
    res = read(fd,block,BSIZE);
    if (res != BSIZE)
    {
        fprintf(stderr,"Cannot read the entire block %d\n",blockNo);
    }

    // let's print the block data as a hexadecimal "dump" ;)
    for (i = 0; i < 64; i++)
    {
    	// print 16 bytes of data in decimal
    	for (j = 0; j < 16; j++)
    	{
    	    printf("%x ", block[i * 16 + j]);

    	    // print a separating ' ' after the first 8 bytes
    	    if (j == 7) putchar(' ');
    	}

    	// print a few whitespaces separating hex from char section
    	printf(" ");

    	//print16 bytes of data as characters replacing unprintable chars with '.'
    	for (j = 0; j<16; j++)
    	{
    	   c = ((block[i*16+j] >= ' ') && (block[i*16+j] <= '~')) ? block[i*16+j] : '.';
    	   putchar(c);
    	}
    	putchar('\n');
    }
    exit(0);
    //never gets here
}
