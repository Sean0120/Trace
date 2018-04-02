#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	i.obj = this;
	vec3f R0 = r.getPosition();
	vec3f Rd = r.getDirection();

	double tMin = -1.0e308; // 1.0e308 is close to infinity... close enough for us!
	double tMax = 1.0e308;
	vec3f Nmin, Nmax;
	double ttemp;
	vec3f min(-0.5, -0.5, -0.5);
	vec3f max(0.5, 0.5, 0.5);
	for (int currentaxis = 0; currentaxis < 3; currentaxis++)
	{
		vec3f N1, N2;
		double vd = Rd[currentaxis];

		// if the ray is parallel to the face's plane (=0.0)
		if (vd == 0.0)
			continue;

		double v1 = min[currentaxis] - R0[currentaxis];
		double v2 = max[currentaxis] - R0[currentaxis];

		// two slab intersections
		double t1 = v1 / vd;
		double t2 = v2 / vd;
		N1[currentaxis] = -1;
		N2[currentaxis] = 1;
		if (t1 > t2) { // swap t1 & t2,let t1 < t2
			ttemp = t1;
			t1 = t2;
			t2 = ttemp;

			ttemp = N1[currentaxis];
			N1[currentaxis] = N2[currentaxis];
			N2[currentaxis] = ttemp;
		}

		//usually this will replace tMin and tMax with t1 and t2
		if (t1 > tMin) {
			tMin = t1;
			Nmin = N1;
		}
		if (t2 < tMax) {
			tMax = t2;
			Nmax = N2;
		}

		if (tMin > tMax) // box is missed
			return false;
		if (tMax < RAY_EPSILON) // box is behind ray
			return false;
	}
	i.setT(tMin);
	i.setN(Nmin);
	return true; 
	
}
