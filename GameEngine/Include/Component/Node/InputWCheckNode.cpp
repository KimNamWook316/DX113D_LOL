
#include "InputWCheckNode.h"
#include "../../Input.h"

CInputWCheckNode::CInputWCheckNode()
{
	SetTypeID(typeid(CInputWCheckNode).hash_code());
}

CInputWCheckNode::CInputWCheckNode(const CInputWCheckNode& Node)
{
}

CInputWCheckNode::~CInputWCheckNode()
{
}

NodeResult CInputWCheckNode::OnStart(float DeltaTime)
{
	const keyState State = CInput::GetInst()->FindKeyState('W');

	if (!State.State[KeyState_Up] && !State.State[KeyState_Down] && !State.State[KeyState_Push])
	{
		return NodeResult::Node_False;
	}

	else
	{
		return NodeResult::Node_True;
	}
}

NodeResult CInputWCheckNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CInputWCheckNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

bool CInputWCheckNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}
