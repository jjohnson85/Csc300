


#include<iostream>
#include<fstream>
#include<time.h>
#include<algorithm>

using namespace std;

void radixSort( int *arry, int size );
void bubbleSort( int *arry, int size );
void allocate( int* arry, int size );
void cpyArry( int *arryOne, int *arryTwo, int size );
void insert( int* arry, int size);

/*****************************************************************************
 *Function: main
 *Author: Jared Johnson
 *Description:
 *Parameters:
 * ***************************************************************************/
int main( int argc, char *argv[] )
{
int *arryOne;
int *arryTwo;
int arryThree[1000];
time_t timer;
int val;
int i = 1;
int counter = 0;
fstream inp;

//Open the first argument for input
inp.open(argv[1], ios::in);

if( !inp.is_open( ) )
{
	cout << "Error opening input file" << endl;
	return -1;
}

//determine the size needed for the arrays
while(inp >> val)
{
	counter++;
}
inp.close();
inp.open(argv[1], ios::in);
cout << counter << endl;
cout << inp.tellg( ) << endl;

//allcate two arrays of the appropriate size
arryOne = new int[counter+1];
arryTwo = new int[counter+1];
//allocate(arryThree, counter + 1);


//insert values from files into both arrays
while( inp >> val)
{

	arryOne[i] = val;
	arryTwo[i] = val;
//	arryThree[i] = val;

	i++;
}
inp.close();

//DEBUG
for(i=0; i< 1000; i++)
{
	arryThree[i] = 1000-i;
}

//sort list One with bubble sort
timer = 0;
bubbleSort(arryOne, counter);
cout << "Timer for Bubble Sort: " << timer << endl;

//sort list Two with radix sort
timer = 0;
radixSort(arryTwo, counter);
cout << "Time for Radix Sort: " << timer << endl;

//sort list Three with std sort
timer = 0;
sort(arryThree, arryThree+1000);
cout << "Timer for STL sort: " << timer << endl;

//End Program
delete [] arryOne;
delete [] arryTwo;
return 0;
}

/*****************************************************************************
 *Function: allocate
 *Author: Jared Johnson
 *Description: Allocates a new array of the desired size, determined by the
size parmeter.
 *Parameters: <in/out> int *arry - integer pointer to the start of the array
              <in> int size - size to make the array
 * ***************************************************************************/
void allocate( int *arry, int size )
{

arry = new int[size];


}

/*****************************************************************************
 *Function: cypArry
 *Author: Jared Johnson
 *Description: Function to copy array one into array two
 *Parameters: <in/out> - int *arryOne - integer pointer to the start of the
array to copy
              <in/out> - int *arryTwo - integer pointer to the start of the 
array to be copied into
 *****************************************************************************/
void cpyArry( int *arryOne, int *arryTwo, int size )
{


}

/*****************************************************************************
 *Function: radixSort
 *Author: 
 *Description:
 *Parameters:
 * ***************************************************************************/
void radixSort( int *arry, int size )
{




}

/*****************************************************************************
 *Function: bubbleSort
 *Author:
 *Description:
 *Parameters:
 * ***************************************************************************/
void bubbleSort( int *arry, int size )
{





}


