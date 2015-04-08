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

void rotateWithLeftChild( avl_tree_node &root, avl_tree_node &left, avl_tree_node &left_right,
avl_tree_node &left_left, fstream &binFile);

void rotateWithLeftChild( avl_tree_node &root, avl_tree_node &left, avl_tree_node &parent, fstream &binFile);

void rotateWithRightChild( avl_tree_node &root, avl_tree_node &right, avl_tree_node &right_left,
avl_tree_node &right_right, fstream &binFile);

void rotateWithRightChild( avl_tree_node &root, avl_tree_node &right, avl_tree_node &parent, fstream &binFile);

void doubleWithLeftChild(avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &right, avl_tree_node &left_right, avl_tree_node &right_left,
 avl_tree_node &left_left, fstream &binFile);

void doubleWithRightChild(avl_tree_node &root, avl_tree_node &left,
 avl_tree_node &right, avl_tree_node &left_right, avl_tree_node &right_left,
 avl_tree_node &right_right, fstream &binFile);

void insert( avl_tree_node &root, fstream &binFile, int &key,
 int parent, int &location );

int height( avl_tree_node &root, fstream &binFile );


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
	//DEBUG
	avl_tree_node root;
	avl_tree_node copy;
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

	binFile.close();
	fin.close();
	return 0;
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
 *Function: balance
 *Author's: Jared Johnson, Christain Sieh
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
			doubleWithRightChild(root, left, right, left_right, right_left, right_right, binFile);
			return true;
		}
		//Else if right subtree is right heavy
		else
		{
			rotateWithRightChild(root, right, right_left, right_right, binFile);
			return true;
		}
	}
	//Else if tree is left heavy
	else if( left.height - right.height > 1)
	{
		//If left subtree is right heavy
		if(left_left.height <  left_right.height)
		{		
			doubleWithLeftChild(root, left,right, left_right, right_left, left_left, binFile);
			return true;
		}	
		//Else if left subtree is left heavy
		else
		{
			rotateWithLeftChild(root, left, left_right, left_left, binFile);
			return true;
		}
	}
	else
	{
		return true;
	}

}

/*****************************************************************************
 *
 *
 *
 *
 * ***************************************************************************/
void rotateWithLeftChild(avl_tree_node &root, avl_tree_node &left,
avl_tree_node &left_right, avl_tree_node &left_left, fstream &binFile)
{
	avl_tree_node temp;

	temp.parent = root.parent;
	temp.file_loc = root.file_loc;

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

	//writing of nodes
	root.height = height(root, binFile);
	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);


	binFile.seekp( C_NODE_SIZE * left.file_loc, ios::beg);
	binFile.write( (char*) &left, C_NODE_SIZE);

	if(left_right.file_loc != -1)
	{
	binFile.seekp( C_NODE_SIZE * left_right.file_loc, ios::beg);
	binFile.write( (char*) &left_right, C_NODE_SIZE);
	}

	if(left.parent != -1)
	{
	binFile.seekp( C_NODE_SIZE * left.parent, ios::beg);
	binFile.read( (char*) &temp, C_NODE_SIZE);

	temp.left_child = left.file_loc;
	
	binFile.seekp( C_NODE_SIZE * temp.file_loc, ios::beg);
	binFile.write( (char*) &temp, C_NODE_SIZE);
	}

	binFile.seekp( C_NODE_SIZE * 0, ios::beg);
	binFile.read( (char*) &temp, C_NODE_SIZE);

	avl_tree_node blah;

	binFile.seekp( C_NODE_SIZE * temp.left_child, ios::beg);
	binFile.read( (char*) &blah, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * temp.right_child, ios::beg);
	binFile.read( (char*) &blah, C_NODE_SIZE);

}

/*****************************************************************************
 *
 *
 *
 *
 * ***************************************************************************/

void rotateWithRightChild(avl_tree_node &root, avl_tree_node &right,
avl_tree_node &right_left, avl_tree_node &right_right, fstream &binFile)
{
	avl_tree_node temp;

	temp.parent = root.parent;
	temp.file_loc = root.file_loc;

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

	//writing of nodes
	root.height = height(root, binFile);
	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);


	binFile.seekp( C_NODE_SIZE * right.file_loc, ios::beg);
	binFile.write( (char*) &right, C_NODE_SIZE);

	if(right_left.file_loc != -1)
	{
	binFile.seekp( C_NODE_SIZE * right_left.file_loc, ios::beg);
	binFile.write( (char*) &right_left, C_NODE_SIZE);
	}

	if(right.parent != -1)
	{
	binFile.seekp( C_NODE_SIZE * right.parent, ios::beg);
	binFile.read( (char*) &temp, C_NODE_SIZE);

	temp.right_child = right.file_loc;
	
	binFile.seekp( C_NODE_SIZE * temp.file_loc, ios::beg);
	binFile.write( (char*) &temp, C_NODE_SIZE);
	}

	binFile.seekp( C_NODE_SIZE * 0, ios::beg);
	binFile.read( (char*) &temp, C_NODE_SIZE);

	avl_tree_node blah;

	binFile.seekp( C_NODE_SIZE * temp.left_child, ios::beg);
	binFile.read( (char*) &blah, C_NODE_SIZE);


	binFile.seekp( C_NODE_SIZE * temp.right_child, ios::beg);
	binFile.read( (char*) &blah, C_NODE_SIZE);

}

/*****************************************************************************
 *
 *
 *
 *
 * ***************************************************************************/
void rotateWithLeftChild(avl_tree_node &root, avl_tree_node &left, avl_tree_node &parent, fstream &binFile)
{
	avl_tree_node temp;

	temp.parent = root.parent;
	root.parent = left.file_loc;
	left.parent = temp.parent;
	//setting root.right_child to null, shoudl be correct
	root.left_child = -1;
	left.right_child = root.file_loc;
	parent.right_child = left.file_loc;

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
 *
 *
 *
 *
 * ***************************************************************************/
void rotateWithRightChild(avl_tree_node &root, avl_tree_node &right, avl_tree_node &parent, fstream &binFile)
{
	avl_tree_node temp;

	temp.parent = root.parent;
	root.parent = right.file_loc;
	right.parent = temp.parent;
	//setting root.right_child to null, shoudl be correct
	root.right_child = -1;
	right.left_child = root.file_loc;
	parent.left_child = right.file_loc;

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
 *
 *
 *
 *
 * ***************************************************************************/
void doubleWithLeftChild(avl_tree_node &root, avl_tree_node &left, avl_tree_node &right,
 avl_tree_node &left_right, avl_tree_node &right_left, avl_tree_node &left_left, fstream &binFile)
{

	//need to fix left_right_left issue
	rotateWithRightChild(left, left_right, root, binFile);

	rotateWithLeftChild(root, left_right, left_left, left, binFile);
}

/*****************************************************************************
 *
 *
 *
 *
 * ***************************************************************************/
void doubleWithRightChild(avl_tree_node &root, avl_tree_node &left, avl_tree_node &right,
 avl_tree_node &left_right, avl_tree_node &right_left, avl_tree_node &right_right, fstream &binFile)

{
	//Need to fix right_left_right issue
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
void insert( avl_tree_node &root, fstream &binFile, int &key, int parent, int &location )
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

