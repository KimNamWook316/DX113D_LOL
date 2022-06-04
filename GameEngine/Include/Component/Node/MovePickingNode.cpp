
#include "MovePickingNode.h"
#include "../../Input.h"
#include "../CameraComponent.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Navigation3DManager.h"

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
		m_Object->AddPath(TargetPos);
	}

	return NodeResult::Node_True;
}

NodeResult CMovePickingNode::OnUpdate(float DeltaTime)
{
	// 만약 NavAgent의 m_PathList가 비었다면 그때 return false
	if (m_Object->IsNavAgentPathListEmpty())
	{
		m_IsEnd = true;
		return NodeResult::Node_False;
	}

	return NodeResult::Node_True;
}

NodeResult CMovePickingNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

bool CMovePickingNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
