
#include <superline/superline.hpp>
#include <iostream>

using namespace NS_SUPERLINE;
using namespace std;

int main(void)
{
	class superline_client sc("/home/jim/Project/superline/Makefile", 111);

	string str = "hello consumer";

	int i = 0;
	while (1)
	{
		cout << "Producting..." << i++ << endl;
		sc.send(str.c_str(), str.size());

		usleep(5000); /**< Fast producting test */
	}

	return 0;
}
