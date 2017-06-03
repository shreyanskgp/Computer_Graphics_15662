#include "triangle.h"

#include "CMU462/CMU462.h"
#include "GL/glew.h"

namespace CMU462 { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, vector<size_t>& v) :
	mesh(mesh), v(v) { }
Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {
  
  // TODO: 
  // compute the bounding box of the triangle
	
	
	BBox tri_BBox = BBox(mesh->positions[v1]);
	tri_BBox.expand(mesh->positions[v2]);
	tri_BBox.expand(mesh->positions[v3]);
	
    return tri_BBox;
  
	//return BBox();
}

bool Triangle::intersect(const Ray& r) const {
  
  // TODO: implement ray-triangle intersection

  Intersection isect;
  
  return intersect(r, &isect);
}

bool Triangle::intersect(const Ray& r, Intersection *isect) const {
  
  // TODO: 
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly

  /*Based on implementation using cross and dot products of vectors
  The three vertices of the triangle be p0, p1 and p2
  The ray is defined by origin o and direction d

  Defining parameters
  e1 = p1-p0, e2= p2-p0 and s = o-p0

  Barycentric coordinates are given by
  u_bar = -((s X e2).d)/((e1 X d).e2)
  v_bar = ((e1 X d).s)/((e1 X d).e2)
  t = -((s X e2).e1)/((e1 X d).e2)
  */

	
	//First checking if BBox intersects
	//BBox tri_bbox = get_bbox();
	//double bb_t0, bb_t1;
	//if (tri_bbox.intersect(r, bb_t0, bb_t1))
	//{
		Vector3D e1, e2, d, s;
		double u_bar, v_bar, t, denom;

		e1 = mesh->positions[v2] - mesh->positions[v1];
		e2 = mesh->positions[v3] - mesh->positions[v1];
		s = r.o - mesh->positions[v1];
		d = r.d;

		denom = dot(cross(e1, d), e2);

		if (denom == 0.) return false; // If the ray is parallel to the plane of triangle

		u_bar = -dot(cross(s, e2), d) / denom;
		v_bar = dot(cross(e1, d), s) / denom;
		t = -dot(cross(s, e2), e1) / denom;

		//Checking validity of barycentric coordinates
		if (u_bar >= 0. && u_bar <= 1. && v_bar >= 0. && v_bar <= 1. && u_bar + v_bar <= 1.)
		{
			if (r.min_t <= t && r.max_t >= t && t< isect->t) //Checking validity of intersection
			{
				r.max_t = t;
				//Ray's t value of hit point
				isect->t = t;
				//Normal based on barycentric coordinates of vertices
				isect->n = mesh->normals[v1] * (1. - u_bar - v_bar) + mesh->normals[v2] * u_bar + mesh->normals[v3] * v_bar;
				isect->n.normalize();
				if (dot(isect->n, -r.d)<0.)
				{
					isect->n = -isect->n;
				}
				//Primitive
				isect->primitive = this;
				//BSDF
				isect->bsdf = mesh->get_bsdf();
				return true;
			}
			else
			{
				return false;
			}
		}
		return false;
	//}
	//return false;
}

void Triangle::draw(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CMU462
