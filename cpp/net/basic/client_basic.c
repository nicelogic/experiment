
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
  int sockfd;
  int len;
  struct sockaddr_in address;
  int res;
  char ch = 'A';

  sockfd = socket( AF_INET, SOCK_STREAM, 0 );

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr( "94.191.64.97" );
  address.sin_port = htons( 9734 );

  len = sizeof( address );
  res = connect( sockfd, ( struct sockaddr * )&address, len );
  if( res == -1 ){
    perror( "oops: connect failure\n" );
    exit ( EXIT_FAILURE );
  }

  write( sockfd, &ch, 1 );
  read( sockfd, &ch, 1 );
  printf( "read from server: %c\n", ch );

  close( sockfd );

  exit( EXIT_SUCCESS );
}
