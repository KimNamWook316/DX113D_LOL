#include "BossBettyDataComponent.h"
#include "GameBehaviorTree.h"
#include "GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "BossBettyDataComponent.h"

CBossBettyDataComponent::CBossBettyDataComponent() :
	m_ThrowFarAttackEnable(false),
	m_FarAttackType(BossBettyFarAttackType::Spin),
    m_ChangeDirLimitAngle(30.f)
{
}

CBossBettyDataComponent::CBossBettyDataComponent(const CBossBettyDataComponent& com)
{
}

CBossBettyDataComponent::~CBossBettyDataComponent()
{
}

void CBossBettyDataComponent::Start()
{
    // m_BossBettySpinCollider 를 Object 의 Component List 에 추가한다.
    // - 그리고 Spin 중간에, Collide 시 Spin Collider Animation 으로 바꾸는 Callback도 세팅한다.
    // - 처음에는 비활성화 시킨다.
    m_BossBettySpinCollider = m_Object->CreateComponent<CColliderBox3D>("BossBettySpinCollide");
    m_BossBettySpinCollider->Enable(false);
    m_BossBettySpinCollider->AddCollisionCallback(Collision_State::Begin, 
        this, &CBossBettyDataComponent::OnChangeFromSpinToSpinCollideWhenCollide);

    // 근거리 사정 거리 판별 Square Pos 위치 만들기 
    //  0: 왼쪽 하단, 1 : 왼쪽 상단, 2 : 오른쪽 상단, 3 : 오른쪽 하단
    const Vector3& ObjectWorldScale = m_Object->GetRootComponent()->GetWorldScale();

    m_PunchLeftSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z * 0.5f);
    m_PunchLeftSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z);
    m_PunchLeftSquarePos[2] = Vector3(0.f, 0.f, ObjectWorldScale.z);
    m_PunchLeftSquarePos[3] = Vector3(0.f, 0.f, ObjectWorldScale.z * 0.5f);

    m_PunchRightSquarePos[0] = Vector3(0.f, 0.f, ObjectWorldScale.z * 0.5f);
    m_PunchRightSquarePos[1] = Vector3(0.f, 0.f, ObjectWorldScale.z);
    m_PunchRightSquarePos[2] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z);
    m_PunchRightSquarePos[3] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z * 0.5f);

    m_SlashLeftSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * -1.f);
    m_SlashLeftSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_SlashLeftSquarePos[2] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z);
    m_SlashLeftSquarePos[3] = Vector3(ObjectWorldScale.x * -1 * 0.5f, 0.f, ObjectWorldScale.z * -1.f);

    m_SlashRightSquarePos[0] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z * -1.f);
    m_SlashRightSquarePos[1] = Vector3(ObjectWorldScale.x * 0.5f, 0.f, ObjectWorldScale.z);
    m_SlashRightSquarePos[2] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_SlashRightSquarePos[3] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * -1.f);

    m_TwoSideFrontSquarePos[0] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z);
    m_TwoSideFrontSquarePos[1] = Vector3(ObjectWorldScale.x * -1 * 1.5f, 0.f, ObjectWorldScale.z * 1.5f);
    m_TwoSideFrontSquarePos[2] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z * 1.5f);
    m_TwoSideFrontSquarePos[3] = Vector3(ObjectWorldScale.x * 1 * 1.5f, 0.f, ObjectWorldScale.z);

    m_RelativePunchRightPos = (m_PunchRightSquarePos[0] + m_PunchRightSquarePos[2]) / 2.f;
    m_RelativePunchLeftPos = (m_PunchLeftSquarePos[0] + m_PunchLeftSquarePos[2]) / 2.f;

    m_RelativeSlashRightPos = (m_SlashLeftSquarePos[0] + m_SlashLeftSquarePos[2]) / 2.f;
    m_RelativeSlashLeftPos = (m_SlashRightSquarePos[0] + m_SlashRightSquarePos[2]) / 2.f;
}

void CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect()
{
	// 양쪽에 
	// 1) 충돌체 활성화
	// 2) Particle 제작
}

void CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect()
{
    // m_RelativePunchRightPos
}

void CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect()
{
    // m_RelativePunchLeftPos
}

void CBossBettyDataComponent::OnBossBettyRoarEffect()
{
	// 1. 앞으로, 원형 형태로 뻗어나가는 형태의 Particle 만들어내기 
}

// 투사체를 던진 이후, 투사체가 바닥, 벽 등에 충돌 이후, 터질 때 효과 
void CBossBettyDataComponent::OnExplodeBettyThrowBallCallback()
{
    // 자기 위치를 기준으로 20개의 위치를 세팅한다.
    // 서서히 Delay 를 주면서 내려오게 할 것이다.
}

void CBossBettyDataComponent::OnChangeFromSpinToSpinCollideWhenCollide(const CollisionResult& Result)
{
    CAnimationSequenceInstance* AnimInst =  dynamic_cast<CAnimationMeshComponent*>(m_Object->GetRootComponent())->GetAnimationInstance();

    if (AnimInst->GetCurrentAnimation()->GetName() == "Spin")
    {
        AnimInst->ChangeAnimation("SpinCollide");
    }
}

void CBossBettyDataComponent::OnBossBettyDisableSpinCollider()
{
    m_BossBettySpinCollider->Enable(false);
}

void CBossBettyDataComponent::OnBossBettyEnableSpinCollider()
{
    m_BossBettySpinCollider->Enable(true);
}
