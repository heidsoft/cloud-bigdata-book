#include "unp.h" 
int main(int argc, char **argv) 
{ 
int sockfd,n; 
struct sockaddr-in servaddr; 
char recvline[MAXLINE +1]; 
sockfd=socket(AF-INET, SOCK-STREAM, 0); 
bzero(&servaddr,sizeof(servaddr)); 
servaddr.sin-family = AF-INET; 
servaddr.sin-port=htons(13); 
inet_pton(AF-INET, argv[1], &servaddr.sin_addr); 
connect(sockfd,(SA*) &servaddr, sizeof(servaddr)); 
n=read(sockfd, recvline, MAXLINE); 
fputs(recvline,stdout); 
exit(0); 
}