#include "Frustum.h"

CFrustum::CFrustum()
{
	// 절두체를 구성하는 8개의 점을 미리 세팅해둔다.
	m_Pos[0] = Vector3(-1.f, 1.f, 0.f); // 왼쪽 상단
	m_Pos[1] = Vector3(-1.f, 1.f, 0.f); // 오른쪽 상단
	m_Pos[2] = Vector3(-1.f, -1.f, 0.f); // 왼쪽 하단
	m_Pos[3] = Vector3(1.f, -1.f, 0.f); // 오른쪽 하단

	// 후방 4개의 점
	m_Pos[4] = Vector3(-1.f, 1.f, 1.f); // 왼쪽 상단
	m_Pos[5] = Vector3(-1.f, 1.f, 1.f); // 오른쪽 상단
	m_Pos[6] = Vector3(-1.f, -1.f, 1.f); // 왼쪽 하단
	m_Pos[7] = Vector3(1.f, -1.f, 1.f); // 오른쪽 하단
}

CFrustum::~CFrustum()
{
}

void CFrustum::Update(Matrix matVP)
{
	// 우리는 결과적으로 World 공간에서의 평면이 필요하다

	// 따라서 월드 공간에서의 8개 점 위치를 구할 것이다.
	// matVP 는 View  행렬 * 투영 행렬
	// 이것의 역행렬을 곱해서, 투영 공간의 점을, 월드 공간으로 이동시켜주는 효과를 낼 수 있다

	Vector3 Pos[8];

	matVP.Inverse();

	for (int i = 0; i < 8; ++i)
	{
		// 각 점을 월드 공간 상으로 이동 시켜준다.
		Pos[i] = m_Pos[i].TransformCoord(matVP);
	}

	// 각각의 면을 구성해준다.
	// 밖에서 평면 안쪽으로 바라보는 것이, 우리의 시선이다.
	// 그러면, 반대로, 밖에서 볼때, 각 평면의 점들이 시계 방향으로 주어져서
	// 해당 면의 법선벡터가 우리 쪽으로 바라볼 수 있도록 세팅해줘야 한다.
	// 아래의 6개 평면들은, 월드 공간에서의 6개 평면 정보가 될 것이다.
	
	
	// Left
	m_Plane[(int)Frustum_Plane_Dir::Left] = XMPlaneFromPoints(Pos[4].Convert(), Pos[0].Convert(),
		Pos[2].Convert());

	// Right
	m_Plane[(int)Frustum_Plane_Dir::Right] = XMPlaneFromPoints(Pos[1].Convert(), Pos[5].Convert(),
		Pos[7].Convert());

	// Top --> 위 방향을 바라보게 끔
	m_Plane[(int)Frustum_Plane_Dir::Top] = XMPlaneFromPoints(Pos[4].Convert(), Pos[5].Convert(),
		Pos[1].Convert());

	// Bottom --> 아래 방향으로 바라보게끔
	m_Plane[(int)Frustum_Plane_Dir::Bottom] = XMPlaneFromPoints(Pos[2].Convert(), Pos[3].Convert(),
		Pos[7].Convert());

	// Near --> 카메라 시작점 쪽을 바라보게끔
	m_Plane[(int)Frustum_Plane_Dir::Near] = XMPlaneFromPoints(Pos[0].Convert(), Pos[1].Convert(),
		Pos[3].Convert());

	// Far --> Near 의 법선 벡터 방향과 반대
	m_Plane[(int)Frustum_Plane_Dir::Far] = XMPlaneFromPoints(Pos[5].Convert(), Pos[4].Convert(),
		Pos[6].Convert());

}

bool CFrustum::FrustumInPoint(const Vector3& Point)
{
	for (int i = 0; i < (int)Frustum_Plane_Dir::Max; ++i)
	{
		// GetX 만 해주는 이유는, 같은 값들이 Vector 의 x,y,z 원소에 들어가므로, 그 중에서 하나의 값만 받아오는 것이다.
		// 아래의 식은, 해당 Plane의 Normal 과 해당 Point 벡터와 내적한 값에 대한 식.
		// ax + by + cz + d (d 는 원점에서 해당 평면까지의 거리 / a,b,c 는 해당 Plane 의 Normal Vector 의 x,y,z 값 / x,y,z 는 현재 Point 의 값)
		// 만약 ax + by + cz + d 가  0 보다 크면, 해당 Point 가 평면 "위"에 존재한다는 것 --> 절두체 밖에 존재한다는 것 -->  안보이는 것 --> Culling이 된다는 것
		// == 0 이라면, 해당 평면 상에 존재 --> 보이는 것
		// < 0 이라면, 평면 아래에 존재 --> 절두체 안쪽에 존재 --> 보이는 것
		float Dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), Point.Convert()));

		if (Dot > 0.f)
			return true;
	}

	return false;
}

bool CFrustum::FrustumInSphere(const SphereInfo& Sphere)
{
	for (int i = 0; i < (int)Frustum_Plane_Dir::Max; ++i)
	{
		// 위와 같은 논리로, 구의 중심점 벡터와 계산을 수행한다.
		float Dot = XMVectorGetX(XMPlaneDotCoord(m_Plane[i].Convert(), Sphere.Center.Convert()));

		// 구의 경우, 해당 중심점에서 반지름을 더해줘서, 원래 평면 아래에 존재하는 것으로 Dot 이라는 결과가 나오지만
		// 해당 중심점에서, 평면 방향으로 반지름을 더해서 파고들면
		// 평면 위에 존재하는 것으로 판별될 수도 있다.
		// 즉, (ax + by + cz + d) + (반지름) = c 의 식을 생각해야 한다.
		if (Dot > Sphere.Radius)
			return true;
	}
	return false;
}
