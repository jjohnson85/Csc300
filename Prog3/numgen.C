#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main( int argc, char * argv[])
{
	ofstream fout;

	fout.open(argv[1]);

	if(!fout)
	{
		cout << "file failed to open" << endl;
	}

	for( int i = 0; i < 10000000; i++)
	{
		fout << rand() % 2000000000 << endl;	
	}
	
	return 0;
}
