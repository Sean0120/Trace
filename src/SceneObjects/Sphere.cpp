#include <cmath>

#include "Sphere.h"

bool Sphere::intersectLocal( const ray& r, isect& i ) const
{
	vec3f v = -r.getPosition();
	double b = v.dot(r.getDirection());
	double discriminant = b*b - v.dot(v) + 1;

	if( discriminant < 0.0 ) {
		return false;
	}

	discriminant = sqrt( discriminant );
	double t2 = b + discriminant;

	if( t2 <= RAY_EPSILON ) {
		return false;
	}

	i.obj = this;

	double t1 = b - discriminant;

	if( t1 > RAY_EPSILON ) {
		i.t = t1;
		i.N = r.at( t1 ).normalize();
	} else {
		i.t = t2;
		i.N = r.at( t2 ).normalize();
	}

	return true;
}

void Sphere::isectToTexture(const isect& i, const vec3f& pos, double& x, double& y) const {
	vec3f Sp(0, 0, 1);
	vec3f Se(1, 0, 0);
	vec3f Sn = i.N;
	double phi = acos(-Sn * Sp);
	y = phi / M_PI;
	if (y == 0 || y == 1)
	{
		x = 0;
		return;
	}
	double theta = acos((Se * Sn) / sin(phi)) / (2 * M_PI);
	if (Sp.cross(Se)*Sn > 0) {
		x = theta;
	}
	else
	{
		x = 1 - theta;
	}
}