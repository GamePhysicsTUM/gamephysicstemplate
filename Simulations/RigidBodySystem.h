
#ifndef RIGIDBODYSYSTEM_h
#define RIGIDBODYSYSTEM_h
#include "Simulator.h"

class RigidBodySystem {
public:
	// Construtors
	
	RigidBodySystem();
	void init(Vec3 position, Vec3 size, float mass);
	float mass;
	Vec3 comPosition;
	Vec3 comVelocity;
	Mat4 intertiaTensorInverse;
	Vec3 angularMomentum;
	Vec3 angularVelocity;
	Quat orientation;
	Vec3 size;

	void precalculateIntertiaInverse();
private:

};
#endif