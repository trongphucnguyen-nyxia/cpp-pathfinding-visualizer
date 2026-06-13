#ifndef BASIC_H_
#define BASIC_H_

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <limits>


double myRand();/* {
	return double( double(rand()) / RAND_MAX);
}*/


namespace mathtool {

  //constants
#ifndef PI
#define PI 3.1415926535897
#endif

#ifndef TWOPI
#define TWOPI 6.2831853071794
#endif

  // Return the square of a.
  template<typename T>
    inline T sqr(const T& a) {return a*a;}

  // Return sign of x
  inline int sign(double x) {return x >=0 ? 1: -1;}

  // Angle conversions
  inline double degToRad(double x) {return x*PI/180;}

  inline double radToDeg(double x) {return x*180/PI;}

  // Degree Trig functions
  inline double sind(double x){return std::sin(degToRad(x));}

  inline double cosd(double x){return std::cos(degToRad(x));}

  inline double tand(double x){return std::tan(degToRad(x));}

  inline double asind(double x){return radToDeg(std::asin(x));}

  inline double acosd(double x){return radToDeg(std::acos(x));}

  inline double atand(double x){return radToDeg(std::atan(x));}

  inline double atan2d(double x, double y){return radToDeg(std::atan2(x,y));}

}

#endif
