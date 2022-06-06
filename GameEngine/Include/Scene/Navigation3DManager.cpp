
#include "Navigation3DManager.h"
#include "../Component/LandScape.h"
#include "../Component/CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Input.h"

CNavigation3DManager::CNavigation3DManager()	:
	m_LandScape(nullptr)
{
}

CNavigation3DManager::~CNavigation3DManager()
{
}

void CNavigation3DManager::SetNavData(CLandScape* NavData)
{
	m_NavData = NavData;
}

void CNavigation3DManager::AddNavResult(const NavResultData& NavData)
{
}

void CNavigation3DManager::SetLandScape(CLandScape* LandScape)
{
	m_LandScape = LandScape;
}

CLandScape* CNavigation3DManager::GetLandScape() const
{
	return m_LandScape;
}

float CNavigation3DManager::GetY(const Vector3& Pos)
{
	if (!m_NavData)
		return Pos.y;

	return m_NavData->GetHeight(Pos);
}

bool CNavigation3DManager::CheckPickingPoint(Vector3& OutPos)
{
	if (!m_LandScape)
		return false;

	CCameraComponent* Current = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Matrix ProjMat = Current->GetProjMatrix();
	Matrix ViewMat = Current->GetViewMatrix();

	Ray ray = CInput::GetInst()->GetRay(ViewMat);
	Vector3 RayDir = ray.Dir;
	Vector3 RayStartPos = ray.Pos;

	float Step = 0.1f;

	while (true)
	{
		Vector3 Point_I1 = RayStartPos;
		Vector3 Point_I2 = RayStartPos + RayDir * Step;

		float Height1 = m_LandScape->GetHeight(Point_I1);
		float Height2 = m_LandScape->GetHeight(Point_I2);

		Vector3 ResultPos;

		if (m_LandScape->CheckInArea(Point_I1, Point_I2, ResultPos))
		{
			OutPos = ResultPos;
			break;
		}

		// LandScape 밖에 클릭하면 피킹 지점 찾지 않도록 예외 처리
		if (Point_I1.x < m_LandScape->GetMin().x && ray.Dir.x <= 0.f)
			return false;

		if (Point_I1.z < m_LandScape->GetMin().z && ray.Dir.z <= 0.f)
			return false;

		if (Point_I1.x > m_LandScape->GetMax().x && ray.Dir.x >= 0.f)
			return false;

		if (Point_I1.z > m_LandScape->GetMax().z && ray.Dir.z >= 0.f)
			return false;

		else
		{
			RayStartPos = Point_I2;
		}
	}

	return true;
}

void CNavigation3DManager::Start()
{
}

bool CNavigation3DManager::Init()
{
	return true;
}

void CNavigation3DManager::Update(float DeltaTime)
{
}