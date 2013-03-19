#include <iostream>
#include <cstdlib>

using namespace std;

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
