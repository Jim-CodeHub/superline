
#include <superline/superline.hpp>
#include <iostream>

using namespace NS_SUPERLINE;
using namespace std;

int main(void)
{
	class superline_server ss("/home/jim/Project/superline/Makefile", 111, 10, 1024*1024);

	cout << ss.get_shmid() << "    ";
	cout << ss.get_semrd() << "    ";
	cout << ss.get_semwr() << "    ";
	cout << ss.get_sems1() << "    ";
	cout << ss.get_sems0() << "\r\n";

	char buff[1024*1024];

	int i = 0;
	while (1)
	{
		cout << "consuming..." << i++ <<endl;
		cout << (char *)ss.recv(buff) <<endl;

		sleep(1); /**< Slow consum test */
	}

	ss.free();

	return 0;
}
