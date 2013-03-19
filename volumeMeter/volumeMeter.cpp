#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <linux/netlink.h>

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

	for(int i=0; i<=10; i++)
	{
		int vol = rand() % 101;
		OutputVolume(vol);

		sleep(1);	
	}

	cout << endl << endl;
	return 0;
}
