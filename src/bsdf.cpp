#include "bsdf.h"

#include <iostream>
#include <algorithm>
#include <utility>

using std::min;
using std::max;
using std::swap;

namespace CMU462 {

  void make_coord_space(Matrix3x3& o2w, const Vector3D& n) {

    Vector3D z = Vector3D(n.x, n.y, n.z);
    Vector3D h = z;
    if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z)) h.x = 1.0;
    else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z)) h.y = 1.0;
    else h.z = 1.0;

    z.normalize();
    Vector3D y = cross(h, z);
    y.normalize();
    Vector3D x = cross(z, y);
    x.normalize();

    o2w[0] = x;
    o2w[1] = y;
    o2w[2] = z;
  }

  // Diffuse BSDF //

  Spectrum DiffuseBSDF::f(const Vector3D& wo, const Vector3D& wi) {
	  return albedo * (1.0 / PI);

  }

  Spectrum DiffuseBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
    
	  Vector3D sample = sampler.get_sample(pdf);
	  *wi = sample;
	  //*pdf = 1.f /(float)(dot(sample, Vector3D(0., 0., 1.))*PI);
	  return albedo * (1.0 / PI);
  }

  // Mirror BSDF //

  Spectrum MirrorBSDF::f(const Vector3D& wo, const Vector3D& wi) {

	  Vector3D wi_gen;
	  reflect(wo, &wi_gen);
	  
	  if ((wi_gen - wi).norm() > EPS_D) return Spectrum();
	  //if(wo.z!=wi.z || (wo - Vector3D(0.,0.,wo.z) -wi + Vector3D(0.,0.,wi.z)).norm() != 0.) return Spectrum();
	  
	  else return reflectance*(1./fabs(wo.z));
  }

  Spectrum MirrorBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

    // TODO:
    // Implement MirrorBSDF
	  reflect(wo, wi);
	  *pdf = 1.f;
	  
    return reflectance*(1./ fabs(wo.z));
  }

  // Glossy BSDF //

  /*
     Spectrum GlossyBSDF::f(const Vector3D& wo, const Vector3D& wi) {
     return Spectrum();
     }

     Spectrum GlossyBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
   *pdf = 1.0f;
   return reflect(wo, wi, reflectance);
   }
   */

  // Refraction BSDF //

  Spectrum RefractionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
	  
	  return Spectrum();
  }

  Spectrum RefractionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

    // TODO:
    // Implement RefractionBSDF
	  bool refract_yes;
	  refract_yes = refract(wo,wi,this->ior);
	  
	  *pdf = 1.f;
	  
	  return this->transmittance;
  }

  // Glass BSDF //

  Spectrum GlassBSDF::f(const Vector3D& wo, const Vector3D& wi) {
    
	Vector3D wi_refract, wi_reflect;
	double theta_o, theta_i, r_parallel, r_perp, F_r, f, eta_t, eta_i;
	bool refract_yes;
	Spectrum refracted;

	refract_yes = refract(wo, &wi_refract, this->ior);
	reflect(wo, &wi_reflect);
	
	if (refract_yes) //If refracted
	{
		if (wo.z > 0.) //Glass to air refraction, wi goes to glass
		{
			eta_i = (double)this->ior;
			eta_t = 1.;
			theta_o = acos(wo.z);
			theta_i = asin(sin(theta_o)*eta_t / eta_i);
		}
		else //Air to glass refraction, wi goes to air
		{
			eta_i = 1.;
			eta_t = (double)this->ior;
			theta_o = PI - acos(wo.z);
			theta_i = asin(sin(theta_o)*eta_t / eta_i);
		}

		r_parallel = (eta_t*cos(theta_i) - eta_i*cos(theta_o)) / (eta_t*cos(theta_i) + eta_i*cos(theta_o));
		r_perp = (eta_i*cos(theta_i) - eta_t*cos(theta_o)) / (eta_i*cos(theta_i) + eta_t*cos(theta_o));
		F_r = (r_parallel*r_parallel + r_perp*r_perp)*0.5;
		f = (eta_t*eta_t / (eta_i*eta_i))*(1 - F_r) / fabs(cos(theta_i));

		if (1 - F_r > F_r) //If transmitted fraction is larger than reflected return refracted ray
		{
			f = ((eta_t*eta_t / (eta_i*eta_i))*(1 - F_r)) / fabs(cos(theta_i));
			
			refracted = f*transmittance;
		}
		else //If transmitted fraction is less than reflected, return reflected ray
		{
			reflect(wo, &wi_refract);
			refracted = F_r*reflectance *(1./ fabs(cos(theta_i)));
		}
		
	}
	else
	{
		refracted= reflectance*(1. / fabs(cos(theta_i)));
	}

	if ((wi - wi_refract).norm() <= EPS_D)
	{
		
		return refracted;
	}

	if ((wi - wi_reflect).norm() <= EPS_D)
	{
		return reflectance;
	}
	
	return Spectrum();
  }

  Spectrum GlassBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {

    // TODO:
    // Compute Fresnel coefficient and either reflect or refract based on it.
	  double r_parallel, r_perp, F_r, f, eta_t, eta_i;
	  Vector3D n(0., 0., 1.);
	  double theta_o, theta_i;
	  bool refract_yes;
	  refract_yes = refract(wo, wi, this->ior);
	  if (refract_yes)
	  {
		  if (wo.z > 0.) //Glass to air refraction, wi goes to glass
		  {
			  eta_i = this->ior;
			  eta_t = 1.;
			  theta_o = acos(wo.z);
			  theta_i = asin(sin(theta_o)*eta_t / eta_i);
		  }
		  else //Air to glass refraction, wi goes to air
		  {
			  eta_i = 1.;
			  eta_t = this->ior;
			  theta_o = PI - acos(wo.z);
			  theta_i = asin(sin(theta_o)*eta_t / eta_i);
		  }


		  r_parallel = (eta_t*cos(theta_i) - eta_i*cos(theta_o)) / (eta_t*cos(theta_i) + eta_i*cos(theta_o));
		  r_perp = (eta_i*cos(theta_i) - eta_t*cos(theta_o)) / (eta_i*cos(theta_i) + eta_t*cos(theta_o));
		  F_r = (r_parallel*r_parallel + r_perp*r_perp)*0.5;
		  double coin_toss = (double)rand() / (double)RAND_MAX;

		  if (coin_toss <= 1 - F_r)
		  {
			  f = ((eta_t*eta_t / (eta_i*eta_i))*(1 - F_r)) / fabs(cos(theta_i));
			  *pdf = (1. - F_r);
			  return f*transmittance;
		  }
		  else
		  {
			  reflect(wo, wi);
			  *pdf = F_r;
			  return F_r*reflectance*(1./ fabs(cos(theta_i)));
		  }

	  }
	  else // Total internal reflection (Glass to glass), wi goes to glass
	  {
		  *pdf = 1.f;
		  return reflectance*(1./fabs(cos(theta_i)));
	  }
  }

  void BSDF::reflect(const Vector3D& wo, Vector3D* wi) {

    // TODO:
    // Implement reflection of wo about normal (0,0,1) and store result in wi.
	  *wi = Vector3D(-wo.x, -wo.y, wo.z);
	  wi->normalize();
  }

  bool BSDF::refract(const Vector3D& wo, Vector3D* wi, float ior) {

    // TODO:
    // Use Snell's Law to refract wo surface and store result ray in wi.
    // Return false if refraction does not occur due to total internal reflection
    // and true otherwise. When dot(wo,n) is positive, then wo corresponds to a
    // ray entering the surface through vacuum.
	  double theta_o, theta_i, critical_ang;
	  Vector3D n(0., 0., 1.);
	  Vector3D n_perp_plane;

	  //Tranforming Z axis to 
	  //Matrix3x3 o2w;
	  //make_coord_space(o2w, cross(n, wo).normalize);
	  //Matrix3x3 w2o = o2w.T();

	  if (dot(wo, n)>= 0.)
	  {
		  //refracted from glass to air- wi coming from glass
		  theta_o = acos(wo.z); //theta_o wrt n
		  theta_i = asin(sin(theta_o)/ior); //theta_i wrt -n
		  n_perp_plane = wo - Vector3D(0., 0., wo.z);
		  n_perp_plane.normalize();
		  *wi = -n - (n_perp_plane)*tan(theta_i);
		  wi->normalize();
		  return true;
	  }
	  else
	  {
		  theta_o = PI - acos(wo.z); //theta_o w.r.t -n
		  critical_ang = asin(1. / ior);

		  //Check for total internal reflection
		  if ((theta_o) >= critical_ang)
		  {
			  //Total internal reflection- wi came from glass
			  
			  reflect(wo, wi);
			  return false;
		  }
		  else
		  {
			  //Refraction from air to glass- wi came from air
			  theta_i = asin(sin(theta_o) * ior); //theta_i wrt n
			  n_perp_plane = wo - Vector3D(0., 0., wo.z);
			  n_perp_plane.normalize();
			  *wi = n - n_perp_plane*tan(theta_i);
			  wi->normalize();
			  return true;
		  }

	  }

/*	  
	  if (dot(wo, n)> 0.)
	  {
		  //refracted from air to glass- wi coming from air
		  theta_o = acos(dot(wo.normalize, n)); //theta_o wrt n
		  theta_i = asin(ior*sin(theta_o)); //theta_i wrt -n

		  *wi = -n - (wo - Vector3D(0., 0., wo.z)).normalize*tan(theta_i);
		  return true;
	  }
	  else
	  {
		  theta_o = PI - theta_o; //theta_o w.r.t -n
		  critical_ang = asin(1. / ior);

		  //Check for total internal reflection
		  if ((theta_o) >= critical_ang)
		  {
			  //Total internal reflection- wi came from air
			  *wi = -n - (wo - Vector3D(0., 0., wo.z)).normalize*tan(theta_o);
			  return false;
		  }
		  else
		  {
			  //Refraction from glass to air- wi came from glass
			  theta_i = asin(sin(theta_o)/ior); //theta_i wrt n
			  *wi = n - (wo - Vector3D(0., 0., wo.z)).normalize*tan(theta_i);
			  return true;
		  }
		  
	  }
	  */
  }

  // Emission BSDF //

  Spectrum EmissionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
    return Spectrum();
  }

  Spectrum EmissionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
    *wi  = sampler.get_sample(pdf);
    return Spectrum();
  }

} // namespace CMU462
