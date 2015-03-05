#include<iostream>
#include<ifstream>

using namespace std;

struct avl_tree_node
{
	int key_value; //tree node value
	int left_child; //index of left child
	int right_child; //index of right child
	int height; //height from here down (0 if leaf)
	int parent; //index of parent (C_NULL if root)
	int file_loc; //index of this node
}
const int C_NULL = -1;
const int C_NODE_SIZE = sizeof(avl_tree_node);

int main( int argv, char argc[])
{

//Empty for now

}
<<<<<<< HEAD

int height(avl_tree_node *root)
{
	//MUHAHAHAHAAHAHAHAHAHAHA

}

<<<<<<< HEAD
int insertAvl( avl_tree_node *root)
{

	//TESTING TESTING

}
=======
>>>>>>> c686dfe055b6c8d96d5b79f9eca1b313e7f7a5bd
=======
int singleRotate(avl_tree_node *root)
{

	//DIFFERENT TEST

}
>>>>>>> branch1
