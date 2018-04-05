#include "BVH_Node.h"



BVH_Node::BVH_Node():box()
{
	left = right = NULL;
}

BVH_Node::BVH_Node(BoundingBox b) {
	box = b;
	left = right = NULL;
}

BVH_Node::~BVH_Node()
{
}

BVH_Leaf::BVH_Leaf(BoundingBox b) {
	box = b;
	left = right = NULL;
}

BVH_Leaf::BVH_Leaf() : BVH_Node() {
	obj = NULL;
}