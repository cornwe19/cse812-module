#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

#define NETLINK_USER 31
#define MAX_PAYLOAD 1024 /* maximum payload size*/
struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

void OutputVolume(int volume)
{
	cout << "\rVolume : [";
	
	for(int i=1; i<=50; i++)
	{
		if(volume >= i*2)
		{
			cout << '|';
		}
		else
		{
			cout << ' ';
		}
	}	

	cout << ']';
	
	cout.flush();
}

int main()
{
	cout << endl;

	sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);  
	if(sock_fd<0)  
	    return -1;  

	memset(&src_addr, 0, sizeof(src_addr));  
	src_addr.nl_family = AF_NETLINK;  
	src_addr.nl_pid = getpid();  /* self pid */  
	/* interested in group 1<<0 */  
	bind(sock_fd, (struct sockaddr*)&src_addr,  
	  sizeof(src_addr));  

	memset(&dest_addr, 0, sizeof(dest_addr));  
	memset(&dest_addr, 0, sizeof(dest_addr));  
	dest_addr.nl_family = AF_NETLINK;  
	dest_addr.nl_pid = 0;   /* For Linux Kernel */  
	dest_addr.nl_groups = 0; /* unicast */  

	nlh = (struct nlmsghdr *)malloc(  
	                      NLMSG_SPACE(MAX_PAYLOAD));  
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));  
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);  
	nlh->nlmsg_pid = getpid();  
	nlh->nlmsg_flags = 0;  

	strcpy((char*)NLMSG_DATA(nlh), "Hello");  

	iov.iov_base = (void *)nlh;  
	iov.iov_len = nlh->nlmsg_len;  
	msg.msg_name = (void *)&dest_addr;  
	msg.msg_namelen = sizeof(dest_addr);  
	msg.msg_iov = &iov;  
	msg.msg_iovlen = 1;  

	printf("Sending message to kernel\n");  
	sendmsg(sock_fd,&msg,0);  
	printf("Waiting for message from kernel\n");  

	/* Read message from kernel */  
	recvmsg(sock_fd, &msg, 0);  
	printf(" Received message payload: %s\n",  
	    NLMSG_DATA(nlh));  
	close(sock_fd);  

	//for(int i=0; i<=10; i++)
	//{
	//	int vol = rand() % 101;
	//	OutputVolume(vol);

	//	sleep(1);	
	//}

	cout << endl << endl;
	return 0;
}
