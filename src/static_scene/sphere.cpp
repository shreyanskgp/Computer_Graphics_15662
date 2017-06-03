#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CMU462 { namespace StaticScene {

bool Sphere::test(const Ray& r, double& t1, double& t2) const {

  // TODO:
  // Implement ray - sphere intersection test.
  // Return true if there are intersections and writing the
  // smaller of the two intersection times in t1 and the larger in t2.

	//Solving the intersection using quadratic form ax2 + bx + c = 0
	double a, b, c, det;

	a = dot(r.d, r.d);
	b = 2.*dot(r.o - o, r.d);
	c = dot(r.o, r.o) + dot(o, o) - 2.*dot(r.o, o) - r2;
	det = b*b - 4.*a*c;

	if (det<0.)
	{
		return false;
	}
	if (det == 0.)
	{
		t1 = -b / (2. * a);
		t2 = t1;
		return true;
	}
	
	t1 = (-b - sqrt(det)) / (2.*a);
	t2 = (-b + sqrt(det)) / (2.*a);
	
	return true;

}

bool Sphere::intersect(const Ray& r) const {

  // TODO:
  // Implement ray - sphere intersection.
  // Note that you might want to use the the Sphere::test helper here.
	Intersection i;
  return intersect(r,&i);

}

bool Sphere::intersect(const Ray& r, Intersection *i) const {

  // TODO:
  // Implement ray - sphere intersection.
  // Note again that you might want to use the the Sphere::test helper here.
  // When an intersection takes place, the Intersection data should be updated
  // correspondingly.
	double t1, t2;
	bool output;

	output = test(r, t1, t2);
	//if (output && t1 < 0. && t2<=r.max_t && t2>=r.min_t) printf("Ganda");
	if (output)
	{
		if (t1 >= 0. && t2 >= 0.)
		{
			if (t1 <= r.max_t && t1 >= r.min_t && t1 < i->t)
			{
				r.max_t = t1;
				//Ray's t value of hit point
				i->t = t1;
				//Normal
				i->n = normal(r.o + r.d*t1);
				i->n.normalize();
				//Primitive
				i->primitive = this;
				//BSDF
				i->bsdf = get_bsdf();
				return output;
			}
		}	
		
		if (t2 <= r.max_t && t2 >= r.min_t && t2 < i->t)
		{
			r.max_t = t2;
			//Ray's t value of hit point
			i->t = t2;
			//Normal
			i->n = normal(r.o + r.d*t2);
			i->n.normalize();
			//Primitive
			i->primitive = this;
			//BSDF
			i->bsdf = get_bsdf();
			return output;
		}
		else
		{
			output = false;
			//return output;
		}
		
		
	}
	else
	{
		output = false;
	}
	
  return output;

}

void Sphere::draw(const Color& c) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color& c) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CMU462
