#include <cmath>
#include <assert.h>
#include "Hyperboloid.h"

bool Hyperboloid::intersectLocal(const ray& r, isect& i) const
{
	vec3f dir = r.getDirection();
	vec3f ori = r.getPosition();
	double a = dir[0] * dir[0] + dir[1] * dir[1] - dir[2] * dir[2];
	double b = 2 * dir[0] * ori[0] - 2 * dir[2] * ori[2] + 2 * dir[1] * ori[1];
	double c = ori[0] * ori[0] + ori[1] * ori[1] - ori[2] * ori[2] - 1;
	double discriminant = b*b - 4 * a*c;


	if (discriminant < 0.0) {
		return false;
	}

	discriminant = sqrt(discriminant);
	//the larger root
	double t2 = (-b + discriminant) / (2 * a);	

	if (t2 <= RAY_EPSILON) {	
		return false;
	}
	double t1 = (-b - discriminant) / (2 * a);
	i.obj = this;	
	//use the gradient to get normal
	if (t1 > RAY_EPSILON) {
		i.t = t1;
		i.N = vec3f(2 * r.at(t1)[0], 2 * r.at(t1)[1], -2 * r.at(t1)[2]).normalize();
	}
	else {
		i.t = t2;
		i.N = vec3f(2 * r.at(t2)[0], 2 * r.at(t2)[1], -2 * r.at(t2)[2]).normalize();
	}

	//make sure the normal is facing outside
	if (r.getDirection().dot(i.N) > 0)
		i.N *= -1.0;
	return true;

}
