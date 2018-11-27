/******************************************************************************
 *
 * Basic 4d vector class 
 *
 *****************************************************************************/
#ifndef GamePhysics_BASICVEC4D_H
#define GamePhysics_BASICVEC4D_H

#include "vectorbase.h"


namespace GamePhysics
{

// basic inlined vector class
template<class Scalar>
class vector4Dim
{
public:
	//! Constructor
	inline vector4Dim() : x(0),y(0),z(0),t(0) {}
	
	//! Copy-Constructor
	inline vector4Dim ( const vector4Dim<Scalar> &v ) : x(v.x), y(v.y), z(v.z),t(v.t) {}

	//! Copy-Constructor
	inline vector4Dim ( const float * v) : x((Scalar)v[0]), y((Scalar)v[1]), z((Scalar)v[2]), t((Scalar)v[3]) {}

	//! Copy-Constructor
	inline vector4Dim ( const double * v) : x((Scalar)v[0]), y((Scalar)v[1]), z((Scalar)v[2]), t((Scalar)v[3]) {}
	
	//! Construct a vector from one Scalar
	inline vector4Dim ( Scalar v) : x(v), y(v), z(v), t(v) {}
		
	//! Construct a vector from four Ss
	inline vector4Dim ( Scalar vx, Scalar vy, Scalar vz, Scalar vw) : x(vx), y(vy), z(vz), t(vw) {}

	//! Construct a vector from four Ss
	//inline vector4Dim(DirectX::XMVECTOR &v ); // TODO CHECK!
	
	// get address of array for OpenGL
	Scalar *getAddress() { return value; }
	
	// Operators
	
	//! Assignment operator
	inline const vector4Dim<Scalar>& operator= ( const vector4Dim<Scalar>& v ) {
		x = v.x;
		y = v.y;
		z = v.z;
		t = v.t;
		return *this;
	}
	//! Assignment operator
	inline const vector4Dim<Scalar>& operator= ( Scalar s ) {
		x = y = z = t = s;
		return *this;
	}
	//! Assign and add operator
	inline const vector4Dim<Scalar>& operator+= ( const vector4Dim<Scalar>& v ) {
		x += v.x;
		y += v.y;
		z += v.z;
		t += v.t;
		return *this;
	}
	//! Assign and add operator
	inline const vector4Dim<Scalar>& operator+= ( Scalar s ) {
		x += s;
		y += s;
		z += s;
		t += s;
		return *this;
	}
	//! Assign and sub operator
	inline const vector4Dim<Scalar>& operator-= ( const vector4Dim<Scalar>& v ) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		t -= v.t;
		return *this;
	}
	//! Assign and sub operator
	inline const vector4Dim<Scalar>& operator-= ( Scalar s ) {
		x -= s;
		y -= s;
		z -= s;
		t -= s;
		return *this;
	}
	//! Assign and mult operator
	inline const vector4Dim<Scalar>& operator*= ( const vector4Dim<Scalar>& v ) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		t *= v.t;
		return *this;
	}
	//! Assign and mult operator
	inline const vector4Dim<Scalar>& operator*= ( Scalar s ) {
		x *= s;
		y *= s;
		z *= s;
		t *= s;
		return *this;
	}
	//! Assign and div operator
	inline const vector4Dim<Scalar>& operator/= ( const vector4Dim<Scalar>& v ) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		t /= v.t;
		return *this;        
	}
	//! Assign and div operator
	inline const vector4Dim<Scalar>& operator/= ( Scalar s ) {        
		x /= s;
		y /= s;
		z /= s;
		t /= s;
		return *this;        
	}
	
	inline void safeDivide (const vector4Dim<Scalar>& v);
	
	//! Negation operator
	inline vector4Dim<Scalar> operator- () const {
		return vector4Dim<Scalar> (-x, -y, -z, -t);        
	}
	
	// binary operator add
	inline vector4Dim<Scalar> operator+ (const vector4Dim<Scalar>&) const;
	// binary operator add
	inline vector4Dim<Scalar> operator+ (Scalar) const;
	// binary operator sub
	inline vector4Dim<Scalar> operator- (const vector4Dim<Scalar>&) const;
	// binary operator sub
	inline vector4Dim<Scalar> operator- (Scalar) const;
	// binary operator mult
	inline vector4Dim<Scalar> operator* (const vector4Dim<Scalar>&) const;
	// binary operator mult
	inline vector4Dim<Scalar> operator* (Scalar) const;
	// binary operator div
	inline vector4Dim<Scalar> operator/ (const vector4Dim<Scalar>&) const;
	// binary operator div
	inline vector4Dim<Scalar> operator/ (Scalar) const;
	
	//! Get smallest component 
	//inline Scalar min() const { return ( x<y ) ? ( ( x<z ) ? x:z ) : ( ( y<z ) ? y:z ); // todo t!!}
	//! Get biggest component
	//inline Scalar max() const { return ( x>y ) ? ( ( x>z ) ? x:z ) : ( ( y>z ) ? y:z ); // todo t!!}
		
	//! Test if all components are zero
	inline bool empty() {
		return x==0 && y==0 && z==0 && t==0;
	}

	//! access operator
	inline Scalar& operator[] ( unsigned int i ) { 
		return value[i];
	}
	//! constant access operator
	inline const Scalar& operator[] ( unsigned int i ) const {
		return value[i];
	}

	//! debug output vector to a string
	std::string toString() const;

	//! actual values
	union {
		Scalar value[4];
		struct {
			Scalar x;
			Scalar y;
			Scalar z;
			Scalar t;
		};
		struct {
			Scalar X;
			Scalar Y;
			Scalar Z;
			Scalar T;
		};
	};

	// zero element
	static const vector4Dim<Scalar> ZERO;
	
protected:

};

//************************************************************************
// binary operators
//************************************************************************
//! Addition operator
template<class Scalar> 
inline vector4Dim<Scalar> vector4Dim<Scalar>::operator+ ( const vector4Dim<Scalar> &v) const
{
	return vector4Dim<Scalar> (value[0]+v.value[0],
			value[1]+v.value[1],
			value[2]+v.value[2],
			value[3]+v.value[3]);
}

//! Addition operator
template<class Scalar>
inline vector4Dim<Scalar>
vector4Dim<Scalar>::operator+(Scalar s) const
{
  return vector4Dim<Scalar>(value[0]+s,
			value[1]+s,
			value[2]+s,
			value[3]+s);
}

template<class Scalar>
inline vector4Dim<Scalar> 
operator+(float s, vector4Dim<Scalar> v) 
{
  return v + s;
}

template<class Scalar>
inline vector4Dim<Scalar> 
operator+(double s, vector4Dim<Scalar> v) 
{
  return v + s;
}

template<class Scalar>
inline vector4Dim<Scalar> 
operator+(int s, vector4Dim<Scalar> v) 
{
  return v + s;
}

//! Subtraction operator
template<class Scalar>
inline vector4Dim<Scalar>
vector4Dim<Scalar>::operator-( const vector4Dim<Scalar> &v ) const
{
  return vector4Dim<Scalar>(value[0]-v.value[0],
			value[1]-v.value[1],
			value[2]-v.value[2],
			value[3]-v.value[3]);
}

//! Subtraction operator
template<class Scalar>
inline vector4Dim<Scalar>
vector4Dim<Scalar>::operator-(Scalar s ) const
{
  return vector4Dim<Scalar>(value[0]-s,
			value[1]-s,
			value[2]-s,
			value[3]-s,);
}

//! Multiplication operator
template<class Scalar>
inline vector4Dim<Scalar> 
vector4Dim<Scalar>::operator* ( const vector4Dim<Scalar>& v ) const
{
	 return vector4Dim<Scalar>(value[0]*v.value[0],
			value[1]*v.value[1],
			value[2]*v.value[2],
			value[3]*v.value[3]);
}
//! Multiplication operator
template<class Scalar>
inline vector4Dim<Scalar>
vector4Dim<Scalar>::operator* (Scalar s) const
{
	return vector4Dim<Scalar>(value[0]*s, value[1]*s, value[2]*s, value[3]*s);
}

//! Multiplication operator
template<class Scalar> 
inline vector4Dim<Scalar>
operator* (float s, vector4Dim<Scalar> v) 
{
  return v * s;
}

template<class Scalar>
inline vector4Dim<Scalar> 
operator*(double s, vector4Dim<Scalar> v) 
{
  return v * s;
}

template<class Scalar>
inline vector4Dim<Scalar> 
operator*(int s, vector4Dim<Scalar> v) 
{
  return v * s;
}

//! Division operator
template<class Scalar>
inline vector4Dim<Scalar>
vector4Dim<Scalar>::operator/ (const vector4Dim<Scalar> & v) const
{
	return vector4Dim<Scalar> (value[0]/v.value[0],
			value[1]/v.value[1],
			value[2]/v.value[2],
			value[3]/v.value[3]);
}

//! Division operator
template<class Scalar>
inline vector4Dim<Scalar>
vector4Dim<Scalar>::operator / (Scalar s) const
{
	return vector4Dim<Scalar> (value[0]/s,
			value[1]/s,
			value[2]/s,
			value[3]/s);
}

//! Safe divide
template<class Scalar>
inline void vector4Dim<Scalar>::safeDivide( const vector4Dim<Scalar> &v )
{
  value[0] = (v.value[0]!=0) ? (value[0] / v.value[0]) : 0;
  value[1] = (v.value[1]!=0) ? (value[1] / v.value[1]) : 0;
  value[2] = (v.value[2]!=0) ? (value[2] / v.value[2]) : 0;
  value[3] = (v.value[3]!=0) ? (value[3] / v.value[3]) : 0;
}

//************************************************************************
// External functions
//************************************************************************

//! Dot product
template<class Scalar>
inline Scalar dot ( const vector4Dim<Scalar> &t, const vector4Dim<Scalar> &v ) {
	return t.x*v.x + t.y*v.y + t.z*v.z + t.t*v.t;    
}

//! Cross product
/*template<class Scalar>
inline vector4Dim<Scalar> cross ( const vector4Dim<Scalar> &t, const vector4Dim<Scalar> &v ) {
	NYI vector4Dim<Scalar> cp (
		( ( t.y*v.z ) - ( t.z*v.y ) ),
		( ( t.z*v.x ) - ( t.x*v.z ) ),
		( ( t.x*v.y ) - ( t.y*v.x ) ) );
	return cp;
}*/


//! Compute the magnitude (length) of the vector
template<class Scalar>
inline Scalar norm ( const vector4Dim<Scalar>& v ) {
	Scalar l = v.x*v.x + v.y*v.y + v.z*v.z + v.t*v.t;
	return ( fabs ( l-1. ) < VECTOR_EPSILON*VECTOR_EPSILON ) ? 1. : sqrt ( l );
}

//! Compute squared magnitude
template<class Scalar>
inline Scalar normSquare ( const vector4Dim<Scalar>& v ) {
	return v.x*v.x + v.y*v.y + v.z*v.z + v.t*v.t;
}

//! Returns a normalized vector
template<class Scalar>
inline vector4Dim<Scalar> getNormalized ( const vector4Dim<Scalar>& v ) {
	Scalar l = v.x*v.x + v.y*v.y + v.z*v.z + v.t*v.t;
	if ( fabs ( l-1. ) < VECTOR_EPSILON*VECTOR_EPSILON )
		return v; /* normalized "enough"... */
	else if ( l > VECTOR_EPSILON*VECTOR_EPSILON )
	{
		Scalar fac = 1./sqrt ( l );
		return vector4Dim<Scalar> ( v.x*fac, v.y*fac, v.z*fac , v.t*fac );
	}
	else
		return vector4Dim<Scalar> ( ( Scalar ) 0 );
}

//! Compute the norm of the vector and normalize it.
/*! \return The value of the norm */
template<class Scalar>
inline Scalar normalize ( vector4Dim<Scalar> &v ) {
	Scalar norm;
	Scalar l = v.x*v.x + v.y*v.y + v.z*v.z + v.t*v.t;
	if ( fabs ( l-1. ) < VECTOR_EPSILON*VECTOR_EPSILON ) {
		norm = 1.;
	} else if ( l > VECTOR_EPSILON*VECTOR_EPSILON ) {
		norm = sqrt ( l );
		v *= 1./norm;        
	} else {
		v = vector4Dim<Scalar>::ZERO;
		norm = 0.;
	}
	return ( Scalar ) norm;
}

template<class Scalar>
inline bool equal(const vector4Dim<Scalar> &v, const vector4Dim<Scalar> &c)
{
  return (ABS(v[0]-c[0]) + 
	  ABS(v[1]-c[1]) + 
	  ABS(v[2]-c[2]) +
	  ABS(v[3]-c[3]) < VECTOR_EPSILON);
}

//! Outputs the object in human readable form as string
template<class Scalar> std::string vector4Dim<Scalar>::toString() const {
	char buf[256];
	snprintf ( buf,256,"<%f,%f,%f,%f>", ( double ) ( *this ) [0], ( double ) ( *this ) [1], ( double ) ( *this ) [2] , ( double ) ( *this ) [3] );
	return std::string ( buf );
}

//! Outputs the object in human readable form to stream
template<class Scalar>
std::ostream& operator<< ( std::ostream& os, const vector4Dim<Scalar>& i ) {
	char buf[256];
	snprintf ( buf,256,"[%d,%d,%d,%d]", (double) i[0], (double) i[1], (double) i[2] , (double) i[3] );
	os << std::string ( buf );
	return os;
}

//! Reads the contents of the object from a stream 
template<class Scalar>
std::istream& operator>> ( std::istream& is, vector4Dim<Scalar>& i ) {
	char c;
	char dummy[4];
	is >> c >> i[0] >> dummy >> i[1] >> dummy >> i[2] >> dummy >> i[3] >> c;
	return is;
}

/**************************************************************************/
// Define default vector alias
/**************************************************************************/

//! 3D vector class of type Real (typically float)
typedef vector4Dim<Real>  Vec4;

// a 3D vector with double precision
typedef vector4Dim<double>  nVec4d; 

// a 3D vector with single precision
typedef vector4Dim<float>   nVec4f; 

//! 3D vector class of type int
typedef vector4Dim<int>  nVec4i;

/* convert int,float and double vectors */
template<class T> inline nVec4i vec42I(T v) { return nVec4i((int)v[0],(int)v[1],(int)v[2],(int)v[3]); }
template<class T> inline nVec4i vec42I(T v0, T v1, T v2, T v3) { return nVec4i((int)v0,(int)v1,(int)v2,(int)v3); }
template<class T> inline nVec4d vec42D(T v) { return nVec4d(v[0],v[1],v[2],v[3]); }
template<class T> inline nVec4i vec42D(T v0, T v1, T v2, T v3) { return nVec4d((double)v0,(double)v1,(double)v2,(double)v3); }
template<class T> inline nVec4f vec42F(T v) { return nVec4f(v[0],v[1],v[2],v[3]); }
template<class T> inline nVec4i vec42F(T v0, T v1, T v2, T v3) { return nVec4f((float)v0,(float)v1,(float)v2,(float)v3); }
template<class T> inline nVec4i vec4round(T v) { return nVec4i((int)round(v[0]),(int)round(v[1]),(int)round(v[2]),(int)round(v[3])); }
template<class T> inline Vec4   vec42R(T v) { return Vec4(v[0],v[1],v[2],v[3]); }

}; // namespace


#endif
