#include "KnightDataComponent.h"
#include "../DataManager.h"
#include "Component/ColliderBox3D.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CKnightDataComponent::CKnightDataComponent()	:
	m_JumpAttackRange(0.f),
	m_MeleeAttackCount(0)
{
	SetTypeID<CKnightDataComponent>();
}

CKnightDataComponent::CKnightDataComponent(const CKnightDataComponent& com)	:
	CMonsterDataComponent(com)
{
	m_JumpAttackRange = com.m_JumpAttackRange;
	m_MeleeAttackCount = 0;
}

CKnightDataComponent::~CKnightDataComponent()
{
}

void CKnightDataComponent::Start()
{
	m_Data = CDataManager::GetInst()->GetObjectData("BossKnight");
	m_MeleeAttackCollider = (CColliderBox3D*)m_Object->FindComponent("MeleeAttackCollider");
}

void CKnightDataComponent::OnInActiveMeleeAttackCollider()
{
	// TODO : Boss Knight - Particle, Cam Shake

	m_MeleeAttackCollider->Enable(false);
}

void CKnightDataComponent::OnHitMeleeAttack(const CollisionResult& Result)
{
	// TODO : Boss Knight - Player Hit 처리
}

void CKnightDataComponent::OnTracePlayerOnlyRotation(float DeltaTime)
{
	// 플레이어 방향으로 회전 범위만큼 회전
	float AnglePlayer = GetAnglePlayer();

	CGameObject* MyObj = m_Object;

	if (abs(AnglePlayer) < m_Data.RotateSpeedPerSec)
	{
		MyObj->AddWorldRotationY(AnglePlayer * DeltaTime);
	}
	else
	{
		MyObj->AddWorldRotationY(m_Data.RotateSpeedPerSec * DeltaTime);
	}
}

void CKnightDataComponent::OnTracePlayerRotationMove(float DeltaTime)
{
	// 플레이어 방향으로 회전 범위만큼 회전
	float AnglePlayer = GetAnglePlayer();

	CGameObject* MyObj = m_Object;

	if (abs(AnglePlayer) < m_Data.RotateSpeedPerSec)
	{
		MyObj->AddWorldRotationY(AnglePlayer * DeltaTime);
	}
	else
	{
		MyObj->AddWorldRotationY(m_Data.RotateSpeedPerSec * DeltaTime);
	}

	// Walk Speed 두배로 이동
	MyObj->AddWorldPosByLocalAxis(AXIS::AXIS_Z, m_Data.MoveSpeed * 2.f);

	// 추적 종료 시 방향 저장
	Vector3 FinalZAxis = ToPlayer();
	m_FinalTraceZAxis = FinalZAxis;
	m_FinalTraceZAxis.Normalize();
}

// 공격 범위, 플레이어와의 각도로 공격을 이어갈지, 멈출지 판단하는 함수
void CKnightDataComponent::OnEndAnimJudgeContinueAttack()
{
	// 애니메이션 끝난 이후 플레이어가 아직 공격 범위 내에 있을 경우
	if (IsPlayerInMeleeAttackRange())
	{
		// 공격 이어가는 상태로
		SetContinueAttack(true);

		// 다음 공격 방향 판단
		float AnglePlayer = GetAnglePlayer();

		// 플레이어와의 각도가 일정 이하인 경우
		if (abs(AnglePlayer) <= 90.f)
		{
			SetAttackRot(Knight_Attack_Rot_Type::Front);
			return;
		}

		CGameObject* Player = m_Object->GetScene()->GetPlayerObject();

		if (!Player)
		{
			return;
		}

		Vector3 PlayerZAxis = Player->GetWorldAxis(AXIS::AXIS_Z);
		Vector3 MyZAxis = m_Object->GetWorldAxis(AXIS::AXIS_Z);

		// 플레이어가 시계방향에 있는지, 반시계방향에 있는지 판단
		Vector3 Cross = PlayerZAxis.Cross(MyZAxis);

		if (Cross.y > 0.f)
		{
			SetAttackRot(Knight_Attack_Rot_Type::CW);
		}
		else
		{
			SetAttackRot(Knight_Attack_Rot_Type::CCW);
		}
	}
	else
	{
		// 후딜레이 상태로 전환
		SetPostAttckDelaying(true);
		SetContinueAttack(false);
	}

}

void CKnightDataComponent::OnActiveMeleeAttackCollider()
{
	m_MeleeAttackCollider->Enable(true);
}
