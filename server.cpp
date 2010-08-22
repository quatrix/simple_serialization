#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "networking.hpp"
#include "bsc_cmd.hpp"


#include <iostream>
#include <sstream>

using namespace std;

#define PORT "3490"  // the port users will be connecting to


void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}


int main(void)
{
	int sockfd, new_fd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

    printf("server: waiting for connections...\n");

	if (tcp_server(&sockfd, PORT) != 0) {
		fprintf(stderr, "error starting tcp_server\n");
		return -1;
	}

    while(1) {  
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        if (new_fd == -1) {
            perror("accept");
            continue;
        }


/*
		bsc_reserve_req r;
		r.id = 15;
*/
		bsc_reserve_res r;
		r.id	= 666;
		r.data 	= 1234;		

		stringstream ss(ios::binary | ios::out);

		r.serialize(ss);

		if (send(new_fd, ss.str().c_str(), ss.str().size(), 0) == -1)
			perror("send");


		close(new_fd);
    }

    return 0;
}

