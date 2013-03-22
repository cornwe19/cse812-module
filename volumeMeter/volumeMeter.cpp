#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#define NETLINK_NITRO 17
////#define MAX_PAYLOAD 2048

#define MAX_PAYLOAD 1024  /* maximum payload size*/
//struct sockaddr_nl src_addr, dest_addr;
//struct nlmsghdr *nlh = NULL;
//struct msghdr msg;
//struct iovec iov;
//int sock_fd;

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

void ReceiveMessage()
{
	int sock_fd;
	struct sockaddr_nl src_addr, dest_addr;
	struct nlmsghdr *nlh = NULL;
	struct msghdr msg;
	struct iovec iov;

	sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_NITRO);

        memset(&src_addr, 0, sizeof(src_addr));
        src_addr.nl_family = AF_NETLINK;
        src_addr.nl_pid = getpid();  /* self pid */
        /* interested in group 1<<0 */
        src_addr.nl_groups = 1;
        bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));

        memset(&dest_addr, 0, sizeof(dest_addr));

        nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
        memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

        iov.iov_base = (void *)nlh;
        iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);

        memset(&msg, 0, sizeof(msg));
        msg.msg_name = (void *)&dest_addr;
        msg.msg_namelen = sizeof(dest_addr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        printf("Waiting for message from kernel\n");

	 /* Read message from kernel */
        recvmsg(sock_fd, &msg, 0);
        printf(" Received message payload: %s\n", NLMSG_DATA(nlh));
        close(sock_fd);
}

void SendMessage()
{
	struct sockaddr_nl s_nladdr, d_nladdr;
        struct msghdr msg ;
        struct nlmsghdr *nlh=NULL ;
        struct iovec iov;
        int fd=socket(PF_NETLINK ,SOCK_RAW , NETLINK_NITRO );

        /* source address */
        memset(&s_nladdr, 0 ,sizeof(s_nladdr));
        s_nladdr.nl_family= AF_NETLINK ;
        s_nladdr.nl_pad=0;
        s_nladdr.nl_pid = getpid();
        bind(fd, (struct sockaddr*)&s_nladdr, sizeof(s_nladdr));

        /* destination address */
        memset(&d_nladdr, 0, sizeof(d_nladdr));
        d_nladdr.nl_family= AF_NETLINK;
        d_nladdr.nl_pad=0;
        d_nladdr.nl_pid = 0; /* destined to kernel */

        /* Fill the netlink message header */
        nlh = (struct nlmsghdr *)malloc(100);
        memset(nlh , 0, 100);
        strcpy((char*)(NLMSG_DATA(nlh)), " Mr. Kernel, Are you ready ?" );
        nlh->nlmsg_len =100;
	nlh->nlmsg_pid = getpid();
        nlh->nlmsg_flags = 1;
        nlh->nlmsg_type = 0;

        /*iov structure */

        iov.iov_base = (void *)nlh;
        iov.iov_len = nlh->nlmsg_len;

        /* msg */
        memset(&msg,0,sizeof(msg));
        msg.msg_name = (void *) &d_nladdr ;
        msg.msg_namelen=sizeof(d_nladdr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        sendmsg(fd, &msg, 0);
}

int main()
{
	cout << endl;

	SendMessage();
	//ReceiveMessage();

	//sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_NITRO);

 	//memset(&src_addr, 0, sizeof(src_addr));
 	//src_addr.nl_family = AF_NETLINK;
 	//src_addr.nl_pid = getpid();  /* self pid */
 	/* interested in group 1<<0 */
 	//src_addr.nl_groups = 1;
 	//bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));

 	//memset(&dest_addr, 0, sizeof(dest_addr));

 	//nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
 	//memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

 	//iov.iov_base = (void *)nlh;
 	//iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD);
 	
	//memset(&msg, 0, sizeof(msg));
	//msg.msg_name = (void *)&dest_addr;
 	//msg.msg_namelen = sizeof(dest_addr);
 	//msg.msg_iov = &iov;
	//msg.msg_iovlen = 1;

	//printf("Waiting for message from kernel\n");

	 /* Read message from kernel */
	//recvmsg(sock_fd, &msg, 0);
 	//printf(" Received message payload: %s\n", NLMSG_DATA(nlh));
 	//close(sock_fd);

	////////////////////////////////////////////////////////////////////

	//struct sockaddr_nl s_nladdr, d_nladdr;
	//struct msghdr msg ;
	//struct nlmsghdr *nlh=NULL ;
	//struct iovec iov;
	//int fd=socket(AF_NETLINK ,SOCK_RAW , NETLINK_NITRO );

	/* source address */
	//memset(&s_nladdr, 0 ,sizeof(s_nladdr));
	//s_nladdr.nl_family= AF_NETLINK ;
	//s_nladdr.nl_pad=0;
	//s_nladdr.nl_pid = getpid();
	//bind(fd, (struct sockaddr*)&s_nladdr, sizeof(s_nladdr));

	/* destination address */
	//memset(&d_nladdr, 0 ,sizeof(d_nladdr));
	//d_nladdr.nl_family= AF_NETLINK ;
	//d_nladdr.nl_pad=0;
	//d_nladdr.nl_pid = 0; /* destined to kernel */

	/* Fill the netlink message header */
	//nlh = (struct nlmsghdr *)malloc(100);
	//memset(nlh , 0 , 100);
	//strcpy((char*)(NLMSG_DATA(nlh)), " Mr. Kernel, Are you ready ?" );
	//nlh->nlmsg_len =100;
	//nlh->nlmsg_pid = getpid();
	//nlh->nlmsg_flags = 1;
	//nlh->nlmsg_type = 0;

	/*iov structure */

	//iov.iov_base = (void *)nlh;
	//iov.iov_len = nlh->nlmsg_len;

	/* msg */
	//memset(&msg,0,sizeof(msg));
	//msg.msg_name = (void *) &d_nladdr ;
	//msg.msg_namelen=sizeof(d_nladdr);
	//msg.msg_iov = &iov;
	//msg.msg_iovlen = 1;
	//sendmsg(fd, &msg, 0);

	//recvmsg(fd, &msg, 0);
    	//printf(" Received message payload: %s\n",
          // NLMSG_DATA(nlh));
	//close(fd);

	for(int i=0; i<=10; i++)
	{
		int vol = rand() % 101;
		OutputVolume(vol);

		sleep(1);	
	}

	cout << endl << endl;
	return 0;
}
