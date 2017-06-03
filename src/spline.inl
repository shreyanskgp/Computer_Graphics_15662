// Given a time between 0 and 1, evaluates a cubic polynomial with
// the given endpoint and tangent values at the beginning (0) and
// end (1) of the interval.  Optionally, one can request a derivative
// of the spline (0=no derivative, 1=first derivative, 2=2nd derivative).
template <class T>
inline T Spline<T>::cubicSplineUnitInterval(
      const T& position0,
      const T& position1,
      const T& tangent0,
      const T& tangent1,
      double normalizedTime,
      int derivative )
{
   // TODO IMPLEMENT ME (TASK 1A)
	double t = normalizedTime;
	double t_2 = t*t;
	double t_3 = t_2*t;

	double h_00, h_10, h_01, h_11;
	
	//Defining hermite bases based on input derivative
	if (derivative == 0)
	{
		h_00 = 2.*t_3 - 3.*t_2 + 1.;
		h_10 = t_3 - 2.*t_2 + t;
		h_01 = -2.*t_3 + 3.*t_2;
		h_11 = t_3 - t_2;
	}
	if (derivative == 1)
	{
		h_00 = 6.*t_2 - 6.*t;
		h_10 = 3.*t_2 - 4.*t + 1.;
		h_01 = -6.*t_2 + 6.*t;
		h_11 = 3.*t_2 - 2.*t;
	}
	if (derivative == 2)
	{
		h_00 = 12.*t - 6.;
		h_10 = 6.*t - 4.;
		h_01 = -12.*t + 6.;
		h_11 = 6.*t - 2.;
	}

	//Defining the output;
	T output = h_00*position0 + h_10*tangent0 + h_01*position1 + h_11*tangent1;
	return output;
}
            
// Returns a state interpolated between the values directly before and after the given time.
template <class T>
inline T Spline<T>::evaluate( double time, int derivative )
{
   // TODO IMPLEMENT ME (TASK 1B)
   
	
	double norm_time;
	T position0, position1, tangent0, tangent1;
	
	//No elements in the map
	if (knots.size() < 1) return T();
   
	//Only one element in map
	if (knots.size() == 1)
	{
		if (derivative == 0) return knots.begin()->second;
		if (derivative == 1||derivative==2) return T();
	}
	
	//Query time before the first knot
	if (time <= knots.begin()->first)
	{
		norm_time = knots.begin()->first - time;
		position0 = knots.begin()->second;
		position1 = knots.begin()->second;
		tangent0 = T();
		tangent1 = T();
	
		if (derivative == 0) return knots.begin()->second;
		if (derivative == 1 || derivative == 2) return T();
		//return cubicSplineUnitInterval(position0, position1, tangent0, tangent1, norm_time, derivative);
	}
	
	
	//Query time after the last knot
	if (time >= knots.rbegin()->first)
	{
		norm_time = time - knots.rbegin()->first;
		position0 = knots.rbegin()->second;
		position1 = knots.rbegin()->second;
		tangent0 = T();
		tangent1 = T();

		if (derivative == 0) return knots.rbegin()->second;
		if (derivative == 1 || derivative == 2) return T();
		//return cubicSplineUnitInterval(position0, position1, tangent0, tangent1, norm_time, derivative);
	}

	
	//Finding upper bound based on the query time
	KnotIter upper_knot = knots.upper_bound(time);
	KnotIter lower_knot = std::prev(knots.upper_bound(time));
	KnotIter lower_lower_knot, upper_upper_knot;

	T k_0, k_1, k_2, k_3;
	double t_0, t_1, t_2, t_3;

	k_1 = lower_knot->second;
	k_2 = upper_knot->second;
	
	t_1 = lower_knot->first;
	t_2 = upper_knot->first;


	if (lower_knot == knots.begin())
	{
		k_0 = k_1 - (k_2 - k_1);
		t_0 = t_1 - (t_2 - t_1);
	}
	else
	{
		lower_lower_knot = std::prev(lower_knot);
		k_0 = lower_lower_knot->second;
		t_0 = lower_lower_knot->first;
	}

	if (upper_knot == std::prev(knots.end()))
	{
		k_3 = k_2 + (k_2 - k_1);
		t_3 = t_2 + (t_2 - t_1);
	}
	else
	{
		//upper_upper_knot = upper_knot++;
		upper_upper_knot = std::next(upper_knot);
		k_3 = upper_upper_knot->second;
		t_3 = upper_upper_knot->first;
	}

	position0 = k_1;
	position1 = k_2;
	tangent0 = (k_2 - k_0) * (t_2 - t_1) / (t_2 - t_0);
	tangent1 = (k_3 - k_1) * (t_2 - t_1) / (t_3 - t_1);
	norm_time = (time - t_1) / (t_2 - t_1);
    return cubicSplineUnitInterval(position0, position1, tangent0, tangent1, norm_time, derivative);
}

// Removes the knot closest to the given time,
//    within the given tolerance..
// returns true iff a knot was removed.
template <class T>
inline bool Spline<T>::removeKnot(double time, double tolerance )
{
   // Empty maps have no knots.
   if( knots.size() < 1 )
   {
      return false;
   }

   // Look up the first element > or = to time.
   typename std::map<double, T>::iterator t2_iter = knots.lower_bound(time);
   typename std::map<double, T>::iterator t1_iter;
   t1_iter = t2_iter;
   t1_iter--;

   if( t2_iter == knots.end() )
   {
      t2_iter = t1_iter;
   }

   // Handle tolerance bounds,
   // because we are working with floating point numbers.
   double t1 = (*t1_iter).first;
   double t2 = (*t2_iter).first;

   double d1 = fabs(t1 - time);
   double d2 = fabs(t2 - time);


   if(d1 < tolerance && d1 < d2)
   {
      knots.erase(t1_iter);
      return true;
   }

   if(d2 < tolerance && d2 < d1)
   {
      knots.erase(t2_iter);
      return t2;
   }

   return false;
}

// Sets the value of the spline at a given time (i.e., knot),
// creating a new knot at this time if necessary.
template <class T>
inline void Spline<T>::setValue( double time, T value )
{
   knots[ time ] = value;
}

template <class T>
inline T Spline<T>::operator()( double time )
{
   return evaluate( time );
}
