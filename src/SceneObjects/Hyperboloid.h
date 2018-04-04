#pragma once
#ifndef _HYPERBOLOID_H_
#define _HYPERBOLOID_H_
#include"..\scene\scene.h"

class Hyperboloid
	: public MaterialSceneObject
{
public:
	Hyperboloid(Scene *scene, Material *mat)
		: MaterialSceneObject(scene, mat)
	{
	}
	virtual bool intersectLocal(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return false; }

};

#endif // _HYPERBOLOID_H_
