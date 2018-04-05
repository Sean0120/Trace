#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class Camera
{
public:
    Camera();
    void rayThrough( double x, double y, ray &r );
    void setEye( const vec3f &eye );
    void setLook( double, double, double, double );
    void setLook( const vec3f &viewDir, const vec3f &upDir );
    void setFOV( double );
    void setAspectRatio( double );

	vec3f getU() { return u; }
	vec3f getV() { return v; }
	vec3f getLook() { return look; }

    double getAspectRatio() { return aspectRatio; }
private:
    mat3f m;                     // rotation matrix
    double normalizedHeight;    // dimensions of image place at unit dist from eye
    double aspectRatio;
    
    void update();              // using the above three values calculate look,u,v
    
    vec3f eye;
    vec3f look;                  // direction to look
    vec3f u,v;                   // u and v in the 
};

#endif
