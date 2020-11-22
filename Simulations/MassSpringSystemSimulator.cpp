#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator() {
    m_fMass = 10;
    m_fStiffness = 40;
    m_fDamping = 0;
    m_iIntegrator = EULER;

    m_springColor = Vec3(50, 50, 50);
    m_externalForce = Vec3(0, -9.81, 0) * m_fMass;
    m_mouse = Point2D();
    m_trackmouse = Point2D();
    m_oldtrackmouse = Point2D();
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
    TwAddVarRW(DUC->g_pTweakBar, "Mass Spheres", TW_TYPE_FLOAT, &m_fMass, "min=0.1 max=100.0 step=0.1");
    TwAddVarRW(DUC->g_pTweakBar, "Stiffness", TW_TYPE_FLOAT, &m_fStiffness, "min=0.0 max=100 step=0.5");
    TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "min=0.00 max=5.00 step=0.05");
}

void MassSpringSystemSimulator::reset()
{
    (this->springs).clear();
    (this->massPoints).clear();

    m_mouse.x = m_mouse.y = 0;
    m_trackmouse.x = m_trackmouse.y = 0;
    m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
    Vec3 scale = Vec3(0.1f, 0.1f, 0.1f);
    for (MassPoint &mp : this->massPoints) {
        this->DUC->drawSphere(mp.position, scale);
    }

    
    this->DUC->beginLine();
    for (Spring& s : this->springs) {
        this->DUC->drawLine(s.mp1.position, m_springColor, s.mp2.position, m_springColor);
    }
    this->DUC->endLine();
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
    this->reset();
    switch (testCase) {
    case 0:
        addSpring(
            addMassPoint({ 0, 0, 0 }, { -1, 0, 0 }, false),
            addMassPoint({ 0, 2, 0 }, { 1, 0, 0 }, false),
            1
        );
        break;
    case 1:
        // 10 mass points, 10 springs
        break;
    }
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
    for (MassPoint& mp : this->massPoints) {
        mp.force = Vec3(0, 0, 0);
        mp.force += m_externalForce;
        mp.force += mp.velocity * -m_fDamping;
    }
    // Internal forces
    for (Spring& s : this->springs) {
        Vec3 elasticForce, differenceVector;
        Vec3 mid_X_mp1, mid_X_mp2;
        float distance = 0;

        switch (m_iIntegrator) {
        case EULER:
            differenceVector = (s.mp1.position - s.mp2.position);
            distance = sqrt(s.mp1.position.squaredDistanceTo(s.mp2.position));
            break;

        case MIDPOINT:
            mid_X_mp1 = s.mp1.position + s.mp1.velocity * (timeStep / 2);
            mid_X_mp2 = s.mp2.position + s.mp2.velocity * (timeStep / 2);

            differenceVector = (mid_X_mp1 - mid_X_mp2);
            distance = sqrt(mid_X_mp1.squaredDistanceTo(mid_X_mp2));
            break;

        case LEAPFROG:
            break;
        }

        elasticForce = (differenceVector / distance) * ((distance - (double)s.initialLength) * -m_fStiffness);
        // Apply to endpoints
        s.mp1.force += elasticForce;
        s.mp2.force += -elasticForce;
    }
    // Integrate forces into motion
    for (MassPoint& mp : this->massPoints) {
        if (mp.isFixed) continue;
        mp.position += mp.velocity * timeStep;
        mp.velocity += (mp.force / m_fMass) * timeStep;

        // Gravity & collision with floor
        float floor_level = -1.0;
        if (mp.position.y < floor_level) {
            mp.position.y = floor_level + (floor_level - mp.position.y);
            mp.velocity.y *= -1;
        }
    }
}

void MassSpringSystemSimulator::onClick(int x, int y)
{
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
}

// Specific Functions
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

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 velocity, bool isFixed)
{
    int index = (this->massPoints).size();
    (this->massPoints).push_back(MassPoint(position, velocity, isFixed));
    return index;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
    (this->springs).push_back(
        Spring((this->massPoints)[masspoint1], (this->massPoints)[masspoint2], initialLength)
    );
}

int MassSpringSystemSimulator::getNumberOfMassPoints() const
{
    return massPoints.size();
}

int MassSpringSystemSimulator::getNumberOfSprings() const
{
    return springs.size();
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index) const
{
    return (this->massPoints)[index].position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index) const
{
    return (this->massPoints)[index].velocity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
}
