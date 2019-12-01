#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem()
{
}

void RigidBodySystem::init(Vec3 position, Vec3 size, float mass)
{
	this->torque = Vec3(0, 0, 0);
	this->forces = Vec3(0, 0, 0);
	this->size = size;
	this->mass = mass;
	precalculateIntertiaInverse();
	comVelocity = Vec3(0, 0, 0);

	this->comPosition = position;
	this->angularMomentum = Vec3(0, 0, 0);
	this->orientation = Quat(0, 0, 0, 1);
	this->angularVelocity = Vec3(0, 0, 0);

	calculateObjToWorldMatrix();
}

void RigidBodySystem::precalculateIntertiaInverse()
{
	float xx = 12 / (mass * (pow(size.y, 2) + pow(size.z, 2)));
	float yy = 12 / (mass * (pow(size.x, 2) + pow(size.z, 2)));
	float zz = 12 / (mass * (pow(size.x, 2) + pow(size.y, 2)));
	intertiaTensorInverse = Mat4(xx, 0, 0, 0,
		0, yy, 0, 0,
		0, 0, zz, 0,
		0, 0, 0, 1);
}

Vec3 RigidBodySystem::worldToObj(Vec3 position) {
	Vec3 objPos = (orientation.getRotMat() *
		Mat4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			comPosition.x, comPosition.y, comPosition.z, 1)).inverse().transformVector(position);
	return objPos;
}

Mat4 RigidBodySystem::calculateObjToWorldMatrix() {
	objToWorldMat = Mat4(size.x, 0, 0, 0,
		0, size.y, 0, 0,
		0, 0, size.z, 0,
		0, 0, 0, 1) *
		orientation.getRotMat() *
		Mat4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			comPosition.x, comPosition.y, comPosition.z, 1);
	return objToWorldMat;
}

bool RigidBodySystem::containsPoint(Vec3 position) {
	if ((position.x > comPosition.x + size.x / 2) || (position.x < comPosition.x - size.x / 2)
		|| (position.y > comPosition.y + size.y / 2) || (position.y < comPosition.y - size.y / 2)
		|| (position.z > comPosition.z + size.z / 2) || (position.z < comPosition.z - size.z / 2)) {
		return false;
	}
	return true;
}
