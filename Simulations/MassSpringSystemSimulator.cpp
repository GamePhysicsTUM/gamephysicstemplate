#include "MassSpringSystemSimulator.h"

MassSpringSystemSimulator::MassSpringSystemSimulator() {
    m_iIntegrator = EULER;
    m_fMass = 10;
    m_fStiffness = 40;
    m_fDamping = 0;
    m_fGravity = 0;
    m_fComplexInitialRatio = 0.50;

    m_springColor = Vec3(50, 50, 50);
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
    TwAddVarRW(DUC->g_pTweakBar, "Stiffness", TW_TYPE_FLOAT, &m_fStiffness, "min=0.0 max=500 step=0.5");
    TwAddVarRW(DUC->g_pTweakBar, "Damping", TW_TYPE_FLOAT, &m_fDamping, "min=0.00 max=5.00 step=0.05");
    TwAddVarRW(DUC->g_pTweakBar, "Gravity", TW_TYPE_FLOAT, &m_fGravity, "min=0.00 max=100.00 step=0.01");
    TwAddVarRW(DUC->g_pTweakBar, "Complex: Edge/Initial", TW_TYPE_FLOAT, &m_fComplexInitialRatio, "min=0.01 max=100.00 step=0.01");
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
        Vec3 positionOffset = Vec3(0, 2, 0);
        float vertexMap[3] = { 0, 1, (1 + sqrt(5)) / 2 };
        vector<Vec3> positions;
        for (int i = 0; i < 3; i++) {
            Vec3 position;
            for (int j = 2; j >= 0; j--) {
                position[(i + j) % 3] = vertexMap[j];
            }
            positions.push_back(position);
            positions.push_back(-position);
            position[(i % 3 + 1) % 3] *= -1;
            positions.push_back(position);
            position[(i % 3 + 1) % 3] *= -1;
            position[(i % 3 + 2) % 3] *= -1;
            positions.push_back(position);
        }
        for(Vec3& position : positions) {
            addMassPoint(position + positionOffset, { 0, 0, 0 }, false);
        }

        float springLength = sqrt(positions[0].squaredDistanceTo(positions[8])) * m_fComplexInitialRatio;
        addSpring(10, 0, springLength);
        addSpring(10, 3, springLength);
        addSpring(10, 5, springLength);
        addSpring(10, 7, springLength);
        addSpring(10, 8, springLength);

        addSpring(11, 1, springLength);
        addSpring(11, 2, springLength);
        addSpring(11, 4, springLength);
        addSpring(11, 6, springLength);
        addSpring(11, 9, springLength);

        addSpring(0, 7, springLength);
        addSpring(7, 5, springLength);
        addSpring(5, 3, springLength);
        addSpring(3, 8, springLength);
        addSpring(8, 0, springLength);

        addSpring(1, 6, springLength);
        addSpring(6, 4, springLength);
        addSpring(4, 2, springLength);
        addSpring(2, 9, springLength);
        addSpring(9, 1, springLength);

        addSpring(3, 1, springLength);
        addSpring(3, 6, springLength);
        addSpring(8, 6, springLength);
        addSpring(8, 4, springLength);
        addSpring(0, 4, springLength);
        addSpring(0, 2, springLength);
        addSpring(7, 2, springLength);
        addSpring(7, 9, springLength);
        addSpring(5, 9, springLength);
        addSpring(5, 1, springLength);
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
        // Gravity
        mp.force += Vec3(0, -m_fGravity, 0) * m_fMass;
        // Damping
        mp.force += mp.velocity * -m_fDamping;
        mp.initialForce = mp.force;
    }
    // Internal forces
    for (Spring& s : this->springs) {
        Vec3 mid_X_mp1, mid_X_mp2;

        Vec3 differenceVector = (s.mp1.position - s.mp2.position);
        float distance = sqrt(s.mp1.position.squaredDistanceTo(s.mp2.position));
        Vec3 elasticForce = (differenceVector / distance) * ((distance - (double)s.initialLength) * -m_fStiffness);
        s.mp1.initialForce += elasticForce;
        s.mp2.initialForce += -elasticForce;

        switch (m_iIntegrator) {
        case EULER:
            break;

        case MIDPOINT:
            mid_X_mp1 = s.mp1.position + s.mp1.velocity * (timeStep / 2);
            mid_X_mp2 = s.mp2.position + s.mp2.velocity * (timeStep / 2);

            differenceVector = (mid_X_mp1 - mid_X_mp2);
            distance = sqrt(mid_X_mp1.squaredDistanceTo(mid_X_mp2));

            elasticForce = (differenceVector / distance) * ((distance - (double)s.initialLength) * -m_fStiffness);
            break;

        case LEAPFROG:
            // v(t + h / 2) = v(t - h / 2) + h * a(t)
            // midV = prevmidV + (mp.force / m_fMass) * timeStep;
            
            // x(t + h) = x(t) + h * v(t + h / 2)
            // mp.position += midV * timeStep;
            break;
        }
        // Apply to endpoints
        s.mp1.force += elasticForce;
        s.mp2.force += -elasticForce;
    }
    // Integrate forces into motion
    for (MassPoint& mp : this->massPoints) {
        if (mp.isFixed) continue;
        Vec3 midVelocity;
        switch (m_iIntegrator) {
        case EULER:
            mp.position += mp.velocity * timeStep;
            break;
        case MIDPOINT:
            midVelocity = mp.velocity + (mp.initialForce / m_fMass) * (timeStep / 2);
            mp.position += midVelocity * timeStep;
            break;
        case LEAPFROG:
            break;
        }
        
        mp.velocity += (mp.force / m_fMass) * timeStep;

        // Floor collision
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
