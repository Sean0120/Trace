#ifndef __BVH_NODE_H__
#define __BVH_NODE_H__

#include "src\scene\scene.h"
class BVH_Node
{
public:
	BVH_Node();
	~BVH_Node();
	BVH_Node(BoundingBox b);
	BoundingBox box;
	BVH_Node* left;
	BVH_Node* right;
};

class BVH_Leaf :
	public BVH_Node
{
public:
	BVH_Leaf();
	BVH_Leaf(BoundingBox b);
	Geometry * obj;
};
#endif
