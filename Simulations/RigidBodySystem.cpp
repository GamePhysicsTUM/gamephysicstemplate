#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem()
{
}

void RigidBodySystem::init(Vec3 position, Vec3 size, float mass)
{
	this->size = size;
	this->mass = mass;
	precalculateIntertiaInverse();
	comVelocity = Vec3(0, 0, 0);
	
	this->comPosition = position;
	this->angularMomentum = Vec3(0, 0, 0);
	this->orientation = Quat(0,0,0,1);
}

void RigidBodySystem::precalculateIntertiaInverse()
{
	float xx = 12 / (mass * (pow(size.y, 2) + pow(size.z, 2)));
	float yy = 12 / (mass * (pow(size.x, 2) + pow(size.z, 2)));
	float zz = 12 / (mass * (pow(size.x, 2) + pow(size.y, 2)));
	/*
	float xx = pow(size.x, 2) * 8;
	float yy = pow(size.y, 2) * 8;
	float zz = pow(size.z, 2) * 8;
	*/
	intertiaTensorInverse = Mat4(xx, 0,0,0,
		0,yy,0,0,
		0,0, zz,0,
		0,0,0,1);
}


