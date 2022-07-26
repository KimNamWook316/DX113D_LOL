
#include "CrowBossBypassCheck.h"
#include "../GameStateComponent.h"
#include "../GameBehaviorTree.h"
#include "../CrowBossDataComponent.h"

CCrowBossBypassCheck::CCrowBossBypassCheck()
{
	SetTypeID(typeid(CCrowBossBypassCheck).hash_code());
}

CCrowBossBypassCheck::CCrowBossBypassCheck(const CCrowBossBypassCheck& Node)	:
	CConditionNode(Node)
{
}

CCrowBossBypassCheck::~CCrowBossBypassCheck()
{
}

// ByPassCheck의 Parent가 SetCurrentNode로 지정된 상태
NodeResult CCrowBossBypassCheck::OnStart(float DeltaTime)
{
	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	Vector3 PlayerOriginPos = Data->GetPlayerOriginPos();
	Vector3 MyOriginPos = Data->GetMyOriginPos();
	Vector3 MyCurrentPos = m_Object->GetWorldPos();

	Vector3 Dir = PlayerOriginPos - MyOriginPos;
	Dir.Normalize();

	m_Object->AddWorldPos(Dir * 6.f * DeltaTime);

	Vector3 OriginDiff = PlayerOriginPos - MyOriginPos;
	Vector3 CurrentDiff = PlayerOriginPos - MyCurrentPos;

	if (OriginDiff.x < 0.f)
	{
		if (CurrentDiff.x > 1.f)
			return NodeResult::Node_True;
	}

	else
	{
		if (CurrentDiff.x < -1.f)
			return NodeResult::Node_True;
	}

	if (OriginDiff.z < 0.f)
	{
		if (CurrentDiff.z > 1.f)
			return NodeResult::Node_True;
	}

	else
	{
		if (CurrentDiff.z < -1.f)
			return NodeResult::Node_True;
	}
	

	return NodeResult::Node_False;
}

NodeResult CCrowBossBypassCheck::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CCrowBossBypassCheck::OnEnd(float DeltaTime)
{
	return NodeResult();
}
