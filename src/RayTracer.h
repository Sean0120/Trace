#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/ray.h"
#include <stack>

class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    vec3f trace( Scene *scene, double x, double y );
	vec3f traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth );
	//for background
	bool loadBackground(char* fn);
	vec3f getBackgroundPixel(int x, int y);

	void getBuffer( unsigned char *&buf, int &w, int &h );
	double aspectRatio();
	void traceSetup( int w, int h );
	void traceLines( int start = 0, int stop = 10000000 );
	void tracePixel( int i, int j );

	bool loadScene( char* fn );

	bool sceneLoaded();
	void traceThreshold(double value) {
		threshold = value;
	}
	vec3f AdaptiveSampling(Scene *scene,int depth, double x, double y);
	Scene *scene;

private:
	unsigned char *buffer;
	int buffer_width, buffer_height;
	int bufferSize;
	double threshold;
	
	bool m_bSceneLoaded;

	stack<double> index;

	//for background
	bool background_loaded;
	unsigned char* background;
	int background_width;
	int background_height;
};

#endif // __RAYTRACER_H__
