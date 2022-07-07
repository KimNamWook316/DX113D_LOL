
#include "PlayerNormalAttackCheckCollider.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "GameObject/GameObject.h"

CPlayerNormalAttackCheckCollider::CPlayerNormalAttackCheckCollider()
{
	SetTypeID<CPlayerNormalAttackCheckCollider>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Box3D;

	AddCollisionCallback<CPlayerNormalAttackCheckCollider>(Collision_State::Begin, this, &CPlayerNormalAttackCheckCollider::AttackSuccess);

	SetCollisionProfile("PlayerAttack");
}

CPlayerNormalAttackCheckCollider::CPlayerNormalAttackCheckCollider(const CPlayerNormalAttackCheckCollider& com) :
	CColliderBox3D(com)
{
}

CPlayerNormalAttackCheckCollider::~CPlayerNormalAttackCheckCollider()
{
}

void CPlayerNormalAttackCheckCollider::Start()
{
}

bool CPlayerNormalAttackCheckCollider::Init()
{
	if (!CColliderBox3D::Init())
		return false;

	return true;
}

void CPlayerNormalAttackCheckCollider::Update(float DeltaTime)
{
	CColliderBox3D::Update(DeltaTime);
}

void CPlayerNormalAttackCheckCollider::PostUpdate(float DeltaTime)
{
	CColliderBox3D::PostUpdate(DeltaTime);
}

void CPlayerNormalAttackCheckCollider::PrevRender()
{
	CColliderBox3D::PrevRender();
}

void CPlayerNormalAttackCheckCollider::Render()
{
	CColliderBox3D::Render();
}

void CPlayerNormalAttackCheckCollider::PostRender()
{
	CColliderBox3D::PostRender();
}

CPlayerNormalAttackCheckCollider* CPlayerNormalAttackCheckCollider::Clone()
{
	return new CPlayerNormalAttackCheckCollider(*this);
}

bool CPlayerNormalAttackCheckCollider::Save(FILE* File)
{
	return CColliderBox3D::Save(File);
}

bool CPlayerNormalAttackCheckCollider::Load(FILE* File)
{
	return CColliderBox3D::Load(File);
}

bool CPlayerNormalAttackCheckCollider::Collision(CColliderComponent* Dest)
{
	return CColliderBox3D::Collision(Dest);
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

	if (Instance->CheckCurrentAnimation("Slash_Light_L") || Instance->CheckCurrentAnimation("Slash_Light_R"))
	{
		Result.Dest->GetGameObject()->SetHit(true);
	}

}
