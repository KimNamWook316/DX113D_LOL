#pragma once

#include "GameInfo.h"
#include "Component/ObjectComponent.h"

class CLoLDataComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CLoLDataComponent();
	CLoLDataComponent(const CLoLDataComponent& com);
	virtual ~CLoLDataComponent();

private:
	LoLData m_Data;

public:
	void SetHP(int HP)
	{
		m_Data.HP = HP;
	}

	void SetMP(int MP)
	{
		m_Data.MP = MP;
	}

	int GetHP()	const
	{
		return m_Data.HP;
	}

	int GetMP()	const
	{
		return m_Data.MP;
	}
};

