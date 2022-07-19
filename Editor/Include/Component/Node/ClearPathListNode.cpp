
#include "ClearPathListNode.h"
#include "../MonsterDataComponent.h"
#include "../GameStateComponent.h"
#include "../MonsterNavAgent.h"

CClearPathListNode::CClearPathListNode()
{
	SetTypeID(typeid(CClearPathListNode).hash_code());
}

CClearPathListNode::CClearPathListNode(const CClearPathListNode& Node) :
	CActionNode(Node)
{
}

CClearPathListNode::~CClearPathListNode()
{
}

NodeResult CClearPathListNode::OnStart(float DeltaTime)
{
	CMonsterDataComponent* Data = dynamic_cast<CMonsterDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	CMonsterNavAgent* Agent = Data->GetMonsterNavAgent();

	if (Agent)
	{
		Agent->ClearPathList();
		Agent->SetCoolStart(true);
	}

	return NodeResult::Node_True;
}

NodeResult CClearPathListNode::OnUpdate(float DeltaTime)
{


	return NodeResult::Node_True;
}

NodeResult CClearPathListNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CClearPathListNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

