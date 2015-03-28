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
void insert( avl_tree_node &root, fstream &binFile, int key );

int main( int argc, char *argv[])
{
	ifstream fin;
	fstream binFile;
	avl_tree_node root;
	avl_tree_node copy;
	int key;

	fin.open( argv[1] );
	
	binFile.open( argv[2], fstream::out);
	binFile.close();

	binFile.open( argv[2], fstream::out | fstream::in | fstream::binary );
	
	if(fin.is_open()== true)
	{
		cout << "Helo Worl" << endl;
	}
	if(binFile.is_open()== true)
	{
		cout << "Hello World" << endl;
	}
	
	if ( fin >> key )
	{
		avl_init( root, binFile, key );
		binFile.seekg(0, ios::beg);
		binFile.read((char*)&copy, C_NODE_SIZE);
		cout << copy.key_value << endl;
		cout << copy.file_loc << endl;
	}
	else
	{
		cerr << "Error: No values in file" << endl;
		return -1;
	}
	/*
	while( fin >> key )
	{
		insert(root, binFile, key);

	}	
	*/
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
	root.file_loc = 1;

	binFile.write((char*)&root, C_NODE_SIZE );
	
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

//Recursively write node records to the binary file
void insert( avl_tree_node *root, fstream &binFile, key )
{	
	//Case if left is empty and key fits in left
	if( root->left == C_NULL && root->key_value < key )
	{
		//Write the new node to the binary file with new location	
		root	
	}
	//Case if right is empty and key fits in right
	else if( root->right == C_NULL && root->key_value > key)
	{
		//Write the new node to the binary file with new location
	
	}
	//Case if left is filled and key moves left
	else if( root-> key_value > key )
	{
		//New root is to the left, read root->left location
		//into current root value
		

		insert(root, binFile, key);
	}
	//Case if right is filled and key moves right
	else if( root-> key_value < key)
	{
		//New root is to the right, read root->right location
		//into current root value		
		
		
		insert(root, binFile, key);
	}
	//If no other case is fulfilled by this point, the key is a repeat
	//exit with an error message
	else
	{
		cerr >> "Key is a repeat: Quitting" >> endl;
		return;
	}

	//At this point we are leaving the recursion begin cheking heights
	//by reaing in current node and its children, if the node is out of
	//balance, run the balancing subroutine
	
	binFile.seekp(C_NODE_SIZE * root->parent, ios::beg);
	binFile.read(root, C_NODE_SIZE);	
	//root is set to parent values and height is incremented
	//call function checkBalance
	if( checkBalance(root, binFile) == false )
	{
		//rebalance, need to know if single or double rotation
	}


}

//Single rotation for out of balance nodes
void singleRotate( avl_tree_node *root )
{



}

//Double rotation for out of balance nodes
void doubleRotate( avl_tree_node *root )
{



}

*/ 
