#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	return kd;
}
