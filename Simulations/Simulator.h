#ifndef SIMULATOR_h
#define SIMULATOR_h
#include <sstream>
#include <iomanip>
#include <random>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "util/vectorbase.h"
#include "DrawingUtilitiesClass.h"

struct Point2D {
	int x,y;
};

using namespace std;
using namespace GamePhysics;

class Simulator{
public:
	// Attributes
	int m_iTestCase;
	DrawingUtilitiesClass * DUC;
	// Functions
	
	virtual const char * getTestCasesStr() = 0;

	virtual void reset() = 0;
	
	/*
	This Function is used to initialize the GUI with customized componnents per simulations 
	input: DUC pointer for the DrawingUtilities to setup the UI componnents
	**for more info on how to use this function take a look at the template simulator 
	*/
	virtual void initUI(DrawingUtilitiesClass * DUC)=0;
	/*
	This Function is used to render one frame from the simulation called by the simulator enviroment
	input: pd3dImmediateContext pointer used only in case of drawing 
	an object from scratch but usually we have all the needed 
	drawing functions in the DrawingUtilityClass
	**for more info on what functions avaialble to use take a look at the DrawingUtilities Class
	*/
	virtual void drawFrame(ID3D11DeviceContext* pd3dImmediateContext) = 0;
	/*
	This Function is used to calculate  called by the simulator enviroment
	input: fElapsedTime is the elapsed time since the last timestep
	*/
	virtual void externalForcesCalculations(float timeElapsed) = 0;
	/*
	This Function is used to simulate one step of the simulation called by the simulator enviroment
	input: timeStep is the time step of the stimulation
	*/
	virtual void simulateTimestep(float timeStep)= 0;
	/*
	This Function is used to notify the simulator that the scene test case is changed 
	so that the needed changes can be handed here
	**for more info on how to use this function take a look at the template simulator 
	*/
	virtual void notifyCaseChanged(int testCase) = 0;

	/*
	this function is used to receive the input from the mouse
	input x,y: the click position in pixels
	*/
	virtual void onClick(int x, int y) = 0;

	/*
	this function is used to receive the input from the mouse
	input x,y: the mouse position in pixels
	*/
	virtual void onMouse(int x, int y) = 0;

};

#endif