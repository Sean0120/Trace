// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include"ui\TraceUI.h"
#include "fileio\bitmap.h"
#include<cstdlib>
#include<ctime>
extern TraceUI* traceUI;
// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
	ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));
	if (traceUI->m_nAdaptive == FALSE) {
		if (traceUI->m_nSamplingSize == 0) {
			scene->getCamera()->rayThrough(x, y, r);
			index = stack<double>();
			index.push(1.0);
			return traceRay(scene, r, vec3f(threshold, threshold, threshold), traceUI->getDepth()).clamp();
		}
		else
		{
			int samplesize = traceUI->m_nSamplingSize;
			vec3f sum(0, 0, 0);
			for (int i = 0; i < samplesize; i++) {
				for (int j = 0; j < samplesize; j++) {
					double rx = double(rand() % 10000) / 10000 - 0.5;
					double ry = double(rand() % 10000) / 10000 - 0.5;
					//cout << rx << " " << ry << endl;
					//scene->getCamera()->rayThrough(x, y, r);
					scene->getCamera()->rayThrough(x + (rx - double(samplesize-1) / 2 + i) / buffer_width, y + (ry - double(samplesize-1) / 2 + j) / buffer_height, r);
					index = stack<double>();
					index.push(1.0);

					sum += traceRay(scene, r, vec3f(threshold, threshold, threshold), traceUI->getDepth()).clamp();

				}
			}
			sum = sum / (samplesize*samplesize);
			return sum;
		}
	}
	else
	{
		return AdaptiveSampling(scene,0, x, y);
	}
}

//do recursive for adaptive sampling
vec3f RayTracer::AdaptiveSampling(Scene *scene,int depth, double x, double y) {
	ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));	
	vec3f sum(0, 0, 0);
	vec3f ray4[4];
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			scene->getCamera()->rayThrough(x + ( i- 0.5 ) / buffer_width/ pow(3,depth), y + (j- 0.5 ) / buffer_height / pow(3, depth), r);
			index = stack<double>();
			index.push(1.0);
			ray4[2*i+j]= traceRay(scene, r, vec3f(threshold, threshold, threshold), traceUI->getDepth()).clamp();
			sum += ray4[2 * i + j];
		}
	}
	sum = sum / (2*2);
	double error = 0; 
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			error += fabsf(sum[i] - ray4[j][i]);
		}
	}
	if (error < 0.1)
	{
		return sum;
	}
	else if (depth == 2) {
		return sum;
	}
	else
	{
		++depth;
		vec3f sum(0, 0, 0);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				sum += AdaptiveSampling(scene, depth , x +(i-1)/pow(3,depth) / buffer_width, y + (j - 1) / pow(3, depth) / buffer_height);
			}
		}
		return sum/9;

	}
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth )
{	
	//if (depth <= 0)
		//return vec3f(0, 0, 0); //terminate recursion

	isect i;

	if( scene->intersect( r, i ) ) {
		// YOUR CODE HERE

		// An intersection occured!  We've got work to do.  For now,
		// this code gets the material for the surface that was intersected,
		// and asks that material to provide a color for the ray.  

		// This is a great place to insert code for recursive ray tracing.
		// Instead of just returning the result of shade(), add some
		// more steps: add in the contributions from reflected and refracted
		// rays.
		double curI = index.top();	//index of current material
		vec3f P = r.at(i.t);	//intersection point
		//cout << P << endl;
		const Material& m = i.getMaterial();
		if (depth <= 0)		//terminating recursion
			return m.shade(scene, r, i);

		//bool get_out = (abs(curI - m.index) < RAY_EPSILON);	//check get out or in
		vec3f uN = i.N.normalize();
		vec3f I = m.shade(scene, r, i);
		
		index.push(curI);		
		vec3f uL = -(r.getDirection().normalize());
		//vec3f uN = i.N.normalize();
		vec3f uR = (2 * (uN.dot(uL))*uN - uL).normalize();
		ray reflected_ray(P, uR);
		double intensity_0 = pow(m.kr[0], traceUI->getDepth() - depth);
		double intensity_1 = pow(m.kr[1], traceUI->getDepth() - depth);
		double intensity_2 = pow(m.kr[2], traceUI->getDepth() - depth);
		if ((intensity_0 > thresh[0] || intensity_1 > thresh[1]|| intensity_2 > thresh[2])&&!m.kr.iszero())
			I = I + prod(m.kr,traceRay(scene, reflected_ray, thresh, depth - 1));
		index.pop();

		bool get_out = (uL.dot(uN) < 0);
		if (get_out) {
			uN = -uN;
			index.push(1.0);
		}
		else
			index.push(m.index);
		double ratio = curI / index.top();
		double cosi = uL.dot(uN);
		bool total_reflect = (sqrt(1 - cosi * cosi)>= 1/ratio);
		if (!total_reflect) {
			double cost = sqrt(1 - ratio * ratio*(1 - cosi * cosi));
			vec3f uT = ((ratio*cosi - cost)*uN - ratio * uL).normalize();
			ray refracted_ray(P, uT);

			intensity_0 = pow(m.kt[0], traceUI->getDepth() - depth);
			intensity_1 = pow(m.kt[1], traceUI->getDepth() - depth);
			intensity_2 = pow(m.kt[2], traceUI->getDepth() - depth);
			if ((intensity_0 > thresh[0] || intensity_1 > thresh[1] || intensity_2 > thresh[2])&&!m.kt.iszero())
				I = I + prod(m.kt, traceRay(scene, refracted_ray, thresh, depth - 1));

		}
		else {
			intensity_0 = pow(m.kr[0], traceUI->getDepth() - depth);
			intensity_1 = pow(m.kr[1], traceUI->getDepth() - depth);
			intensity_2 = pow(m.kr[2], traceUI->getDepth() - depth);
			if ((intensity_0 > thresh[0] || intensity_1 > thresh[1] || intensity_2 > thresh[2]) && !m.kr.iszero())
				I = I + prod(vec3f(1,1,1)-m.kr, traceRay(scene, reflected_ray, thresh, depth - 1));
		}
		index.pop();

		return I.clamp();
	
	} else {
		// No intersection.  This ray travels to infinity, so we color
		// it according to the background color, which in this (simple) case
		// is just black.
		if (background_loaded) {
			//vec3f d = r.getDirection();
			//vec3f p = r.getPosition();
			vec3f x = scene->getCamera()->getU();
			vec3f y = scene->getCamera()->getV();
			vec3f z = scene->getCamera()->getLook();
			double dis_x = r.getDirection().dot(x);
			double dis_y = r.getDirection().dot(y);
			double dis_z = r.getDirection().dot(z);
			return getBackgroundPixel(background_width*(dis_x / dis_z + 0.5), (dis_y / dis_z + 0.5)*background_height);
		}

		return vec3f( 0.0, 0.0, 0.0 );
	}
}

RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	background = NULL;
	background_loaded = false;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;

	if (background) delete[] background;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

bool RayTracer::loadBackground(char* fn) {
	unsigned char* data;
	int width, height;

	if ((data = readBMP(fn, width, height)) == NULL) {
		fl_alert("Can't load bitmap file");
		background_loaded = false;
		return false;
	}

	// reflect the fact of loading the new image
	background_width = width;
	background_height = height;

	//delete old
	if (background) delete[] background;
	background = data;

	background_loaded = true;
	return true;
}

vec3f RayTracer::getBackgroundPixel(int x, int y) {
	if (x < 0)
		x = 0;
	else if (x >= background_width)
		x = background_width - 1;
	if (y < 0)
		y = 0;
	else if (y >= background_height)
		y = background_height - 1;

	double r = ((double)(*(background + 3 * (y*background_width + x)))) / 256.0;
	double g = ((double)(*(background + 3 * (y*background_width + x)+1))) / 256.0;
	double b = ((double)(*(background + 3 * (y*background_width + x)+2))) / 256.0;
	
	return vec3f(r, g, b);
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( scene,x,y );

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}