#pragma once

#include "GameInfo.h"

class CLoLBuildingManager
{

public:
	void Init();

private:
	class CGameObject* m_EnemyNexus;
	class CGameObject* m_FriendNexus;

private:

	DECLARE_SINGLE(CLoLBuildingManager)
};

