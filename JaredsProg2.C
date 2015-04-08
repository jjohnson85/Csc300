/*****************************************************************************
 *Program: Program 2 
 *Author's: Jared Johnson, Christian Sieh
 *Class: CSC 300, Data Structures
 *Instructor:
 *Date: 4/7/2015
 *Description: This program recieves integer input from a text file and builds
an AVL tree in a binary file
 *Input: Integers from a text file ex: numbers.txt
 *Output: Binary file AVL tree ex: Output.tree
 *Usage: 
 *Known bugs: large input with many repeat numbers caused seg fault
 *
 *
 *
 *
 *
 * ***************************************************************************/

#include<iostream>
#include<fstream>

using namespace std;

struct avl_tree_node
{
	int key_value; //tree node value
	int left_child; //index of left child
	int right_child; //index of right child
	int height; //height from here down (0 if leaf)
	int parent; //index of parent (C_NULL if root)
	int file_loc; //index of this node
};
const int C_NULL = -1;
const int C_NODE_SIZE = sizeof(avl_tree_node);


/***********************Function Prototypes***********************************/
void avl_init( avl_tree_node &root, fstream &binFile, int  key );

void rotateWithLeftChild( avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &left_right, avl_tree_node &left_left, fstream &binFile);

void rotateWithLeftChild( avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &parent, fstream &binFile);

void rotateWithRightChild( avl_tree_node &root, avl_tree_node &right,
 avl_tree_node &right_left, avl_tree_node &right_right, fstream &binFile);

void rotateWithRightChild( avl_tree_node &root, avl_tree_node &right,
 avl_tree_node &parent, fstream &binFile);

void doubleWithLeftChild(avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &right, avl_tree_node &left_right, avl_tree_node &right_left,
 avl_tree_node &left_left, fstream &binFile);

void doubleWithRightChild(avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &right, avl_tree_node &left_right, avl_tree_node &right_left,
 avl_tree_node &right_right, fstream &binFile);

void insert( avl_tree_node &root, fstream &binFile, int &key,
 int parent, int &location );

int height( avl_tree_node &root, fstream &binFile );

void findMinMax( avl_tree_node &root, fstream &binFile, int &min, int &max, int parent);


/*****************************************************************************
 *Function: Main
 *Author: Jared Johnson
 *Description: Main opens files for input and output, processes input, and
outputs to the console the height of the tree, the root value, the smallest
leaf value, and the largest leaf value. If no output file exists, one will be
created with the users input.
 *Parameters: <in> int argc - command line argument count
              <in> char *argv[] - command line arguments
 * ***************************************************************************/
int main( int argc, char *argv[])
{
	ifstream fin;
	fstream binFile;
	
	avl_tree_node root;
	int min = -2;
	int max = -2;
	int key = C_NULL;
	int location = 1;
	int parent = 0;

	//Opent the text file for input
	fin.open( argv[1] );
	if(!fin)
	{
		cout << "Input file failed to open" << endl;
		return -1;
	}
	
	//Open the output file once to create a new file if no file exists
	binFile.open( argv[2], fstream::out);
	if(!binFile)
	{
		cout << "Output file failed to open" << endl;
		return -1;
	}
	binFile.close();
	
	//Open the file again this time in binary mode, for input and output
	binFile.open( argv[2], fstream::out | fstream::in | fstream::binary );
	if(!binFile)
	{
		cout << "Output file failed to open" << endl;
		return -1;
	}	
	//Check for the first key value and initialize the root
	if ( fin >> key )
	{
		avl_init( root, binFile, key );
	}
	else
	{
		cout << "Error: No values in file" << endl;
		return -1;
	}
	
	//insert values into the AVL tree and increment location in the file
	//for next write
	while( fin >> key )
	{
		binFile.seekg(0, ios::beg);
		binFile.read((char*)&root, C_NODE_SIZE);
		insert(root, binFile, key, parent, location );
		location++;
	}
	
	
	//get the smallest leaf value
	binFile.seekg(0, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);
	parent = 0;

	findMinMax(root, binFile, min, max, parent);

	binFile.seekg(0, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);

	cout << "The height of this tree: " << root.height << endl;
	cout << "Root value: " << root.key_value << endl;
	cout << "Smallest value in leaf: " << min << endl;
	cout << "Largest value in leaf: " << max << endl;

	binFile.close();
	fin.close();
	return 0;
}

/*****************************************************************************
 *Function: findMinMax
 *Author: Jared Johnson
 *Description: Traverses the final avl tree in the binary file to find both
the min and max values in leaf nodes.
 *Parameters: <in/out> avl_tree_node &root - primary avl node for tree traversal
              <in/out> fstream &binFile - binary input/output file
              <in/out> int &min - minimum leaf node value
              <in/out> int &max - maximum leaf node value
              <in> int parent - parent value to return to in recursion
 * ***************************************************************************/
void findMinMax( avl_tree_node &root, fstream &binFile, int &min, int &max,
 int parent)
{
	if( root.left_child == C_NULL && root.right_child == C_NULL )
	{
		if( root.key_value < min || min == -2 )
		{
			min = root.key_value;
		}
		else if( root.key_value > max || max == -2 )
		{
			max = root.key_value;
		}
	}	
	
	parent = root.file_loc;

	if( root.left_child != C_NULL )
	{
	binFile.seekg(C_NODE_SIZE*root.left_child, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);


	findMinMax(root, binFile, min, max, parent);
		
	binFile.seekg(C_NODE_SIZE*parent, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);
	}

	if( root.right_child != C_NULL )
	{
	binFile.seekg(C_NODE_SIZE*root.right_child, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);

	findMinMax(root, binFile, min, max, parent);

	binFile.seekg(C_NODE_SIZE*parent, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);
	}
}


/*****************************************************************************
 *Function: avl_init
 *Author:Jared Johnson
 *Description: Initializes the root value for the tree to grow from, written
to the beggining of the binary output file with location 0, key_value key,
height 0, and null children.
 *Parameters: <in/out> avl_tree_node &root - primary avl_node for building tree
              <in/out> fstream &binFile - binary input/output file
              <in> int key - key value for root node
 * ***************************************************************************/
void avl_init( avl_tree_node &root, fstream &binFile, int  key )
{

	root.key_value = key;
	root.left_child = C_NULL;
	root.right_child = C_NULL;
	root.height = 0;
	root.parent = C_NULL;
	root.file_loc = 0;

	binFile.write((char*)&root, C_NODE_SIZE );
	
}

/*****************************************************************************
 *Function: Balance
 *Author's: Jared Johnson, Christian Sieh
 *Description: Reads nessessary values to determining the balance of the tree,
avl nodes around root value passed in, left, right, left_left, left_right,
right_left, and right_right. Once an imbalace is detected this function calls
the respective rotaion function to rebalance the tree.
 *Parameters: <in/out> avl_tree_node &root - primary avl_node for building tree
              <in/out> fstream &binFile - binary input/output file
 * ***************************************************************************/
bool balance( avl_tree_node &root, fstream &binFile )
{
	avl_tree_node left;
	avl_tree_node right;
	avl_tree_node left_left;
	avl_tree_node left_right;
	avl_tree_node right_left;
	avl_tree_node right_right;

	//read left node
	if(root.left_child != -1)
	{ 
		binFile.seekp( C_NODE_SIZE * root.left_child, ios::beg );
		binFile.read((char*)&left, C_NODE_SIZE );
	}
	else
	{
		left.height = -1;
	}

	//read right node
	if(root.right_child != -1)
	{
		binFile.seekp( C_NODE_SIZE * root.right_child, ios::beg );
		binFile.read((char*)&right, C_NODE_SIZE );
	}
	else
	{
		right.height = -1;
	}

	
	//read children of left node if not null
	if(left.height != -1)
	{
		if(left.left_child != -1)
		{
			binFile.seekp( C_NODE_SIZE * left.left_child, ios::beg );
			binFile.read((char*)&left_left, C_NODE_SIZE);
		}
		else
		{
			left.left_child = -1;
			left_left.file_loc = -1;
			left_left.height = -1;
		}
		if(left.right_child != -1)
		{
			binFile.seekp( C_NODE_SIZE * left.right_child, ios::beg );
			binFile.read((char*)&left_right, C_NODE_SIZE);
		}
		else
		{
			left.right_child = -1;
			left_right.file_loc = -1;
			left_right.height = -1;
		}
	}

	//read children of right node if not null
	if(right.height != -1)
	{
		if(right.left_child != -1)
		{
			binFile.seekp( C_NODE_SIZE * right.left_child, ios::beg );
			binFile.read((char*)&right_left, C_NODE_SIZE);
		}
		else
		{
			right.left_child = -1;
			right_left.file_loc = -1;
			right_left.height = -1;
		}
		if(right.right_child != -1)
		{
			binFile.seekp( C_NODE_SIZE * right.right_child, ios::beg );
			binFile.read((char*)&right_right, C_NODE_SIZE);
		}
		else
		{
			right.right_child = -1;
			right_right.file_loc = -1;
			right_right.height = -1;
		}
	}
	
	//Check balance
	//If tree is right heavy
	if( left.height - right.height < -1)
	{
		//If right subtree is left heavy
		if(right_left.height > right_right.height)
		{
			doubleWithRightChild(root, left, right, left_right,
			 right_left, right_right, binFile);
			return true;
		}
		//Else if right subtree is right heavy
		else
		{
			rotateWithRightChild(root, right, right_left,
			 right_right, binFile);
			return true;
		}
	}
	//Else if tree is left heavy
	else if( left.height - right.height > 1)
	{
		//If left subtree is right heavy
		if(left_left.height <  left_right.height)
		{		
			doubleWithLeftChild(root, left,right, left_right,
			 right_left, left_left, binFile);
			return true;
		}	
		//Else if left subtree is left heavy
		else
		{
			rotateWithLeftChild(root, left, left_right,
			 left_left, binFile);
			return true;
		}
	}
	else
	{
		return true;
	}

}

/*****************************************************************************
 *Function: rotateWithLeftChild
 *Author's: Christian Sieh
 *Description: Takes the root supplied and puts it as root's left child's
	       right child and updates all nodes accordingly. Also writes the
	       nodes back to the file.
 *Parameters: <in/out> avl_tree_node &root - primary avl_node for building tree
	      <in/out> avl_tree_node &left - root's left child
	      <in/out> avl_tree_node &left_right - left's right child
	      <in/out> avl_tree_node &left_left - left's left child
              <in/out> fstream &binFile - binary input/output file
 * ***************************************************************************/
void rotateWithLeftChild(avl_tree_node &root, avl_tree_node &left,
avl_tree_node &left_right, avl_tree_node &left_left, fstream &binFile)
{
	avl_tree_node temp;

	//Save root variables in temp
	temp.parent = root.parent;
	temp.file_loc = root.file_loc;
	
	//If it's the root of the tree this code lets us swap
	//the location of left and root in the file
	if(root.parent == -1)
	{
		root.file_loc = left.file_loc;
		left.file_loc = temp.file_loc;	
	
		left_left.parent = left.file_loc;
		binFile.seekp( C_NODE_SIZE * left_left.file_loc, ios::beg);
		binFile.write( (char*) &left_left, C_NODE_SIZE);
	}

	root.left_child = left_right.file_loc;
	left_right.parent = root.file_loc;
	left.right_child = root.file_loc;
	root.parent = left.file_loc;
	left.parent = temp.parent;	

	//Fixes Height
	root.height = height(root, binFile);

	//Writes out the nodes
	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);


	binFile.seekp( C_NODE_SIZE * left.file_loc, ios::beg);
	binFile.write( (char*) &left, C_NODE_SIZE);

	//Writes out left_right if it's not null
	if(left_right.file_loc != -1)
	{
		binFile.seekp( C_NODE_SIZE * left_right.file_loc, ios::beg);
		binFile.write( (char*) &left_right, C_NODE_SIZE);
	}

	//Fixes parent/children "pointers" if necessary
	if(left.parent != -1)
	{
		binFile.seekp( C_NODE_SIZE * left.parent, ios::beg);
		binFile.read( (char*) &temp, C_NODE_SIZE);

		temp.left_child = left.file_loc;
	
		binFile.seekp( C_NODE_SIZE * temp.file_loc, ios::beg);
		binFile.write( (char*) &temp, C_NODE_SIZE);
	}
}

/*****************************************************************************
 *Function: rotateWithRightChild
 *Author's: Christian Sieh
 *Description: Takes the root supplied and puts it as root's right child's
	       left child and updates all nodes accordingly. Also writes the
	       nodes back to the file.
 *Parameters: <in/out> avl_tree_node &root - primary avl_node for building tree
	      <in/out> avl_tree_node &right - root's right child
	      <in/out> avl_tree_node &right_left - right's left child
	      <in/out> avl_tree_node &right_right - right's right child
              <in/out> fstream &binFile - binary input/output file
 * ***************************************************************************/
void rotateWithRightChild(avl_tree_node &root, avl_tree_node &right,
avl_tree_node &right_left, avl_tree_node &right_right, fstream &binFile)
{
	avl_tree_node temp;
	
	//Stores root variables before they get overwritten
	temp.parent = root.parent;
	temp.file_loc = root.file_loc;

	//Allows us to swap right and root in the file
	if(root.parent == -1)
	{
		root.file_loc = right.file_loc;
		right.file_loc = temp.file_loc;	
	
		right_right.parent = right.file_loc;
		binFile.seekp( C_NODE_SIZE * right_right.file_loc, ios::beg);
		binFile.write( (char*) &right_right, C_NODE_SIZE);
	}


	root.right_child = right_left.file_loc;
	right_left.parent = root.file_loc;
	right.left_child = root.file_loc;
	root.parent = right.file_loc;
	right.parent = temp.parent;	

	//Fixes Height
	root.height = height(root, binFile);

	//Write out nodes
	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);


	binFile.seekp( C_NODE_SIZE * right.file_loc, ios::beg);
	binFile.write( (char*) &right, C_NODE_SIZE);

	//If we need to write out right_left
	if(right_left.file_loc != -1)
	{
		binFile.seekp( C_NODE_SIZE * right_left.file_loc, ios::beg);
		binFile.write( (char*) &right_left, C_NODE_SIZE);
	}

	//Used to fix parent/children "pointers" if necessary
	if(right.parent != -1)
	{
		binFile.seekp( C_NODE_SIZE * right.parent, ios::beg);
		binFile.read( (char*) &temp, C_NODE_SIZE);

		temp.right_child = right.file_loc;
	
		binFile.seekp( C_NODE_SIZE * temp.file_loc, ios::beg);
		binFile.write( (char*) &temp, C_NODE_SIZE);
	}
}

/*****************************************************************************
 *Function: rotateWithLeftChild
 *Author's: Christian Sieh
 *Description: Takes the root supplied and puts it as root's left child's
	       right child and updates all nodes accordingly. Also writes the
	       nodes back to the file. This is an overloaded function that
	       differs from the one about because it only rotates using two
	       nodes.
 *Parameters: <in/out> avl_tree_node &root - primary avl_node for building tree
	      <in/out> avl_tree_node &left - root's left child
	      <in/out> avl_tree_node &parent - the parent of root, used to 	
		       update children.
              <in/out> fstream &binFile - binary input/output file
 * ***************************************************************************/
void rotateWithLeftChild(avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &parent, fstream &binFile)
{
	avl_tree_node temp;

	//Use temp to save variables we are overwriting
	//Swap the parent of root and left
	temp.parent = root.parent;
	root.parent = left.file_loc;
	left.parent = temp.parent;

	root.left_child = -1;

	//Make root left's right child
	left.right_child = root.file_loc;
	parent.right_child = left.file_loc;

	//Allows us to change the root of the tree and the left node's
	//file locations
	if(root.parent == -1)
	{
	temp.file_loc = root.file_loc;
	root.file_loc = left.file_loc;
	left.file_loc = temp.file_loc;	
	}

	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * left.file_loc, ios::beg);
	binFile.write( (char*) &left, C_NODE_SIZE);
	
	binFile.seekp( C_NODE_SIZE * parent.file_loc, ios::beg);
	binFile.write( (char*) &parent, C_NODE_SIZE );
	
	//Correct the height for root and left
	root.height = height(root, binFile);

	left.height = height(left, binFile);

	//writing of nodes
	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * left.file_loc, ios::beg);
	binFile.write( (char*) &left, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * parent.file_loc, ios::beg);
	binFile.write( (char*) &parent, C_NODE_SIZE);
}

/*****************************************************************************
 *Function: rotateWithRightChild
 *Author's: Christian Sieh
 *Description: Takes the root supplied and puts it as root's right child's
	       left child and updates all nodes accordingly. Also writes the
	       nodes back to the file. This is an overloaded function that
	       differs from the one about because it only rotates using two
	       nodes.
 *Parameters: <in/out> avl_tree_node &root - primary avl_node for building tree
	      <in/out> avl_tree_node &left - root's left child
	      <in/out> avl_tree_node &parent - the parent of root, used to 	
		       update children.
              <in/out> fstream &binFile - binary input/output file
 * ***************************************************************************/
void rotateWithRightChild(avl_tree_node &root, avl_tree_node &right,
 avl_tree_node &parent, fstream &binFile)
{
	avl_tree_node temp;

	//Use temp to preserve variables we are overwriting
	//Switch parent of root and right
	temp.parent = root.parent;
	root.parent = right.file_loc;
	right.parent = temp.parent;

	root.right_child = -1;
	
	//Make root right's left child
	right.left_child = root.file_loc;
	parent.left_child = right.file_loc;

	//This allows us to swap root and right child's location
	//in the binary file if necessary
	if(root.parent == -1)
	{
	temp.file_loc = root.file_loc;
	root.file_loc = right.file_loc;
	right.file_loc = temp.file_loc;	
	}

	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * right.file_loc, ios::beg);
	binFile.write( (char*) &right, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * parent.file_loc, ios::beg);
	binFile.write( (char*) &parent, C_NODE_SIZE);
	
	//correct the height for root and right
	root.height = height(root, binFile);

	right.height = height(right, binFile);

	//writing of nodes
	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * right.file_loc, ios::beg);
	binFile.write( (char*) &right, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * parent.file_loc, ios::beg);
	binFile.write( (char*) &parent, C_NODE_SIZE);
}

/*****************************************************************************
 *Function: doubleWithLeftChild
 *Author's: Christian Sieh
 *Description: This function first calls rotateWithRightChild on root's left
	       child to make sure it's children are in order and then
	       rotateWithLeftChild is called on root to put root as it's left
	       child's right child
 *Parameters: <in/out> avl_tree_node &root - primary avl_node for building tree
	      <in/out> avl_tree_node &left - root's left child
	      <in/out> avl_tree_node &right - root's right child
	      <in/out> avl_tree_node &left_right - left's right child
	      <in/out> avl_tree_node &right_left - right's left child
	      <in/out> avl_tree_node &left_left - left's left child
              <in/out> fstream &binFile - binary input/output file
 * ***************************************************************************/
void doubleWithLeftChild(avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &right, avl_tree_node &left_right, avl_tree_node &right_left,
 avl_tree_node &left_left, fstream &binFile)
{
	rotateWithRightChild(left, left_right, root, binFile);

	rotateWithLeftChild(root, left_right, left_left, left, binFile);
}

/*****************************************************************************
 *Function: doubleWithrightChild
 *Author's: Christian Sieh
 *Description: This function first calls rotateWithLeftChild on root's right
	       child to make sure it's children are in order and then
	       rotateWithRightChild is called on root to put root as it's right
	       child's left child
 *Parameters: <in/out> avl_tree_node &root - primary avl_node for building tree
	      <in/out> avl_tree_node &left - root's left child
	      <in/out> avl_tree_node &right - root's right child
	      <in/out> avl_tree_node &left_right - left's right child
	      <in/out> avl_tree_node &right_left - right's left child
	      <in/out> avl_tree_node &right_right - right's right child
              <in/out> fstream &binFile - binary input/output file
 * ***************************************************************************/
void doubleWithRightChild(avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &right, avl_tree_node &left_right, avl_tree_node &right_left,
 avl_tree_node &right_right, fstream &binFile)

{
	rotateWithLeftChild(right, right_left, root, binFile);

	rotateWithRightChild(root, right_left, right_right, right, binFile);
}

/*****************************************************************************
 *Function: insert
 *Author: Jared Johnson
 *Description: inserts new nodes into the binary file. recursively determines
the parent and children values by reading throught the tree and comparing
current key value to insert with key value in current root in the recursion.
 *Parameters: <in/out> avl_tree_node &root - primary avl node for building tree
              <in/out> fstream &binFile - binary file for input/output
              <in/out> int &key - key value to be inserted
              <in> parent - parent value at current point in recursion
              <in/out> int &location - location for new node to assign to
parent
 * ***************************************************************************/
void insert( avl_tree_node &root, fstream &binFile, int &key,
 int parent, int &location )
{	
	
	//Case if left is empty and key fits in left
	if( root.left_child == C_NULL && root.key_value > key)
	{
		parent = root.file_loc;

		//Write the location for new left child to parent
		root.left_child = location;
		binFile.seekp(C_NODE_SIZE*root.file_loc, ios::beg);
		binFile.write((char*)&root, C_NODE_SIZE);

		//Write the new node to the binary file with new location left	
		root.key_value = key;
		root.left_child = C_NULL;
		root.right_child = C_NULL;
		root.height = 0;
		root.parent = parent;
		root.file_loc = location;

		binFile.seekp(C_NODE_SIZE*location, ios::beg);
		binFile.write((char*)&root, C_NODE_SIZE);
			
	}	
	//Case if right is empty and key fits in right
	else if( root.right_child == C_NULL && root.key_value < key)
	{
		parent = root.file_loc;

		//Wite the location for the new right child to parent
		root.right_child = location;
		binFile.seekp(C_NODE_SIZE*root.file_loc, ios::beg);
		binFile.write((char*)&root, C_NODE_SIZE);

		//Write the new node to the binary file with new location right
		root.key_value = key;
		root.left_child = C_NULL;
		root.right_child = C_NULL;
		root.height = 0;
		root.parent = parent;
		root.file_loc = location;
		
		binFile.seekp(C_NODE_SIZE*location, ios::beg);
		binFile.write((char*)&root, C_NODE_SIZE);
		
	}
	//Case if left is filled and key moves left
	else if( root.key_value > key)
	{
		//New root is to the left, read root->left location
		//into current root value
		parent = root.file_loc;

		binFile.seekg(C_NODE_SIZE*root.left_child, ios::beg);
		binFile.read((char*)&root, C_NODE_SIZE);
		
		insert(root, binFile, key, parent, location );
	}
	//Case if right is filled and key moves right
	else if( root.key_value < key ) 
	{
		//New root is to the right, read root->right location
		//into current root value
		parent = root.file_loc;
		
		binFile.seekg(C_NODE_SIZE*root.right_child, ios::beg);	
		binFile.read((char*)&root, C_NODE_SIZE);
		
		insert(root, binFile, key, parent, location );
	}

	else if( root.key_value == key)
	{
		return;
	}

	
	//read root value at location parent for access to current
	//node in the recursion
	if (parent != C_NULL)
	{	
	binFile.seekg(C_NODE_SIZE*parent, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);
	
	root.height = height(root, binFile)+1;

	binFile.seekg(C_NODE_SIZE*parent, ios::beg);
	binFile.write((char*)&root, C_NODE_SIZE);

	balance(root, binFile);
	}	
}

/*****************************************************************************
 *Function: height
 *Author: Jared Johnson
 *Description: Reads to left to get height at left, and then to right to get
height at right, and detemines the maximum of the two for the height of the 
root value passed in for the subtree.
 *Parameters: <in/out> avl_tree_noe &root - primary avl node for building tree
also in this case the root value of a sub tree who's height needs to be 
determined
              <in/out> fstream &binFile - binary input/output file
 * ***************************************************************************/
int height( avl_tree_node &root, fstream &binFile )
{
	int leftHeight;
	int rightHeight;
	int rootLoc = root.file_loc;
	
	//read the heights of both left and right children from parent
	if( root.left_child != C_NULL)
	{
		binFile.seekg(C_NODE_SIZE*root.left_child, ios::beg);
		binFile.read((char*)&root, C_NODE_SIZE);
		leftHeight = root.height;

		binFile.seekg(C_NODE_SIZE*rootLoc, ios::beg);
		binFile.read((char*)&root, C_NODE_SIZE);
	}
	else
	{
		leftHeight = 0;
	}

	if( root.right_child != C_NULL)
	{
		binFile.seekg(C_NODE_SIZE*root.right_child, ios::beg);
		binFile.read((char*)&root, C_NODE_SIZE);
		rightHeight = root.height;

		binFile.seekg(C_NODE_SIZE*rootLoc, ios::beg);
		binFile.read((char*)&root, C_NODE_SIZE);
	}
	else
	{
		rightHeight = 0;
	}
	
	if( leftHeight > rightHeight )
	{
		return leftHeight;
	}
	else
	{
		return rightHeight;
	}	

}

