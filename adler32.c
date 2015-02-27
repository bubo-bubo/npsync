#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <zlib.h>


#define BUF_SIZE (1<<20)

/*
  IO performance is the bottleneck, no super-optimization is required
  gcc -O6 -Wall adler32.c -o adler32 -lz
*/

int
main( int argc, char *argv[] )
{
  int fd=-1;
  u_char buf[BUF_SIZE];
  ssize_t size=0;
  u_int32_t adler = adler32( 0L, Z_NULL, 0 );
  if ( argc == 2 ) {
    if ( (fd=open( argv[1], O_RDONLY )) == -1 ) {
      long e=errno;
      perror( argv[1] );
      exit( e );
    }
  } else if ( argc == 1 ) {
    fd = STDIN_FILENO; /* defined in unistd.h */
  } else {
    fprintf( stderr, "Usage: %s FILE_NAME\n", argv[0] );
    exit( 1 );
  }
  while ( (size=read( fd, buf, BUF_SIZE )) > 0 ) {
    adler = adler32( adler, buf, size );
  }
  if ( size < 0 ) {
    long e=errno;
    perror( argv[1] );
    exit( e );
  } else {
    printf( "%x\n", adler );
  }
  close( fd );
  return 0;
}
