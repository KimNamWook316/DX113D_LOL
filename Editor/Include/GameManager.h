#pragma once

#include "GameInfo.h"

// 포탑, 넥서스, 억제기들의 모든 위치를 알고 있으며
// 미니언, 정글몹, 용, 바론의 생성 주기에 관여
class CGameManager
{

private:
	std::vector<Vector3> m_vecTurretPos;
	std::vector<Vector3> m_vecNexusPos;
	//std::vector<Vector3> 

public:
	const Vector3 GetNearTurretPos(const Vector3& Pos);

	DECLARE_SINGLE(CGameManager)
};

