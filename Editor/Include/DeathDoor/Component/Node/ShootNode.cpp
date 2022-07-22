
#include "ShootNode.h"
#include "RotateAttackDirectionNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../PlayerDataComponent.h"
#include "../PlayerHookComponent.h"
#include "../PlayerBowComponent.h"
#include "../../../Object/PlayerHook.h"
#include "Component/Node/CompositeNode.h"

CShootNode::CShootNode()	:
	m_InRestoreCam(false)
{
	SetTypeID(typeid(CShootNode).hash_code());
}

CShootNode::CShootNode(const CShootNode& Node) :
	CActionNode(Node)
{
}

CShootNode::~CShootNode()
{
}

NodeResult CShootNode::OnStart(float DeltaTime)
{
	/*CPlayerDataComponent* Comp = m_Object->FindObjectComponentFromType<CPlayerDataComponent>();

	Player_Ability Ability = Comp->GetPlayerAbility();*/

	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	//std::string ObjectName = m_Object->GetName();

	//std::string SequenceName;

	//if (Ability == Player_Ability::Hook)
	//	SequenceName = ObjectName + "Hook";
	//else if (Ability == Player_Ability::Arrow)
	//	SequenceName == ObjectName + "Arrow";

	//if (m_AnimationMeshComp)
	//{
	//	CAnimationSequenceInstance* Instance = m_AnimationMeshComp->GetAnimationInstance();

	//	// AnimationSequenceInstance에서 m_ChangeTime이 있기 때문에, 바로 애니메이션이 바뀌는게 아니라 m_ChangeTime에 걸쳐서 애니메이션이 바뀌는데
	//	// CurrentAnimation은 ShootNode이라서 ChangeAnimation해도 아무것도 안하고 바로 return되고 ChangeAnimation은 Idle같은 시퀀스로 설정되어 있는 상태에서
	//	// 이 상태에서 OnUpdate로 들어가고 Idle로 완전히 Sequence가 바뀐 상태에서 ShootNode::OnUpdate에서 CurrentAnimation은 ShootNode이라고 생각하지만
	//	// CurrentAnimation은 결국 Idle로 될 것이고, ChangeAnimation은 nullptr가 될 것이므로 OnUpdate의 else에만 계속 들어오게 되는 문제가 생기므로
	//	// 이런 상황에서는 KeepAnimation을 호출해서 ChangeAnimation을 지우고 CurrentAnimation은 Attack으로 유지시켜준다
	//	if (Instance->GetChangeAnimation() && Instance->GetCurrentAnimation()->GetName() == SequenceName)
	//		Instance->KeepCurrentAnimation();

	//	else
	//		Instance->ChangeAnimation(SequenceName);
	//}

	//m_Object->SetNoInterrupt(false);
	//m_CallStart = true;

	return NodeResult::Node_True;
}

NodeResult CShootNode::OnUpdate(float DeltaTime)
{
	CPlayerDataComponent* Comp = m_Object->FindObjectComponentFromType<CPlayerDataComponent>();

	Player_Ability Ability = Comp->GetPlayerAbility();

	CRotateAttackDirectionNode* Node = (CRotateAttackDirectionNode*)((CCompositeNode*)(m_Parent->GetParent()->GetParent()))->FindChildByType<CRotateAttackDirectionNode>();
	Vector3 Point = Node->GetPickingPoint();

	Vector3 ObjectPos = m_Object->GetWorldPos();
	Vector3 Dir = Vector3(Point.x, 0.f, Point.z) - Vector3(ObjectPos.x, 0.f, ObjectPos.z);
	Dir.Normalize();

	if (Ability == Player_Ability::Hook)
	{
		if (m_InRestoreCam)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();

			bool RestoreEnd = Scene->RestoreCamera(50.f, DeltaTime);

			if (RestoreEnd)
			{
				m_IsEnd = true;
				m_CallStart = false;
				m_Owner->SetCurrentNode(nullptr);
				m_InRestoreCam = false;

				return NodeResult::Node_True;
			}

			else
			{
				return NodeResult::Node_Running;
			}
		}

		CPlayerHookComponent* Comp = m_Object->FindComponentFromType<CPlayerHookComponent>();
		bool HookCollision = false;
		if (Comp)
		{
			HookResult Result = Comp->ShootHook(Dir, DeltaTime);

			if (Result == HookResult::NoCollision)
			{
				m_InRestoreCam = true;
				return NodeResult::Node_Running;
			}

			else if (Result == HookResult::Collision)
			{
				m_Owner->SetCurrentNode(this);
				return NodeResult::Node_Running;
				// 충돌된 지점까지 플레이어가 날아가고 있는중
			}

			else if(Result == HookResult::OnShoot)
			{
				m_Owner->SetCurrentNode(this);
				return NodeResult::Node_Running;
			}

			else if (Result == HookResult::CollisionEnd)
			{
				m_Owner->SetCurrentNode(nullptr);
				return NodeResult::Node_True;
			}

		}

		else
		{
			// CancleShootNode를 CurrentNode로 만들어서 카메라 원래 위치로 복구
			m_Owner->SetCurrentNode(((CCompositeNode*)m_Parent->GetParent())->GetChild(1));
		}

		return NodeResult::Node_True;
	}

	else if (Ability == Player_Ability::Arrow)
	{
		if (m_InRestoreCam)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();

			bool RestoreEnd = Scene->RestoreCamera(50.f, DeltaTime);

			if (RestoreEnd)
			{
				m_IsEnd = true;
				m_CallStart = false;
				m_Owner->SetCurrentNode(nullptr);
				m_InRestoreCam = false;

				return NodeResult::Node_True;
			}

			else
			{
				return NodeResult::Node_Running;
			}
		}

		else
		{
			CPlayerBowComponent* BowComp = m_Object->FindComponentFromType<CPlayerBowComponent>();

			BowComp->ShootArrow(Dir);
		}
	}
}

NodeResult CShootNode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;

	return NodeResult::Node_True;
}

NodeResult CShootNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
