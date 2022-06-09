
#include "MovePickingNode.h"
#include "../../Input.h"
#include "../CameraComponent.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/Navigation3DManager.h"
#include "../BehaviorTree.h"
#include "../AnimationMeshComponent.h"
#include "../../Collision/Collision.h"
#include "../ColliderComponent.h"
#include "../ColliderBox3D.h"
#include "../ColliderSphere.h"

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
		std::vector<CColliderComponent*> vecTestCollider;
		std::vector<int>		vecTestIndex;
		CSceneCollision* Collision = CSceneManager::GetInst()->GetScene()->GetCollision();

		int CountX = Collision->GetSectionCountX();
		int CountZ = Collision->GetSectionCountZ();

		int Idx = Collision->GetSectionIndex(TargetPos);
		int IdxX = Collision->GetSectionIndexX(TargetPos);
		int IdxZ = Collision->GetSectionIndexZ(TargetPos);

		if (IdxX == 0 && IdxZ == 0)
		{
			vecTestIndex.push_back(Idx);
		}

		else if (IdxX == 0)
		{
			vecTestIndex.push_back(Idx - CountX);
			vecTestIndex.push_back(Idx);
		}

		else if (IdxZ == 0)
		{
			vecTestIndex.push_back(Idx - 1);
			vecTestIndex.push_back(Idx);
		}

		//else if (IdxX == Collision->GetSectionCountX() - 1)
		//{
		//	vecTestIndex.push_back(Idx - 1);
		//	vecTestIndex.push_back(Idx);
		//}

		//else if (Idx == Collision->GetSectionCountZ() - 1)
		//{
		//	vecTestIndex.push_back(Idx - CountX);
		//	vecTestIndex.push_back(Idx);
		//}

		else
		{
			vecTestIndex.push_back(Idx - CountX - 1);
			vecTestIndex.push_back(Idx);
		}

		size_t Count = vecTestIndex.size();
		bool Same = false;

		for (size_t i = 0; i < Count; ++i)
		{
			std::vector<CColliderComponent*> vecCollider = Collision->GetSectionCollider(vecTestIndex[i]);
			size_t Count2 = vecCollider.size();

			for (size_t j = 0; j < Count2; ++j)
			{
				// 이미 있는 Collider는 또 넣으면 안된다
				size_t TestCount = vecTestCollider.size();
				for (size_t k = 0; k < TestCount; ++k)
				{
					if (vecTestCollider[k] == vecCollider[j])
					{
						Same = true;
						break;
					}
				}

				if (!Same)
					vecTestCollider.push_back(vecCollider[j]);

				Same = false;
			}
		}

		Count = vecTestCollider.size();

		for (size_t i = 0; i < Count; ++i)
		{
			// 피킹한 충돌체가 내가 가진 충돌체면 검사X
			if (m_Object->CheckSceneComponent(vecTestCollider[i]))
				continue;

			if (vecTestCollider[i]->CheckType<CColliderBox3D>())
			{
				Vector3 HitPoint;
				CCameraComponent* CurrentCamera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
				Ray ray = CInput::GetInst()->GetRay(CurrentCamera->GetViewMatrix());
				bool Collision = CCollision::CollisionRayToBox3D(HitPoint, ray, ((CColliderBox3D*)vecTestCollider[i])->GetInfo());

				// 피킹했는데 지형이 아닌 특정 오브젝트랑 충돌하는 경우
				if (Collision)
				{
					m_Object->SetAttackTarget(vecTestCollider[i]->GetGameObject());
					m_Object->ClearPath();
					m_Object->AddPath(vecTestCollider[i]->GetGameObject()->GetWorldPos());
					CAnimationSequenceInstance* Instance = m_Owner->GetAnimationMeshComp()->GetAnimationInstance();

					Instance->ChangeAnimation(m_Object->GetName() + "_Run");

					m_IsEnd = true;

					return NodeResult::Node_True;
				}
			}

			else if (vecTestCollider[i]->CheckType<CColliderSphere>())
			{
				Vector3 HitPoint;
				CCameraComponent* CurrentCamera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();
				Ray ray = CInput::GetInst()->GetRay(CurrentCamera->GetViewMatrix());
				bool Collision = CCollision::CollisionRayToSphere(HitPoint, ray, ((CColliderSphere*)vecTestCollider[i])->GetInfo());

				// 피킹했는데 지형이 아닌 특정 오브젝트랑 충돌하는 경우
				if (Collision)
				{
					m_Object->SetAttackTarget(vecTestCollider[i]->GetGameObject());
					m_Object->ClearPath();
					m_Object->AddPath(vecTestCollider[i]->GetGameObject()->GetWorldPos());
					CAnimationSequenceInstance* Instance = m_Owner->GetAnimationMeshComp()->GetAnimationInstance();

					Instance->ChangeAnimation(m_Object->GetName() + "_Run");

					m_IsEnd = true;

					return NodeResult::Node_True;
				}
			}
		}

		// 특정 오브젝트가 아닌 지형을 피킹한 경우
		m_Object->SetAttackTarget(nullptr);
		m_Object->ClearPath();
		m_Object->AddPath(TargetPos);
		CAnimationSequenceInstance* Instance = m_Owner->GetAnimationMeshComp()->GetAnimationInstance();

		Instance->ChangeAnimation(m_Object->GetName() + "_Run");

		m_IsEnd = true;
	}

	return NodeResult::Node_True;
}

NodeResult CMovePickingNode::OnUpdate(float DeltaTime)
{
	// 만약 NavAgent의 m_PathList가 비었다면 그때 return false해서 IdleNode로 가도록
	if (!m_Object->IsNavAgentPathListEmpty())
	{
		//m_Object->SetInterruptEnable(false);
		return NodeResult::Node_True;
	}

	else
	{
		//m_Object->SetInterruptEnable(true);
		return NodeResult::Node_False;
	}

	return NodeResult::Node_None;
}

NodeResult CMovePickingNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

bool CMovePickingNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
