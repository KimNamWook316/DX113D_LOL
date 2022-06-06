
#include "MovePickingNode.h"
#include "../../Input.h"
#include "../CameraComponent.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Navigation3DManager.h"
#include "../BehaviorTree.h"
#include "../AnimationMeshComponent.h"

CMovePickingNode::CMovePickingNode()
{
	SetTypeID(typeid(CMovePickingNode).hash_code());
}

CMovePickingNode::CMovePickingNode(const CMovePickingNode& Node)	:
	CActionNode(Node)
{
}

CMovePickingNode::~CMovePickingNode()
{
}

NodeResult CMovePickingNode::OnStart(float DeltaTime)
{
	CNavigation3DManager* Manager = m_Object->GetScene()->GetNavigation3DManager();

	Vector3 TargetPos;
	if (Manager->CheckPickingPoint(TargetPos))
	{
		m_Object->ClearPath();
		m_Object->AddPath(TargetPos);
		CAnimationSequenceInstance* Instance = m_Owner->GetAnimationMeshComp()->GetAnimationInstance();

		Instance->ChangeAnimation(m_Object->GetName() + "_Run");

		m_IsEnd = true;
	}

	return NodeResult::Node_True;
}

NodeResult CMovePickingNode::OnUpdate(float DeltaTime)
{
	// 만약 NavAgent의 m_PathList가 비었다면 그때 return false해서 IdleNode로 가도록
	if (!m_Object->IsNavAgentPathListEmpty())
	{
		//m_Object->SetInterruptEnable(false);
		return NodeResult::Node_True;
	}

	else
	{
		//m_Object->SetInterruptEnable(true);
		return NodeResult::Node_False;
	}

	return NodeResult::Node_None;
}

NodeResult CMovePickingNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

bool CMovePickingNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
