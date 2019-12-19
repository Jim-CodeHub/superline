
#include <superline/superline.hpp>
#include <iostream>

using namespace NS_SUPERLINE;
using namespace std;

int main(void)
{
	class superline_server ss("/home/jim/superline/Makefile", 111, 10, 1024*1024);

	char *buff[1024];

	cout << ss.recv(buff) <<endl;

	return 0;
}
