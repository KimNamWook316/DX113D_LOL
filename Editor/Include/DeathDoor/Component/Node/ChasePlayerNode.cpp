#include "ChasePlayerNode.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Navigation3DManager.h"
#include "Component/BehaviorTree.h"	
#include "Component/AnimationMeshComponent.h"	
#include "../MonsterDataComponent.h"
#include "../GameStateComponent.h"
#include "../MonsterNavAgent.h"

CChasePlayerNode::CChasePlayerNode()
{
	SetTypeID(typeid(CChasePlayerNode).hash_code());
}

CChasePlayerNode::CChasePlayerNode(const CChasePlayerNode& Node)	:
	CActionNode(Node)
{
}

CChasePlayerNode::~CChasePlayerNode()
{
}

void CChasePlayerNode::Init()
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	m_DataComp = (dynamic_cast<CMonsterDataComponent*>(m_Owner->GetOwner()));
	m_Nav = m_DataComp->GetMonsterNavAgent();

	m_MoveSpeed = m_DataComp->GetMoveSpeed();
	m_RotationSpeed = m_DataComp->GetRotateSpeed();

	CScene* Scene = m_Owner->GetOwner()->GetScene();
	m_PlayerObj = Scene->GetPlayerObject();
}

NodeResult CChasePlayerNode::OnStart(float DeltaTime)
{
	m_Owner->SetCurrentNode(this);

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();
	AnimInst->ChangeAnimation("Run");

	m_RootComp = m_Owner->GetOwner()->GetGameObject()->GetRootComponent();
	Vector3 PlayerPos = m_PlayerObj->GetWorldPos();

	m_Nav->FindPath(m_RootComp, PlayerPos);
	m_Nav->SetMoveSpeed(m_MoveSpeed);
	m_Nav->SetRotationSpeed(m_RotationSpeed);

	return NodeResult::Node_True;
}

NodeResult CChasePlayerNode::OnUpdate(float DeltaTime)
{
	bool InMeleeAttackRange = m_DataComp->IsPlayerInMeleeAttackRange();

	if (InMeleeAttackRange)
	{
		m_DataComp->SetCurrentNodeNull();
		m_Nav->ClearPathList();
		m_DataComp->SetMoveSpeed(0.f);
		m_DataComp->SetCurRotSpeed(0.f);

		return NodeResult::Node_True;
	}

	bool NavCool = m_Nav->GetPathFindEnable();

	if (NavCool)
	{
		bool NavEmpty = m_Nav->IsEmptyPathList();

		if (!NavEmpty)
		{
			bool Rotaiting = m_Nav->IsRotaiting();

			// 바라보는 방향과 길의 방향이 맞지 않아 돌고 있는 도중에는 포지션 변경하지 않는다
			if (Rotaiting)
			{
				m_Nav->SetMoveSpeed(0.f);
			}
			else
			{
				m_Nav->SetMoveSpeed(m_MoveSpeed);
			}
		}
		return NodeResult::Node_True;
	}
	else
	{
		// 쿨타임마다 다시 찾는다.
		Vector3 PlayerPos = m_PlayerObj->GetWorldPos();
		m_Nav->FindPath(m_RootComp, PlayerPos);
	}

	return NodeResult::Node_True;
}

NodeResult CChasePlayerNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
