
#include "GameBehaviorTree.h"
#include "Component/Node/SelectorNode.h"
#include "Component/Node/SequenceNode.h"
#include "Node/MoveNode.h"
#include "Node/IdleNode.h"
#include "Node/MoveInputCheckNode.h"
#include "Node/NoInterruptNode.h"
#include "Node/NormalAttack.h"
#include "Node/MouseLButtonCheckNode.h"
#include "Node/MouseRButtonCheckNode.h"
#include "Node/MouseRButtonUpCheckNode.h"
#include "Node/NegateNode.h"
#include "Node/RotateAttackDirectionNode.h"
#include "Node/ReadyToShoot.h"
#include "Node/ShootNode.h"
#include "Node/CancleShootNode.h"
#include "Node/AddFallingFloorCallbackNode.h"
#include "Node/Lockstone3TriggerBoxHitCheck.h"
#include "Node/CheckDetectRangeNode.h"
#include "Node/FindPathNode.h"
#include "Node/Lockstone3TriggerBoxAction.h"
#include "Node/ClearPathListNode.h"
#include "Node/PathFindEnableCheck.h"
#include "Node/HitBackNode.h"
#include "Node/HitCheckNode.h"
#include "Node/RollInputCheck.h"
#include "Node/PlayerRoll.h"

// Public Nodes
#include "Node/MeleeRangeCheckNode.h"
#include "Node/PostAttackDelayCheck.h"

// Boss - Knight
#include "Node/BossKnightFinalAttackCheck.h"
#include "Node/BossKnightJumpAttackRangeCheck.h"
#include "Node/BossKnightContinueAttackNode.h"
#include "Node/BossKnightFinalAttackNode.h"
#include "Node/BossKnightJumpAttackNode.h"
#include "Node/BossKnightMeleeAttackNode.h"
#include "Node/BossKnightSlamEnd.h"

#include "GameStateComponent.h"

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

CGameStateComponent* CGameBehaviorTree::GetOwner() const
{
	return dynamic_cast<CGameStateComponent*>(m_Owner);
}


CNode* CGameBehaviorTree::LoadNode(CNode* Parent, size_t TypeID)
{
	CNode* NewNode = nullptr;
	CGameObject* OwnerObject = m_Owner->GetGameObject();

	// TODO : Node 종류 추가될 때 마다 추가해주기
	if (TypeID == typeid(CSelectorNode).hash_code())
	{
		NewNode = MakeNode<CSelectorNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CSequenceNode).hash_code())
	{
		NewNode = MakeNode<CSequenceNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CNoInterruptNode).hash_code())
	{
		NewNode = MakeNode<CNoInterruptNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CIdleNode).hash_code())
	{
		NewNode = MakeNode<CIdleNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMoveNode).hash_code())
	{
		CMoveNode* NewNode = new CMoveNode;
		NewNode->SetParent(Parent);
		NewNode->SetOwner(this);
		NewNode->SetObject(m_Owner->GetGameObject());
		NewNode->SetAnimationMeshComponent(m_AnimationMeshComp);

		CNavAgent* Agent = NewNode->GetGameObject()->FindObjectComponentFromType<CNavAgent>();

		if (Agent)
			NewNode->SetNavAgent(Agent);

		return NewNode;
	}

	else if (TypeID == typeid(CMoveInputCheckNode).hash_code())
	{
		NewNode = MakeNode<CMoveInputCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMouseLButtonCheckNode).hash_code())
	{
		NewNode = MakeNode<CMouseLButtonCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMouseRButtonCheckNode).hash_code())
	{
		NewNode = MakeNode<CMouseRButtonCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMouseRButtonUpCheckNode).hash_code())
	{
		NewNode = MakeNode<CMouseRButtonUpCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CReadyToShoot).hash_code())
	{
		NewNode = MakeNode<CReadyToShoot>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CShootNode).hash_code())
	{
		NewNode = MakeNode<CShootNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CCancleShootNode).hash_code())
	{
		NewNode = MakeNode<CCancleShootNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CNormalAttack).hash_code())
	{
		NewNode = MakeNode<CNormalAttack>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CRotateAttackDirectionNode).hash_code())
	{
		NewNode = MakeNode<CRotateAttackDirectionNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CNegateNode).hash_code())
	{
		NewNode = MakeNode<CNegateNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CAddFallingFloorCallbackNode).hash_code())
	{
		NewNode = MakeNode<CAddFallingFloorCallbackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CLockstone3TriggerBoxHitCheck).hash_code())
	{
		NewNode = MakeNode<CLockstone3TriggerBoxHitCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CLockstone3TriggerBoxAction).hash_code())
	{
		NewNode = MakeNode<CLockstone3TriggerBoxAction>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CCheckDetectRangeNode).hash_code())
	{
		NewNode = MakeNode<CCheckDetectRangeNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CFindPathNode).hash_code())
	{
		NewNode = MakeNode<CFindPathNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CMeleeRangeCheckNode).hash_code())
	{
		NewNode = MakeNode<CMeleeRangeCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CPostAttackDelayCheck).hash_code())
	{
		NewNode = MakeNode<CPostAttackDelayCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightFinalAttackCheck).hash_code())
	{
		NewNode = MakeNode<CBossKnightFinalAttackCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightJumpAttackRangeCheck).hash_code())
	{
		NewNode = MakeNode<CBossKnightJumpAttackRangeCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightContinueAttackNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightContinueAttackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightFinalAttackNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightFinalAttackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightJumpAttackNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightJumpAttackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightMeleeAttackNode).hash_code())
	{
		NewNode = MakeNode<CBossKnightMeleeAttackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CBossKnightSlamEnd).hash_code())
	{
		NewNode = MakeNode<CBossKnightSlamEnd>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CClearPathListNode).hash_code())
	{
		NewNode = MakeNode<CClearPathListNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CPathFindEnableCheck).hash_code())
	{
		NewNode = MakeNode<CPathFindEnableCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CHitCheckNode).hash_code())
	{
		NewNode = MakeNode<CHitCheckNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CHitBackNode).hash_code())
	{
		NewNode = MakeNode<CHitBackNode>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CRollInputCheck).hash_code())
	{
		NewNode = MakeNode<CRollInputCheck>(Parent, OwnerObject);
	}

	else if (TypeID == typeid(CPlayerRoll).hash_code())
	{
		NewNode = MakeNode<CPlayerRoll>(Parent, OwnerObject);
	}

	else
	{
	NewNode = MakeNode<CPlayerRoll>(Parent, OwnerObject);
	}

	return NewNode;
}
