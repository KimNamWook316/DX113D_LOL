
#include "InputRCheckNode.h"
#include "../../Input.h"

CInputRCheckNode::CInputRCheckNode()
{
	SetTypeID(typeid(CInputRCheckNode).hash_code());
}

CInputRCheckNode::CInputRCheckNode(const CInputRCheckNode& Node)
{
}

CInputRCheckNode::~CInputRCheckNode()
{
}

NodeResult CInputRCheckNode::OnStart(float DeltaTime)
{
	const keyState State = CInput::GetInst()->FindKeyState('R');

	if (!State.State[KeyState_Up] && !State.State[KeyState_Down] && !State.State[KeyState_Push])
	{
		return NodeResult::Node_False;
	}

	else
	{
		return NodeResult::Node_True;
	}
}

NodeResult CInputRCheckNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CInputRCheckNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

bool CInputRCheckNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}
