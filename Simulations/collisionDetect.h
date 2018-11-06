// header file:
#include <DirectXMath.h>
#include <Vector>
using namespace DirectX;

// the return structure, with these values, you should be able to calculate the impulse
// the depth shouldn't be used in your impulse calculation, it is a redundant value
// if the normalWorld == XMVectorZero(), no collision
struct CollisionInfo{
	bool isValid;                          // whether there is a collision point, true for yes
	GamePhysics::Vec3 collisionPointWorld; // the position of the collision point in world space
	GamePhysics::Vec3 normalWorld;         // the direction of the impulse to A, negative of the collision face of A
	float depth;                           // the distance of the collision point to the surface, not necessary.
};

// tool data structures/functions called by the collision detection method, you can ignore the details here
namespace collisionTools{
	struct Projection{
		float min, max;
	};


	inline std::vector<XMVECTOR> discritizeObject(const XMMATRIX& obj2World)
	{
		const XMVECTOR centerWorld = XMVector3Transform(XMVectorZero(), obj2World);
		XMVECTOR edges[3];
		std::vector<XMVECTOR> results;
		for (int precession = 0.1; precession <= 0.5; precession += 0.1)
		{
			for (size_t i = 0; i < 3; ++i)
				edges[i] = XMVector3TransformNormal(XMVectorSetByIndex(XMVectorZero(), precession, i), obj2World);
			results.push_back(centerWorld - edges[0] - edges[1] - edges[2]);
			results.push_back(centerWorld + edges[0] - edges[1] - edges[2]);
			results.push_back(centerWorld - edges[0] + edges[1] - edges[2]);
			results.push_back(centerWorld + edges[0] + edges[1] - edges[2]);
			results.push_back(centerWorld - edges[0] - edges[1] + edges[2]);
			results.push_back(centerWorld + edges[0] - edges[1] + edges[2]);
			results.push_back(centerWorld - edges[0] + edges[1] + edges[2]);
			results.push_back(centerWorld + edges[0] + edges[1] + edges[2]);
		}
	}

	inline XMVECTOR getVectorConnnectingCenters(const XMMATRIX& obj2World_A, const XMMATRIX& obj2World_B)
	{
		const XMVECTOR centerWorld_A = XMVector3Transform(XMVectorZero(), obj2World_A);
		const XMVECTOR centerWorld_B = XMVector3Transform(XMVectorZero(), obj2World_B);
		return centerWorld_B - centerWorld_A;

	}

	// Get Corners
	inline std::vector<XMVECTOR> getCorners(const XMMATRIX& obj2World)
	{
		const XMVECTOR centerWorld = XMVector3Transform(XMVectorZero(), obj2World);
		XMVECTOR edges[3];
		for (size_t i = 0; i < 3; ++i)
			edges[i] = XMVector3TransformNormal(XMVectorSetByIndex(XMVectorZero(), 0.5f, i), obj2World);
		std::vector<XMVECTOR> results;
		results.push_back(centerWorld - edges[0] - edges[1] - edges[2]);
		results.push_back(centerWorld + edges[0] - edges[1] - edges[2]);
		results.push_back(centerWorld - edges[0] + edges[1] - edges[2]);
		results.push_back(centerWorld + edges[0] + edges[1] - edges[2]); // this +,+,-
		results.push_back(centerWorld - edges[0] - edges[1] + edges[2]);
		results.push_back(centerWorld + edges[0] - edges[1] + edges[2]); //this +,-,+
		results.push_back(centerWorld - edges[0] + edges[1] + edges[2]); //this -,+,+
		results.push_back(centerWorld + edges[0] + edges[1] + edges[2]);//this +,+,+
		return results;
	}

	// Get Rigid Box Size
	inline XMVECTOR getBoxSize(const XMMATRIX& obj2World)
	{
		XMVECTOR size = XMVectorZero();
		XMVECTOR edges[3];
		for (size_t i = 0; i < 3; ++i){
			edges[i] = XMVector3TransformNormal(XMVectorSetByIndex(XMVectorZero(), 0.5f, i), obj2World);
			XMVECTOR length = XMVector3Length(edges[i]);

			size = XMVectorSetByIndex(size, 2.0f*XMVectorGetByIndex(length, 0), i);
		}
		return size;
	}

	// Get important Edges
	inline std::vector<XMVECTOR> getImportantEdges(const XMMATRIX& obj2World)
	{
		XMVECTOR xaxis = XMVectorSet(1, 0, 0, 1);
		XMVECTOR yaxis = XMVectorSet(0, 1, 0, 1);
		XMVECTOR zaxis = XMVectorSet(0, 0, 1, 1);
		XMVECTOR edge1 = XMVector3TransformNormal(xaxis, obj2World);
		XMVECTOR edge2 = XMVector3TransformNormal(yaxis, obj2World);
		XMVECTOR edge3 = XMVector3TransformNormal(zaxis, obj2World);
		std::vector<XMVECTOR> results;
		results.push_back(edge1);
		results.push_back(edge2);
		results.push_back(edge3);
		return results;
	}

	// Get the Normal to the faces
	inline std::vector<XMVECTOR> getAxisNormalToFaces(const XMMATRIX& obj2World)
	{
		std::vector<XMVECTOR> edges;
		XMVECTOR xaxis = XMVectorSet(1, 0, 0, 1);
		XMVECTOR yaxis = XMVectorSet(0, 1, 0, 1);
		XMVECTOR zaxis = XMVectorSet(0, 0, 1, 1);
		XMVECTOR edge1 = XMVector3Normalize(XMVector3TransformNormal(xaxis, obj2World));
		XMVECTOR edge2 = XMVector3Normalize(XMVector3TransformNormal(yaxis, obj2World));
		XMVECTOR edge3 = XMVector3Normalize(XMVector3TransformNormal(zaxis, obj2World));
		std::vector<XMVECTOR> results;
		edges.push_back(edge1);
		edges.push_back(edge2);
		edges.push_back(edge3);
		return edges;
	}


	// Get the pair of edges
	inline std::vector<XMVECTOR> getPairOfEdges(const XMMATRIX& obj2World_A, const XMMATRIX& obj2World_B)
	{
		std::vector<XMVECTOR> edges1 = getAxisNormalToFaces(obj2World_A);
		std::vector<XMVECTOR> edges2 = getAxisNormalToFaces(obj2World_B);

		std::vector<XMVECTOR> results;
		for (int i = 0; i < edges1.size(); i++)
		{
			for (int j = 0; j<edges2.size(); j++)
			{
				XMVECTOR vector = XMVector3Cross(edges1[i], edges2[j]);
				if (XMVectorGetX(XMVector3Length(vector)) > 0)
					results.push_back(XMVector3Normalize(vector));
			}
		}
		return results;
	}

	// project a shape on an axis
	inline Projection project(const XMMATRIX& obj2World, XMVECTOR axis)
	{
		// Get corners
		std::vector<XMVECTOR> cornersWorld = getCorners(obj2World);
		float min = XMVectorGetX(XMVector3Dot(cornersWorld[0], axis));
		float max = min;
		for (int i = 1; i < cornersWorld.size(); i++)
		{
			float p = XMVectorGetX(XMVector3Dot(cornersWorld[i], axis));
			if (p < min) {
				min = p;
			}
			else if (p > max) {
				max = p;
			}
		}
		Projection projection;
		projection.max = max;
		projection.min = min;
		return projection;
	}

	inline bool overlap(Projection p1, Projection p2)
	{
		return !((p1.max > p2.max && p1.min > p2.max) || (p2.max > p1.max && p2.min > p1.max));
	}

	inline float getOverlap(Projection p1, Projection p2)
	{
		return XMMin(p1.max, p2.max) - XMMax(p1.min, p2.min);
	}

	static inline XMVECTOR contactPoint(
		const XMVECTOR &pOne,
		const XMVECTOR &dOne,
		float oneSize,
		const XMVECTOR &pTwo,
		const XMVECTOR &dTwo,
		float twoSize,

		// If this is true, and the contact point is outside
		// the edge (in the case of an edge-face contact) then
		// we use one's midpoint, otherwise we use two's.
		bool useOne)
	{
		XMVECTOR toSt, cOne, cTwo;
		float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
		float denom, mua, mub;

		smOne = XMVectorGetX(XMVector3LengthSq(dOne));
		smTwo = XMVectorGetX(XMVector3LengthSq(dTwo));
		dpOneTwo = XMVectorGetX(XMVector3Dot(dTwo, dOne));

		toSt = pOne - pTwo;
		dpStaOne = XMVectorGetX(XMVector3Dot(dOne, toSt));
		dpStaTwo = XMVectorGetX(XMVector3Dot(dTwo, toSt));

		denom = smOne * smTwo - dpOneTwo * dpOneTwo;

		// Zero denominator indicates parrallel lines
		if (abs(denom) < 0.0001f) {
			return useOne ? pOne : pTwo;
		}

		mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
		mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

		// If either of the edges has the nearest point out
		// of bounds, then the edges aren't crossed, we have
		// an edge-face contact. Our point is on the edge, which
		// we know from the useOne parameter.
		if (mua > oneSize ||
			mua < -oneSize ||
			mub > twoSize ||
			mub < -twoSize)
		{
			return useOne ? pOne : pTwo;
		}
		else
		{
			cOne = pOne + dOne * mua;
			cTwo = pTwo + dTwo * mub;

			return cOne * 0.5 + cTwo * 0.5;
		}
	}

	inline XMVECTOR handleVertexToface(const XMMATRIX& obj2World, const XMVECTOR& toCenter)
	{
		std::vector<XMVECTOR> corners = getCorners(obj2World);
		float min = 1000;
		XMVECTOR vertex;
		for (int i = 0; i < corners.size(); i++)
		{
			float value = XMVectorGetX(XMVector3Dot(corners[i], toCenter));
			if (value < min)
			{
				vertex = corners[i];
				min = value;
			}
		}

		return vertex;
	}


	inline CollisionInfo checkCollisionSATHelper(const XMMATRIX& obj2World_A, const XMMATRIX& obj2World_B, XMVECTOR size_A, XMVECTOR size_B)
	{
		CollisionInfo info;
		info.isValid = false;
		XMVECTOR collisionPoint = XMVectorZero();
		float smallOverlap = 10000.0f;
		XMVECTOR axis;
		int index;
		int fromWhere = -1;
		bool bestSingleAxis = false;
		XMVECTOR toCenter = getVectorConnnectingCenters(obj2World_A, obj2World_B);
		std::vector<XMVECTOR> axes1 = getAxisNormalToFaces(obj2World_A);
		std::vector<XMVECTOR> axes2 = getAxisNormalToFaces(obj2World_B);
		std::vector<XMVECTOR> axes3 = getPairOfEdges(obj2World_A, obj2World_B);
		// loop over the axes1
		for (int i = 0; i < axes1.size(); i++) {
			// project both shapes onto the axis
			Projection p1 = project(obj2World_A, axes1[i]);
			Projection p2 = project(obj2World_B, axes1[i]);
			// do the projections overlap?
			if (!overlap(p1, p2)) {
				// then we can guarantee that the shapes do not overlap
				return info;
			}
			else{
				// get the overlap
				float o = getOverlap(p1, p2);
				// check for minimum
				if (o < smallOverlap) {
					// then set this one as the smallest
					smallOverlap = o;
					axis = axes1[i];
					index = i;
					fromWhere = 0;
				}
			}
		}
		// loop over the axes2
		for (int i = 0; i < axes2.size(); i++) {
			// project both shapes onto the axis
			Projection p1 = project(obj2World_A, axes2[i]);
			Projection p2 = project(obj2World_B, axes2[i]);
			// do the projections overlap?
			if (!overlap(p1, p2)) {
				// then we can guarantee that the shapes do not overlap
				return  info;
			}
			else{
				// get the overlap
				float o = getOverlap(p1, p2);
				// check for minimum
				if (o < smallOverlap) {
					// then set this one as the smallest
					smallOverlap = o;
					axis = axes2[i];
					index = i;
					fromWhere = 1;
					bestSingleAxis = true;
				}
			}
		}
		int whichEdges = 0;
		// loop over the axes3
		for (int i = 0; i < axes3.size(); i++) {
			// project both shapes onto the axis
			Projection p1 = project(obj2World_A, axes3[i]);
			Projection p2 = project(obj2World_B, axes3[i]);
			// do the projections overlap?
			if (!overlap(p1, p2)) {
				// then we can guarantee that the shapes do not overlap
				return info;
			}
			else{
				// get the overlap
				float o = getOverlap(p1, p2);
				// check for minimum
				if (o < smallOverlap) {
					// then set this one as the smallest
					smallOverlap = o;
					axis = axes3[i];
					index = i;
					whichEdges = i;
					fromWhere = 2;
				}
			}
		}
		// if we get here then we know that every axis had overlap on it
		// so we can guarantee an intersection
		XMVECTOR normal;
		switch (fromWhere){
		case 0:{
				   normal = axis;
				   if (XMVectorGetX(XMVector3Dot(axis, toCenter)) <= 0)
				   {
					   normal = normal * -1.0f;
				   }
				   collisionPoint = handleVertexToface(obj2World_B, toCenter);
		}break;
		case 1:{
				   normal = axis;
				   if (XMVectorGetX(XMVector3Dot(axis, toCenter)) <= 0)
				   {
					   normal = normal * -1.0f;
				   }
				   collisionPoint = handleVertexToface(obj2World_A, toCenter*-1);
		}break;
		case 2:{
				   XMVECTOR axis = XMVector3Normalize(XMVector3Cross(axes1[whichEdges / 3], axes2[whichEdges % 3]));
				   normal = axis;
				   if (XMVectorGetX(XMVector3Dot(axis, toCenter)) <= 0)
				   {
					   normal = normal * -1.0f;
				   }
				   XMVECTOR ptOnOneEdge = XMVectorSet(0.5, 0.5, 0.5, 1);
				   XMVECTOR ptOnTwoEdge = XMVectorSet(0.5, 0.5, 0.5, 1);

				   for (int i = 0; i < 3; i++)
				   {
					   if (i == whichEdges / 3) ptOnOneEdge = XMVectorSetByIndex(ptOnOneEdge, 0, i);
					   else if (XMVectorGetX(XMVector3Dot(axes1[i], normal)) < 0) ptOnOneEdge = XMVectorSetByIndex(ptOnOneEdge, -XMVectorGetByIndex(ptOnOneEdge, i), i);

					   if (i == whichEdges % 3) ptOnTwoEdge = XMVectorSetByIndex(ptOnTwoEdge, 0, i);
					   else if (XMVectorGetX(XMVector3Dot(axes2[i], normal)) > 0) ptOnTwoEdge = XMVectorSetByIndex(ptOnTwoEdge, -XMVectorGetByIndex(ptOnTwoEdge, i), i);
				   }
				   ptOnOneEdge = XMVector3Transform(ptOnOneEdge, obj2World_A);
				   ptOnTwoEdge = XMVector3Transform(ptOnTwoEdge, obj2World_B);
				   collisionPoint = contactPoint(ptOnOneEdge,
					   axes1[whichEdges / 3],
					   (float)XMVectorGetByIndex(size_A, (whichEdges / 3)),
					   ptOnTwoEdge,
					   axes2[whichEdges % 3],
					   XMVectorGetByIndex(size_B, (whichEdges % 3)),
					   bestSingleAxis);
		}break;
		}


		info.isValid = true;
		info.collisionPointWorld = collisionPoint;
		info.depth = smallOverlap;
		info.normalWorld = normal*-1;
		return info;
	}
}

/* params:
obj2World_A, the transfer matrix from object space of A to the world space
obj2World_B, the transfer matrix from object space of B to the world space
*/
inline CollisionInfo checkCollisionSAT(GamePhysics::Mat4& obj2World_A, GamePhysics::Mat4& obj2World_B) {
	using namespace collisionTools;
	XMMATRIX MatA = obj2World_A.toDirectXMatrix(), MatB = obj2World_B.toDirectXMatrix();
	XMVECTOR calSizeA = getBoxSize(MatA);
	XMVECTOR calSizeB = getBoxSize(MatB);
	
	return checkCollisionSATHelper(MatA, MatB, calSizeA, calSizeB);
}

// example of using the checkCollisionSAT function
inline void testCheckCollision(int caseid){

	if (caseid == 1){// simple examples, suppose that boxes A and B are cubes and have no rotation
		GamePhysics::Mat4 AM; AM.initTranslation(1.0, 1.0, 1.0);// box A at (1.0,1.0,1.0)
		GamePhysics::Mat4 BM; BM.initTranslation(2.0, 2.0, 2.0);  //box B at (2.0,2.0,2.0)

		// check for collision
		CollisionInfo simpletest = checkCollisionSAT(AM, BM);// should find out a collision here
		if (!simpletest.isValid)
			std::printf("No Collision\n");
		else {
			std::printf("collision detected at normal: %f, %f, %f\n", simpletest.normalWorld.x, simpletest.normalWorld.y, simpletest.normalWorld.z);
			std::printf("collision point : %f, %f, %f\n", (simpletest.collisionPointWorld).x, (simpletest.collisionPointWorld).y, simpletest.collisionPointWorld.z);
		}
		// case 1 result:
		// collision detected at normal: -1.000000, -0.000000, -0.000000
		// collision point : 1.500000, 1.500000, 1.500000
		// Box A should be pushed to the left
	}
	else if (caseid == 2){// case 2, collide at a corner of Box B:
		GamePhysics::Mat4 AM, BM;
		AM.initTranslation(0.2f, 5.0f, 1.0f); // box A moves(0.2f, 5.0f, 1.0f) from origin
		BM.initRotationZ(45); // box B rotates 45 degree around axis z
		// box A size(9,2,3), box B size(5.656854f, 5.656854f, 2.0f)
		GamePhysics::Mat4 SizeMat;
		SizeMat.initScaling(9.0f, 2.0f, 3.0f);
		AM = SizeMat * AM;
		SizeMat.initScaling(5.656854f, 5.656854f, 2.0f);
		BM = SizeMat * BM;
		// check for collision
		CollisionInfo simpletest = checkCollisionSAT(AM, BM);// should find out a collision here

		if (!simpletest.isValid)
			std::printf("No Collision\n");
		else {
			std::printf("collision detected at normal: %f, %f, %f\n", simpletest.normalWorld.x, simpletest.normalWorld.y, simpletest.normalWorld.z);
			std::printf("collision point : %f, %f, %f\n", (simpletest.collisionPointWorld).x, (simpletest.collisionPointWorld).y, simpletest.collisionPointWorld.z);
		}
		// case 2 result:
		// collision detected at normal : 0.000000, 1.000000, 0.000000
		// collision point : 0.000000, 4.000000, 1.000000
	}
	else if (caseid == 3){// case 3, collide at a corner of Box A:
		// box A first rotates 45 degree around axis z
		// box A moves(-2.0f, 0.0f, 1.0f) from origin,(-2.0f,0.0f,1.0f) is the centre position of A in world space
		// box A size(2.829f, 2.829f, 2.0f)
		GamePhysics::Mat4 AM_rot; AM_rot.initRotationZ(45);
		GamePhysics::Mat4 AM_tra; AM_tra.initTranslation(-2.0f, 0.0f, 1.0f);
		GamePhysics::Mat4 AM_sca; AM_sca.initScaling(2.829f, 2.829f, 2.0f);
		// get the object 2 world matrix of A
		GamePhysics::Mat4 AM = AM_sca * AM_rot * AM_tra; // pay attention to the order! 
		// order, since we are working with the DirectX, we use left-handed matrixes!

		// box B first rotates 90 degree around axis z
		// box B then moves (1.0f,0.5f,0.0f) from origin, (1.0f,0.5f,0.0f) is also the centre position of B in world space
		// box B size(9.0f, 2.0f, 4.0f)
		GamePhysics::Mat4 BM_rot; BM_rot.initRotationZ(90);
		GamePhysics::Mat4 BM_tra; BM_tra.initTranslation(1.0f, 0.5f, 0.0f);
		GamePhysics::Mat4 BM_sca; BM_sca.initScaling(9.0f, 2.0f, 4.0f);
		GamePhysics::Mat4 BM = BM_sca * BM_rot * BM_tra; // pay attention to the order! 

		// check for collision
		CollisionInfo simpletest = checkCollisionSAT(AM, BM);// should find out a collision here

		if (!simpletest.isValid)
			std::printf("No Collision\n");
		else {
			std::printf("collision detected at normal: %f, %f, %f\n", simpletest.normalWorld.x, simpletest.normalWorld.y, simpletest.normalWorld.z);
			std::printf("collision point : %f, %f, %f\n", (simpletest.collisionPointWorld).x, (simpletest.collisionPointWorld).y, simpletest.collisionPointWorld.z);
		}
		// case 3 result:
		// collision detected at normal: -1.000000, 0.000000, -0.000000
		// collision point : 0.000405, 0.000000, 0.000000
	}
}
