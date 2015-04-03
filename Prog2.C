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

	binFile.seekg(C_NODE_SIZE*0, ios::beg);
	binFile.read((char*)&root, C_NODE_SIZE);
	cout << "left_child_loc: " << root.left_child << endl;
	cout << "right_child_loc: " << root.right_child << endl;
	cout << "height is: " << root.height << endl;
	cout << "parent is: " << root.parent << endl;	
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
/*
//Check the balance of the tree
//May not be necessary if height is check when leaving
//insert recursion
bool balance( avl_tree_node* root, fstream &binFile )
{
	avl_tree_node left;
	avl_tree_node right;
	avl_tree_node left_left;
	avl_tree_node left_right;
	avl_tree_node right_left;
	avl_tree_node right_right;

	//read left node 
	binFile.seekp( C_NODE_SIZE * root->left, ios::beg );
	binFile.read( left, C_NODE_SIZE );
	
	//read right node
	binFile.seekp( C_NODE_SIZE * root->right, ios::beg );
	binFile.read( right, C_NODE_SIZE );

	//Check balance
	if( left->height - right->height < -1)
	{
		if(left_left->height - left_right->height)
		{


		}
	}
	else if( left->height - right->height > 1)
	{
		if(right_left->height - right_right->height)
		{


		}	
	}
	else
	{
		return true;
	}

}
*/
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

	cout << "Height at location: " << parent << " is: " << root.height << endl;
	binFile.seekg(C_NODE_SIZE*parent, ios::beg);
	binFile.write((char*)&root, C_NODE_SIZE);
	}	
}

//Function to update height
int height( avl_tree_node &root, fstream &binFile )
{
	int leftHeight;
	int rightHeight;
	
	//read the heights of both left and right children from parent
	if( root.left_child != C_NULL)
	{
		binFile.seekg(C_NODE_SIZE*root.left_child, ios::beg);
		binFile.read((char*)&root, C_NODE_SIZE);
		leftHeight = root.height;

		binFile.seekg(C_NODE_SIZE*root.parent, ios::beg);
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

		binFile.seekg(C_NODE_SIZE*root.parent, ios::beg);
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
/*
//Single rotation for out of balance nodes
void singleRotate( avl_tree_node *root )
{



}

//Double rotation for out of balance nodes
void doubleRotate( avl_tree_node *root )
{



}
*/
 
