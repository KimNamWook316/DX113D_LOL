#pragma once

#include "../../GameInfo.h"

class CState
{
public:
	CState();
	CState(const CState& state);
	virtual ~CState() = 0;

public:
	virtual void Update(float deltaTime) = 0;
	virtual void PostUpdate(float deltaTime) = 0;
	virtual CState* Clone() = 0;

public:
	virtual void OnStart() = 0;
	virtual void OnEnd() = 0;

public:
	bool IsEnd() const
	{
		return m_IsEnd;
	}

	void SetOwner(class CStateComponent* com)
	{
		m_Owner = com;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	void SetOwnerObject(CGameObject* Object)
	{
		m_OwnerObject = Object;
	}

	CGameObject* GetOwnerObject()	const
	{
		return m_OwnerObject;
	}

protected:
	class CStateComponent* m_Owner;
	CGameObject* m_OwnerObject;
	bool m_IsEnd;
	std::string m_Name;
};

