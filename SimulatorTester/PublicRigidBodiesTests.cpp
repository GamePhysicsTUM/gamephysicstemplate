#include "CppUnitTest.h"
#include "RigidBodySystemSimulator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SimulatorTester
{		
	TEST_CLASS(PublicRigidBodiesTests)
	{
	public: 
		void setupBaseTest(RigidBodySystemSimulator * rbss) {
			rbss->m_iTestCase = TESTCASEUSEDTORUNTEST;
			rbss->addRigidBody(Vec3(-0.1f, -0.2f, 0.1f), Vec3(0.4f, 0.2f, 0.2f), 100.0f);
		
			rbss->addRigidBody(Vec3(0.0f, 0.2f, 0.0f), Vec3(0.4f, 0.2f, 0.2f), 100.0);
			rbss->setOrientationOf(1, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI)*0.25f));
			rbss->setVelocityOf(1,Vec3(0.0f, -0.1f, 0.05f));
		}

		TEST_METHOD(TestRigidBodiesInitialization)
		{
			RigidBodySystemSimulator * rbss = new RigidBodySystemSimulator();
			setupBaseTest(rbss);
			Assert::AreEqual(2,(int)rbss->getNumberOfRigidBodies(),0.0001f,L"Number of Rigid bodies is not right",LINE_INFO());
			Vec3 pos = rbss->getPositionOfRigidBody(0);
			Assert::AreEqual(-0.1f,(float)pos.x,0.0001f,L"X coordinate of body 0 is not right",LINE_INFO());
			Assert::AreEqual(-0.2f,(float)pos.y,0.0001f,L"Y coordinate of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.1f,(float)pos.z,0.0001f,L"Z coordinate of body 0 is not right",LINE_INFO());
			Vec3 vel = rbss->getLinearVelocityOfRigidBody(0);
			Assert::AreEqual(0.0f,(float)vel.x,0.0001f,L"X componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.0f,(float)vel.y,0.0001f,L"Y componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.0f,(float)vel.z,0.0001f,L"Z componnent of body 0 is not right",LINE_INFO());
			Vec3 angvel = rbss->getAngularVelocityOfRigidBody(0);
			Assert::AreEqual(0.0f,(float)angvel.x,0.0001f,L"X componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.0f,(float)angvel.y,0.0001f,L"Y componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.0f,(float)angvel.z,0.0001f,L"Z componnent of body 0 is not right",LINE_INFO());
			pos = rbss->getPositionOfRigidBody(1);
			Assert::AreEqual(0.0f,(float)pos.x,0.0001f,L"X coordinate of body 1 is not right",LINE_INFO());
			Assert::AreEqual(0.2f,(float)pos.y,0.0001f,L"Y coordinate of body 1 is not right",LINE_INFO());
			Assert::AreEqual(0.0f,(float)pos.z,0.0001f,L"Z coordinate of body 1 is not right",LINE_INFO());
			vel = rbss->getLinearVelocityOfRigidBody(1);
			Assert::AreEqual(0.0f,(float)vel.x,0.0001f,L"X componnent of body 1 is not right",LINE_INFO());
			Assert::AreEqual(-0.1f,(float)vel.y,0.0001f,L"Y componnent of body 1 is not right",LINE_INFO());
			Assert::AreEqual(0.05f,(float)vel.z,0.0001f,L"Z componnent of body 1 is not right",LINE_INFO());
			angvel = rbss->getAngularVelocityOfRigidBody(1);
			Assert::AreEqual(0.0f,(float)angvel.x,0.0001f,L"X componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.0f,(float)angvel.y,0.0001f,L"Y componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.0f,(float)angvel.z,0.0001f,L"Z componnent of body 0 is not right",LINE_INFO());
			delete rbss;
		}

		TEST_METHOD(TestRigidBodiesAfterForceApplication)
		{
			RigidBodySystemSimulator * rbss = new RigidBodySystemSimulator();
			setupBaseTest(rbss);
			rbss->applyForceOnBody(0,Vec3(0.0,0.0f,0.0),Vec3(0,0,200));
			for(int i =0; i < 4;i++)
				rbss->simulateTimestep(0.1);
			Vec3 pos = rbss->getPositionOfRigidBody(0);
			Assert::AreEqual(-0.1f,(float)pos.x,0.0001f,L"X coordinate of body 0 is not right",LINE_INFO());
			Assert::AreEqual(-0.2f,(float)pos.y,0.0001f,L"Y coordinate of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.16f,(float)pos.z,0.0001f,L"Z coordinate of body 0 is not right",LINE_INFO());
			Vec3 vel = rbss->getLinearVelocityOfRigidBody(0);
			Assert::AreEqual(0.0f,(float)vel.x,0.0001f,L"X componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.0f,(float)vel.y,0.0001f,L"Y componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(0.2f,(float)vel.z,0.0001f,L"Z componnent of body 0 is not right",LINE_INFO());
			Vec3 angvel = rbss->getAngularVelocityOfRigidBody(0);
			Assert::AreEqual(5.9064f,(float)angvel.x,0.0001f,L"X componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(-1.7891f,(float)angvel.y,0.0001f,L"Y componnent of body 0 is not right",LINE_INFO());
			Assert::AreEqual(-1.0204f,(float)angvel.z,0.0001f,L"Z componnent of body 0 is not right",LINE_INFO());
			delete rbss;
		}

		TEST_METHOD(TestRigidBodiesOneStepGivenTableTest)
		{
			RigidBodySystemSimulator * rbss = new RigidBodySystemSimulator();

			rbss->m_iTestCase = TESTCASEUSEDTORUNTEST;
			rbss->addRigidBody(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.6f, 0.5f), 2.0f);
			rbss->setOrientationOf(0, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI)* 0.5f));
			rbss->applyForceOnBody(0, Vec3(0.3f, 0.5f, 0.25f), Vec3(1.0f, 1.0f, 0.0f));
			rbss->simulateTimestep(2.0);

			Vec3 pos = rbss->getPositionOfRigidBody(0);
			Assert::AreEqual(0.0000f, (float)pos.x, 0.0001f, L"X coordinate of position of body 0 is not right", LINE_INFO());
			Assert::AreEqual(0.0000f, (float)pos.y, 0.0001f, L"Y coordinate of position of body 0 is not right", LINE_INFO());
			Assert::AreEqual(0.0000f, (float)pos.z, 0.0001f, L"Z coordinate of position of body 0 is not right", LINE_INFO());
			Vec3 vel = rbss->getLinearVelocityOfRigidBody(0);
			Assert::AreEqual(1.0000f, (float)vel.x, 0.0001f, L"X componnent of velocity of body 0 is not right", LINE_INFO());
			Assert::AreEqual(1.0000f, (float)vel.y, 0.0001f, L"Y componnent of velocity of body 0 is not right", LINE_INFO());
			Assert::AreEqual(0.0000f, (float)vel.z, 0.0001f, L"Z componnent of velocity of body 0 is not right", LINE_INFO());
			Vec3 angvel = rbss->getAngularVelocityOfRigidBody(0);
			Assert::AreEqual(-2.4000f, (float)angvel.x, 0.0001f, L"X componnent of angular velocity of body 0 is not right", LINE_INFO());
			Assert::AreEqual(4.9180f, (float)angvel.y, 0.0001f, L"Y componnent of angular velocity of body 0 is not right", LINE_INFO());
			Assert::AreEqual(-1.7647f, (float)angvel.z, 0.0001f, L"Z componnent of angular velocity of body 0 is not right", LINE_INFO());

			Vec3 xa_world = Vec3(-0.3f, -0.5f, -0.25f) - pos;
			Vec3 velocityA = vel + cross(angvel, xa_world);
			Assert::AreEqual(-1.11186f, (float)velocityA.x, 0.0001f, L"X componnent of the velocity at the given point is not right", LINE_INFO());
			Assert::AreEqual(0.929412f, (float)velocityA.y, 0.0001f, L"Y componnent of the velocity at the given point is not right", LINE_INFO());
			Assert::AreEqual(2.67541f, (float)velocityA.z, 0.0001f, L"Z componnent of the velocity at the given point is not right", LINE_INFO());
			delete rbss;
		}
	};
}