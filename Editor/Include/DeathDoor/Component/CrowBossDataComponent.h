#pragma once

#include "MonsterDataComponent.h"

class CCrowBossDataComponent :
    public CMonsterDataComponent
{
	friend class CGameObject;

private:
	CCrowBossDataComponent();
	CCrowBossDataComponent(const CCrowBossDataComponent& com);
	virtual ~CCrowBossDataComponent();

public:
	virtual void Start() override;

private:
	int m_ChainAttackCount;
	bool m_StartFlying; // 쇠사슬타고 날아가기 시작했는데
	bool m_StartJump; // 점프 시작했는지
	bool m_InFlying;
	bool m_StartStomp;
	//float m_SpinAmount;
	int m_Phase;
	bool m_PhasePick;
	// Bypass하는 기준을 잡기 위해 플레이어의 순간적 위치를 설정한다
	Vector3 m_PlayerOriginPos;
	Vector3 m_MyOriginPos;

public:
	void SetPlayerOriginPos(const Vector3& Pos)
	{
		m_PlayerOriginPos = Pos;
	}

	void SetMyOriginPos(const Vector3& Pos)
	{
		m_MyOriginPos = Pos;
	}

	void SetPhasePick(bool Pick)
	{
		m_PhasePick = Pick;
	}

	void SetPhase(int Phase)
	{
		m_Phase = Phase;
	}

	void SetStartJump(bool Start)
	{
		m_StartJump = Start;
	}

	const Vector3& GetPlayerOriginPos()	const
	{
		return m_PlayerOriginPos;
	}

	const Vector3& GetMyOriginPos()	const
	{
		return m_MyOriginPos;
	}

	bool GetStartJump()	const
	{
		return m_StartJump;
	}

	bool GetStartStomp()	const
	{
		return m_StartStomp;
	}

	void AddChainAttackCount()
	{
		++m_ChainAttackCount;
	}

	int GetPhase()	const
	{
		return m_Phase;
	}

	bool GetPhasePick()	const
	{
		return m_PhasePick;
	}

	int GetChainAttackCount() const
	{
		return m_ChainAttackCount;
	}

	void SetChainAttackCount(int Count)
	{
		m_ChainAttackCount = Count;
	}

	void SetStartFlying(bool Start)
	{
		m_StartFlying = Start;
	}

	bool IsStartFlying()	const
	{
		return m_StartFlying;
	}

	void SetInFlying(bool InFlying)
	{
		m_InFlying = InFlying;
	}

	bool IsInFlying()	const
	{
		return m_InFlying;
	}

	void SetStartStomp(bool Start)
	{
		m_StartStomp = Start;
	}

	bool IsStartStomp()	const
	{
		return m_StartStomp;
	}

public:
	void OnEndCrowBossJump();
};

