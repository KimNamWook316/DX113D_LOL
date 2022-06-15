
#include "LoLBuildingManager.h"
#include "GameObject/GameObject.h"
#include "Component/BuildingComponent.h"

DEFINITION_SINGLE(CLoLBuildingManager)

CLoLBuildingManager::CLoLBuildingManager()	:
	m_FriendNexus(nullptr),
	m_EnemyNexus(nullptr)
{

}

CLoLBuildingManager::~CLoLBuildingManager()
{

}

void CLoLBuildingManager::Init()
{
}

void CLoLBuildingManager::SetFriendHomePos(const Vector3& Pos)
{
	m_FriendHomePos = Pos;
}

void CLoLBuildingManager::SetEnemyHomePos(const Vector3& Pos)
{
	m_EnemyHomePos = Pos;
}

const Vector3& CLoLBuildingManager::GetFriendHomePos() const
{
	return m_FriendHomePos;
}

const Vector3& CLoLBuildingManager::GetEnemyHomePos() const
{
	return m_EnemyHomePos;
}

void CLoLBuildingManager::SetEnemyNexus(CGameObject* Nexus)
{
	m_EnemyNexus = Nexus;
}

void CLoLBuildingManager::SetFriendNexus(CGameObject* Nexus)
{
	m_FriendNexus = Nexus;
}

CGameObject* CLoLBuildingManager::GetEnemyNexus() const
{
	return m_EnemyNexus;
}

CGameObject* CLoLBuildingManager::GetFriendNexus() const
{
	return m_FriendNexus;
}

CGameObject* CLoLBuildingManager::GetFrontmostBuilding(LoLLine Line, bool Enemy)
{
	CGameObject* Building = nullptr;
	if (Enemy)
	{
		Building = GetBuildingFromLine(Line, m_EnemyNexus);
		if (!Building)
		{
			// 왼쪽 쌍둥이 타워가 살아있을 경우
			if (m_EnemyNexus->GetChildObject(0))
			{
				return m_EnemyNexus->GetChildObject(0);
			}

			// 오른쪽 쌍둥이 타워가 살아있을 경우
			else if (m_EnemyNexus->GetChildObject(1))
			{
				return m_EnemyNexus->GetChildObject(1);
			}

			else
				return m_EnemyNexus;
		}
	}

	else
	{
		Building = GetBuildingFromLine(Line, m_FriendNexus);

		if (!Building)
		{
			// 왼쪽 쌍둥이 타워가 살아있을 경우
			if (m_FriendNexus->GetChildObject(0))
			{
				return m_FriendNexus->GetChildObject(0);
			}

			// 오른쪽 쌍둥이 타워가 살아있을 경우
			else if (m_FriendNexus->GetChildObject(1))
			{
				return m_FriendNexus->GetChildObject(1);
			}

			else
				return m_FriendNexus;
		}
	}
}

CGameObject* CLoLBuildingManager::GetBuildingFromLine(LoLLine Line, CGameObject* NexusObj) const
{

	size_t Count = NexusObj->GetChildObjectCount();
	CBuildingComponent* BuildingComp = nullptr;
	CGameObject* LineRootObj = nullptr;
	for (size_t i = 0; i < Count; ++i)
	{
		LineRootObj = NexusObj->GetChildObject(i);
		BuildingComp = LineRootObj->FindComponentFromType<CBuildingComponent>();

		// 이때 BuildingComp는 해당 라인에 최상위 건물의 BuildingComp
		if (BuildingComp->GetBuildingLine() == Line)
			break;
	}

	// 인자로 넘겨준 라인의 건물이 모두 파괴되고 없다면
	if (!LineRootObj)
		return nullptr;
	
	else
	{
		while (LineRootObj->GetChildObjectCount() > 0)
			LineRootObj = LineRootObj->GetChildObject(0);

		return LineRootObj;
	}

	return nullptr;
}


