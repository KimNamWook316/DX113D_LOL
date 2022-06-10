#pragma once

#include "../../GameInfo.h"

class CStateManager
{
friend class CSceneManager;

	CStateManager();
	~CStateManager();

private:
	std::unordered_map<std::string, class CState*>	m_mapState;

public:
	void Init();

public:
	void AddState(const std::string& Name, class CState* State);
	class CState* FindState(const std::string& Name);

public:
	void CheckAirborneTarget(const NotifyParameter& Param);
	void FindRestraintTarget(const NotifyParameter& Param);
	void FindStunTarget(const NotifyParameter& Param);
	void FindKnockBackTarget(const NotifyParameter& Param);

};

