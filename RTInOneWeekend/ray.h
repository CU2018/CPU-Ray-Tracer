#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
		ray() {}
		ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction){}

		point3 origin() const { return orig; }  // return the origin point
		vec3 direction() const { return dir; }  // return the direction vector

		point3 at(double t) const {
			return orig + t * dir;  // return the end point
		}

public:
	point3 orig;  // A origin point
	vec3 dir; // direction vector
};

#endif // ! RAY_H
