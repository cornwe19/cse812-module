#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netlink/netlink.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

using namespace std;

#define MY_MSG_TYPE (0x10 + 2)

int main()
{
	cout << endl;

	struct nl_sock *nls;
    char msg[] = { 0xde, 0xad, 0xbe, 0xef, 0x90, 0x0d, 0xbe, 0xef };
    int ret;

    nls = nl_socket_alloc();
    if (!nls) {
        printf("bad nl_socket_alloc\n");
        return EXIT_FAILURE;
    }

    ret = nl_connect(nls, NETLINK_USERSOCK);
    if (ret < 0) {
        nl_perror(ret, "nl_connect");
        nl_socket_free(nls);
        return EXIT_FAILURE;
    }

    ret = nl_send_simple(nls, MY_MSG_TYPE, 0, msg, sizeof(msg));
    if (ret < 0) {
        nl_perror(ret, "nl_send_simple");
        nl_close(nls);
        nl_socket_free(nls);
        return EXIT_FAILURE;
    } else {
        printf("sent %d bytes\n", ret);
    }

    nl_close(nls);
    nl_socket_free(nls);

	//for(int i=0; i<=10; i++)
	//{
	//	int vol = rand() % 101;
	//	OutputVolume(vol);

	//	sleep(1);	
	//}

	cout << endl << endl;
	return 0;
}
