#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_externalForce = Vec3(0, 0, 0);
	masspointsCounter = 0;
	springsCounter = 0;
	m_fMass = 0;
	m_fStiffness = 0;
	m_fDamping = 0;

	//DEMO 1,2,3
	this->addMassPoint(Vec3(0.0, 0.0f, 0), Vec3(-1.0, 0.0f, 0), true);
	this->addMassPoint(Vec3(0.0, 2.0f, 0), Vec3(1.0, 0.0f, 0), true);
	this->addSpring(0, 1, 1.0);

	//Demo 4
	this->addMassPoint(Vec3(0.1f, 0.1f, 0.1f), Vec3(0, 0, 0), false);//2
	this->addMassPoint(Vec3(0.1f, -0.1f, 0.1f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(0.1f, 0.1f, -0.1f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(0.1f, -0.1f, -0.1f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(-0.1f, 0.1f, 0.1f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(-0.1f, -0.1f, 0.1f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(-0.1f, 0.1f, -0.1f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(-0.1f, -0.1f, -0.1f), Vec3(0, 0, 0), false);//9
	
	this->addMassPoint(Vec3(0, 0.161f, 0.062f), Vec3(0, 0, 0), false);//10
	this->addMassPoint(Vec3(0, -0.161f, 0.062f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(0, 0.161f, -0.062f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(0, -0.161f, -0.062f), Vec3(0, 0, 0), false);//13
	
	this->addMassPoint(Vec3(0.062f, 0, 0.161f), Vec3(0, 0, 0), false);//14
	this->addMassPoint(Vec3(-0.062f, 0, 0.161f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(0.062f, 0, -0.161f), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(-0.062f, 0, -0.161f), Vec3(0, 0, 0), false);//17

	this->addMassPoint(Vec3(0.161f, 0.062f, 0), Vec3(0, 0, 0), false);//18
	this->addMassPoint(Vec3(-0.161f, 0.062f, 0), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(0.161f, -0.062f, 0), Vec3(0, 0, 0), false);
	this->addMassPoint(Vec3(-0.161f, -0.062f, 0), Vec3(0, 0, 0), false);//21

	this->addMassPoint(Vec3(0, 0, 0), Vec3(0, 0, 0), false);//22

	Vec3 diff = masspoints[2].position - masspoints[22].position;
	float distance = sqrt(dot(diff, diff));
	diff = masspoints[2].position - masspoints[18].position;
	float edge = sqrt(dot(diff, diff));

	this->addSpring(2, 18, edge);
	this->addSpring(2, 14, edge);
	this->addSpring(2, 10, edge);

	this->addSpring(3, 20, edge);
	this->addSpring(3, 11, edge);
	this->addSpring(3, 14, edge);

	this->addSpring(4, 18, edge);
	this->addSpring(4, 12, edge);
	this->addSpring(4, 16, edge);

	this->addSpring(5, 20, edge);
	this->addSpring(5, 16, edge);
	this->addSpring(5, 13, edge);

	this->addSpring(6, 15, edge);
	this->addSpring(6, 19, edge);
	this->addSpring(6, 10, edge);

	this->addSpring(7, 21, edge);
	this->addSpring(7, 11, edge);
	this->addSpring(7, 15, edge);

	this->addSpring(8, 19, edge);
	this->addSpring(8, 17, edge);
	this->addSpring(8, 12, edge);
	
	this->addSpring(9, 21, edge);
	this->addSpring(9, 17, edge);
	this->addSpring(9, 13, edge);

	this->addSpring(14, 15, edge);
	this->addSpring(16, 17, edge);

	this->addSpring(18, 20, edge);
	this->addSpring(19, 21, edge);

	this->addSpring(10, 12, edge);
	this->addSpring(11, 13, edge);

	
	for (int i = 2; i < getNumberOfMassPoints() - 1; i++) {
		this->addSpring(i, 22, distance);
	}
	
}

void MassSpringSystemSimulator::resetPositions() {
	masspoints[0].position = Vec3(0, 0, 0);
	masspoints[0].velocity = Vec3(-1, 0, 0);
	masspoints[1].position = Vec3(0, 2, 0);
	masspoints[1].velocity = Vec3(1, 0, 0);
	masspoints[2].position = Vec3(0.1f, 0.1f, 0.1f);
	masspoints[3].position = Vec3(0.1f, -0.1f, 0.1f);
	masspoints[4].position = Vec3(0.1f, 0.1f, -0.1f);
	masspoints[5].position = Vec3(0.1f, -0.1f, -0.1f);
	masspoints[6].position = Vec3(-0.1f, 0.1f, 0.1f);
	masspoints[7].position = Vec3(-0.1f, -0.1f, 0.1f);
	masspoints[8].position = Vec3(-0.1f, 0.1f, -0.1f);
	masspoints[9].position = Vec3(-0.1f, -0.1f, -0.1f);
	masspoints[10].position = Vec3(0, 0.161f, 0.062f);
	masspoints[11].position = Vec3(0, -0.161f, 0.062f);
	masspoints[12].position = Vec3(0, 0.161f, -0.062f);
	masspoints[13].position = Vec3(0, -0.161f, -0.062f);
	masspoints[14].position = Vec3(0.062f, 0, 0.161f);
	masspoints[15].position = Vec3(-0.062f, 0, 0.161f);
	masspoints[16].position = Vec3(0.062f, 0, -0.161f);
	masspoints[17].position = Vec3(-0.062f, 0, -0.161f);
	masspoints[18].position = Vec3(0.161f, 0.062f, 0);
	masspoints[19].position = Vec3(-0.161f, 0.062f, 0);
	masspoints[20].position = Vec3(0.161f, -0.062f, 0);
	masspoints[21].position = Vec3(-0.161f, -0.062f, 0);
	masspoints[22].position = Vec3(0, 0, 0);
	for (int i = 2; i < getNumberOfMassPoints(); i++) {
		masspoints[i].velocity = Vec3(0, 0, 0);
	}
}

const char * MassSpringSystemSimulator::getTestCasesStr()
{
	return "Demo1, Demo2, Demo3, Demo4";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	
	if (m_iTestCase == 3) {
		TwType TW_TYPE_INTEGRATOR = TwDefineEnumFromString("Integrator", "Euler, Leapfrog, Midpoint");
		TwAddVarRW(DUC->g_pTweakBar, "Integrator", TW_TYPE_INTEGRATOR, &m_iIntegrator, "");
		m_iIntegrator = MIDPOINT;
		oldIntegrator = m_iIntegrator;
		TwAddVarRW(DUC->g_pTweakBar, "Mass", TW_TYPE_FLOAT, &m_fMass, "min=0.05 step=0.01 max=0.5");
		TwAddVarRW(DUC->g_pTweakBar, "Stiffness", TW_TYPE_FLOAT, &m_fStiffness, "min=100 max=1500");
		TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "min=0 max=1 step=0.01");


	}
}

void MassSpringSystemSimulator::reset()
{
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void MassSpringSystemSimulator::drawLine(int point1, int point2) {
	DUC->beginLine();
	DUC->drawLine(masspoints[point1].position, Vec3(0, 255, 0), masspoints[point2].position, Vec3(0, 255, 0));
	DUC->endLine();

	DUC->drawSphere(masspoints[point1].position, Vec3(0.01f, 0.01f, 0.01f));
	DUC->drawSphere(masspoints[point2].position, Vec3(0.01f, 0.01f, 0.01f));
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext * pd3dImmediateContext)
{
	switch (m_iTestCase)
	{
	case 0:
		drawLine(0, 1);
		break;
	case 1:
		drawLine(0, 1);
		break;
	case 2:
		drawLine(0, 1);
		break;
	case 3:
		for (int j = 1; j < getNumberOfSprings(); j++) {
			DUC->beginLine();
			DUC->drawLine(masspoints[springs[j].first].position, Vec3(0, 255, 0), masspoints[springs[j].second].position, Vec3(0, 255, 0));
			DUC->endLine();
		}

		for (int i = 2; i < getNumberOfMassPoints(); i++) {
			DUC->drawSphere(masspoints[i].position, Vec3(0.01f, 0.01f, 0.01f));
		}

		break;
	default:
		break;
	}
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{

	m_iTestCase = testCase;
	resetPositions();

	switch (m_iTestCase)
	{
	case 0:
		gravity = (0, 0, 0);
		resetPositions();
		setMass(10);
		setStiffness(40);
		setDampingFactor(0);
		simulateEulerStep(1, 0, 0.1f);
		resetPositions();
		simulateMidpointStep(1, 0, 0.1f);
		
		break;
	case 1:
		gravity = Vec3(0, 0, 0);
		resetPositions();
		setMass(10);
		setStiffness(40);
		setDampingFactor(0);
		simulateEulerStep(10, 0, 0.005f);
		break;
	case 2:
		gravity = Vec3(0, 0, 0);
		resetPositions();
		setMass(10);
		setStiffness(40);
		setDampingFactor(0);
		simulateMidpointStep(10, 0, 0.005f);
		break;
	case 3:
		setMass(0.05f);
		setStiffness(500);
		setDampingFactor(0.1f);
		gravity = Vec3(0, -m_fMass * 10, 0);
		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
}

void MassSpringSystemSimulator::clamp(int point, float min, float max) {
	//clam Y axis
	if (masspoints[point].position.y < min) {
		masspoints[point].position.y = min;
		masspoints[point].velocity *= -1;
	}
	else {
		if (masspoints[point].position.y > max) {
			masspoints[point].position.y = max;
			masspoints[point].velocity *= -1;
		}
	}
	//clamp X axis
	if (
		masspoints[point].position.x < min) {
		masspoints[point].position.x = min;
		masspoints[point].velocity *= -1;
	}
	else {
		if (
			masspoints[point].position.x > max) {
			masspoints[point].position.x = max;
			masspoints[point].velocity *= -1;
		}
	}
	//clamp Z axis
	if (
		masspoints[point].position.z < min) {
		masspoints[point].position.z = min;
		masspoints[point].velocity *= -1;
	}
	else {
		if (
			masspoints[point].position.z > max) {
			masspoints[point].position.z = max;
			masspoints[point].velocity *= -1;
		}
	}
	
}
void MassSpringSystemSimulator::simulateEulerStep(int steps, int spring, float time) {
	Vec3 diff;
	Vec3 springForce;
	Vec3 damping;
	Vec3 forces;
	Vec3 acceleration;

	for (int i = 0; i < steps; i++) {
		diff = masspoints[springs[spring].first].position - masspoints[springs[spring].second].position;

		if (diff.x != 0 || diff.y != 0 || diff.z != 0) {
			springForce = (m_fStiffness * (-sqrtf(dot(diff, diff)) + springs[spring].initL) / sqrtf(dot(diff, diff))) * diff;

			//First point position
			masspoints[springs[spring].first].position += time * masspoints[springs[spring].first].velocity;
			//First point velocity
			damping = m_fDamping * masspoints[springs[spring].first].velocity;
			forces = springForce + m_externalForce - damping;
			acceleration = (forces / m_fMass) + gravity;
			masspoints[springs[spring].first].velocity += time * acceleration;

			//Second point position
			masspoints[springs[spring].second].position += time * masspoints[springs[spring].second].velocity;
			//Second point velocity
			damping = m_fDamping * masspoints[springs[spring].second].velocity;
			forces = -springForce + m_externalForce - damping;
			acceleration = (forces / m_fMass) + gravity;
			masspoints[springs[spring].second].velocity += time * acceleration;

			if (m_iTestCase == 3) {
				clamp(springs[spring].first, -0.5f, 0.5f);
				clamp(springs[spring].second, -0.5f, 0.5f);

			}
			else {
				if (m_iTestCase == 0 || m_iTestCase == 1) {
					cout << "After " << i + 1 << " Euler steps: \n";
					cout << "point 0 vel (" << masspoints[springs[spring].first].velocity << "), pos (" << masspoints[springs[spring].first].position << ") \n";
					cout << "point 1 vel (" << masspoints[springs[spring].second].velocity << "), pos (" << masspoints[springs[spring].second].position << ") \n";
				}
			}
		}
		
	}
}

void MassSpringSystemSimulator::simulateMidpointStep(int steps, int spring, float time) {
	Vec3 diff;
	Vec3 springForce;
	Vec3 damping;
	Vec3 forces;
	Vec3 oldAcceleration;

	
	for (int i = 0; i < steps; i++) {
		diff = masspoints[springs[spring].first].position - masspoints[springs[spring].second].position;
		if (diff.x != 0 || diff.y != 0 || diff.z != 0) {
			Vec3 tempPos1 = masspoints[springs[spring].first].position + (0.5f * time * masspoints[springs[spring].first].velocity);

			Vec3 tempPos2 = masspoints[springs[spring].second].position + 0.5f * time * masspoints[springs[spring].second].velocity;

			springForce = (m_fStiffness * (-sqrtf(dot(diff, diff)) + springs[spring].initL) / sqrtf(dot(diff, diff))) * diff;
			damping = m_fDamping * masspoints[springs[spring].first].velocity;
			forces = springForce + m_externalForce - damping;
			oldAcceleration = (forces / m_fMass) + (gravity);
			Vec3 tempVel1 = masspoints[springs[spring].first].velocity + (0.5f * time * oldAcceleration);

			damping = m_fDamping * masspoints[springs[spring].second].velocity;
			forces = -springForce + m_externalForce - damping;
			oldAcceleration = (forces / m_fMass) + (gravity);
			Vec3 tempVel2 = masspoints[springs[spring].second].velocity + (0.5f * time * oldAcceleration);

			diff = tempPos1 - tempPos2;

			springForce = (m_fStiffness * (-sqrtf(dot(diff, diff)) + springs[spring].initL) / sqrtf(dot(diff, diff))) * diff;
			damping = m_fDamping * tempVel1;
			forces = springForce + m_externalForce - damping;
			Vec3 tempAcc1 = (forces / m_fMass) + (gravity);

			damping = m_fDamping * tempVel2;
			forces = -springForce + m_externalForce - damping;
			Vec3 tempAcc2 = (forces / m_fMass) + (gravity);

			masspoints[springs[spring].first].position += time * tempVel1;
			masspoints[springs[spring].first].velocity += time * tempAcc1;
			masspoints[springs[spring].second].position += time * tempVel2;
			masspoints[springs[spring].second].velocity += time * tempAcc2;

			if (m_iTestCase == 3) {
				clamp(springs[spring].first, -0.5f, 0.5f);
				clamp(springs[spring].second, -0.5f, 0.5f);
			}
			else {
				if (m_iTestCase == 0 || m_iTestCase == 2) {
					cout << "After " << i + 1 << " Midpoint steps: \n";
					cout << "point 0 vel (" << masspoints[springs[spring].first].velocity << "), pos (" << masspoints[springs[spring].first].position << ") \n";
					cout << "point 1 vel (" << masspoints[springs[spring].second].velocity << "), pos (" << masspoints[springs[spring].second].position << ") \n";
				}
			}
		}
	}
}



void MassSpringSystemSimulator::simulateLeapfrogStep(int steps, int spring, float time) {
	Vec3 diff;
	Vec3 springForce;
	Vec3 damping;
	Vec3 forces;
	Vec3 acceleration;


	for (int i = 0; i < steps; i++) {
		diff = masspoints[springs[spring].first].position - masspoints[springs[spring].second].position;
		if (diff.x != 0 || diff.y != 0 || diff.z != 0) {
			springForce = (m_fStiffness * (-sqrtf(dot(diff, diff)) + springs[spring].initL) / sqrtf(dot(diff, diff))) * diff;
			damping = m_fDamping * masspoints[springs[spring].first].velocity;
			forces = springForce + m_externalForce - damping;
			acceleration = (forces / m_fMass) + (gravity);
			masspoints[springs[spring].first].velocity += time * 0.5f * acceleration;
			masspoints[springs[spring].first].position += time * masspoints[springs[spring].first].velocity;

			springForce = (m_fStiffness * (-sqrtf(dot(diff, diff)) + springs[spring].initL) / sqrtf(dot(diff, diff))) * (-diff);
			damping = m_fDamping * masspoints[springs[spring].second].velocity;
			forces = springForce + m_externalForce - damping;
			acceleration = (forces / m_fMass) + (gravity);
			masspoints[springs[spring].second].velocity += time * 0.5f * acceleration;
			masspoints[springs[spring].second].position += time * masspoints[springs[spring].second].velocity;
			if (m_iTestCase == 3) {
				clamp(springs[spring].first, -0.5f, 0.5f);
				clamp(springs[spring].second, -0.5f, 0.5f);
			}
		}
	}

}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	if (m_iTestCase == 3) {
		Point2D mouseDiff;
		mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
		mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;
		if (mouseDiff.x != 0 || mouseDiff.y != 0)
		{
			Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
			worldViewInv = worldViewInv.inverse();
			Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
			Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
			// find a proper scale!
			float inputScale = 0.0005f;
			inputWorld = inputWorld * inputScale;
			m_externalForce = inputWorld;
		}
	}
	
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	if (m_iIntegrator != oldIntegrator) {
		oldIntegrator = m_iIntegrator;
		//resetPositions();
	}
	if (m_externalForce.x != 0) {
		m_externalForce.x /= 2;
	}
	if (m_externalForce.y != 0) {
		m_externalForce.y /= 2;
	}
	if (m_externalForce.z != 0) {
		m_externalForce.z /= 2;
	}
	if (m_iTestCase == 3) {
		for (int i = 1; i < getNumberOfSprings(); i++) {
			switch (m_iIntegrator) {
			case EULER:
				simulateEulerStep(1, i, timeStep);
				break;
			case MIDPOINT:
				simulateMidpointStep(1, i, timeStep);
				break;
			case LEAPFROG:
				simulateLeapfrogStep(1, i, timeStep);
				break;
			}
		}
	}
}

void MassSpringSystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void MassSpringSystemSimulator::setMass(float mass)
{
	m_fMass = mass;
}

void MassSpringSystemSimulator::setStiffness(float stiffness)
{
	m_fStiffness = stiffness;
}

void MassSpringSystemSimulator::setDampingFactor(float damping)
{
	m_fDamping = damping;
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed)
{
	masspointsCounter++;
	Masspoint masspoint;
	masspoint.position = position;
	masspoint.velocity = Velocity;
	masspoints[masspointsCounter - 1] = masspoint;
	return masspointsCounter - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
	springsCounter++;
	Spring spring;
	spring.first = masspoint1;
	spring.second = masspoint2;
	spring.initL = initialLength;
	springs[springsCounter - 1] = spring;
}

int MassSpringSystemSimulator::getNumberOfMassPoints()
{
	return masspointsCounter;
}

int MassSpringSystemSimulator::getNumberOfSprings()
{
	return springsCounter;
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
{
	return masspoints[index].position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
{
	return masspoints[index].velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
	m_externalForce += force;
}
