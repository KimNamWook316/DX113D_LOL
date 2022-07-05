
#include "Alistar.h"
#include "AlistarAnimation.h"
#include "Input.h"

#include "Component/StateComponent.h"
#include "Component/Node/SelectorNode.h"
#include "Component/Node/SequenceNode.h"
#include "Component/Node/IdleNode.h"
#include "Component/Node/RunNode.h"
#include "Component/Node/MoveInputCheckNode.h"

CAlistar::CAlistar()
{
}

CAlistar::CAlistar(const CAlistar& obj)	:
	CGameObject(obj)
{
}

CAlistar::~CAlistar()
{
}

bool CAlistar::Init()
{
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Arm = CreateComponent<CArm>("Arm");
	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_Mesh->AddChild(m_Arm);

	m_Arm->AddChild(m_Camera);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);

	m_Mesh->SetMesh("AlistarMesh");
	m_Mesh->CreateAnimationInstance<CAlistarAnimation>();

	m_Animation = (CAlistarAnimation*)m_Mesh->GetAnimationInstance();

	m_Mesh->SetRelativeScale(0.02f, 0.02f, 0.02f);

	m_Arm->SetOffset(0.f, 2.f, 0.f);
	m_Arm->SetRelativeRotation(25.f, 0.f, 0.f);
	m_Arm->SetTargetDistance(10.f);

	CStateComponent* StateComponent = CreateComponent<CStateComponent>("AlistarStateComponent");
	StateComponent->SetAnimationMeshComponent(m_Mesh);

	CSelectorNode* Root = StateComponent->CreateTreeNode<CSelectorNode>("RootSelector");
	CSequenceNode* MoveInputSequence = StateComponent->CreateTreeNode<CSequenceNode>("MoveInputSequence");
	
	CMoveInputCheckNode* MoveCheck = StateComponent->CreateTreeNode<CMoveInputCheckNode>("MoveInputCheckNode");
	CRunNode* RunNode = StateComponent->CreateTreeNode<CRunNode>("RunNode");
	CIdleNode* IdleNode = StateComponent->CreateTreeNode<CIdleNode>("IdleNode");


	Root->AddChild(MoveInputSequence);
	MoveInputSequence->AddChild(MoveCheck);
	MoveInputSequence->AddChild(RunNode);

	Root->AddChild(IdleNode);

	

	CInput::GetInst()->SetKeyCallback<CAlistar>("MoveFront", KeyState_Down,
		this, &CAlistar::MoveFront);

	return true;
}

void CAlistar::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (CInput::GetInst()->GetWheelDir())
	{
		float Length = m_Arm->GetTargetDistance() +
			CInput::GetInst()->GetWheelDir() * 0.1f;

		m_Arm->SetTargetDistance(Length);
	}
}

void CAlistar::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CAlistar* CAlistar::Clone()
{
	return new CAlistar(*this);
}

void CAlistar::MoveFront(float DeltaTime)
{
	//m_BehaviorTree->PushStartNode("RunNode");
}
