/******************************************************************************
 *
 * Basic simple Quaternion class
 *
 *****************************************************************************/


#ifndef UTIL_QUATERIONS_H
#define UTIL_QUATERIONS_H

#include "matrixbase.h"
#include "vectorbase.h"


#define smax(a,b) ((a>b)?(a):(b))
#define scpysign(a,b) ((a)*(b)>=0?(a):-(a))

namespace GamePhysics {

template<class Scalar>
class Quaternion 
{
public:
	Scalar x,y,z,w;

	Quaternion () : x(0),y(0),z(0),w(0) {}
	
	Quaternion (Scalar nx, Scalar ny, Scalar nz, Scalar nw) : 
		x(nx),y(ny),z(nz),w(nw) {}
	
	Quaternion (const Quaternion &q)
	{
		x=q.x; y=q.y; z=q.z; w=q.w;
	}

	Quaternion (const matrix4x4<Scalar> &M)
	{
		w = sqrt( smax( 0, 1 + M.value[0][0] + M.value[1][1] + M.value[2][2] ) ) / 2;
		x = sqrt( smax( 0, 1 + M.value[0][0] - M.value[1][1] - M.value[2][2] ) ) / 2;
		y = sqrt( smax( 0, 1 - M.value[0][0] + M.value[1][1] - M.value[2][2] ) ) / 2;
		z = sqrt( smax( 0, 1 - M.value[0][0] - M.value[1][1] + M.value[2][2] ) ) / 2;
		x = scpysign( x, M.value[2][1] - M.value[1][2] );
		y = scpysign( y, M.value[0][2] - M.value[2][0] );
		z = scpysign( z, M.value[1][0] - M.value[0][1] );
	}

	Quaternion (const vector3Dim<Scalar>& axis, Scalar angle)
	{
		Scalar mult = sin(angle*.5);
		x = axis.x * mult;
		y = axis.y * mult;
		z = axis.z * mult;
		w = cos (angle*.5);
	}

	Quaternion (Scalar rx, Scalar ry, Scalar rz)
	{
		Quaternion qx(vector3Dim<Scalar> (1.,0.,0.),-rx);
		Quaternion qy(vector3Dim<Scalar> (0.,1.,0.),-ry);
		Quaternion qz(vector3Dim<Scalar> (0.,0.,1.),-rz);
		Quaternion q = qz*qy*qx;
		x=q.x; y=q.y; z=q.z; w=q.w;
	}

	matrix4x4<Scalar> getRotMat() const
	{
		DirectX::XMVECTOR v  = XMVectorSet(x,y,z,w);
		matrix4x4<Scalar> M(DirectX::XMMatrixRotationQuaternion(v));
		return M;
	}

	vector3Dim<Scalar> getAxis() const
	{
		Scalar phi2 = acos(w);
		vector3Dim<Scalar> axis = vector3Dim<Scalar> (x,y,z) * (1./sin(phi2));
		normalize(axis);
		return axis * 2.* phi2;
	}
	inline const Quaternion operator+=(const Quaternion &q)  { x+=q.x; y+=q.y; z+=q.z; w+=q.w; return *this;	};
	inline const Quaternion operator+=(const Scalar m)  { x+=m; y+=m; z+=m; w+=m; return *this;	};
	inline const Quaternion operator-=(const Quaternion &q)  { x-=q.x; y-=q.y; z-=q.z; w-=q.w; return *this;	};
	inline const Quaternion operator-=(const Scalar m)  { x-=m; y-=m; z-=m; w-=m; return *this;	};	
	inline const Quaternion operator*=(const Quaternion &q) 
	{
		vector3Dim<Scalar> v1(x,y,z), v2(q.x,q.y,q.z);
		vector3Dim<Scalar> nv = v1*q.w + v2*w + cross(v2,v1);
		Scalar nw = w*q.w - (v1.x*v2.x+v1.y*v2.y+v1.z*v2.z);
		x = nv.x;
		y = nv.y;
		z = nv.z;
		w = nw;
		return *this;
	}
	inline const Quaternion operator*=(const Scalar m)  { x*=m;y*=m;z*=m;w*=m; return *this;	};
	inline const Quaternion operator/=(const Scalar m)  { x/=m;y/=m;z/=m;w/=m; return *this;	};
	inline const Quaternion operator+(const Quaternion &q) const { return Quaternion(x+q.x,y+q.y,z+q.z,w+q.w);		};
	inline const Quaternion operator-(const Quaternion &q) const { return Quaternion(x-q.x,y-q.y,z-q.z,w-q.w);		};
	inline const Quaternion operator*(const Scalar m) const {	return Quaternion(x*m,y*m,z*m,w*m);		};
	inline const Quaternion operator-() const { return Quaternion(-x,-y,-z,-w);	 };
	inline Scalar dot(const Quaternion &q) const { return x*q.x+y*q.y+z*q.z+w*q.w; }
	inline Scalar normSq() const { return x*x+y*y+z*z+w*w; }
	inline Scalar norm() const { return sqrt(normSq()); }
	inline const Quaternion unit() const { Scalar d=1./norm(); return Quaternion(x*d,y*d,z*d,w*d); }
	inline const DirectX::XMVECTOR toDirectXQuat(){return XMVectorSet(x,y,z,w);};

	inline const Quaternion operator*(const Quaternion &q) const
	{
		vector3Dim<Scalar> v1(x,y,z), v2(q.x,q.y,q.z);
		vector3Dim<Scalar> nv = v1*q.w + v2*w + cross(v2,v1);
		Scalar nw = w*q.w - (v1.x*v2.x+v1.y*v2.y+v1.z*v2.z);

		return Quaternion(nv.x,nv.y,nv.z,nw);
	}

	static const Quaternion slerp(Quaternion q, Quaternion p, Scalar t)
	{
		Scalar cosphi = q.dot(p);

		if(cosphi < 0.0f)
		{
			cosphi *= -1.0f;
			q = -q;
		}

		const Scalar DOT_THRESHOLD = 0.9995f;
		if (cosphi > DOT_THRESHOLD) {
			// interpolate linearly
			return (q + (p - q) * t).unit();
		}

		Scalar sinphi = sqrt(1. - cosphi * cosphi);
		Scalar phi = acos(cosphi);

		Quaternion res = q * (sin( phi * (1.-t) ) / sinphi) + p * (sin( phi * t) / sinphi);

		return res;
	}
	


};
template<class Scalar>
inline Quaternion<Scalar> operator+(int s, Quaternion<Scalar> &q) { return q + s; }
template<class Scalar>
inline Quaternion<Scalar> operator+(float s, Quaternion<Scalar> &q) { return q + s; }
template<class Scalar>
inline Quaternion<Scalar> operator+(double s, Quaternion<Scalar> &q) { return q + s; }

template<class Scalar>
inline Quaternion<Scalar> operator*(int s, Quaternion<Scalar> &q) { return q * s; }
template<class Scalar>
inline Quaternion<Scalar> operator*(float s, Quaternion<Scalar> &q) { return q * s; }
template<class Scalar>
inline Quaternion<Scalar> operator*(double s, Quaternion<Scalar> &q) { return q * s; }

template<class Scalar>
std::ostream&
operator<<( std::ostream& os, const Quaternion<Scalar>& q)
{
	char buf[256];
	snprintf(buf,256,"<%g,%g,%g,%g>", (double)q.x,(double)q.y,(double)q.z,(double)q.w);
	os << std::string(buf); 
	return os;
}

typedef Quaternion<Real> Quat;

}


#endif // __Quaternion_h__


