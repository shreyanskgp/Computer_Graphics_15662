#include "bvh.h"

#include "CMU462/CMU462.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CMU462 { namespace StaticScene {

  BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
      size_t max_leaf_size) {

    this->primitives = _primitives;

    // TODO:
    // Construct a BVH from the given vector of primitives and maximum leaf
    // size configuration. The starter code build a BVH aggregate with a
    // single leaf node (which is also the root) that encloses all the
    // primitives.

    BBox bb;
    for (size_t i = 0; i < primitives.size(); ++i) {
      bb.expand(primitives[i]->get_bbox());
    }

	/*cout << primitives[0] << endl;
	Primitive *temp;
	temp = primitives[0];
	std::sort(primitives.begin(), primitives.end(), primitive_sort_x());
	cout << primitives[0] << endl;
	std::find(primitives.begin(), primitives.end(), temp);
	cout << temp << endl;*/
    root = new BVHNode(bb, 0, primitives.size());
	create_BVH(root, max_leaf_size);
  }

  BVHAccel::~BVHAccel() {

    // TODO:
    // Implement a proper destructor for your BVH accelerator aggregate

  }

  BBox BVHAccel::get_bbox() const {
    return root->bb;
  }

  bool BVHAccel::intersect(const Ray &ray) const {

    // TODO:
    // Implement ray - bvh aggregate intersection test. A ray intersects
    // with a BVH aggregate if and only if it intersects a primitive in
    // the BVH that is not an aggregate.
    bool hit;
	Intersection i;
	hit = false;
	hit = intersect(ray, &i);
	return hit;

	/*
	  for (size_t p = 0; p < primitives.size(); ++p) {
      if(primitives[p]->intersect(ray)) hit = true;
    }

    return hit;
	*/
  }

  bool BVHAccel::intersect(const Ray &ray, Intersection *i) const {

    // TODO:
    // Implement ray - bvh aggregate intersection test. A ray intersects
    // with a BVH aggregate if and only if it intersects a primitive in
    // the BVH that is not an aggregate. When an intersection does happen.
    // You should store the non-aggregate primitive in the intersection data
    // and not the BVH aggregate itself.

    bool hit;
	hit = false;
	//hit = new bool();
	
	intersect_recursive(ray, i, get_root(), hit);

    
	
	//for (size_t p = 0; p < primitives.size(); ++p) {
	//if(primitives[p]->intersect(ray, i)) *hit = true;
	//}
	
	

    return hit;

  }

  void BVHAccel::create_BVH(BVHNode* root, size_t max_leaf_size)
  {
	  //Terminating condition
	  
	  if (root->range <= max_leaf_size)
	  {
		  //printf("Terminated");
		  return;
	  }
	  
	  //Defining splitting coordinate and plane
	  double plane_coord;
	  int dim;
	  size_t start_l, range_l, start_r, range_r;

	  start_l = root->start;

	  //Decision to split
	  bool split_yes;
	  //printf("\nHello");
	  split_yes = split_decision_node(root, &plane_coord, &dim);
	  //cout << dim << endl;
	  //cout << plane_coord << endl;
	  //cout << split_yes << endl;

	  //printf("\nHello1");
	  if (split_yes)
	  {
		  for (size_t i = root->start; i < root->start + root->range; i++)
		  {
			  if (primitives[i]->get_bbox().centroid().x >= plane_coord && dim == 1)
			  {
				  range_l = i - root->start;
				  start_r = i;
				  break;
			  }
			  if (primitives[i]->get_bbox().centroid().y >= plane_coord && dim == 2)
			  {
				  range_l = i - root->start;
				  start_r = i;
				  break;
			  }
			  if (primitives[i]->get_bbox().centroid().z >= plane_coord && dim == 3)
			  {
				  range_l = i - root->start;
				  start_r = i;
				  //printf("\nOnly once");
				  break;
			  }
		  }

		  range_r = root->range - range_l;
		  BVHNode *left_child, *right_child;
		  BBox left_child_BB, right_child_BB;
		  for (size_t i = start_l; i < (start_l + range_l); i++)
		  {
			  left_child_BB.expand(primitives[i]->get_bbox());
			  //cout << primitives[i]->get_bbox() << endl;
			  
		  }
		  //printf("\n");
		  for (size_t i = start_r; i < (start_r + range_r); i++)
		  {
			  right_child_BB.expand(primitives[i]->get_bbox());
			  //cout << primitives[i]->get_bbox() << endl;
		  }
		  //printf("\nDone by me");
		  //cout << start_l << endl;
		  //cout << range_l << endl;
		  //cout << start_r << endl;
		  //cout << range_r << endl;
		  //cout << root->range << endl;
		  //cout << left_child_BB << endl;
		  //cout << right_child_BB << endl;
		  //cout << root->bb << endl;

		  left_child = new BVHNode(left_child_BB, start_l, range_l);
		  right_child = new BVHNode(right_child_BB, start_r, range_r);

		  root->l = left_child;
		  root->r = right_child;

		  create_BVH(left_child, max_leaf_size);
		  create_BVH(right_child, max_leaf_size);
	  }
	  //printf("Hello");
	  return;
	

	  //Calculate minimum cost of x,y,z split
	  //Find minimum among x,y,z
	  //Compare between split and no split

	  //If split cost is lower- sort and split accordingly
	  //Assign members to the node-L and R and update their bb,start and range
	  //Recursive call for both L and R
  }
  bool BVHAccel::split_decision_node(BVHNode* root, double *plane_coord, int *dim)
  {
	  //Declaring planes
	  vector<Vector3D> planes(8), cost_matric(8);
	  //vector<Vector3D> num_primitives(8), surface_area(8);
	  Vector3D plane_distance = root->bb.extent*(1. / double(planes.size()));
	  Vector3D num_pri_temp, surf_area_temp, surf_area_not;


	  BBox temp_x, temp_y, temp_z;
	  BBox temp_x_not, temp_y_not, temp_z_not;
	  
	  //Defining planes based on the BB of root
	  for (int i = 0; i < planes.size(); i++)
	  {
		  planes[i] = root->bb.min + double(i)* plane_distance;
		  //num_primitives[i] = 0;
		  //surface_area[i] = 0;
	  }

	  //
	  
	  //Looping over all primitives
	  //std::sort(primitives[root->start], primitives[root->start + root->range], primitive_sort_x());

	  for (size_t i = 0; i < planes.size(); ++i)
	  {
		  
		  num_pri_temp = Vector3D(0., 0., 0.);
		  surf_area_temp = Vector3D(0., 0., 0.);
		  BBox temp_x;
		  temp_y = BBox();
		  temp_z = BBox();
		  temp_x_not = BBox();
		  temp_y_not = BBox();
		  temp_z_not = BBox();
		  
		  for (size_t k = 0; k < root->range; ++k)
		  {
			  auto j = primitives[root->start + k];
			  //printf("\nHellosd");
			  //cout << j << endl;
			  //cout << '\n' << endl;
			  //cout << j->get_bbox() << endl;
			  if (j->get_bbox().centroid().x < (planes[i].x + plane_distance.x))
			  {
				  num_pri_temp.x = num_pri_temp.x + 1;
				  temp_x.expand(j->get_bbox());
				  //surface_area[i].x = surface_area[i].x + j->get_bbox().surface_area;
				  surf_area_temp.x = surf_area_temp.x + j->get_bbox().surface_area();
			  }
			  else
			  {
				  temp_x_not.expand((j->get_bbox()));
				  surf_area_not.x = surf_area_not.x + j->get_bbox().surface_area();
			  }

			  if (j->get_bbox().centroid().y < (planes[i].y + plane_distance.y))
			  {
				  num_pri_temp.y = num_pri_temp.y + 1;
				  temp_y.expand(j->get_bbox());
				  //surface_area[i].y = surface_area[i].y + j->get_bbox().surface_area;
				  surf_area_temp.y = surf_area_temp.y + j->get_bbox().surface_area();
			  }
			  else
			  {
				  temp_y_not.expand((j->get_bbox()));
				  surf_area_not.y = surf_area_not.y + j->get_bbox().surface_area();
			  }

			  if (j->get_bbox().centroid().z < (planes[i].z + plane_distance.z))
			  {
				  num_pri_temp.z = num_pri_temp.z + 1;
				  temp_z.expand(j->get_bbox());
				  //surface_area[i].z = surface_area[i].z + j->get_bbox().surface_area();
				  surf_area_temp.z = surf_area_temp.z + j->get_bbox().surface_area();
			  }
			  else
			  {
				  temp_z_not.expand((j->get_bbox()));
				  surf_area_not.z = surf_area_not.z + j->get_bbox().surface_area();
			  }
		  }

		  
		  //num_primitives[i] = num_primitives[i] + num_primitives[i - 1];
		  //surface_area[i] = surface_area[i] + surface_area[i - 1];

		  if (i == 7)
		  {
			  //printf("\nHellosd1");
			  cost_matric[i].x = (surf_area_temp.x == 0. ? (0.) : (temp_x.surface_area())*num_pri_temp.x); 
			  cost_matric[i].y = (surf_area_temp.y == 0. ? (0.) : (temp_y.surface_area())*num_pri_temp.y);
			  cost_matric[i].z = (surf_area_temp.z == 0. ? (0.) : (temp_z.surface_area())*num_pri_temp.z);
		  }
		  else
		  {
			  //printf("\nHellosd2");
			  cost_matric[i].x = (surf_area_temp.x == 0. ? (INF_D) : (temp_x.surface_area())*num_pri_temp.x) + (surf_area_not.x == 0 ? INF_D : (temp_x_not.surface_area())*(root->range - num_pri_temp.x));
			  cost_matric[i].y = (surf_area_temp.y == 0. ? (INF_D) : (temp_y.surface_area())*num_pri_temp.y) + (surf_area_not.y == 0 ? INF_D : (temp_y_not.surface_area())*(root->range - num_pri_temp.y));
			  cost_matric[i].z = (surf_area_temp.z == 0. ? (INF_D) : (temp_z.surface_area())*num_pri_temp.z) + (surf_area_not.z == 0 ? INF_D : (temp_z_not.surface_area())*(root->range - num_pri_temp.z));
		  }
		  //printf("\nHellosd3");
		  //cout << cost_matric[i] << endl;
	  }
	   
	  
	  

	  //Finding min cost
	  int index_to_split, plane_to_split;
	  double min_cost = cost_matric[0].x;
	  for (int i = 0; i < planes.size(); i++)
	  {
		  //cost_matric[i].x = num_primitives[i].x * surface_area[i].x;
		  //cost_matric[i].y = num_primitives[i].y * surface_area[i].y;
		  //cost_matric[i].z = num_primitives[i].z * surface_area[i].z;

		  if (cost_matric[i].x <= cost_matric[i].y)
		  {
			  if (cost_matric[i].x <= cost_matric[i].z && cost_matric[i].x <= min_cost)
			  {
				  min_cost = cost_matric[i].x;
				  index_to_split = i;
				  plane_to_split = 1;
			  }
			  if (cost_matric[i].z <= cost_matric[i].x && cost_matric[i].z <= min_cost)
			  {
				  min_cost = cost_matric[i].z;
				  index_to_split = i;
				  plane_to_split = 3;
			  }
		  }
		  else
		  {
			  if (cost_matric[i].y <= cost_matric[i].z && cost_matric[i].y <= min_cost)
			  {
				  min_cost = cost_matric[i].y;
				  index_to_split = i;
				  plane_to_split = 2;
			  }
			  if (cost_matric[i].z <= cost_matric[i].y && cost_matric[i].z <= min_cost)
			  {
				  min_cost = cost_matric[i].z;
				  index_to_split = i;
				  plane_to_split = 3;
			  }
		  }
	  }

	  //Finding minimum cost
	  if (index_to_split == 7)
	  {
		  return false;
	  }

		  
	  //x plane costs

	  if (plane_to_split == 1)
	  {
		  *plane_coord = planes[index_to_split].x + plane_distance.x;
		  std::sort(primitives.begin() + (root->start), primitives.begin() + (root->start + root->range), primitive_sort_x());
	  }
	  else if (plane_to_split == 2)
	  {
		  *plane_coord = planes[index_to_split].y + plane_distance.y;
		  std::sort(primitives.begin() + (root->start), primitives.begin() + (root->start + root->range), primitive_sort_y());
	  }
	  else
	  {
		  *plane_coord = planes[index_to_split].z + plane_distance.z;
		  std::sort(primitives.begin() + (root->start), primitives.begin() + (root->start + root->range), primitive_sort_z());
		  //printf("sorted in z yay!");
	  }
	  

	  *dim = (int)plane_to_split;
	  return true;
  }

  //Recurvive function finding intersection
  void BVHAccel::intersect_recursive(const Ray & ray, Intersection * i, BVHNode* root, bool &hit) const
  {
	  double t0, t1;
	  Intersection inter;
	  
	  bool hit_temp;
	  
	  //If there is no intersection of ray and BBox of root node
	  if (!(root->bb.intersect(ray, t0, t1)))
	  {
		  //hit = false;
		  return;
	  }
	  else
	  {
		  //ray.min_t = (t0 < 0.) ? 0. : t0;
	  }
	  
	  if (root->isLeaf())
	  {
		  for (size_t p = root->start; p < root->start + root->range; ++p)
		  {			 
			  hit_temp = primitives[p]->intersect(ray, &inter);
			  if (hit_temp)
			  {
				  
				  
				  if (inter.t < i->t && inter.t <= ray.max_t && inter.t >=ray.min_t)
				  {
					  *i = inter;
					  hit = true;
				  }
				  else {
					  hit = false;
				  }
				 
			  }
			  
		  }

		  
	  }
	  else
	  {
		intersect_recursive(ray, i, root->l, hit);
		intersect_recursive(ray, i, root->r, hit);
	  }
  }

}  // namespace StaticScene
}  // namespace CMU462
