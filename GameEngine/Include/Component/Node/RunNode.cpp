
#include "RunNode.h"
#include "../AnimationMeshComponent.h"
#include "../../Input.h"
#include "../BehaviorTree.h"

CRunNode::CRunNode()
{
	SetTypeID(typeid(CRunNode).hash_code());
}

CRunNode::CRunNode(const CRunNode& Node)	:
	CActionNode(Node)
{
}

CRunNode::~CRunNode()
{
}


NodeResult CRunNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "Run";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
		m_CallStart = true;
	}

	return NodeResult::Node_True;
}

NodeResult CRunNode::OnUpdate(float DeltaTime)
{
	// 실제 이동(Test를 위해 스피드를 상수로, 방향도 전방으로 일단 설정)
	m_Object->AddWorldPos(0.f, 0.f, 10.f * DeltaTime);

	return NodeResult::Node_True;
}

NodeResult CRunNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

bool CRunNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
