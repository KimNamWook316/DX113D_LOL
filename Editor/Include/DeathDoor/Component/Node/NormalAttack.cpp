
#include "NormalAttack.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CNormalAttack::CNormalAttack()
{
	SetTypeID(typeid(CNormalAttack).hash_code());
}

CNormalAttack::CNormalAttack(const CNormalAttack& Node)	:
	CActionNode(Node)
{
}

CNormalAttack::~CNormalAttack()
{
}

NodeResult CNormalAttack::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ObjectName = m_Object->GetName();

	std::string SequenceName;

	if (m_Object->GetObjectType() == Object_Type::Player)
	{
		SequenceName = ObjectName + "SlashL";
		//SequenceName = "SlashL";
	}

	else
	{

	}

	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);

	if (m_AnimationMeshComp)
	{
		CAnimationSequenceInstance* Instance = m_AnimationMeshComp->GetAnimationInstance();

		// AnimationSequenceInstance에서 m_ChangeTime이 있기 때문에, 바로 애니메이션이 바뀌는게 아니라 m_ChangeTime에 걸쳐서 애니메이션이 바뀌는데
		// CurrentAnimation은 NormalAttack이라서 ChangeAnimation해도 아무것도 안하고 바로 return되고 ChangeAnimation은 Idle같은 시퀀스로 설정되어 있는 상태에서
		// 이 상태에서 OnUpdate로 들어가고 Idle로 완전히 Sequence가 바뀐 상태에서 NormalAttack::OnUpdate에서 CurrentAnimation은 NormalAttack이라고 생각하지만
		// CurrentAnimation은 결국 Idle로 될 것이고, ChangeAnimation은 nullptr가 될 것이므로 OnUpdate의 else에만 계속 들어오게 되는 문제가 생기므로
		// 이런 상황에서는 KeepAnimation을 호출해서 ChangeAnimation을 지우고 CurrentAnimation은 Attack으로 유지시켜준다
		if (Instance->GetChangeAnimation() && Instance->GetCurrentAnimation()->GetName() == SequenceName)
			Instance->KeepCurrentAnimation();

		else
			Instance->ChangeAnimation(SequenceName);
	}

	m_Object->SetNoInterrupt(false);
	m_CallStart = true;
	
	return NodeResult::Node_True;
}

NodeResult CNormalAttack::OnUpdate(float DeltaTime)
{
	m_Object->SetNoInterrupt(false);

	CAnimationSequenceInstance* Instance = m_AnimationMeshComp->GetAnimationInstance();
	if (!Instance->IsCurrentAnimLoop() && Instance->IsCurrentAnimEnd())
	{
		m_Object->SetNoInterrupt(false);

		m_IsEnd = true;
		m_CallStart = false;
		m_Owner->SetCurrentNode(nullptr);
		return NodeResult::Node_False;
	}

	else
	{
		m_Owner->SetCurrentNode(this);

		return NodeResult::Node_Running;
	}
}

NodeResult CNormalAttack::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;

	return NodeResult::Node_True;
}

NodeResult CNormalAttack::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
