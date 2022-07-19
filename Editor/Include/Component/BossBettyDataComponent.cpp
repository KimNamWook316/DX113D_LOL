#include "BossBettyDataComponent.h"

CBossBettyDataComponent::CBossBettyDataComponent() :
	m_ThrowFarAttackEnable(false),
	m_FarAttackType(BossBettyFarAttackType::Spin)
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
}

void CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect()
{
	// 양쪽에 
	// 1) 충돌체 활성화
	// 2) Particle 제작
}

void CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect()
{
}

void CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect()
{
}

void CBossBettyDataComponent::OnBossBettyRoarEffect()
{
	// 앞으로, 원형 형태로 뻗어나가는 형태의 Particle 만들어내기 
}
