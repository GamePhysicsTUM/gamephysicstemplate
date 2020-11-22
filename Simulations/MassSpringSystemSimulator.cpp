#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator() {
    m_fMass = 10;
    m_fStiffness = 40;
    m_fDamping = 0.05;
    m_iTestCase = 0;
    m_iIntegrator = EULER;
}

const char* MassSpringSystemSimulator::getTestCasesStr()
{
	return "2-point, Complex";
}

const char* MassSpringSystemSimulator::getIntegratorStr()
{
    return "Euler, Leapfrog, Midpoint";
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;
    TwType TW_TYPE_INTEGRATOR = TwDefineEnumFromString("Integrator", getIntegratorStr());
    TwAddVarRW(DUC->g_pTweakBar, "Integrator", TW_TYPE_INTEGRATOR, &m_iIntegrator, "");
    TwAddVarRW(DUC->g_pTweakBar, "Mass Spheres", TW_TYPE_FLOAT, &m_fMass, "min=0.1 step=0.1");
    TwAddVarRW(DUC->g_pTweakBar, "Stiffness", TW_TYPE_FLOAT, &m_fStiffness, "min=0.5 step=0.5");
    TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "min=0.01 step=0.01");
}

void MassSpringSystemSimulator::reset()
{
    m_mouse.x = m_mouse.y = 0;
    m_trackmouse.x = m_trackmouse.y = 0;
    m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
    switch (m_iTestCase)
    {
    case 0:
        cout << "2-point !\n";
        break;
    case 1:
        cout << "Complex!\n";
        break;
    default:
        cout << "Empty Test!\n";
        break;
    }
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
    switch (m_iIntegrator)
    {
    case 0:
        // position = old_position + timeStep * old_velocity
        // velocity = old_velocity + timeStep / mass * Force
        break;
    case 1:
        // intermediate_position = old_position + 0.5 * timeStep * old_velocity
        // intermediate_velocity = old_velocity + 0.5 * timeStep / mass * Force
        // position = old_position + timeStep * intermediate_velocity
        // velocity = old_velocity + timeStep * intermediate_Force
        break;
    default:break;
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
	return 0;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
}

int MassSpringSystemSimulator::getNumberOfMassPoints()
{
    switch (m_iTestCase)
    {
    case 0:
        return 2;
    case 1:
        return 10;
    default:break;
    }
}

int MassSpringSystemSimulator::getNumberOfSprings()
{
    switch (m_iTestCase)
    {
    case 0:
        return 1;
    case 1:
        return 10;
    default:break;
    }
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index)
{
	return Vec3();
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index)
{
	return Vec3();
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
}
