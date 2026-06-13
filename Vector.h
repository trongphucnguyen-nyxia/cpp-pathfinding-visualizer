#ifndef VECTOR_H_
#define VECTOR_H_

#include "Basic.h"

namespace mathtool{

  class Vector3d {
    public:

      //construction
      Vector3d(const double& _x = double(), const double& _y = double(), const double& _z = double()) {
	m_v[0] = _x; m_v[1] = _y; m_v[2] = _z;
      }
      Vector3d(const Vector3d& _v) {
	m_v[0] = _v[0]; m_v[1] = _v[1]; m_v[2] = _v[2];
      }
      Vector3d(const double (&_t)[3]) {
	m_v[0] = _t[0]; m_v[1] = _t[1]; m_v[2] = _t[2];
      }
      template <class Generator> Vector3d(const Generator& g){
	m_v[0] = g(); m_v[1] = g(); m_v[2] = g();
      }

      //assignment
      Vector3d& operator()(const double& _x, const double& _y, const double& _z){
	m_v[0] = _x; m_v[1] = _y; m_v[2] = _z;
	return *this;
      }
      Vector3d& operator=(const Vector3d& _v) {
	m_v[0] = _v[0]; m_v[1] = _v[1]; m_v[2] = _v[2];
	return *this;
      }
      Vector3d& operator=(const double _v[3]) {
	m_v[0] = _v[0]; m_v[1] = _v[1]; m_v[2] = _v[2];
	return *this;
      }

      //access
	  double& operator[](int idx) { return m_v[idx]; }
	  const double& operator[](int idx) const { return m_v[idx]; }
	  /*
      typedef double (&arr)[3];
      typedef const double (&carr)[3];
      operator arr() {return m_v;}
      operator carr() const {return m_v;}
	  */
      const double* begin() const {return m_v;}
      const double* end() const {return begin()+3;}
      double* begin() {return m_v;}
      double* end() {return begin()+3;}

      //equality
      bool operator==(const Vector3d& _v) const {
	return m_v[0] == _v[0] && m_v[1] == _v[1] && m_v[2] == _v[2];
      }
      //inequality
      bool operator!=(const Vector3d& _v) const {
	return !(*this == _v);
      }

      //self addition
      Vector3d& operator+=(const Vector3d& _v) {
	m_v[0] += _v[0]; m_v[1] += _v[1]; m_v[2] += _v[2];
	return *this;
      }
      //self subtraction
      Vector3d& operator-=(const Vector3d& _v) {
	m_v[0] -= _v[0]; m_v[1] -= _v[1]; m_v[2] -= _v[2];
	return *this;
      }
      //self scalar multiply
      Vector3d& operator*=(const double& _d) {
	m_v[0] *= _d; m_v[1] *= _d; m_v[2] *= _d;
	return *this;
      }
      //self scalar divide
      Vector3d& operator/=(const double& _d) {
	m_v[0] /= _d; m_v[1] /= _d; m_v[2] /= _d;
	return *this;
      }
      //self component *
      Vector3d& operator^=(const Vector3d& _v) {
	m_v[0] *= _v[0]; m_v[1] *= _v[1]; m_v[2] *= _v[2];
	return *this;
      }
      //self cross product
      Vector3d& operator%=(const Vector3d& _v) {
	double v0 = m_v[0], v1 = m_v[1], v2 = m_v[2];
	m_v[0] = v1 * _v[2] - v2 * _v[1];
	m_v[1] = v2 * _v[0] - v0 * _v[2];
	m_v[2] = v0 * _v[1] - v1 * _v[0];
	return *this;
      }

      //negation
      Vector3d operator-() const {
	return Vector3d(-m_v[0], -m_v[1], -m_v[2]);
      }
      //addition
      Vector3d operator+(const Vector3d& _v) const {
	return Vector3d(m_v[0] + _v[0], m_v[1] + _v[1], m_v[2] + _v[2]);
      }
      //subtraction
      Vector3d operator-(const Vector3d& _v) const {
	return Vector3d(m_v[0] - _v[0], m_v[1] - _v[1], m_v[2] - _v[2]);
      }
      //scalar multiply
      Vector3d operator*(const double& _d) const {
	return Vector3d(m_v[0] * _d, m_v[1] * _d, m_v[2] * _d);
      }
      //scalar divide
      Vector3d operator/(const double& _d) const {
	return Vector3d(m_v[0] / _d, m_v[1] / _d, m_v[2] / _d);
      }
      //component *
      Vector3d operator^(const Vector3d& _v) const {
	return Vector3d(m_v[0] * _v[0], m_v[1] * _v[1], m_v[2] * _v[2]);
      }
      //cross product
      Vector3d operator%(const Vector3d& _v) const {
	Vector3d v(*this);
	return v %= _v;
      }

      //dot product
      double operator*(const Vector3d& _v) const {
	return m_v[0]*_v[0] + m_v[1]*_v[1] + m_v[2]*_v[2];
      }
      //magnitude
      double norm() const {
	return std::sqrt(normsqr());
      }
      //magnitude squared
      double normsqr() const {
	return (*this)*(*this);
      }
      //normalized vector
      Vector3d& selfNormalize() {
	double n = norm();
	if(n < std::numeric_limits<double>::epsilon())
	  return *this = Vector3d();
	return *this /= n;
      }
      Vector3d normalize() const {
	double n = norm();
	if(n < std::numeric_limits<double>::epsilon())
	  return Vector3d();
	return *this / n;
      }
      //Projections
      //find |component| of this along _v's direction
      double comp(Vector3d& _v) {
	return (*this) * _v.normalize();
      }
      //find vector component of this in _v's direction
      //Vector3d proj(Vector3d& _v) {
      //  return (*this * _v)/(_v * _v) * _v;
      //}
      //find vector component of this orthogonal to _v's direction
      //Vector3d orth(Vector3d& _v) {
      //  return *this - proj(_v);
      //}
      //scale vector
      Vector3d& selfScale(const double& _l) {
	double n = norm();
	if(n < std::numeric_limits<double>::epsilon())
	  return *this = Vector3d();
	return *this *= (_l/n);
      }
      Vector3d scale(const double& _l) const {
	double n = norm();
	if(n < std::numeric_limits<double>::epsilon())
	  return Vector3d();
	return *this * (_l/n);
      }

      //rotate vector
      Vector3d& rotateX(double _rad) {
	double c = cos(_rad), s = sin(_rad);
	return operator()(m_v[0], m_v[1]*c - m_v[2]*s, m_v[1]*s + m_v[2]*c);
      }
      Vector3d& rotateXd(double _deg) {return rotateX(degToRad(_deg));}
      Vector3d& rotateY(double _rad) {
	double c = cos(_rad), s = sin(_rad);
	return operator()(m_v[0]*c + m_v[2]*s, m_v[1], -m_v[0]*s + m_v[2]*c);
      }
      Vector3d& rotateYd(double _deg) {return rotateY(degToRad(_deg));}
      Vector3d& rotateZ(double _rad) {
	double c = cos(_rad), s = sin(_rad);
	return operator()(m_v[0]*c - m_v[1]*s, m_v[0]*s + m_v[1]*c, m_v[2]);
      }
      Vector3d& rotateZd(double _deg) {return rotateZ(degToRad(_deg));}

      //reset
      void reset() {
	m_v[0]=0; m_v[1]=0; m_v[2]=0;
      }
      double GetX() { return m_v[0]; }
      double GetY() { return m_v[1]; }
      double GetZ() { return m_v[2]; }
      void SetX(double d) { m_v[0] = d; }
      void SetY(double d) { m_v[1] = d; }
      void SetZ(double d) { m_v[2] = d; }

    private:
      double m_v[3];
  };


  //////////////////////////////////////////////////////////////////////////////
  // Useful functions. Input/Output and commutativity on multiply
  //////////////////////////////////////////////////////////////////////////////
  //for commutativity of scalar multiply

  inline Vector3d operator*(const double& _d, const Vector3d& _v) {
    return _v*_d;
  }


  inline std::ostream& operator<<(std::ostream& _os, const Vector3d& _v) {
    for(size_t i = 0; i<3; ++i) _os << _v[i] << " ";
    return _os;
  }



}


#endif
