
#include "Navigation3DManager.h"

CNavigation3DManager::CNavigation3DManager()
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

float CNavigation3DManager::GetY(const Vector3& Pos)
{
	if (!m_NavData)
		return Pos.y;

	return m_NavData->GetHeight(Pos);
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