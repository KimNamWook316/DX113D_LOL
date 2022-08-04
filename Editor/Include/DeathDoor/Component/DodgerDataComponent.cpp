#include "DodgerDataComponent.h"
#include "../DataManager.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "PlayerDataComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/AnimationMeshComponent.h"

CDodgerDataComponent::CDodgerDataComponent()	:
	m_AttackCount(0),
	m_DashReady(false),
	m_DashRotaiting(false)
{
	SetTypeID<CDodgerDataComponent>();
}

CDodgerDataComponent::CDodgerDataComponent(const CDodgerDataComponent& com)	:
	CMonsterDataComponent(com)
{
	m_AttackCount = 0;
	m_DashReady = false;
}

CDodgerDataComponent::~CDodgerDataComponent()
{
}

void CDodgerDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("Dodger");

	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();

	AnimInst->AddNotify("AttackReady", "OnAttackReady", 0, (CMonsterDataComponent*)this, &CMonsterDataComponent::OnEnableLeftLookPlayer);
	AnimInst->SetEndFunction("AttackReady", this, &CDodgerDataComponent::OnAttack1ReadyEnd);
	AnimInst->AddNotify("AttackReady2", "OnAttackReady2", 0, (CMonsterDataComponent*)this, &CMonsterDataComponent::OnEnableLeftLookPlayer);
	AnimInst->SetEndFunction("AttackReady2", this, &CDodgerDataComponent::OnAttack1ReadyEnd);

	AnimInst->AddNotify("Attack", "OnAttackMove", 0, this, &CDodgerDataComponent::OnAttackMoveStart);
	AnimInst->AddNotify("Attack", "OnAttackMove1End", 5, this, &CDodgerDataComponent::OnAttackMove1End);
	AnimInst->AddNotify("Attack2", "OnAttackMove", 0, this, &CDodgerDataComponent::OnAttackMoveStart);
	AnimInst->AddNotify("Attack2", "OnAttackMove2End", 5, this, &CDodgerDataComponent::OnAttackMove2End);

	AnimInst->AddNotify("Dash", "OnDashReady", 0, this, &CDodgerDataComponent::OnDashReady);
	AnimInst->AddNotify("Dash", "OnDashStart", 8, this, &CDodgerDataComponent::OnDashStart);
	AnimInst->SetEndFunction("Dash", this, &CDodgerDataComponent::OnDashEnd);
}

void CDodgerDataComponent::Update(float DeltaTime)
{
	CMonsterDataComponent::Update(DeltaTime);

	if (m_DashRotaiting)
	{
		m_Object->AddWorldRotationY(m_CurRotSpeed * DeltaTime);
	}
}

void CDodgerDataComponent::OnAttack1ReadyEnd()
{
	OnDisableLookPlayer();

	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();
	AnimInst->ChangeAnimation("Attack");
}

void CDodgerDataComponent::OnAttack2ReadyEnd()
{
	OnDisableLookPlayer();

	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();
	AnimInst->ChangeAnimation("Attack2");
}

void CDodgerDataComponent::OnAttackMoveStart()
{
	m_CurMoveSpeed = m_Data.MoveSpeed / 2.f;

	// 공격 시점의 플레이어 위치를 기억해서, 대쉬 방향을 판별
	CGameObject* Player = m_Scene->GetPlayerObject();

	if (Player)
	{
		m_PrevPlayerPos = Player->GetWorldPos();
	}

	OnEnableMoveZ();
}

void CDodgerDataComponent::OnAttackMove1End()
{
	m_CurMoveSpeed = 0.f;

	OnDisableMoveZ();
}

void CDodgerDataComponent::OnAttackMove2End()
{
	m_CurMoveSpeed = 0.f;

	OnDisableMoveZ();

	m_DashReady = true;
}

void CDodgerDataComponent::OnDashReady()
{
	// 공격 시작 시점의 플레이어 위치와 현재 플레이어 위치를 기준으로
	// 플레이어가 이동한 동선과 똑같은 동선으로 대쉬함
	CGameObject* Player = m_Scene->GetPlayerObject();

	if (Player)
	{
		Vector3 PlayerPos = Player->GetWorldPos();

		Vector3 DashDir = PlayerPos - m_PrevPlayerPos;
		Vector3 MyPos = m_Object->GetWorldPos();

		m_DashDest = MyPos + DashDir;

		// 내가 바라보는 방향과 현재 대쉬해야 할 방향의 각도를 통해, 애니메이션 프레임동안 회전 속도를 결정함
		Vector3 MyLookDir = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
		float Angle = m_DashDest.Angle(MyLookDir);

		CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();
		float FrameTime = AnimInst->GetAnimationFrameTime("Dash");

		m_CurRotSpeed = Angle / FrameTime;

		m_DashRotaiting = true;
	}
}

void CDodgerDataComponent::OnDashStart()
{
	m_HitBox->Enable(false);

	m_CurMoveSpeed = m_Data.MoveSpeed * 3.f;
	m_MoveZ = true;
	m_Dash = true;

	m_DashRotaiting = false;
	m_CurRotSpeed = 0.f;
}

void CDodgerDataComponent::OnDashEnd()
{
	m_HitBox->Enable(true);

	m_CurMoveSpeed = 0.f;
	m_MoveZ = false;

	m_CurRotSpeed = 0.f;

	m_DashReady = false;
	m_Dash = false;

	SetCurrentNodeNull();
}

