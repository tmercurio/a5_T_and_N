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

    printf("map of %s\n\n", argv[1]);

    blockNo = 1;  // Reading data from the first block

    // move file read position to BSIZE*blockNo from the start of the file
    lseek(fd,BSIZE*blockNo,SEEK_SET);

    //read the block
    res = read(fd,block,BSIZE);
    if (res != BSIZE)
    {
        fprintf(stderr,"Cannot read the entire block %d\n",blockNo);
    }

    int tot_blocks, data_blocks, num_inodes, num_log_blocks, first_log_block;
    int first_inode_block, first_free_map_block;

    tot_blocks = block[4] | (block[5] << 8) | (block[6] << 16) | (block[7] << 24);
    data_blocks = block[8] | (block[9] << 8) | (block[10] << 16) | (block[11] << 24);
    num_inodes = block[12] | (block[13] << 8) | (block[14] << 16) | (block[15] << 24);
    num_log_blocks = block[16] | (block[17] << 8) | (block[18] << 16) | (block[19] << 24);
    first_log_block = block[20] | (block[21] << 8) | (block[22] << 16) | (block[23] << 24);
    first_inode_block = block[24] | (block[25] << 8) | (block[26] << 16) | (block[27] << 24);
    first_free_map_block = block[28] | (block[29] << 8) | (block[30] << 16) | (block[31] << 24);

    printf("B"); // Boot block that is always first
    printf("S"); // Superblock that is always second

    for (i = 2; i < tot_blocks; i++) {
        if (i < first_inode_block)
            printf("L");
        else if (i < first_free_map_block)
            printf("I");
        else if (i < (tot_blocks - data_blocks))
            printf("T");
        else
            printf("D");
    }

    printf("\n");
    // let's print the block data as a hexadecimal "dump" ;)
    for (i = 0; i < 3; i++)
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
