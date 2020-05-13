#include <stdio.h>
#include <sys/stat.h>

int main ( void )
{
    struct stat file_stat;

    printf ("__WORDSIZE: %d\n",__WORDSIZE);
    printf ("__USE_MISC: %d\n",__USE_MISC);
    printf ("__USE_XOPEN2K8: %d\n",__USE_XOPEN2K8);

    printf ("file_stat len: %ld\n", sizeof file_stat);

    long p =  (long)(&file_stat);

    printf ("file_stat.st_dev          pos: %3ld   len: %2ld\n", (long)(&file_stat.st_dev) - p,           sizeof file_stat.st_dev);
    printf ("file_stat.st_ino          pos: %3ld   len: %2ld\n", (long)(&file_stat.st_ino) - p,           sizeof file_stat.st_ino);
    printf ("file_stat.st_mode         pos: %3ld   len: %2ld\n", (long)(&file_stat.st_mode) - p,          sizeof file_stat.st_mode);
    printf ("file_stat.st_nlink        pos: %3ld   len: %2ld\n", (long)(&file_stat.st_nlink) - p,         sizeof file_stat.st_nlink);
    printf ("file_stat.st_uid          pos: %3ld   len: %2ld\n", (long)(&file_stat.st_uid) - p,           sizeof file_stat.st_uid);
    printf ("file_stat.st_gid          pos: %3ld   len: %2ld\n", (long)(&file_stat.st_gid) - p,           sizeof file_stat.st_gid);
    printf ("file_stat.st_rdev         pos: %3ld   len: %2ld\n", (long)(&file_stat.st_rdev) - p,          sizeof file_stat.st_rdev);
    printf ("file_stat.st_size         pos: %3ld   len: %2ld\n", (long)(&file_stat.st_size) - p,          sizeof file_stat.st_size);
    printf ("file_stat.st_blksize      pos: %3ld   len: %2ld\n", (long)(&file_stat.st_blksize) - p,       sizeof file_stat.st_blksize);
    printf ("file_stat.st_blocks       pos: %3ld   len: %2ld\n", (long)(&file_stat.st_blocks) - p,        sizeof file_stat.st_blocks);
    printf ("file_stat.st_atim  pos: %3ld   len: %2ld\n", (long)(&file_stat.st_atim) - p,   sizeof file_stat.st_atim.tv_sec);
    printf ("file_stat.st_atim.tv_nsec pos: %3ld   len: %2ld\n", (long)(&file_stat.st_atim.tv_nsec) - p,  sizeof file_stat.st_atim.tv_nsec);
    printf ("file_stat.st_mtim  pos: %3ld   len: %2ld\n", (long)(&file_stat.st_mtim) - p,   sizeof file_stat.st_mtim.tv_sec);
    printf ("file_stat.st_mtim.tv_nsec pos: %3ld   len: %2ld\n", (long)(&file_stat.st_mtim.tv_nsec) - p,  sizeof file_stat.st_mtim.tv_nsec);
    printf ("file_stat.st_ctim  pos: %3ld   len: %2ld\n", (long)(&file_stat.st_ctim) - p,   sizeof file_stat.st_ctim.tv_sec);
    printf ("file_stat.st_ctim.tv_nsec pos: %3ld   len: %2ld\n", (long)(&file_stat.st_ctim.tv_nsec) - p,  sizeof file_stat.st_ctim.tv_nsec);

    return 0;
}
