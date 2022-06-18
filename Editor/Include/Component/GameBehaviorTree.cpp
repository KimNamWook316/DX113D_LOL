
#include "GameBehaviorTree.h"
#include "Component/Node/SelectorNode.h"
#include "Component/Node/SequenceNode.h"
#include "Node/MoveNode.h"
#include "Node/IdleNode.h"
#include "Node/MoveInputCheckNode.h"
#include "Node/NoInterruptNode.h"
#include "Node/CheckAttackTarget.h"
#include "Node/NormalAttack.h"
#include "Node/SkillEndCheckNode.h"
#include "Node/InSkillCheck.h"
#include "Node/CheckTurretAttackTarget.h"
#include "Node/CheckTurretAttackFrequency.h"
#include "Node/NegateNode.h"
#include "Component/StateComponent.h"

CGameBehaviorTree::CGameBehaviorTree()
{
}

CGameBehaviorTree::CGameBehaviorTree(const CGameBehaviorTree& Tree) :
	CBehaviorTree(Tree)
{
}

CGameBehaviorTree::~CGameBehaviorTree()
{
}

bool CGameBehaviorTree::Init()
{
	return false;
}

void CGameBehaviorTree::Start()
{
}


CNode* CGameBehaviorTree::LoadNode(CNode* Parent, size_t TypeID)
{
	// TODO : Node 종류 추가될 때 마다 추가해주기
	if (TypeID == typeid(CSelectorNode).hash_code())
	{
		CSelectorNode* NewNode = new CSelectorNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CSequenceNode).hash_code())
	{
		CSequenceNode* NewNode = new CSequenceNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CNoInterruptNode).hash_code())
	{
		CNoInterruptNode* NewNode = new CNoInterruptNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CIdleNode).hash_code())
	{
		CIdleNode* NewNode = new CIdleNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CMoveNode).hash_code())
	{
		CMoveNode* NewNode = new CMoveNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CMoveInputCheckNode).hash_code())
	{
		CMoveInputCheckNode* NewNode = new CMoveInputCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CCheckAttackTarget).hash_code())
	{
		CCheckAttackTarget* NewNode = new CCheckAttackTarget;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CNormalAttack).hash_code())
	{
		CNormalAttack* NewNode = new CNormalAttack;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CSkillEndCheckNode).hash_code())
	{
		CSkillEndCheckNode* NewNode = new CSkillEndCheckNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CInSkillCheck).hash_code())
	{
		CInSkillCheck* NewNode = new CInSkillCheck;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CCheckTurretAttackTarget).hash_code())
	{
		CCheckTurretAttackTarget* NewNode = new CCheckTurretAttackTarget;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CCheckTurretAttackFrequency).hash_code())
	{
		CCheckTurretAttackFrequency* NewNode = new CCheckTurretAttackFrequency;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}

	else if (TypeID == typeid(CNegateNode).hash_code())
	{
		CNegateNode* NewNode = new CNegateNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		return NewNode;
	}


	return nullptr;
}
