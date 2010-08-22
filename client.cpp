#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "networking.hpp"
#include "bsc_cmd.hpp"

#include <arpa/inet.h>

#include <iostream>
#include <sstream>

#define PORT "3490" 
#define HOST "localhost"

#define MAXDATASIZE 100 // max number of bytes we can get at once 

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXDATASIZE];


	if (tcp_client(&sockfd, HOST, PORT) != 0) {
		fprintf(stderr, "failed to connect to %s:%s\n", HOST, PORT);
		return -1;
	}


    if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
        exit(1);
    }

	stringstream ss(ios::binary | ios::in | ios::out );
	ss.write(buf, numbytes);


	bsc_cmd r, *rr;
	
	rr = r.unserialize(ss);

	cout << "r->class_type: " << rr->get_class_type() << endl;

	if (rr->get_class_type() == BSC_RESERVE_REQ)
	{
		bsc_reserve_req *r_req = (bsc_reserve_req *)rr;
		cout << "r_req->id: " << r_req->id << endl;
	}
	else if (rr->get_class_type() == BSC_RESERVE_RES)
	{
		bsc_reserve_res *r_res = (bsc_reserve_res *)rr;
		cout << "r_res->id: " << r_res->id << endl;
		cout << "r_res->data: " << r_res->data << endl;

	}

    close(sockfd);

    return 0;
}

