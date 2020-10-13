#include "rtweekend.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "moving_sphere.h"
#include "bvh.h"
#include "aarect.h"
#include "box.h"
#include "constant_medium.h"

#include <iostream>

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	// If the ray hits nothing, return the background color.
	if (!world.hit(r, 0.001, infinity, rec))
		return background;

	ray scattered;
	color attenuation;
	color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

////Ray color with scattered reflectance
////color ray_color(const ray& r, const hittable& world, int depth) {
//color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
//	hit_record rec;
//
//	// If we've exceeded the ray bounce limit, no more light is gathered.
//	if (depth <= 0)
//		return color(0, 0, 0);
//
//	if (world.hit(r, 0.001, infinity, rec)) {
//		//ray scattered;
//		//color attenuation;
//		//if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
//		//	return attenuation;
//		//	// return attenuation * ray_color(scattered, world, depth - 1);
//		//return color(0, 0, 0);
//		return background;
//	}
//
//	ray scattered;
//	color attenuation;
//	color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
//
//	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
//		return emitted;
//
//	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
//
//	/*vec3 unit_direction = unit_vector(r.direction());
//	auto t = 0.5 * (unit_direction.y() + 1.0);
//	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);*/
//}

// random scene
hittable_list random_scene() {
	hittable_list world;

	// auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	/*auto ground_material = make_shared<lambertian>(make_shared<solid_color>(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));*/
	// with checker
	auto checker = make_shared<checker_texture>(
		make_shared<solid_color>(0.2, 0.3, 0.1),
		make_shared<solid_color>(0.9, 0.9, 0.9)
		);

	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					//diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(make_shared<solid_color>(albedo));
					// add for moving spheres
					auto center2 = center + vec3(0, random_double(0, 0.5), 0);
					// world.add(make_shared<sphere>(center, 0.2, sphere_material));
					world.add(make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
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

	auto material2 = make_shared<lambertian>(make_shared<solid_color>(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return hittable_list(make_shared<bvh_node>(world, 0.0, 1.0));
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(
        make_shared<solid_color>(0.2, 0.3, 0.1),  // green
        make_shared<solid_color>(0.9, 0.9, 0.9) //close to white
    );

    objects.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));  // lower sphere
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));  // upper sphere

    return objects;
}

hittable_list two_perlin_spheres() {
	hittable_list objects;

	auto pertext = make_shared<noise_texture>(5.0);
	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	return objects;
}

//hittable_list earth() {
//	auto earth_texture = make_shared<image_texture>("earthmap.jpg");
//	auto earth_surface = make_shared<lambertian>(earth_texture);
//	auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);
//
//	return hittable_list(globe);
//}

hittable_list simple_light() {
	hittable_list objects;

	auto pertext = make_shared<noise_texture>(4);
	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

	auto difflight = make_shared<diffuse_light>(make_shared<solid_color>(4, 4, 4));
	objects.add(make_shared<sphere>(point3(0, 7, 0), 2, difflight));
	objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

	return objects;
}

hittable_list cornell_box() {
	hittable_list objects;

	auto red = make_shared<lambertian>(make_shared<solid_color>(.65, .05, .05));
	auto white = make_shared<lambertian>(make_shared<solid_color>(.73, .73, .73));
	auto green = make_shared<lambertian>(make_shared<solid_color>(.12, .45, .15));
	auto light = make_shared<diffuse_light>(make_shared<solid_color>(15, 15, 15));

	objects.add(make_shared<flip_face>(make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.add(make_shared<flip_face>(make_shared<xz_rect>(0, 555, 0, 555, 555, white)));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<flip_face>(make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

	// add two boxes
	/*objects.add(make_shared<box>(point3(130, 0, 65), point3(295, 165, 230), white));
	objects.add(make_shared<box>(point3(265, 0, 295), point3(430, 330, 460), white));*/

	//rotate and translate two boxes --- standard cornell box
	shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));
	objects.add(box1);

	shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));
	objects.add(box2);

	return objects;
}

hittable_list cornell_smoke() {
	hittable_list objects;

	auto red = make_shared<lambertian>(make_shared<solid_color>(.65, .05, .05));
	auto white = make_shared<lambertian>(make_shared<solid_color>(.73, .73, .73));
	auto green = make_shared<lambertian>(make_shared<solid_color>(.12, .45, .15));
	auto light = make_shared<diffuse_light>(make_shared<solid_color>(7, 7, 7));

	objects.add(make_shared<flip_face>(make_shared<yz_rect>(0, 555, 0, 555, 555, green)));
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
	objects.add(make_shared<flip_face>(make_shared<xz_rect>(0, 555, 0, 555, 555, white)));
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(make_shared<flip_face>(make_shared<xy_rect>(0, 555, 0, 555, 555, white)));

	shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box1 = make_shared<rotate_y>(box1, 15);
	box1 = make_shared<translate>(box1, vec3(265, 0, 295));

	shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	box2 = make_shared<rotate_y>(box2, -18);
	box2 = make_shared<translate>(box2, vec3(130, 0, 65));

	objects.add(make_shared<constant_medium>(box1, 0.01, make_shared<solid_color>(0, 0, 0)));
	objects.add(make_shared<constant_medium>(box2, 0.01, make_shared<solid_color>(1, 1, 1)));

	return objects;
}

hittable_list final_scene() {
	hittable_list boxes1;
	auto ground = make_shared<lambertian>(make_shared<solid_color>(0.48, 0.83, 0.53));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double(1, 101);
			auto z1 = z0 + w;

			boxes1.add(make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
		}
	}

	hittable_list objects;

	objects.add(make_shared<bvh_node>(boxes1, 0, 1));

	auto light = make_shared<diffuse_light>(make_shared<solid_color>(7, 7, 7));
	objects.add(make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	auto center1 = point3(400, 400, 200);
	auto center2 = center1 + vec3(30, 0, 0);
	auto moving_sphere_material =
		make_shared<lambertian>(make_shared<solid_color>(0.7, 0.3, 0.1));
	objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

	objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
	objects.add(make_shared<sphere>(
		point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 10.0)
		));

	auto boundary = make_shared<sphere>(point3(360, 150, 145), 70, make_shared<dielectric>(1.5));
	objects.add(boundary);
	objects.add(make_shared<constant_medium>(
		boundary, 0.2, make_shared<solid_color>(0.2, 0.4, 0.9)
		));
	boundary = make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
	objects.add(make_shared<constant_medium>(
		boundary, .0001, make_shared<solid_color>(1, 1, 1)));

	//auto emat = make_shared<lambertian>(make_shared<image_texture>("earthmap.jpg"));
	//objects.add(make_shared<sphere>(point3(400, 200, 400), 100, emat));
	auto pertext = make_shared<noise_texture>(0.1);
	objects.add(make_shared<sphere>(point3(220, 280, 300), 80, make_shared<lambertian>(pertext)));

	hittable_list boxes2;
	auto white = make_shared<lambertian>(make_shared<solid_color>(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
	}

	objects.add(make_shared<translate>(
		make_shared<rotate_y>(
			make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
		vec3(-100, 270, 395)
		)
	);

	return objects;
}

int main()
{
	/*const int image_width = 256;
	const int image_height = 256;*/
	const auto aspect_ratio = 1.0 / 1.0;
	const int image_width = 300;
	//const auto aspect_ratio = double(image_width) / image_height;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 1000;
	const int max_depth = 50;
	const color background(0, 0, 0);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255" << std::endl;
	

	// auto world = random_scene();

	// auto world = two_spheres();

	//auto world = two_perlin_spheres();
	//auto world = simple_light();
	// auto world = cornell_box();
	// auto world = cornell_smoke();
	auto world = final_scene();
	point3 lookfrom(278, 278, -800);
	point3 lookat(278, 278, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.0;
	auto vfov = 40.0;

	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
	//auto world = earth();
	/*point3 lookfrom = point3(0, 0, 12);
	point3 lookat = point3(0, 0, 0);
	auto dist_to_focus = 20.0;*/
	

	// two_perlin_spheres() look
	//point3 lookfrom(13, 2, 3);
	//point3 lookat(0, 0, 0);
	//vec3 vup(0, 1, 0);
	//auto dist_to_focus = 10.0;
	////// time0 and time1 apperture captuer
	//auto aperture = 0.0;

	//// camera from time0 to time1
	//camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color;
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, background, world, max_depth);
			}
			write_color(std::cout, pixel_color, samples_per_pixel);
		}
	}


	//for (int j = image_height - 1; j >= 0; --j)
	//{
	//	std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
	//	for (int i = 0; i < image_width; ++i)
	//	{
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
	//		color pixel_color(0, 0, 0);
	//		// add camera and use samples
	//		for (int s = 0; s < samples_per_pixel; ++s) {
	//			auto u = (i + random_double()) / (image_width - 1);
	//			auto v = (j + random_double()) / (image_height - 1);
	//			ray r = cam.get_ray(u, v);
	//			//pixel_color += ray_color(r, world);
	//			// pixel_color += ray_color(r, world, max_depth);
	//			pixel_color += ray_color(r, background, world, max_depth);
	//		}
	//		write_color(std::cout, pixel_color, samples_per_pixel);


	//	}
	//}
	std::cerr << "\nDone.\n";
}
