#include "TemplateSimulator.h"

TemplateSimulator::TemplateSimulator()
{
	m_iTestCase = 0;
	m_vfMovableObjectPos = Vec3();
	m_vfMovableObjectFinalPos = Vec3();
	m_vfRotate = Vec3();
	m_iNumSpheres    = 100;
	m_fSphereSize    = 0.05f;
}

const char * TemplateSimulator::getTestCasesStr(){
	return "Teapot,Random Objects,Triangle";
}

void TemplateSimulator::reset(){
		m_mouse.x = m_mouse.y = 0;
		m_trackmouse.x = m_trackmouse.y = 0;
		m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
}

void TemplateSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	switch (m_iTestCase)
	{
	case 0:break;
	case 1:
		TwAddVarRW(DUC->g_pTweakBar, "Num Spheres", TW_TYPE_INT32, &m_iNumSpheres, "min=1");
		TwAddVarRW(DUC->g_pTweakBar, "Sphere Size", TW_TYPE_FLOAT, &m_fSphereSize, "min=0.01 step=0.01");
		break;
	case 2:break;
	default:break;
	}
}

void TemplateSimulator::notifyCaseChanged(int testCase)
{
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case 0:
		cout << "Teapot !\n";
		m_vfMovableObjectPos = Vec3(0, 0, 0);
		m_vfRotate = Vec3(0, 0, 0);
		break;
	case 1:
		cout << "Random Object!\n";
		m_iNumSpheres = 100;
		m_fSphereSize = 0.05f;
		break;
	case 2:
		cout << "Triangle !\n";
		break;
	default:
		cout << "Empty Test!\n";
		break;
	}
}

void TemplateSimulator::externalForcesCalculations(float timeElapsed)
{
	// Apply the mouse deltas to g_vfMovableObjectPos (move along cameras view plane)
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
		float inputScale = 0.001f;
		inputWorld = inputWorld * inputScale;
		m_vfMovableObjectPos = m_vfMovableObjectFinalPos + inputWorld;
	}
	else {
		m_vfMovableObjectFinalPos = m_vfMovableObjectPos;
	}
}

void TemplateSimulator::simulateTimestep(float timeStep)
{
	// update current setup for each frame
	switch (m_iTestCase)
	{// handling different cases
	case 0:
		// rotate the teapot
		m_vfRotate.x += timeStep;
		if (m_vfRotate.x > 2 * M_PI) m_vfRotate.x -= 2.0f * (float)M_PI;
		m_vfRotate.y += timeStep;
		if (m_vfRotate.y > 2 * M_PI) m_vfRotate.y -= 2.0f * (float)M_PI;
		m_vfRotate.z += timeStep;
		if (m_vfRotate.z > 2 * M_PI) m_vfRotate.z -= 2.0f * (float)M_PI;

		break;
	default:
		break;
	}
}

void TemplateSimulator::drawSomeRandomObjects()
{
    std::mt19937 eng;
    std::uniform_real_distribution<float> randCol( 0.0f, 1.0f);
    std::uniform_real_distribution<float> randPos(-0.5f, 0.5f);
    for (int i=0; i<m_iNumSpheres; i++)
    {
		DUC->setUpLighting(Vec3(),0.4*Vec3(1,1,1),100,0.6*Vec3(randCol(eng),randCol(eng), randCol(eng)));
		DUC->drawSphere(Vec3(randPos(eng),randPos(eng),randPos(eng)),Vec3(m_fSphereSize, m_fSphereSize, m_fSphereSize));
    }
}

void TemplateSimulator::drawMovableTeapot()
{
	DUC->setUpLighting(Vec3(),0.4*Vec3(1,1,1),100,0.6*Vec3(0.97,0.86,1));
	DUC->drawTeapot(m_vfMovableObjectPos,m_vfRotate,Vec3(0.5,0.5,0.5));
}

void TemplateSimulator::drawTriangle()
{
	DUC->DrawTriangleUsingShaders();
}

void TemplateSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	switch(m_iTestCase)
	{
	case 0: drawMovableTeapot();break;
	case 1: drawSomeRandomObjects();break;
	case 2: drawTriangle();break;
	}
}

void TemplateSimulator::onClick(int x, int y)
{
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}

void TemplateSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
	m_trackmouse.x = x;
	m_trackmouse.y = y;
}
