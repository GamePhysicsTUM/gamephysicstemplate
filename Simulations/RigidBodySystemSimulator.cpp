#include "RigidBodySystemSimulator.h"

RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	m_externalForce = Vec3(0, 0, 0);
	count = 0;
	damping = 0;
	iterator = 4;

	//Demo 1
	addRigidBody(Vec3(0, 0, 0), Vec3(1, 0.6f, 0.5f), 2);
	setOrientationOf(0, Quat(0, 0, sqrt(2) / 2, sqrt(2) / 2));
	applyForceOnBody(0, Vec3(0.3f, 0.5f, 0.25f), Vec3(1, 1, 0));

	//Demo 2
	addRigidBody(Vec3(-0.2f, -0.1f, 0.1f), Vec3(0.2, 0.6f, 0.5f), 2);
	applyForceOnBody(1, Vec3(-0.1f, -0.4f, 0.35f), Vec3(1, 1, 0));

	//Demo3
	addRigidBody(Vec3(0.2f, 0.3f, 0), Vec3(0.1f, 0.1f, 0.1f), 4);
	setOrientationOf(2, Quat(0, sin(M_PI / 8), 0, cos(M_PI / 8)) * Quat(0, 0, sin(M_PI / 8), cos(M_PI / 8)));
	setVelocityOf(2, Vec3(-0.1f, -0.1f, 0));

	addRigidBody(Vec3(0.15f, 0.05f, 0), Vec3(0.2f, 0.2f, 0.1f), 8);

	//Demo4
	addRigidBody(Vec3(-0.25f, -0.4f, 0), Vec3(0.1f, 0.1f, 0.1f), 1);
	addRigidBody(Vec3(0.25f, -0.4f, 0), Vec3(0.1f, 0.1f, 0.1f), 1);

	addRigidBody(Vec3(0.1f, -0.2f, 0), Vec3(0.1f, 0.1f, 0.1f), 1);
	addRigidBody(Vec3(-0.15f, -0.2f, 0), Vec3(0.1f, 0.1f, 0.1f), 1);

	//Floor and walls
	addRigidBody(Vec3(0, -0.55f, 0), Vec3(1, 0.025f, 1), 1000000);
	m_pRigidBodySystem[8].calculateObjToWorldMatrix();
	addRigidBody(Vec3(0, 0.51f, 0), Vec3(1, 0.025f, 1), 1000000);
	m_pRigidBodySystem[9].calculateObjToWorldMatrix();

	addRigidBody(Vec3(-0.51f, 0, 0), Vec3(0.025f, 1, 1), 1000000);
	m_pRigidBodySystem[10].calculateObjToWorldMatrix();
	addRigidBody(Vec3(0.51f, 0, 0), Vec3(0.025f, 1, 1), 1000000);
	m_pRigidBodySystem[11].calculateObjToWorldMatrix();

	addRigidBody(Vec3(0, 0, -0.51f), Vec3(1, 1, 0.025f), 1000000);
	m_pRigidBodySystem[12].calculateObjToWorldMatrix();
	addRigidBody(Vec3(0, 0, 0.51f), Vec3(1, 1, 0.025f), 1000000);
	m_pRigidBodySystem[13].calculateObjToWorldMatrix();
}

const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return "Demo1, Demo2, Demo3, Demo4";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	if (m_iTestCase == 3) {
		TwType TW_TYPE_INTEGRATOR = TwDefineEnumFromString("Apply force on", "Red, Yellow, Green, White");
		TwAddVarRW(DUC->g_pTweakBar, "Apply force on", TW_TYPE_INTEGRATOR, &objectColor, "");
		objectColor = RED;
	}
}

void RigidBodySystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	Mat4 objToWorld1;

	switch (m_iTestCase)
	{
	case 0:
		DUC->setUpLighting(Vec3(1, 1, 1), Vec3(0, 0, 0), 1, Vec3(0, 0, 0));
		m_pRigidBodySystem[0].calculateObjToWorldMatrix();
		DUC->drawRigidBody(m_pRigidBodySystem[0].objToWorldMat);
		break;
	case 1:
		DUC->setUpLighting(Vec3(1, 1, 1), Vec3(0, 0, 0), 1, Vec3(0, 0, 0));
		m_pRigidBodySystem[1].calculateObjToWorldMatrix();
		DUC->drawRigidBody(m_pRigidBodySystem[1].objToWorldMat);
		break;
	case 2:
		DUC->setUpLighting(Vec3(1, 1, 1), Vec3(0, 0, 0), 1, Vec3(0, 0, 0));
		m_pRigidBodySystem[2].calculateObjToWorldMatrix();
		DUC->drawRigidBody(m_pRigidBodySystem[2].objToWorldMat);

		m_pRigidBodySystem[3].calculateObjToWorldMatrix();
		DUC->drawRigidBody(m_pRigidBodySystem[3].objToWorldMat);
		break;
	case 3:
		m_pRigidBodySystem[4].calculateObjToWorldMatrix();
		DUC->setUpLighting(Vec3(1, 0, 0), Vec3(0, 0, 0), 1, Vec3(0, 0, 0));
		DUC->drawRigidBody(m_pRigidBodySystem[4].objToWorldMat);

		m_pRigidBodySystem[5].calculateObjToWorldMatrix();
		DUC->setUpLighting(Vec3(1, 1, 0), Vec3(0, 0, 0), 1, Vec3(0, 0, 0));
		DUC->drawRigidBody(m_pRigidBodySystem[5].objToWorldMat);

		m_pRigidBodySystem[6].calculateObjToWorldMatrix();
		DUC->setUpLighting(Vec3(0, 1, 0), Vec3(0, 0, 0), 1, Vec3(0, 0, 0));
		DUC->drawRigidBody(m_pRigidBodySystem[6].objToWorldMat);

		m_pRigidBodySystem[7].calculateObjToWorldMatrix();
		DUC->setUpLighting(Vec3(1, 1, 1), Vec3(0, 0, 0), 1, Vec3(0, 0, 0));
		DUC->drawRigidBody(m_pRigidBodySystem[7].objToWorldMat);
		break;
	default:
		break;
	}
}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	resetPositions();
	switch (m_iTestCase)
	{
	case 0:
		integrateRigidBody(0, 2);
		break;
	case 1:
		for (int i = 0; i < 100; i++) {
			integrateRigidBody(1, 0.01f);
		}
		break;
	case 2:

		break;
	case 3:
		m_externalForce = Vec3(0, -100, 0);
		damping = 8;
		break;
	default:
		break;
	}
}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{
	switch (m_iTestCase) {
	case 1:
		integrateRigidBody(1, timeStep);
		break;
	case 2:
		collInfo = checkCollisionSAT(m_pRigidBodySystem[2].objToWorldMat, m_pRigidBodySystem[3].objToWorldMat);
		if (collInfo.isValid) {
			handleCollision(2, 3);
		}
		integrateRigidBody(2, timeStep);
		integrateRigidBody(3, timeStep);
		break;
	case 3:
		for (int j = iterator + 1; j < 8; j++) {
			collInfo = checkCollisionSAT(m_pRigidBodySystem[iterator].objToWorldMat, m_pRigidBodySystem[j].objToWorldMat);
			if (collInfo.isValid) {
				handleCollision(iterator, j);
			}
		}
		for (int k = 8; k < 14; k++) {
			collInfo = checkCollisionSAT(m_pRigidBodySystem[iterator].objToWorldMat, m_pRigidBodySystem[k].objToWorldMat);

			if (collInfo.isValid) {
				handleWallCollision(iterator, k);
			}
		}
		integrateRigidBody(iterator, timeStep);

		if (iterator < 8) {
			iterator++;
			break;
		}
		else {
			iterator = 4;
		}
		break;
	default:
		break;
	}
}
void RigidBodySystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void RigidBodySystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

int RigidBodySystemSimulator::getNumberOfRigidBodies()
{
	return count;
}

Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i)
{
	return m_pRigidBodySystem[i].comPosition;
}

Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i)
{
	return  m_pRigidBodySystem[i].comVelocity;
}

Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i)
{
	return m_pRigidBodySystem[i].angularVelocity;
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force)
{
	m_pRigidBodySystem[i].forces += force;
	m_pRigidBodySystem[i].torque += cross(loc, force);
}

void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass)
{
	m_pRigidBodySystem[count].init(position, size, mass);
	count++;
}

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation)
{
	m_pRigidBodySystem[i].orientation = orientation;
}

void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity)
{
	m_pRigidBodySystem[i].comVelocity = velocity;
}

void RigidBodySystemSimulator::integrateRigidBody(int i, float step)
{
	m_pRigidBodySystem[i].comPosition += step * m_pRigidBodySystem[i].comVelocity;
	m_pRigidBodySystem[i].comVelocity += (step * (((m_pRigidBodySystem[i].forces - (damping * m_pRigidBodySystem[i].comVelocity)) / m_pRigidBodySystem[i].mass) + m_externalForce));

	Quat temp = Quat(m_pRigidBodySystem[i].angularVelocity.x, m_pRigidBodySystem[i].angularVelocity.y, m_pRigidBodySystem[i].angularVelocity.z, 0) * m_pRigidBodySystem[i].orientation;

	m_pRigidBodySystem[i].orientation += (step * 0.5f) * temp;
	float norm = m_pRigidBodySystem[i].orientation.norm();
	m_pRigidBodySystem[i].orientation = (1 / norm) * m_pRigidBodySystem[i].orientation;

	m_pRigidBodySystem[i].angularMomentum += step * (m_pRigidBodySystem[i].torque - (0.5f * m_pRigidBodySystem[i].angularMomentum));
	Mat4 transposeRotation = m_pRigidBodySystem[i].orientation.getRotMat();
	transposeRotation.transpose();
	m_pRigidBodySystem[i].intertiaTensorInverse = m_pRigidBodySystem[i].orientation.getRotMat() * m_pRigidBodySystem[i].intertiaTensorInverse * transposeRotation;
	m_pRigidBodySystem[i].angularVelocity = m_pRigidBodySystem[i].intertiaTensorInverse * m_pRigidBodySystem[i].angularMomentum;

	m_pRigidBodySystem[i].forces = Vec3(0, 0, 0);
	m_pRigidBodySystem[i].torque = Vec3(0, 0, 0);

	if (m_iTestCase == 0) {
		cout << "Angular Velocity after one Euler step: " << m_pRigidBodySystem[i].angularVelocity << "\n";
		Vec3 pointVelocity = m_pRigidBodySystem[i].comVelocity + cross(m_pRigidBodySystem[i].angularVelocity, Vec3(-0.3f, -0.5f, -0.25f));
		cout << "Linear Velocity at point (-0.3, -0.5, -0.25) after one Euler step: " << pointVelocity << "\n";
	}
}

void RigidBodySystemSimulator::handleCollision(int objA, int objB) {
	Vec3 posA = m_pRigidBodySystem[objA].worldToObj(collInfo.collisionPointWorld);
	Vec3 velA = m_pRigidBodySystem[objA].comVelocity + cross(m_pRigidBodySystem[objA].angularVelocity, posA);
	Vec3 posB = m_pRigidBodySystem[objB].worldToObj(collInfo.collisionPointWorld);
	Vec3 velB = m_pRigidBodySystem[objB].comVelocity + cross(m_pRigidBodySystem[objB].angularVelocity, posB);
	Vec3 velRel = velA - velB;
	float normalVel = dot(collInfo.normalWorld, velRel);
	float c = 1;
	if (normalVel < 0) {
		float impulse;

		impulse = -(1 + c) * dot(velRel, collInfo.normalWorld) /
			((1 / m_pRigidBodySystem[objA].mass) +
			(1 / m_pRigidBodySystem[objB].mass) +
				dot(((cross(m_pRigidBodySystem[objA].intertiaTensorInverse * cross(posA, collInfo.normalWorld), posA)) +
				(cross(m_pRigidBodySystem[objB].intertiaTensorInverse * cross(posB, collInfo.normalWorld), posB))), collInfo.normalWorld));
		//ObjA
		m_pRigidBodySystem[objA].comPosition += collInfo.depth * collInfo.normalWorld;
		m_pRigidBodySystem[objA].comVelocity += impulse * collInfo.normalWorld / m_pRigidBodySystem[objA].mass;
		m_pRigidBodySystem[objA].angularMomentum += (cross(posA, impulse * collInfo.normalWorld));

		//ObjB
		m_pRigidBodySystem[objB].comPosition -= collInfo.depth * collInfo.normalWorld;
		m_pRigidBodySystem[objB].comVelocity -= impulse * collInfo.normalWorld / m_pRigidBodySystem[objB].mass;
		m_pRigidBodySystem[objB].angularMomentum -= (cross(posB, impulse * collInfo.normalWorld));
	}
}

void RigidBodySystemSimulator::handleWallCollision(int objA, int objB)
{
	Vec3 posA = Vec3(0, 0, 0);

	Vec3 velA = m_pRigidBodySystem[objA].comVelocity + cross(m_pRigidBodySystem[objA].angularVelocity, posA);
	float normalVel = dot(collInfo.normalWorld, velA);
	float c = 1;
	if (normalVel < 0) {
		float impulse;
		impulse = -(1 + c) * dot(velA, collInfo.normalWorld) /
			((1 / m_pRigidBodySystem[objA].mass) +
				dot(cross(m_pRigidBodySystem[objA].intertiaTensorInverse * cross(posA, collInfo.normalWorld), posA), collInfo.normalWorld));
		//ObjA

		m_pRigidBodySystem[objA].comVelocity += impulse * collInfo.normalWorld / m_pRigidBodySystem[objA].mass;
		m_pRigidBodySystem[objA].angularMomentum += (cross(posA, impulse * collInfo.normalWorld));
	}
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{
	if (m_iTestCase == 1 || m_iTestCase == 3) {
		Point2D mouseDiff;
		mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
		mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;

		if (mouseDiff.x != 0 || mouseDiff.y != 0)
		{
			Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
			worldViewInv = worldViewInv.inverse();
			Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
			Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
			Vec3 contact = worldViewInv.transformVector(Vec3(m_trackmouse.x, m_trackmouse.y, 0));

			// find a proper scale!
			float inputScale = 0.1f;
			inputWorld = inputWorld * inputScale;
			if (m_iTestCase == 1)
				applyForceOnBody(1, 0.001f * contact, inputWorld * 0.001f);
			if (m_iTestCase == 3) {
				switch (objectColor) {
				case RED:
					applyForceOnBody(4, Vec3(0, 0, 0), inputWorld * 1);
					break;
				case YELLOW:
					applyForceOnBody(5, Vec3(0, 0, 0), inputWorld * 1);
					break;
				case GREEN:
					applyForceOnBody(6, Vec3(0, 0, 0), inputWorld * 1);
					break;
				case WHITE:
					applyForceOnBody(7, Vec3(0, 0, 0), inputWorld * 1);
					break;
				default:
					break;
				}
			}
		}
	}
}

void RigidBodySystemSimulator::resetPositions() {

	m_externalForce = Vec3(0, 0, 0);
	damping = 0;

	//Reset Demo1
	m_pRigidBodySystem[0].init(Vec3(0, 0, 0), m_pRigidBodySystem[0].size, m_pRigidBodySystem[0].mass);
	setOrientationOf(0, Quat(0, 0, sqrt(2) / 2, sqrt(2) / 2));
	applyForceOnBody(0, Vec3(0.3f, 0.5f, 0.25f), Vec3(1, 1, 0));

	//Reset Demo2
	m_pRigidBodySystem[1].init(Vec3(-0.2f, -0.1f, 0.1f), m_pRigidBodySystem[1].size, m_pRigidBodySystem[1].mass);

	//Reset Demo3
	m_pRigidBodySystem[2].init(Vec3(0.2f, 0.3f, 0), m_pRigidBodySystem[2].size, m_pRigidBodySystem[2].mass);
	setOrientationOf(2, Quat(0, sin(M_PI / 8), 0, cos(M_PI / 8)) * Quat(0, 0, sin(M_PI / 8), cos(M_PI / 8)));
	setVelocityOf(2, Vec3(-0.1f, -0.1f, 0));

	m_pRigidBodySystem[3].init(Vec3(0.15f, 0.05f, 0), m_pRigidBodySystem[3].size, m_pRigidBodySystem[3].mass);

	//Demo4
	m_pRigidBodySystem[4].init(Vec3(-0.25f, -0.4f, 0), m_pRigidBodySystem[4].size, m_pRigidBodySystem[4].mass);
	m_pRigidBodySystem[5].init(Vec3(0.25f, -0.4f, 0), m_pRigidBodySystem[5].size, m_pRigidBodySystem[5].mass);
	m_pRigidBodySystem[6].init(Vec3(0.1f, -0.2f, 0), m_pRigidBodySystem[6].size, m_pRigidBodySystem[6].mass);
	m_pRigidBodySystem[7].init(Vec3(-0.15f, -0.2f, 0), m_pRigidBodySystem[7].size, m_pRigidBodySystem[7].mass);
}
