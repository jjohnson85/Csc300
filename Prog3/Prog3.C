


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
 *Description:
 *Parameters:
 * ***************************************************************************/
int main( int argc, char *argv[] )
{
int *arryOne;
int *arryTwo;
int *arryThree;
int *arryFour;
time_t timerStart;
time_t timerEnd;
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

//Reopen file (until I can figure out why I cant seem to seek to the beggining)
inp.open(argv[1], ios::in);

cout << counter << endl;
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
timerStart = time(NULL);
bubbleSort(arryOne, counter);
timerEnd = time(NULL);
cout << "Timer for Bubble Sort: " << timerEnd - timerStart << endl;

//sort list Two with radix sort
timerStart = time(NULL);
radixSort(arryTwo, counter+1);
timerEnd = time(NULL);
cout << "Time for Radix Sort: " << timerEnd - timerStart << endl;

//sort list Three with std sort
timerStart = time(NULL);
sort(arryThree, arryThree+counter+1);
timerEnd = time(NULL);
cout << "Timer for STL sort: " << timerEnd - timerStart << endl;

//sort list Four with heapsort
timerStart = time(NULL);
heapsort(arryFour, counter+1);
timerEnd = time(NULL);
cout << "Time for Heapsort: " << timerEnd - timerStart << endl;

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
 *Author: 
 *Description:
 *Parameters:
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

radixList.resize(256);

while( step <= 4 )
{
	j = 0;
	//insert values in arry into their respective lists for radix sort
	for( i = 0; i < size; i++)
	{
		//calculate which list the current number in the array goes into
		loc =(int)(arry[i]/stepFactor) % 256;
		radixList[loc].push(arry[i]);
	//	cout << "Pushing: " << arry[i] << " To: "<< loc << endl;
		/*
		//On the first pass only
		if( step == 1 )
		{
			//determine largest value
			if( max < arry[i] )
			{
				max = arry[i];
			}	
		}
		*/
	}
	
	/*
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
	*/
	
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
 *Author:
 *Description:
 *Parameters:
 * ***************************************************************************/
void bubbleSort( int *arry, int size )
{
int i = 0;
int n = 0;
int temp = 0;

while( n != size )
{
	for( i=0; i < size; i++ )
	{
	
		if(arry[i] > arry[i+1])
		{
			temp = arry[i];
			arry[i] = arry[i+1];
			arry[i+1] = temp;
		}		

	}
	n++;
}
}

/*****************************************************************************
 *Function: Heapsort
 *Author: Christian Sieh
 *Description: 
 *Parameters:
 * ***************************************************************************/
void heapsort( int *arry, int size)
{
	for( int i = size / 2 - 1; i >= 0; --i)
		siftdown(arry, i, size);
	for( int j = size - 1; j > 0; --j)
	{
		swap( arry[0], arry[j] );
		siftdown( arry, 0, j);
	}
}

inline int leftChild( int i )
{
	return 2 * i + 1;
}


/*****************************************************************************
 *Function: Siftdown
 *Author:
 *Description:
 *Parameters:
 * ***************************************************************************/
void siftdown(int *arry, int i, int n)
{
	int child;
	int tmp;
	
	for(tmp = arry[i]; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if( child != n-1 && arry[child] < arry[child+1])
			++child;
		if(tmp < arry[child])
			arry[i] = arry[child];
		else
			break;
	}
	arry[i] = tmp;
}

/*****************************************************************************
 *Function: convertBase
 *Author:
 *Description:
 *Parameters:
 * *******/
