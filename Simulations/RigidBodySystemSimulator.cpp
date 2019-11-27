#include "RigidBodySystemSimulator.h"

RigidBodySystemSimulator::RigidBodySystemSimulator()
{
	m_externalForce = Vec3(0, 0, 0);
	torque = Vec3(0, 0, 0);
	count = 0;
}

const char * RigidBodySystemSimulator::getTestCasesStr()
{
	return "Demo1, Demo2, Demo3, Demo4";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;

}

void RigidBodySystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	Mat4 objToWorld;
	switch (m_iTestCase)
	{
	case 0:
		DUC->drawRigidBody(
			Mat4(m_pRigidBodySystem[0].size.x, 0, 0, 0,
				0, m_pRigidBodySystem[0].size.y, 0, 0,
				0, 0, m_pRigidBodySystem[0].size.z, 0,
				0, 0, 0, 1) *
			m_pRigidBodySystem[0].orientation.getRotMat() *
			Mat4(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				m_pRigidBodySystem[0].comPosition.x, m_pRigidBodySystem[0].comPosition.y, m_pRigidBodySystem[0].comPosition.z, 1));
		break;
	case 1:
		objToWorld = Mat4(m_pRigidBodySystem[1].size.x, 0, 0, 0,
			0, m_pRigidBodySystem[1].size.y, 0, 0,
			0, 0, m_pRigidBodySystem[1].size.z, 0,
			0, 0, 0, 1) *
			m_pRigidBodySystem[1].orientation.getRotMat() *
			Mat4(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				m_pRigidBodySystem[1].comPosition.x, m_pRigidBodySystem[1].comPosition.y, m_pRigidBodySystem[1].comPosition.z, 1);
		DUC->drawRigidBody(objToWorld);
		DUC->drawSphere(Vec3(0, 0, 0), Vec3(0.01f, 0.01f, 0.01f));
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}


}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case 0:
		addRigidBody(Vec3(0, 0, 0), Vec3(1, 0.6f, 0.5f), 2);
		setOrientationOf(0, Quat(0, 0, sqrt(2) / 2, sqrt(2) / 2));
		applyForceOnBody(0, Vec3(0.3f, 0.5f, 0.25f), Vec3(1, 1, 0));
		IntegrateRigidBody(0, 2);
		break;
	case 1:
		addRigidBody(Vec3(-0.2f, -0.1f, 0.1f), Vec3(0.2, 0.6f, 0.5f), 2);

		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed)
{
	if (m_iTestCase == 1) {
		Point2D mouseDiff;
		mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
		mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;

		Vec3 contact =  - Vec3(m_oldtrackmouse.x, m_oldtrackmouse.y, 0);

		if (mouseDiff.x != 0 || mouseDiff.y != 0)
		{
			Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
			worldViewInv = worldViewInv.inverse();
			Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
			Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);

			contact = worldViewInv.transformVectorNormal(contact);

			// find a proper scale!
			float inputScale = 0.001f;
			contact = contact * inputScale;
			inputWorld = inputWorld * inputScale;
			m_externalForce += inputWorld;
			torque += cross(inputWorld, contact);
		}
	}
}

void RigidBodySystemSimulator::simulateTimestep(float timeStep)
{

	if (m_iTestCase == 1) {
		IntegrateRigidBody(1, timeStep);
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
	m_externalForce = force;
	Vec3 objloc = (m_pRigidBodySystem[i].orientation.getRotMat() *
		Mat4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			m_pRigidBodySystem[i].comPosition.x, m_pRigidBodySystem[i].comPosition.y, m_pRigidBodySystem[i].comPosition.z, 1)).inverse().transformVector(loc);
	torque = cross(objloc, force);

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

void RigidBodySystemSimulator::IntegrateRigidBody(int i, float step)
{
	m_pRigidBodySystem[i].comPosition += step * m_pRigidBodySystem[i].comVelocity;
	m_pRigidBodySystem[i].comVelocity += (step * m_externalForce / m_pRigidBodySystem[i].mass);
	m_externalForce = Vec3(0, 0, 0);

	Quat temp = Quat(m_pRigidBodySystem[i].angularVelocity.x, m_pRigidBodySystem[i].angularVelocity.y, m_pRigidBodySystem[i].angularVelocity.z, 0) * m_pRigidBodySystem[i].orientation;

	m_pRigidBodySystem[i].orientation += (step * 0.5f) * temp;
	float norm = m_pRigidBodySystem[i].orientation.norm();
	m_pRigidBodySystem[i].orientation = (1 / norm) * m_pRigidBodySystem[i].orientation;

	m_pRigidBodySystem[i].angularMomentum += step * torque;
	Mat4 transposeRotation = m_pRigidBodySystem[i].orientation.getRotMat();
	transposeRotation.transpose();
	m_pRigidBodySystem[i].intertiaTensorInverse = m_pRigidBodySystem[i].orientation.getRotMat() * m_pRigidBodySystem[i].intertiaTensorInverse * transposeRotation;
	m_pRigidBodySystem[i].angularVelocity = m_pRigidBodySystem[i].intertiaTensorInverse * m_pRigidBodySystem[i].angularMomentum;
	torque = Vec3(0, 0, 0);
	Vec3 pointVelocity = m_pRigidBodySystem[i].comVelocity + cross(m_pRigidBodySystem[i].angularVelocity, Vec3(-0.3f, -0.5f, -0.25f));

	
	if (m_iTestCase == 0) {
		cout << "Angular Velocity after one Euler step: " << m_pRigidBodySystem[i].angularVelocity << "\n";
		cout << "Linear Velocity at point (-0.3, -0.5, -0.25) after one Euler step: " << pointVelocity << "\n";
	}
	/*
	else {
		if (m_iTestCase == 1) {
			cout << "Position of rigid body after one Euler step: " << m_pRigidBodySystem[i].comPosition << "\n";
			cout << "Linear velocity of rigid body after one Euler step: " << m_pRigidBodySystem[i].comVelocity << "\n";

		}
	}*/

}
