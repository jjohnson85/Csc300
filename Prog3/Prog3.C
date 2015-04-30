/*****************************************************************************
 *Program: Program 3
 *Author: Jared Johnson and Christian Sieh
 *Class: CSC 300, Data Structures
 *Instructor: Dr. Edward Corwin
 *Date: 5/1/2015
 *Description: This program takes in a text file consisting of interger values
		it then reads all of these values into an array and creates
		3 copies of that array. A Bubble Sort, Radix Sort, and
		Heapsort is then run on each of these different arrays. 
 *Input: Integers from a text file ex: numbers.txt
 *Output: 
 *Usage:
 *Known Bugs:
 * ***************************************************************************/

#include<iostream>
#include<fstream>
#include<time.h>
#include<algorithm>
#include<queue>
#include<cmath>
#include<iterator>
#include<vector>

using namespace std;

void radixSort( int *arry, int size );
void bubbleSort( int *arry, int size );
void heapsort( int *arry, int size );
void siftdown( int *arry, int i, int n);

/*****************************************************************************
 *Function: main
 *Author: Jared Johnson
 *Description: main function calls all other functions and times each one.
main also handles output to console and opens and closes files using command
line arguments.
 *Parameters: <in> int argc - command line argument count
              <in> char *argv[] - comand line argument string
 * ***************************************************************************/
int main( int argc, char *argv[] )
{
int *arryOne;
int *arryTwo;
int *arryThree;
int *arryFour;
clock_t timer;
int val;
int i = 1;
int counter = 0;
fstream inp;
cout.setf(ios::fixed, ios::floatfield);
cout.setf(ios::showpoint);

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

//Reopen file (until I can figure out why I cant seem to seek to the beggining)
inp.open(argv[1], ios::in);

cout << "Number of values: " << counter << endl;
cout << inp.tellg( ) << endl;

//allcate arrays of the appropriate size
arryOne = new int[counter+1];
arryTwo = new int[counter+1];
arryThree = new int[counter+1];
arryFour = new int[counter+1];

//insert values from files into all arrays
while( inp >> val)
{

	arryOne[i] = val;
	arryTwo[i] = val;
	arryThree[i] = val;
	arryFour[i] = val;

	i++;
}
inp.close();


//sort list One with bubble sort
timer = clock();
bubbleSort(arryOne, counter);
timer = clock() - timer;
cout << "Timer for Bubble Sort: " << ((float)timer)/CLOCKS_PER_SEC << endl;

//sort list Two with radix sort
timer = clock();
radixSort(arryTwo, counter+1);
timer = clock() - timer;
cout << "Time for Radix Sort: " << ((float)timer)/CLOCKS_PER_SEC << endl;

//sort list Three with std sort
timer = clock();
sort(arryThree, arryThree+counter+1);
timer = clock() - timer;
cout << "Timer for STL sort: " << ((float)timer)/CLOCKS_PER_SEC << endl;

//sort list Four with heapsort
timer = clock();
heapsort(arryFour, counter+1);
timer = clock() - timer;
cout << "Time for Heapsort: " << ((float)timer)/CLOCKS_PER_SEC << endl;

//DEBUG TO CHECK ORDER

for( i = 0; i<=10; i++)
{

cout << arryFour[i] << "\t" << arryThree[i] << "\t" << arryTwo[i] << "\t" << arryOne[i] << endl;
}


//End Program
delete [] arryOne;
delete [] arryTwo;
delete [] arryThree;
delete [] arryFour;
return 0;
}

/*****************************************************************************
 *Function: radixSort
 *Author: Jared Johnson
 *Description: radixSort will sort a list of integers found in the passed array
by determining a value at each place in the base 256 number, and inserting it
into a queue. After each pass, it will pop the values from the queue, sorting
the digits that pass. 
 *Parameters: <in/out> int *arry - array of values to sort
              <in> int size - size of the array to sort
 * ***************************************************************************/
void radixSort( int *arry, int size )
{
vector<queue<int> > radixList;
int loc;
int j = 0;
int i = 0;
int step = 0;
int maxDigits = 0;
int max = 0;
long int stepFactor = 1;

//Create a 256 queue vectors
radixList.resize(256);

while( step <= maxDigits || maxDigits == 0 )
{
	j = 0;
	//insert values in arry into their respective lists for radix sort
	for( i = 0; i < size; i++)
	{
		//calculate which list the current number in the array goes into
		loc =(int)(arry[i]/stepFactor) % 256;
		radixList[loc].push(arry[i]);
	//	cout << "Pushing: " << arry[i] << " To: "<< loc << endl;
		
		//On the first pass only
		if( step == 1 )
		{
			//determine largest value
			if( max < arry[i] )
			{
				max = arry[i];
			}	
		}
		
	}
	
	
	//After the first pass only
	if( step == 1 )
	{
		//Calculate the max number of base 256 digits
		while( max != 0 )
		{
			max /= 256;
			maxDigits++;
		}
	}
	
	
	//Increment Step
	step++;
	stepFactor *= 256;
	//write values back into list
	for( i = 0; i < 256; i++ )
	{
			
		for( j ; radixList[i].size( ) != 0; j++ )
		{
			arry[j] = radixList[i].front( );
	//		cout << "Writing from List: "<< i << " : "
	//		 << radixList[i].front() <<
	//		" Over: " << arry[j] << endl;
			radixList[i].pop( );
		}
	}
}	
}

/*****************************************************************************
 *Function: bubbleSort
 *Author: Jared Johnson
 *Description: sorts an array of integer values using the bubble sort algorithm
loops though all values in the array and checks if values need to be swapped.
Does this n times to sort the list. Quits if time taken is greater than 10 
seconds.
 *Parameters: <in/out>  int *arry - array to sort
              <in> int size - size of the array
 * ***************************************************************************/
void bubbleSort( int *arry, int size )
{
int i = 0;
int n = 0;
int temp = 0;
time_t start;
time_t end;

//Set start time and begin looping through array
start = time(NULL);
while( n != size )
{
	//calculate time at end of each pass
	//if the time taken is greater than 10
	//cout and return
	end = time(NULL);
	if( end - start > 10 )
	{
		cout << "Bubble Sort time greater than 10s " << endl;
		return;
	}
	//move through array
	for( i=0; i < size; i++ )
	{
		//Check adjacent value and swap if needed
		if(arry[i] > arry[i+1])
		{
			temp = arry[i];
			arry[i] = arry[i+1];
			arry[i+1] = temp;
		}		

	}
	//increment number of passes
	n++;
}
}

/*****************************************************************************
 *Function: Heapsort
 *Author: Christian Sieh
 *Description: This function will do a heapsort on the given array of ints
 *Parameters: 
	     int *arry - An array that holds the list of numbers to be sorted
	     int size - The size of array 'arry'
 * ***************************************************************************/
void heapsort( int *arry, int size)
{
	//Build Heap
	for( int i = size / 2 - 1; i >= 0; --i)
		siftdown(arry, i, size);
	//Delete Max
	for( int j = size - 1; j > 0; --j)
	{		
		swap( arry[0], arry[j] );
		siftdown( arry, 0, j);
	}
}

//Simple inline function used in siftdown
inline int leftChild( int i )
{
	return 2 * i + 1;
}


/*****************************************************************************
 *Function: Siftdown
 *Author: Christian Sieh
 *Description: This function fixes the heap after we move a value. It starts
		at i and uses that as the root. The function then repairs
		down from there.
 *Parameters:
	     int *arry - The array that holds all the numbers to be sorted
	     int i - The iterator from the function heapsort. Used as root
			of tree that needs to be repaired
	     int size - The size of the array
 * ***************************************************************************/
void siftdown(int *arry, int i, int size)
{
	int child;
	int temp;
	
	//
	for(temp = arry[i]; leftChild(i) < size; i = child)
	{
		child = leftChild(i);
		if( child != size-1 && arry[child] < arry[child+1])
			++child;
		if(temp < arry[child])
			arry[i] = arry[child];
		else
			break;
	}
	arry[i] = temp;
}
