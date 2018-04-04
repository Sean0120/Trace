#include <cmath>

#include "light.h"
#include "../ui/TraceUI.h"
extern TraceUI* traceUI;

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	vec3f intensity = vec3f(1, 1, 1);
	ray r(P, this->getDirection(P));
	this->getScene()->acc_shadow_attenuation(r, intensity);
	//bool isShadow = false;
	//while(this->getScene()->intersect(r, i) == true){
	//	if (i.getMaterial().kt.iszero()) {
	//		isShadow = true;
	//		break;
	//	}
	//	else {
	//		vec3f tmpP = r.at(i.t);
	//		ray tmpR(tmpP, this->getDirection(P));
	//		r = tmpR;
	//		intensity = prod(intensity, i.getMaterial().kt);
	//	}
	//}
	//if (isShadow)
	//	return vec3f(0, 0, 0);
	//else
	return intensity;
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE
	
	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	double cA = traceUI->getConstAttenuation();
	double lA = traceUI->getLinearAttenuation();
	double qA = traceUI->getQuadraAttenuation();
	double atten = ((position - P).length_squared()*qA + (position - P).length()*lA + cA);
	if (abs(atten)<=RAY_EPSILON)
		return 1;
	return minimum(1.0, 1.0 / atten);

}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	vec3f intensity = vec3f(1,1,1);
	ray r(P, this->getDirection(P));
	this->getScene()->acc_shadow_attenuation(r, intensity);
	//bool isShadow = false;
	//while(this->getScene()->intersect(r, i) == true){
	//	if (i.getMaterial().kt.iszero()) {
	//		isShadow = true;
	//		break;
	//	}
	//	else {
	//		vec3f tmpP = r.at(i.t);
	//		ray tmpR(tmpP, this->getDirection(P));
	//		r = tmpR;
	//		intensity = prod(intensity, i.getMaterial().kt);
	//	}
	//}
	//if (isShadow)
	//	return vec3f(0, 0, 0);
	//else
	return intensity;
}

vec3f SpotLight::shadowAttenuation(const vec3f& P) const {
	isect i;
	vec3f intensity = vec3f(1, 1, 1);
	ray r(P, this->getDirection(P));
	bool isShadow = false;
	while (this->getScene()->intersect(r, i) == true) {
		if (i.getMaterial().kt.iszero()) {
			isShadow = true;
			break;
		}
		else {
			vec3f tmpP = r.at(i.t);
			ray tmpR(tmpP, this->getDirection(P));
			r = tmpR;
			intensity = prod(intensity, i.getMaterial().kt);
		}
	}
	if (isShadow)
		return vec3f(0, 0, 0);
	else
		return intensity;
}

double SpotLight::distanceAttenuation(const vec3f& P) const {
	
	double cA = traceUI->getConstAttenuation();
	double lA = traceUI->getLinearAttenuation();
	double qA = traceUI->getQuadraAttenuation();
	double atten = ((position - P).length_squared()*qA + (position - P).length()*lA + cA);
	if (abs(atten) <= RAY_EPSILON)
		atten = 1;
	else
		atten = minimum(1.0, 1.0 / atten);

	double angle = acos((this->getDirection(P)).dot(this->getAxis(P)));
	if (angle > coneAngle)
		return 0.0;
	else if (angle > coneAngle - coneDeltaAngle && angle <= coneAngle)
		return atten*pow(cos(angle), beamDistribution)*(1 - 1 / (coneDeltaAngle*coneDeltaAngle)*pow(angle - (coneAngle - coneDeltaAngle), 2));
	else
		return atten*pow(cos(angle), beamDistribution);
}

vec3f SpotLight::getDirection(const vec3f& P) const {
	return (position - P).normalize();
}

vec3f SpotLight::getColor(const vec3f& P) const {
	return color;
}

vec3f SpotLight::getAxis(const vec3f& P) const {
	return -orientation.normalize();
}

vec3f WarnLight::shadowAttenuation(const vec3f& P) const
{
	// YOUR CODE HERE:
	// You should implement shadow-handling code here.
	vec3f intensity = vec3f(1, 1, 1);
	ray r(P, this->getDirection(P));
	this->getScene()->acc_shadow_attenuation(r, intensity);
	//bool isShadow = false;
	//while(this->getScene()->intersect(r, i) == true){
	//	if (i.getMaterial().kt.iszero()) {
	//		isShadow = true;
	//		break;
	//	}
	//	else {
	//		vec3f tmpP = r.at(i.t);
	//		ray tmpR(tmpP, this->getDirection(P));
	//		r = tmpR;
	//		intensity = prod(intensity, i.getMaterial().kt);
	//	}
	//}
	//if (isShadow)
	//	return vec3f(0, 0, 0);
	//else
	return intensity;
}

double WarnLight::distanceAttenuation(const vec3f& P) const {

	double cA = traceUI->getConstAttenuation();
	double lA = traceUI->getLinearAttenuation();
	double qA = traceUI->getQuadraAttenuation();
	double atten = ((position - P).length_squared()*qA + (position - P).length()*lA + cA);
	if (abs(atten) <= RAY_EPSILON)
		atten = 1;
	else
		atten = minimum(1.0, 1.0 / atten);
	bool in_range = (P[0] >= x_min && P[0] <= x_max && P[1] >= y_min && P[1] <= y_max && P[2] >= z_min && P[2] <= z_max);
	if (!in_range)
		return 0.0;
	double angle = acos((this->getDirection(P)).dot(this->getAxis(P)));
	if (angle > coneAngle)
		return 0.0;
	else {
		double r = pow(cos(angle), p_factor);
		return r * atten;
	}
}

vec3f WarnLight::getDirection(const vec3f& P) const {
	return (position - P).normalize();
}

vec3f WarnLight::getColor(const vec3f& P) const {
	return color;
}

vec3f WarnLight::getAxis(const vec3f& P) const {
	return -orientation.normalize();
}