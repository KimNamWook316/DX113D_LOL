#pragma once

#include "Component/StateComponent.h"

class CGameStateComponent :
	public CStateComponent
{
	friend class CGameObject;

protected:
	CGameStateComponent();
	CGameStateComponent(const CGameStateComponent& com);
	virtual ~CGameStateComponent();

};

