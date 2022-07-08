
#include "PlayerNormalAttackCheckCollider.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "GameObject/GameObject.h"
#include "Input.h"

CPlayerNormalAttackCheckCollider::CPlayerNormalAttackCheckCollider()
{
	SetTypeID<CPlayerNormalAttackCheckCollider>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Box3D;

	AddCollisionCallback<CPlayerNormalAttackCheckCollider>(Collision_State::Stay, this, &CPlayerNormalAttackCheckCollider::AttackSuccess);

	SetCollisionProfile("PlayerAttack");
}

CPlayerNormalAttackCheckCollider::CPlayerNormalAttackCheckCollider(const CPlayerNormalAttackCheckCollider& com) :
	CColliderSphere(com)
{
}

CPlayerNormalAttackCheckCollider::~CPlayerNormalAttackCheckCollider()
{
}

void CPlayerNormalAttackCheckCollider::Start()
{
	CColliderSphere::Start();
}

bool CPlayerNormalAttackCheckCollider::Init()
{
	if (!CColliderSphere::Init())
		return false;

	return true;
}

void CPlayerNormalAttackCheckCollider::Update(float DeltaTime)
{
	CColliderSphere::Update(DeltaTime);
}

void CPlayerNormalAttackCheckCollider::PostUpdate(float DeltaTime)
{
	CColliderSphere::PostUpdate(DeltaTime);
}

void CPlayerNormalAttackCheckCollider::PrevRender()
{
	CColliderSphere::PrevRender();
}

void CPlayerNormalAttackCheckCollider::Render()
{
	CColliderSphere::Render();
}

void CPlayerNormalAttackCheckCollider::PostRender()
{
	CColliderSphere::PostRender();
}

CPlayerNormalAttackCheckCollider* CPlayerNormalAttackCheckCollider::Clone()
{
	return new CPlayerNormalAttackCheckCollider(*this);
}

bool CPlayerNormalAttackCheckCollider::Save(FILE* File)
{
	return CColliderSphere::Save(File);
}

bool CPlayerNormalAttackCheckCollider::Load(FILE* File)
{
	return CColliderSphere::Load(File);
}

bool CPlayerNormalAttackCheckCollider::Collision(CColliderComponent* Dest)
{
	return CColliderSphere::Collision(Dest);
}

bool CPlayerNormalAttackCheckCollider::CollisionMouse(const Vector2& MousePos)
{
	return false;
}

void CPlayerNormalAttackCheckCollider::AttackSuccess(const CollisionResult& Result)
{
	CAnimationMeshComponent* Comp = m_Object->FindComponentFromType<CAnimationMeshComponent>();

	if (!Comp)
		return;

	CAnimationSequenceInstance* Instance = Comp->GetAnimationInstance();

	if (!Instance)
		return;

	if (CInput::GetInst()->GetMouseLButtonClick())
	{
		auto iter = m_PrevCollisionList.begin();
		auto iterEnd = m_PrevCollisionList.end();

		for (; iter != iterEnd; ++iter)
		{
			Vector3 Vec1 = ((*iter)->GetWorldPos() - m_Object->GetWorldPos());
			Vec1.Normalize();
			Vector3 Vec2 = m_Object->GetMoveDir();

			if(Vec1.Dot(Vec2) > 0.f)
				(*iter)->GetGameObject()->SetHit(true);
		}
	}

}
