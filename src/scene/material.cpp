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
	/*vec3f I = ke;
	vec3f Ia=prod(ka, scene->getAmbientLight());
	vec3f trans = vec3f(1, 1, 1) - kt;
	Ia = prod(Ia, trans);
	I = I + Ia;
	vec3f P = r.at(i.t);
	for (Scene::cliter it = scene->beginLights(); it != scene->endLights(); it++) {
		vec3f diffuse_term, spec_term;
		vec3f uN = i.N.normalize();
		vec3f uL = (*it)->getDirection(r.at(i.t)).normalize();
		diffuse_term = prod(kd *maximum(0, uN.dot(uL)),trans);
		
		vec3f uR = (2 * (uN.dot(uL))*uN - uL).normalize();
		vec3f uV = -(r.getDirection().normalize());
		spec_term = ks * pow(maximum(0, uR.dot(uV)), shininess*128.0);

		vec3f atten = prod((*it)->getColor(P) ,(*it)->shadowAttenuation(P)*(*it)->distanceAttenuation(P));
		I += prod(atten, (diffuse_term + spec_term));
	}
	return I.clamp();*/
	vec3f Ia = prod(ka, scene->getAmbientLight());
	vec3f trans = vec3f(1, 1, 1) - kt;
	Ia = prod(Ia, trans);
	vec3f I = ke + Ia;
	vec3f P = r.at(i.t);
	for (Scene::cliter it = scene->beginLights(); it != scene->endLights(); it++) {
		vec3f diffuse_term, spec_term;
		vec3f uN = i.N.normalize();
		vec3f uL = (*it)->getDirection(r.at(i.t)).normalize();
		if (traceUI->m_nTexture) {

			double x = 0;
			double y = 0;
			i.obj->isectToTexture(i, P, x, y);
			x *= scene->m_nTextureWidth;
			y *= scene->m_nTextureHeight;
			if (y < 0) {
				y = 0;
			}
			if (y >= scene->m_nTextureHeight)
			{
				y = scene->m_nTextureHeight - 1;
			}
			if (x < 0) {
				x = 0;
			}
			if (x >= scene->m_nTextureWidth)
			{
				x = scene->m_nTextureWidth - 1;
			}
			//cout << x << " " << y << " " << scene->m_nTextureWidth << " " << scene->m_nTextureHeight << endl;
			vec3f Texture(scene->m_ucBitmap[3 * (int(y) * scene->m_nTextureWidth + int(x))] / 255.0,
				scene->m_ucBitmap[3 * (int(y) * scene->m_nTextureWidth + int(x)) + 1] / 255.0,
				scene->m_ucBitmap[3 * (int(y) * scene->m_nTextureWidth + int(x)) + 2] / 255.0);
			//std::cout << Texture << std::endl;
			diffuse_term = prod(Texture *maximum(0, uN.dot(uL)), trans);
			//cout << int(3 * (y * scene->m_nTextureWidth + x)) << endl;
			//cout << Texture << endl;
			//cout << "_______________________________" << endl;
		}
		else {
			diffuse_term = prod(kd *maximum(0, uN.dot(uL)), trans);
		}

		vec3f uR = (2 * (uN.dot(uL))*uN - uL).normalize();
		vec3f uV = -(r.getDirection().normalize());
		spec_term = ks * pow(maximum(0, uR.dot(uV)), shininess*128.0);

		vec3f atten = prod((*it)->getColor(P), (*it)->shadowAttenuation(P)*(*it)->distanceAttenuation(P));
		I += prod(atten, (diffuse_term + spec_term));
	}
	//I += ke;
	return I.clamp();
}
