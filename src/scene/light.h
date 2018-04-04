#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"
#include "../ui/TraceUI.h"
extern TraceUI* traceUI;

class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;

protected:
	Light( Scene *scene, const vec3f& col )
		: SceneElement( scene ), color( col ) {}

	vec3f 		color;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
		: Light( scene, color ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;

protected:
	vec3f 		orientation;
};

class PointLight
	: public Light
{
public:
	PointLight( Scene *scene, const vec3f& pos, const vec3f& color )
		: Light( scene, color ), position( pos ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;
	void set_constant_falloff(double constant) {
		constant_falloff = constant;
		traceUI->setConstAttenuation(constant);
	}
	void set_linear_falloff(double linear) {
		linear_falloff = linear;
		traceUI->setLinearAttenuation(linear);
	}
	void set_quadratic_falloff(double quadratic) {
		quadratic_falloff = quadratic;
		traceUI->setQuadraAttenuation(quadratic);
	}
protected:
	vec3f position;
	double constant_falloff;
	double linear_falloff;
	double quadratic_falloff;
};

class SpotLight
	:public Light
{
public:
	SpotLight(Scene *scene, const vec3f& color, const vec3f& pos, const vec3f& ori)
		: Light(scene, color), position(pos), orientation(ori) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation(const vec3f& P) const;
	virtual vec3f getColor(const vec3f& P) const;
	virtual vec3f getDirection(const vec3f& P) const;
	virtual vec3f getAxis(const vec3f& P) const;
	void set_constant_falloff(double constant) {
		constant_falloff = constant;
		traceUI->setConstAttenuation(constant);
	}
	void set_linear_falloff(double linear) {
		linear_falloff = linear;
		traceUI->setLinearAttenuation(linear);
	}
	void set_quadratic_falloff(double quadratic) {
		quadratic_falloff = quadratic;
		traceUI->setQuadraAttenuation(quadratic);
	}

	void set_coneAngle(double angle) {
		coneAngle = angle;
	}

	void set_coneDeltaAngle(double delta) {
		coneDeltaAngle = delta;
	}

	void set_beamDistribution(double distribution) {
		beamDistribution = distribution;
	}

protected:
	vec3f position;
	vec3f orientation;
	double coneAngle;
	double coneDeltaAngle;
	double beamDistribution;
	double constant_falloff;
	double linear_falloff;
	double quadratic_falloff;
};

#endif // __LIGHT_H__
