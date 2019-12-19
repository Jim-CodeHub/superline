
#include <superline/superline.hpp>
#include <iostream>

using namespace NS_SUPERLINE;
using namespace std;

int main(void)
{
	class superline_client sc("/home/jim/superline/Makefile", 111);

	string str = "hello consumer";

	sc.send(str.c_str(), str.size());

	return 0;
}
