#include "GameManager.h"

DEFINITION_SINGLE(CGameManager)

CGameManager::CGameManager() :
	m_FriendTopTurret2Order(nullptr),
	m_FriendTopTurret3Order(nullptr),
	m_FriendMidTurret1Order(nullptr),
	m_FriendMidTurret2Order(nullptr),
	m_FriendMidTurret3Order(nullptr),
	m_FriendBottomTurret1Order(nullptr),
	m_FriendBottomTurret2Order(nullptr),
	m_FriendBottomTurret3Order(nullptr),
	m_EnemyTurretTop1Order(nullptr),
	m_EnemyTurretTop2Order(nullptr),
	m_EnemyTurretTop3Order(nullptr),
	m_EnemyTurretMid1Order(nullptr),
	m_EnemyTurretMid2Order(nullptr),
	m_EnemyTurretMid3Order(nullptr),
	m_EnemyTurretBottom1Order(nullptr),
	m_EnemyTurretBottom2Order(nullptr),
	m_EnemyTurretBottom3Order(nullptr)
{
}

CGameManager::~CGameManager()
{

}

void CGameManager::SetTurret(CGameObject* Turret, LoLLine line, int Order, bool Enemy)
{
	if (Enemy)
	{
		switch (line)
		{
		case LoLLine::Top:
			if (Order == 1)
			{
				m_EnemyTurretTop1Order = Turret;
				return;
			}

			else if (Order == 2)
			{
				m_EnemyTurretTop2Order = Turret;
				return;
			}

			else if (Order == 3)
			{
				m_EnemyTurretTop3Order = Turret;
				return;
			}
			break;
		case LoLLine::Mid:
			if (Order == 1)
			{

			}

			else if (Order == 1)
			{

			}

			else if (Order == 2)
			{

			}
			break;
		case LoLLine::Bottom:
			break;
		}

	}

	else
	{

	}
}

void CGameManager::GetTurret(LoLLine line, int Order, bool Enemy)
{
}

const Vector3 CGameManager::GetNearTurretPos(const Vector3& Pos)
{
	return Vector3();
}
