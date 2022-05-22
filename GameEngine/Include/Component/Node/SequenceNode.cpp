
#include "SequenceNode.h"
#include "../BehaviorTree.h"

CSequenceNode::CSequenceNode()
{
	SetTypeID(typeid(CSequenceNode).hash_code());
}

CSequenceNode::CSequenceNode(const CSequenceNode& Node)	:
	CCompositeNode(Node)
{
}

CSequenceNode::~CSequenceNode()
{
}

bool CSequenceNode::Invoke(float DeltaTime)
{
	auto iter = GetChildrenList().begin();
	auto iterEnd = GetChildrenList().end();

	for (; iter != iterEnd; ++iter)
	{
		// Sequence는 하나의 자식이라도 false를 리턴하면 자신도 false를 리턴
		if (!(*iter)->Invoke(DeltaTime))
		{
			// 내 자식중 하나라도 false라는 것은 이번 프레임에 내 자식들의 Action을 실행하지 않겠다는 의미이므로
			// 다음 프레임에 다시 나의 자식을 순회해서 모두 true를 반환해서 자식들의 Action을 실행할때는
			// OnStart부터 호출하게 해야하므로 모두 m_CallStart를 false로 만든다
			SetChildrenCallStart(false);
			return false;
		}
	}

	return true;
}

NodeResult CSequenceNode::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSequenceNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSequenceNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

