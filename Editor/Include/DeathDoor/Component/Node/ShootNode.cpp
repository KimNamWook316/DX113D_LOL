
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
#include "../PlayerBombComponent.h"
#include "../../../Object/PlayerHook.h"
#include "Component/Node/CompositeNode.h"

CShootNode::CShootNode()	:
	m_InRestoreCam(false),
	m_CameraMoveSpeed(50.f),
	m_CameraMoveTime(0.f)
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
	m_CurrentCamPos = m_Object->GetScene()->GetCameraManager()->GetCurrentCamera()->GetWorldPos();

	// Shoot방향으로 카메라가 움직이기 전에 ReadyToShoot 노드에서 저장해놓은 원래 카메라 위치
	Vector3 CamOriginPos = m_Object->GetScene()->GetOriginCamPos();
	float Dist = m_CurrentCamPos.Distance(CamOriginPos);

	m_CameraMoveTime = Dist / m_CameraMoveSpeed;

	m_CallStart = true;

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
			bool RestoreEnd = Scene->RestoreCamera(m_CameraMoveTime, m_CurrentCamPos, DeltaTime);

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

		m_CallStart = true;

		return NodeResult::Node_True;
	}

	else if (Ability == Player_Ability::Arrow)
	{
		if (m_InRestoreCam)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();
			bool RestoreEnd = Scene->RestoreCamera(m_CameraMoveTime, m_CurrentCamPos, DeltaTime);

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

			if (BowComp)
			{
				BowComp->ShootArrow(Dir);

				m_Owner->SetCurrentNode(this);

				BowComp->HideBow();
			}

			m_InRestoreCam = true;
		}

		m_CallStart = true;

		return NodeResult::Node_True;
	}

	else if (Ability == Player_Ability::Bomb)
	{
		if (m_InRestoreCam)
		{
			CScene* Scene = CSceneManager::GetInst()->GetScene();
			bool RestoreEnd = Scene->RestoreCamera(m_CameraMoveTime, m_CurrentCamPos, DeltaTime);

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
			CPlayerBombComponent* BombComp = m_Object->FindObjectComponentFromType<CPlayerBombComponent>();

			// ReadyToShoot 노드에서 Bomb Lift가 아직 안끝났는데 RButton을 떼면 BombComponent와 Bomb Object를 reset 시키고 
			// BombComponent의 m_IsClearBomb를 true로 만들어준다
			if (BombComp->IsClearBomb())
			{
				BombComp->SetClearBomb(false);

			}

			else if(!BombComp->IsBeforeLift())
			{
				BombComp->ShootBomb(Dir);
			}

			m_InRestoreCam = true;

			if (BombComp->IsBeforeLift())
			{
				m_InRestoreCam = false;
			}

			m_Owner->SetCurrentNode(this);

			m_IsEnd = false;
		}

		m_CallStart = true;

		return NodeResult::Node_True;
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
