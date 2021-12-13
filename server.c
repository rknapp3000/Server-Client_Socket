/* Server code */

/* Author Ryan Knapp
 
** Assignment 6
*/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define LISTEN_PORT 8080
int main(int argc, char *argv[])
{
int sock_listen, sock_recv;
int i, addr_size, bytes_received;
int incoming_len;
int recv_msg_size;
struct sockaddr_in my_addr, recv_addr;
int select_ret;
fd_set readfds;
struct timeval timeout={0,0};
struct sockaddr remote_addr;
char buf[BUF_SIZE];
    
/* create socket for listening */
sock_listen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
if (sock_listen < 0)
{
printf("socket() failed\n");
exit(0);
}
/* builds the local address structure */
memset(&my_addr, 0, sizeof(my_addr));
my_addr.sin_family = AF_INET;
my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
my_addr.sin_port = htons((unsigned short)LISTEN_PORT);

/* binds the socket to the local ip address */
i = bind(sock_listen, (struct sockaddr *) &my_addr,
sizeof(my_addr));
if (i < 0)
{
printf("bind() failed\n");
exit(0);
}
/* assigns listener to variable so that we can perform error handling */
i = listen(sock_listen, 5);
if (i < 0)
{
printf("listen() failed\n");
exit(0);
}

int pid; // declaring process identifier before the loop
while (1)
{
    // set up a new socket to receieve data on
      addr_size = sizeof(recv_addr);
    
    //FIXME if not working moved the below line within the while loop
      sock_recv = accept(sock_listen, (struct sockaddr*)&recv_addr,&addr_size);
    
    if (sock_recv == -1)
            return 1;
    
    // close the listener on the socket if creating the child process fails
    if ((pid = fork()) == 0) {
           close(sock_listen);

      while (1) {
          bytes_received = recv(sock_recv, buf, BUF_SIZE, 0);
          
          // if there are no bytes received, client has disconnected
          if (bytes_received == 0) {
                  printf("A client has disconnected from the server\n");
                  close(sock_recv);
                  break;
          }
        
          //Close the socket if the condition below equals 10
          else if (strcmp(buf, "shutdown program") == 10) {
                    printf("A client has disconnected from the server\n");
                    close(sock_recv);
                    break;
                }
         else {
              buf[bytes_received] = 0;
              printf("Message received from client: %s\n", buf); // displays message on server
            }
        }
    }
}
    
close(sock_recv);
close(sock_listen);

}
