#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CMU462 {

  bool BBox::intersect(const Ray& r, double& t0, double& t1) const {

    // TODO:
    // Implement ray - bounding box intersection test
    // If the ray intersected the bouding box within the range given by
    // t0, t1, update t0 and t1 with the new intersection times.
	  //double tx1 = (min.x - r.o.x)*r.inv_d.x;
	  //double tx2 = (max.x - r.o.x)*r.inv_d.x;
	  Vector3D tmin, tmax;
	  double t_f_min, t_f_max;

	  if (r.sign[0]==0)
	  {
		  tmin.x = (min.x - r.o.x)*r.inv_d.x;
		  tmax.x = (max.x - r.o.x)*r.inv_d.x;
	  }
	  else
	  {
		  tmin.x = (max.x - r.o.x)*r.inv_d.x;
		  tmax.x = (min.x - r.o.x)*r.inv_d.x;
	  }

	  if (r.sign[1]==0)
	  {
		  tmin.y = (min.y - r.o.y)*r.inv_d.y;
		  tmax.y = (max.y - r.o.y)*r.inv_d.y;
	  }
	  else
	  {
		  tmin.y = (max.y - r.o.y)*r.inv_d.y;
		  tmax.y = (min.y - r.o.y)*r.inv_d.y;
	  }

	  if (r.sign[2]==0)
	  {
		  tmin.z = (min.z - r.o.z)*r.inv_d.z;
		  tmax.z = (max.z - r.o.z)*r.inv_d.z;
	  }
	  else
	  {
		  tmin.z = (max.z - r.o.z)*r.inv_d.z;
		  tmax.z = (min.z - r.o.z)*r.inv_d.z;
	  }

	  //No intersection
	  if ((tmin.x > tmax.y) || (tmin.y > tmax.x))
	  {
		  return false;
	  }
	  
	  t_f_min = std::max(tmin.x, tmin.y);
	  t_f_max = std::min(tmax.x, tmax.y);

	  //No intersection
	  if ((t_f_min > tmax.z) || (tmin.z > t_f_max))
	  {
		  return false;
	  }

	  t_f_min = std::max(t_f_min, tmin.z);
	  t_f_max = std::min(t_f_max, tmax.z);

	  t0 = t_f_min;
	  t1 = t_f_max;

	  return true;

	  /*
	  double tmin = std::min(tx1, tx2);
	  double tmax = std::max(tx1, tx2);

	  double ty1 = (min.y - r.o.y)*r.inv_d.y;
	  double ty2 = (max.y - r.o.y)*r.inv_d.y;

	  tmin = std::max(tmin, std::min(ty1, ty2));
	  tmax = std::min(tmax, std::max(ty1, ty2));

	  double tz1 = (min.z - r.o.z)*r.inv_d.z;
	  double tz2 = (max.z - r.o.z)*r.inv_d.z;

	  tmin = std::max(tmin, std::min(tz1, tz2));
	  tmax = std::min(tmax, std::max(tz1, tz2));

	  t0 = tmin;
	  t1 = tmax;

	  return true;
	  if (r.max_t >= tmin && tmin >= r.min_t)
	  {
		  return tmax >= tmin && tmax >= 0;
	  }

    return false;
	*/
	  

  }

  void BBox::draw(Color c) const {

    glColor4f(c.r, c.g, c.b, c.a);

    // top
    glBegin(GL_LINE_STRIP);
    glVertex3d(max.x, max.y, max.z);
    glVertex3d(max.x, max.y, min.z);
    glVertex3d(min.x, max.y, min.z);
    glVertex3d(min.x, max.y, max.z);
    glVertex3d(max.x, max.y, max.z);
    glEnd();

    // bottom
    glBegin(GL_LINE_STRIP);
    glVertex3d(min.x, min.y, min.z);
    glVertex3d(min.x, min.y, max.z);
    glVertex3d(max.x, min.y, max.z);
    glVertex3d(max.x, min.y, min.z);
    glVertex3d(min.x, min.y, min.z);
    glEnd();

    // side
    glBegin(GL_LINES);
    glVertex3d(max.x, max.y, max.z);
    glVertex3d(max.x, min.y, max.z);
    glVertex3d(max.x, max.y, min.z);
    glVertex3d(max.x, min.y, min.z);
    glVertex3d(min.x, max.y, min.z);
    glVertex3d(min.x, min.y, min.z);
    glVertex3d(min.x, max.y, max.z);
    glVertex3d(min.x, min.y, max.z);
    glEnd();

  }

  std::ostream& operator<<(std::ostream& os, const BBox& b) {
    return os << "BBOX(" << b.min << ", " << b.max << ")";
  }

} // namespace CMU462
