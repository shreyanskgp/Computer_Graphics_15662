#include "sampler.h"
#include "time.h"
#include "stdlib.h"

namespace CMU462 {

  // Uniform Sampler2D Implementation //

  Vector2D UniformGridSampler2D::get_sample() const {

    // TODO:
    // Implement uniform 2D grid sampler

	double x = double(rand()) / double(RAND_MAX);
	double y = double(rand()) / double(RAND_MAX);
    return Vector2D(x,y);

  }

  // Uniform Hemisphere Sampler3D Implementation //

  Vector3D UniformHemisphereSampler3D::get_sample() const {

    double Xi1 = (double)(std::rand()) / (double)RAND_MAX;
    double Xi2 = (double)(std::rand()) / (double)RAND_MAX;

    double theta = acos(Xi1);
    double phi = 2.0 * PI * Xi2;

    double xs = sinf(theta) * cosf(phi);
    double ys = sinf(theta) * sinf(phi);
    double zs = cosf(theta);

    return Vector3D(xs, ys, zs);

  }

  Vector3D CosineWeightedHemisphereSampler3D::get_sample() const {
	  double Xi1 = (double)(std::rand()) / (double)RAND_MAX;
	  double Xi2 = (double)(std::rand()) / (double)RAND_MAX;

	  double theta = acos(sqrt(1.0 - Xi1));
	  double phi = 2.0 * PI * Xi2;

	  double xs = sinf(theta) * cosf(phi);
	  double ys = sinf(theta) * sinf(phi);
	  double zs = cosf(theta);

	  return Vector3D(xs, ys, zs);

  }

  Vector3D CosineWeightedHemisphereSampler3D::get_sample(float *pdf) const {
    // You may implement this, but don't have to.
	  double Xi1 = (double)(std::rand()) / (double)RAND_MAX;
	  double Xi2 = (double)(std::rand()) / (double)RAND_MAX;

	  double theta = acos(sqrt(1.0 - Xi1));
	  double phi = 2.0 * PI * Xi2;

	  double xs = sin(theta) * cos(phi);
	  double ys = sin(theta) * sin(phi);
	  double zs = cos(theta);
	  *pdf = std::max(0.,((float)zs)/PI);
	  //printf("ganda");
	  return Vector3D(xs, ys, zs);
	  
  }


} // namespace CMU462
