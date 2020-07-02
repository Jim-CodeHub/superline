
#include <superline/superline.hpp>
#include <iostream>

using namespace NS_SUPERLINE;
using namespace std;

int main(void)
{
	class superline_client sc("/home/jim/Project/superline/Makefile", 111);

	cout << sc.get_shmid() << "    ";
	cout << sc.get_semrd() << "    ";
	cout << sc.get_semwr() << "    ";
	cout << sc.get_sems1() << "    ";
	cout << sc.get_sems0() << "\r\n";

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
