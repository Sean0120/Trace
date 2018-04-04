#pragma once
#ifndef _ELIPTIC_H_
#define _ELIPTIC_H_
#include"..\scene\scene.h"

class Eliptic
	: public MaterialSceneObject
{
public:
	Eliptic(Scene *scene, Material *mat)
		: MaterialSceneObject(scene, mat)
	{
	}
	virtual bool intersectLocal(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return false; }

};

#endif // _HYPERBOLOID_H_
