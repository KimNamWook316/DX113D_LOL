
#include "CrowBossPhasePickNode.h"
#include "../GameStateComponent.h"
#include "../GameBehaviorTree.h"
#include "../CrowBossDataComponent.h"
#include "Scene/Scene.h"
#include "Component/Node/CompositeNode.h"

CCrowBossPhasePickNode::CCrowBossPhasePickNode()
{
	SetTypeID(typeid(CCrowBossPhasePickNode).hash_code());
}

CCrowBossPhasePickNode::CCrowBossPhasePickNode(const CCrowBossPhasePickNode& Node) :
	CConditionNode(Node)
{
}

CCrowBossPhasePickNode::~CCrowBossPhasePickNode()
{
}

NodeResult CCrowBossPhasePickNode::OnStart(float DeltaTime)
{
	//int Phase = rand() % 4 + 2; // 2 ~ 5

	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	int Phase;
	if(Data->IsPhaseQueueEmpty())
		Phase = rand() % 4 + 2;
	else
	{
		Phase = Data->GetPhaseQueueFront();
		Data->PopPhaseQueue();
	}

	Data->SetPhasePick(true);
	Data->SetPhase(Phase);
	
	CGameObject* Player = m_Object->GetScene()->GetPlayerObject();
	Vector3 PlayerPos = Player->GetWorldPos();

	Data->SetPlayerOriginPos(PlayerPos);
	Data->SetMyOriginPos(m_Object->GetWorldPos());

	if (Phase == 2)
	{
		CCompositeNode* Parent = (CCompositeNode*)m_Parent;

		CNode* Phase2Node = ((CCompositeNode*)Parent->GetChild(1))->GetChild(0);

		m_Owner->SetCurrentNode(Phase2Node);
	}

	else if (Phase == 3)
	{
		CCompositeNode* Parent = (CCompositeNode*)m_Parent;

		CNode* Phase3Node = ((CCompositeNode*)Parent->GetChild(1))->GetChild(1);

		m_Owner->SetCurrentNode(Phase3Node);
	}

	else if (Phase == 4)
	{
		CCompositeNode* Parent = (CCompositeNode*)m_Parent;

		CNode* Phase2Node = ((CCompositeNode*)Parent->GetChild(1))->GetChild(2);

		m_Owner->SetCurrentNode(Phase2Node);
	}

	else if (Phase == 4)
	{
		CCompositeNode* Parent = (CCompositeNode*)m_Parent;

		CNode* Phase2Node = ((CCompositeNode*)Parent->GetChild(1))->GetChild(3);

		Data->SetSpittingStart(true);

		m_Owner->SetCurrentNode(Phase2Node);
	}

	return NodeResult::Node_False;
}

NodeResult CCrowBossPhasePickNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CCrowBossPhasePickNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
