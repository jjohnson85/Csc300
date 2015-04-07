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

void avl_init( avl_tree_node &root, fstream &binFile, int  key );

void rotateWithLeftChild( avl_tree_node &root, avl_tree_node &left, avl_tree_node &left_right,
avl_tree_node &left_left, fstream &binFile);

void rotateWithLeftChild( avl_tree_node &root, avl_tree_node &left, fstream &binFile);

void rotateWithRightChild( avl_tree_node &root, avl_tree_node &right, avl_tree_node &right_left,
avl_tree_node &right_right, fstream &binFile);

void rotateWithRightChild( avl_tree_node &root, avl_tree_node &right, fstream &binFile);

void doubleWithLeftChild(avl_tree_node &root, avl_tree_node &left, avl_tree_node &right,
 avl_tree_node &left_right, avl_tree_node &right_left, avl_tree_node &left_left, fstream &binFile);

void doubleWithRightChild(avl_tree_node &root, avl_tree_node &left, avl_tree_node &right,
 avl_tree_node &left_right, avl_tree_node &right_left, avl_tree_node &right_right, fstream &binFile);

void insert( avl_tree_node &root, fstream &binFile, int &key, int parent, int &location );

int height( avl_tree_node &root, fstream &binFile );

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
		cout << "root value is: " << root.key_value << endl;
		insert(root, binFile, key, parent, location );
		location++;
	}	
	int i = 0;


	for( i=0; i < 5 ; i++ )
	{
	cout << i << endl;
	binFile.seekg(C_NODE_SIZE*i, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);
	cout << "key value is: " << root.key_value << endl;
	cout << "location is: " << root.file_loc << endl;
	cout << "left_child_loc: " << root.left_child << endl;
	cout << "right_child_loc: " << root.right_child << endl;
	cout << "height is: " << root.height << endl;
	cout << "parent is: " << root.parent << endl;	
	cout << "-----------------------------------------" << endl;
	}
	binFile.close();
	fin.close();
	return 0;
}

//Write the root node to the front of the file
void avl_init( avl_tree_node &root, fstream &binFile, int  key )
{

	root.key_value = key;
	root.left_child = C_NULL;
	root.right_child = C_NULL;
	root.height = 0;
	root.parent = C_NULL;
	root.file_loc = 0;

	binFile.write((char*)&root, C_NODE_SIZE );
	//DEBUG
	cout << "Key: " << root.key_value << endl <<
		"Left Child: " << root.left_child << endl <<
		"Right Child: " << root.right_child << endl;
}

//Check the balance of the tree
//May not be necessary if height is check when leaving
//insert recursion
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

//	if( left.height - right.height >= 1 || left.height - right.height <= -1 )
//	{
//		cout << "!!!!!!!!!!!!!!!" << endl;	
//		return false;
//	}
	
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

void rotateWithLeftChild(avl_tree_node &root, avl_tree_node &left,
avl_tree_node &left_right, avl_tree_node &left_left, fstream &binFile)
{
	avl_tree_node temp;
	cout << "Left 4 node!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	
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
	cout << "ROOT FILE LOC: " << root.file_loc;
	cout << "ROOT VALUE: " << root.key_value;

	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);

	cout << "LEFT FILE LOC: " << left.file_loc;
	cout << "LEFT VALUE: " << left.key_value;
	
	binFile.seekp( C_NODE_SIZE * left.file_loc, ios::beg);
	binFile.write( (char*) &left, C_NODE_SIZE);

	if(left_right.file_loc != -1)
	{
	binFile.seekp( C_NODE_SIZE * left_right.file_loc, ios::beg);
	binFile.write( (char*) &left_right, C_NODE_SIZE);
	}

	cout << "LEFT PARENT!!!!!!!: " << left.parent << endl;
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

	cout << "Root Location: " << temp.file_loc << endl;
	cout << "Root Value: " << temp.key_value << endl;
	cout << "Root Parent: " << temp.parent << endl;
	cout << "Root Left Child: " << temp.left_child << endl;
	cout << "Root Right Child: " << temp.right_child << endl;

	binFile.seekp( C_NODE_SIZE * temp.left_child, ios::beg);
	binFile.read( (char*) &blah, C_NODE_SIZE);

	cout << "Left Location: " << blah.file_loc << endl;
	cout << "Left Value: " << blah.key_value << endl;
	cout << "Left Parent: " << blah.parent << endl;
	cout << "Left Left Child: " << blah.left_child << endl;
	cout << "Left Right Child: " << blah.right_child << endl;
	
	binFile.seekp( C_NODE_SIZE * temp.right_child, ios::beg);
	binFile.read( (char*) &blah, C_NODE_SIZE);

	cout << "Right Location: " << blah.file_loc << endl;
	cout << "Right Value: " << blah.key_value << endl;
	cout << "Right Parent: " << blah.parent << endl;	
	cout << "Right Left Child: " << blah.left_child << endl;
	cout << "Right Right Child: " << blah.right_child << endl;
}

void rotateWithRightChild(avl_tree_node &root, avl_tree_node &right,
avl_tree_node &right_left, avl_tree_node &right_right, fstream &binFile)
{
	avl_tree_node temp;
	cout << "Right 4 node!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	
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
	cout << "ROOT FILE LOC: " << root.file_loc;
	cout << "ROOT VALUE: " << root.key_value;

	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);

	cout << "Right FILE LOC: " << right.file_loc;
	cout << "Right VALUE: " << right.key_value;
	
	binFile.seekp( C_NODE_SIZE * right.file_loc, ios::beg);
	binFile.write( (char*) &right, C_NODE_SIZE);

	if(right_left.file_loc != -1)
	{
	binFile.seekp( C_NODE_SIZE * right_left.file_loc, ios::beg);
	binFile.write( (char*) &right_left, C_NODE_SIZE);
	}

	cout << "Right PARENT!!!!!!!: " << right.parent << endl;
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

	cout << "Root Location: " << temp.file_loc << endl;
	cout << "Root Value: " << temp.key_value << endl;
	cout << "Root Parent: " << temp.parent << endl;
	cout << "Root Left Child: " << temp.left_child << endl;
	cout << "Root Right Child: " << temp.right_child << endl;

	binFile.seekp( C_NODE_SIZE * temp.left_child, ios::beg);
	binFile.read( (char*) &blah, C_NODE_SIZE);

	cout << "Left Location: " << blah.file_loc << endl;
	cout << "Left Value: " << blah.key_value << endl;
	cout << "Left Parent: " << blah.parent << endl;
	cout << "Left Left Child: " << blah.left_child << endl;
	cout << "Left Right Child: " << blah.right_child << endl;
	
	binFile.seekp( C_NODE_SIZE * temp.right_child, ios::beg);
	binFile.read( (char*) &blah, C_NODE_SIZE);

	cout << "Right Location: " << blah.file_loc << endl;
	cout << "Right Value: " << blah.key_value << endl;
	cout << "Right Parent: " << blah.parent << endl;	
	cout << "Right Left Child: " << blah.left_child << endl;
	cout << "Right Right Child: " << blah.right_child << endl;
}
void rotateWithLeftChild(avl_tree_node &root, avl_tree_node &left, fstream &binFile)
{
	avl_tree_node temp;

	cout << "Not Writing One?????????????????????????????????????????????????????????????" << endl;

	temp.parent = root.parent;

	root.parent = left.file_loc;
	left.parent = temp.parent;
	left.right_child = root.file_loc;

	if(root.parent == -1)
	{
	temp.file_loc = root.file_loc;
	root.file_loc = left.file_loc;
	left.file_loc = temp.file_loc;	
	}

	//writing of nodes
	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * left.file_loc, ios::beg);
	binFile.write( (char*) &left, C_NODE_SIZE);
}

void rotateWithRightChild(avl_tree_node &root, avl_tree_node &right, fstream &binFile)
{
	avl_tree_node temp;

	temp.parent = root.parent;
	cout << "2 NODE RIGHT???????????????????????????????????????????????????????????????????" << endl;

	cout << "Root Parent: " << root.parent << endl;
	cout << "Root Value: " << root.key_value << endl;
	cout << "Root Left Child: " << root.left_child << endl;
	cout << "Root Right Child: " << root.right_child << endl;
	cout << "Root Location: " << root.file_loc << endl;
	cout << "Root Height: " << root.height << endl;

	cout << "Right Parent: " << right.parent << endl;
	cout << "Right Value: " << right.key_value << endl;
	cout << "Right Left Child: " << right.left_child << endl;
	cout << "Right Right Child: " << right.right_child << endl;
	cout << "Right Location: " << right.file_loc << endl;
	cout << "Right Height: " << right.height << endl << endl;

	root.parent = right.file_loc;
	right.parent = temp.parent;
	//setting root.right_child to null, shoudl be correct
	root.right_child = -1;
	right.left_child = root.file_loc;

	if(root.parent == -1)
	{
	temp.file_loc = root.file_loc;
	root.file_loc = right.file_loc;
	right.file_loc = temp.file_loc;	
	}

	root.height = height(root, binFile);

	cout << "Root Parent: " << root.parent << endl;
	cout << "Root Value: " << root.key_value << endl;
	cout << "Root Left Child: " << root.left_child << endl;
	cout << "Root Right Child: " << root.right_child << endl;
	cout << "Root Location: " << root.file_loc << endl;
	cout << "Root Height: " << root.height << endl;

	cout << "Right Parent: " << right.parent << endl;
	cout << "Right Value: " << right.key_value << endl;
	cout << "Right Left Child: " << right.left_child << endl;
	cout << "Right Right Child: " << right.right_child << endl;
	cout << "Right Location: " << right.file_loc << endl;
	cout << "Right Height: " << right.height << endl << endl;

	right.height = height(right, binFile) + 1;
	
	cout << "Root Parent: " << root.parent << endl;
	cout << "Root Value: " << root.key_value << endl;
	cout << "Root Left Child: " << root.left_child << endl;
	cout << "Root Right Child: " << root.right_child << endl;
	cout << "Root Location: " << root.file_loc << endl;
	cout << "Root Height: " << root.height << endl;

	cout << "Right Parent: " << right.parent << endl;
	cout << "Right Value: " << right.key_value << endl;
	cout << "Right Left Child: " << right.left_child << endl;
	cout << "Right Right Child: " << right.right_child << endl;
	cout << "Right Location: " << right.file_loc << endl;
	cout << "Right Height: " << right.height << endl << endl;

	//writing of nodes
	binFile.seekp( C_NODE_SIZE * root.file_loc, ios::beg);
	binFile.write( (char*) &root, C_NODE_SIZE);

	binFile.seekp( C_NODE_SIZE * right.file_loc, ios::beg);
	binFile.write( (char*) &right, C_NODE_SIZE);
}

void doubleWithLeftChild(avl_tree_node &root, avl_tree_node &left, avl_tree_node &right,
 avl_tree_node &left_right, avl_tree_node &right_left, avl_tree_node &left_left, fstream &binFile)
{
	//need to fix left_right_left issue
	rotateWithRightChild(left, left_right, binFile);
	rotateWithLeftChild(root, left, left_right, left_left, binFile);
}

void doubleWithRightChild(avl_tree_node &root, avl_tree_node &left, avl_tree_node &right,
 avl_tree_node &left_right, avl_tree_node &right_left, avl_tree_node &right_right, fstream &binFile)

{
	//Need to fix right_left_right issue
	rotateWithLeftChild(right, right_left, binFile);
	rotateWithRightChild(root, right, right_left, right_right, binFile);
}

//Recursively write node records to the binary file
void insert( avl_tree_node &root, fstream &binFile, int &key, int parent, int &location )
{	
	
	//Case if left is empty and key fits in left
	if( root.left_child == C_NULL && root.key_value > key)
	{
		parent = root.file_loc;
		cout << "Comparing key value to: " << root.key_value << endl;
		//Write the location for new left child to parent
		root.left_child = location;
		binFile.seekp(C_NODE_SIZE*root.file_loc, ios::beg);
		binFile.write((char*)&root, C_NODE_SIZE);
		cout << "Insert key value: " << key << " to the left" << endl;
		cout << "Parent is: " << parent << endl;
		cout << "Location is: " << location << endl;
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
		cout << "Comparing key value to: " << root.key_value << endl;
		//Wite the location for the new right child to parent
		root.right_child = location;
		binFile.seekp(C_NODE_SIZE*root.file_loc, ios::beg);
		binFile.write((char*)&root, C_NODE_SIZE);
		cout << "Insert key value: " << key << " to the right" << endl;
		cout << "Parent is: " << parent << endl;
		cout << "Location is: " << location << endl;
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
		cout << "Parent value is now: " << root.file_loc << endl;
		binFile.seekg(C_NODE_SIZE*root.left_child, ios::beg);
		binFile.read((char*)&root, C_NODE_SIZE);
		cout << "Key value to the left is: " << root.key_value << endl;
		insert(root, binFile, key, parent, location );
		cout << "It got here" << endl;
	}
	//Case if right is filled and key moves right
	else if( root.key_value < key ) 
	{
		//New root is to the right, read root->right location
		//into current root value
		parent = root.file_loc;
		cout << "Parent value is now: " << root.file_loc << endl;		
		binFile.seekg(C_NODE_SIZE*root.right_child, ios::beg);	
		binFile.read((char*)&root, C_NODE_SIZE);
		cout << "Key value to the right is: " << root.key_value << endl;
		insert(root, binFile, key, parent, location );
		cout << "It got here also"<< endl;
	}

	else if( root.key_value == key)
	{
		cout << key << " :Key is a repeat: Did not insert" << endl;
		return;
	}

	
	//read root value at location parent for access to current
	//node in the recursion
	cout << "--------------------------------------------" << endl;
	cout << "Leaving Recursion, current location is: " << parent << endl;

	if (parent != C_NULL)
	{	
	binFile.seekg(C_NODE_SIZE*parent, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);
	
	root.height = height(root, binFile)+1;

	cout << "Height at location: " << root.parent << " is: " << root.height << endl;
	cout << "Root Parent " << root.parent << endl;
	cout << "Root Value: " << root.key_value << endl;
	cout << "Root Left Child " << root.left_child << endl;
	cout << "Root Rigth Child " << root.right_child << endl;
	cout << "Root File_Loc " << root.file_loc << endl;

	binFile.seekg(C_NODE_SIZE*parent, ios::beg);
	binFile.write((char*)&root, C_NODE_SIZE);

	balance(root, binFile);
	}	
}

//Function to update height
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
