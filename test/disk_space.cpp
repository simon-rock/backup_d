#include <stdio.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <error.h>
#define Gsize (1024.00*1024.00*1024.00)
#define Msize (1024.00*1024.00)
#ifndef EXT2_SUPER_MAGIC
#define EXT2_SUPER_MAGIC 0xef53
#endif
int main()
{
        long long blocks,bfree;
        struct statfs fs;
        if(statfs("/",&fs)<0)
        {
                perror("statfs");
                exit(0);
        }
      printf("%x\n",fs.f_type);      /* type of filesystem (see below) */
      printf("%ld\n",fs.f_bsize);    /* optimal transfer block size */
      printf("%ld\n",fs.f_blocks);   /* total data blocks in file system */
      printf("%ld\n",fs.f_bfree);    /* free blocks in fs */
      printf("%ld\n",fs.f_bavail);   /* free blocks avail to non-superuser */
      printf("%ld\n",fs.f_files);    /* total file nodes in file system */
      printf("%ld\n",fs.f_ffree);    /* free file nodes in fs */
      printf("%d\n",fs.f_fsid);     /* file system id */
      printf("%ld\n",fs.f_namelen);  /* maximum length of filenames */
        blocks=fs.f_blocks;
        bfree=fs.f_bavail;
      printf(" %lld\n",blocks);
        if(fs.f_type==EXT2_SUPER_MAGIC)
        {
                //printf("Total size of / is %f G\n",blocks*fs.f_bsize/Gsize);
				printf("Total size of / is %lld G\n",blocks*fs.f_bsize/1024);
                //printf("Free size of / is %f G\n",bfree*fs.f_bsize/Gsize);
				printf("Free size of / is %lld G\n",bfree*fs.f_bsize/1024);
        }
}