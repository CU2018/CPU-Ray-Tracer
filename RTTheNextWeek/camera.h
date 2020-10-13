// camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:
	//camera() {
	//	lower_left_corner = vec3(-2.0, -1.0, -1.0);
	//	horizontal = vec3(4.0, 0.0, 0.0);
	//	vertical = vec3(0.0, 2.0, 0.0);
	//	origin = point3(0.0, 0.0, 0.0);
	//}
	camera(point3 lookfrom,
		point3 lookat,
		vec3   vup,
		double vfov,
		double aspect_ratio,
		double aperture,
		double focus_dist,
		double t0 = 0,
		double t1 = 0
	) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		// auto focal_length = 1.0;

		/*origin = point3(0.0, 0.0, 0.0);
		horizontal = vec3(viewport_width, 0.0, 0.0);
		vertical = vec3(0.0, viewport_height, 0.0);
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0.0, 0.0, focal_length);*/
		origin = lookfrom;
	/*	horizontal = viewport_width * u;
		vertical = viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;*/
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 -focus_dist * w;

		lens_radius = aperture / 2;
		time0 = t0;
		time1 = t1;
	}

	ray get_ray(double s, double t) const { 
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		// return ray(origin+offset, lower_left_corner + s*horizontal + t*vertical - origin-offset);
		return ray(
			origin + offset, 
			lower_left_corner + s * horizontal + t * vertical - origin - offset,
			random_double(time0, time1)); // generating rays at a random time between time1 and time2
	}

private:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens_radius;  // indicating the blurness
	double time0, time1;   // for generating rays at a random time between time1 and time2
};

#endif // !CAMERA_H
