#ifndef MASSSPRINGSYSTEMSIMULATOR_h
#define MASSSPRINGSYSTEMSIMULATOR_h
#include "Simulator.h"

// Do Not Change
#define EULER 0
#define LEAPFROG 1
#define MIDPOINT 2
// Do Not Change

struct MassPoint {
    MassPoint(Vec3 position, Vec3 velocity, bool isFixed)
        : position(position), velocity(velocity), isFixed(isFixed) {
    }
    Vec3 position;
    Vec3 velocity;
    Vec3 force, initialForce;
    bool isFixed;
};

struct Spring {
    Spring(MassPoint& mp1, MassPoint& mp2, float initialLength) :
        mp1(mp1), mp2(mp2), initialLength(initialLength) {
    }
    MassPoint& mp1, & mp2;
    float initialLength;
};


class MassSpringSystemSimulator:public Simulator{
public:
    // Construtors
    MassSpringSystemSimulator();
    
    // UI Functions
    void initUI(DrawingUtilitiesClass * DUC);
    void reset();
    void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
    void notifyCaseChanged(int testCase);
    void externalForcesCalculations(float timeElapsed);
    void simulateTimestep(float timeStep);
    void onClick(int x, int y);
    void onMouse(int x, int y);

    // Specific Functions
    const char* getTestCasesStr();
    const char* getIntegratorStr();

    void setMass(float mass);
    void setStiffness(float stiffness);
    void setDampingFactor(float damping);
    int addMassPoint(Vec3 position, Vec3 velocity, bool isFixed);
    void addSpring(int masspoint1, int masspoint2, float initialLength);
    int getNumberOfMassPoints() const;
    int getNumberOfSprings() const;
    Vec3 getPositionOfMassPoint(int index) const;
    Vec3 getVelocityOfMassPoint(int index) const;
    void applyExternalForce(Vec3 force);
    
    // Do Not Change
    void setIntegrator(int integrator) {
        m_iIntegrator = integrator;
    }

private:
    // Data Attributes
    float m_fMass;
    float m_fStiffness;
    float m_fDamping;
    int m_iIntegrator;

    std::vector<MassPoint> massPoints;
    std::vector<Spring> springs;

    // UI Attributes
    Vec3 m_springColor;
    Vec3 m_externalForce;
    Point2D m_mouse;
    Point2D m_trackmouse;
    Point2D m_oldtrackmouse;
};
#endif
