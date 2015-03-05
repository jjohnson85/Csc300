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

int height(avl_tree_node *root)
{


}

