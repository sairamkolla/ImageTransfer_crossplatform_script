#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


using namespace cv;
using namespace std;

int sockfd, newsockfd, portno;

void error(const char *msg){
    perror(msg);
    exit(1);
}

void setup_server(int argc,char** argv){
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    


}
int main( int argc, char** argv ){

    setup_server(argc,argv);
    char buffer[256];

    bzero(buffer,256);
    int n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Message from client: %s\n",buffer);
        
    Mat image;
    image = imread("a.jpg",CV_LOAD_IMAGE_COLOR);   // Read the file
    image = (image.reshape(0,1));
    int  imgSize = image.total()*image.elemSize();

    n = send(newsockfd,image.data,imgSize, 0);
    n = write(newsockfd,"EOM",3);
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    printf("Message from client: %s\n",buffer);
    close(sockfd);
    close(newsockfd);
    return 0;
}