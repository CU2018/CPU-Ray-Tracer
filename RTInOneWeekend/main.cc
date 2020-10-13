#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <iostream>

//// bool hit_sphere(const vec3& center, double radius, const ray& r) {
//double hit_sphere(const vec3 & center, double radius, const ray & r) {
//	vec3 oc = r.origin() - center;  // vec( a -c )
//	//auto a = dot(r.direction(), r.direction());  // t-squared
//	//auto b = 2.0 * dot(oc, r.direction());
//	//auto c = dot(oc, oc) - radius * radius;
//	//auto discriminant = b * b - 4 * a * c;  //b^2 - 4ac  for getting positive or negative
//
//	// simplified version of dsicriminant
//	auto a = r.direction().length_squared();
//	auto half_b = dot(oc, r.direction());
//	auto c = oc.length_squared() - radius * radius;
//	auto discriminant = half_b * half_b - a * c;
//	// return (discrimiinant > 0);  // return true if positive -- hit the sphere 2 intersections
//	if (discriminant < 0) {
//		return -1.0;
//	}
//	else {
//		// return (-b - sqrt(discriminant)) / (2.0 * a);
//		return (-half_b - sqrt(discriminant)) / a;
//	}
//}
//
//// linearly blends white and blue depending on the height of the y coordinate after scaling y to unit length
//color ray_color(const ray& r) {
//	//if (hit_sphere(point3(0, 0, -1), 0.5, r)) {
//	//	return color(1, 0, 0);  // red sphere
//	//}
//	auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
//	if (t > 0.0) {  // hit the sphere; -1 not hit
//		vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));  // end point of the ray (intersection point) minus origin of the sphere
//		return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
//	}
//	// -1 not hit the sphere
//	vec3 unit_direction = unit_vector(r.direction());  // get the unit vector of direction vec
//	// auto t = 0.5 * (unit_direction.y() + 1.0); // get the doulbe t
//	t = 0.5 * (unit_direction.y() + 1.0);  // calculate the end point of t
//	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);  // calculate the color ray  white_blue
//	//return (1.0 - t) * color(0.5, 0.7, 1.0) + t * color(.0, 1.0, 1.0);  //blue_white
//}

//color ray_color(const ray& r, const hittable& world) {
//	hit_record rec;
//	if (world.hit(r, 0, infinity, rec)) {
//		//point3 target = rec.p + rec.normal + random_in_unit_sphere();
//		//return 0.5 * ray_color(ray(rec.p, target - rec.p), world);
//		return 0.5 * (rec.normal + color(1, 1, 1));
//		// use the random direction gernator
//		// point3 target = rec.p + rec.normal + random_in_unit_sphere(); // s = (P + vec3(n) + vec3(rand))
//		// return 0.5 * (ray_color(ray(rec.p, target - rec.p), world));  // s - p
//	}
//	vec3 unit_direction = unit_vector(r.direction());
//	auto t = 0.5 * (unit_direction.y() + 1.0);
//	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//}

// limited recursion
//color ray_color(const ray& r, const hittable& world, int depth) {
//	hit_record rec;
//	// If we've exceeded the ray bounce limit, no more light is gathered.
//	if (depth <= 0)
//		return color(0, 0, 0);
//
//	// 0.001 for fixing shadow acne
//	if (world.hit(r, 0, infinity, rec)) {
//		point3 target = rec.p + rec.normal + random_in_unit_sphere();
//		return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
//	}
//
//	vec3 unit_direction = unit_vector(r.direction());
//	auto t = 0.5 * (unit_direction.y() + 1.0);
//	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//	// return color(1.0, 0, 0);
//}

// lambertian reflection
//color ray_color(const ray& r, const hittable& world, int depth) {
//	hit_record rec;
//	// If we've exceeded the ray bounce limit, no more light is gathered.
//	if (depth <= 0)
//		return color(0, 0, 0);
//
//	// 0.001 for fixing shadow acne
//	if (world.hit(r, 0, infinity, rec)) {
//		point3 target = rec.p + rec.normal + random_unit_vector();
//		return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
//	}
//
//	vec3 unit_direction = unit_vector(r.direction());
//	auto t = 0.5 * (unit_direction.y() + 1.0);
//	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//	// return color(1.0, 0, 0);
//}

// alternative way to calculate diffuse light
//color ray_color(const ray& r, const hittable& world, int depth)
//{
//	hit_record rec;
//	// if exceed the ray bounce limit, no more light is gathered
//	if (depth <= 0)
//		return color(0, 0, 0); 
//	if (world.hit(r, 0.001, infinity, rec)) {
//		point3 target = rec.p + random_in_hemisphere(rec.normal); // add the random normal in hemisphere only 
//		return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
//	}
//
//	vec3 unit_direction = unit_vector(r.direction());
//	auto t = 0.5 * (unit_direction.y() + 1.0);
//	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//}

//Ray color with scattered reflectance
color ray_color(const ray& r, const hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec)) {
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

// random scene
hittable_list random_scene() {
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					//diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					//metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}
	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

int main()
{
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 368;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255" << std::endl;
	// camera info
	//point3 lower_left_corner(-2.0, -1.0, -1.0);
	//point3 origin(0.0, 0.0, 0.0);
	//vec3 horizontal(4.0, 0.0, 0.0);  // x vector
	//vec3 vertical(0.0, 2, 0.0);  // y vector
	// point3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, 1); // get the lower left corner point
	
	
	//two spheres : one sphere at center, one ground
	//hittable_list world;
	//world.add(make_shared<sphere>(point3(0, 0, -1), 0.5)); // add the sphere
	//world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));   // ground


	// three spheres for relectance
	//hittable_list world;

	//world.add(make_shared<sphere>(
	//	point3(0, 0, -1), 0.5, make_shared<lambertian>(color(0.7, 0.3, 0.3))));  // middle ball


	//world.add(make_shared<sphere>(
	//	point3(0, -100.5, -1), 100, make_shared<lambertian>(color(0.8, 0.8, 0.0))));  // ground

	//// world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<metal>(color(.8, .6, .2), 1.0)));
	////world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, make_shared<metal>(color(.8, .8, .8), 1.0)));
	//world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<dielectric>(1.5)));  // right ball
	//world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5)));  // left ball

	// left: glass; middle: lambertain; right: metal
	/*world.add(make_shared<sphere>(
		point3(0, 0, -1), 0.5, make_shared<lambertian>(color(0.1, 0.2, 0.5))));

	world.add(make_shared<sphere>(
		point3(0, -100.5, -1), 100, make_shared<lambertian>(color(0.8, 0.8, 0.0))));

	world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<metal>(color(.8, .6, .2), 0.0)));
	world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5)));
	*/
	// camera cam(point3(-2, 2, 1), point3(0, 0, -1), vup, 90, aspect_ratio); // distant view
	// camera cam(point3(-2, 2, 1), point3(0, 0, -1), vup, 20, aspect_ratio);  // zoom in (change vfov from 90 to 20)
	// big apperture vs small apperture
	auto world = random_scene();
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	//auto aperture = 2.0;
	auto aperture = 0.1;

	// camera
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
	// left: hollow glass; middle: lambertain; right: metal
	//world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, make_shared<lambertian>(color(.1, .2, .5))));
	//world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, make_shared<lambertian>(color(.8, .8, 0.))));
	//world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<metal>(color(.8, .6, .2), 0.3)));
	//world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5))); // larger ball 
	//world.add(make_shared<sphere>(point3(-1, 0, -1), -0.45, make_shared<dielectric>(1.5)));  // smaller ball inside: normal is inside; radius is negative
	// camera
	//camera cam;

	// camera
	/*camera cam(90, double(image_width) / image_height);
	auto R = cos(pi / 4);
	world.add(make_shared<sphere>(point3(-R, 0, -1), R, make_shared<lambertian>(color(0, 0, 1))));
	world.add(make_shared<sphere>(point3(R, 0, -1), R, make_shared<lambertian>(color(1, 0, 0))));*/


	for (int j = image_height - 1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			/*auto g = double(i) / (image_width - 1);
			auto r = double(j) / (image_height - 1);
			auto b = 0.25;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			std::cout << ir << ' ' << ig << ' ' << ib << '\n';*/
			
			//color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
			//auto u = double(i) / (image_width - 1);
			//auto v = double(j) / (image_height - 1);
			//ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			//// color pixel_color = ray_color(r);
			//color pixel_color = ray_color(r, world);
			//write_color(std::cout, pixel_color);
			color pixel_color(0, 0, 0);
			// add camera and use samples
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				//pixel_color += ray_color(r, world);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}
	std::cerr << "\nDone.\n";
}
