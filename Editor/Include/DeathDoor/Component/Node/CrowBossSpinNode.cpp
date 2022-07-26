
#include "CrowBossSpinNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../CrowBossDataComponent.h"
#include "../MonsterNavAgent.h"
#include "Scene/Scene.h"


CCrowBossSpinNode::CCrowBossSpinNode()	:
	m_AccRotation(0.f),
	m_SpinDegree(0.f),
	m_AccSlidingTime(0.f),
	m_RotateCW(true)
{
	SetTypeID(typeid(CCrowBossSpinNode).hash_code());
}

CCrowBossSpinNode::CCrowBossSpinNode(const CCrowBossSpinNode& Node) :
	CActionNode(Node)
{
}

CCrowBossSpinNode::~CCrowBossSpinNode()
{
}

void CCrowBossSpinNode::Init()
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();
}

NodeResult CCrowBossSpinNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	Vector3 MyOriginPos = Data->GetMyOriginPos();
	Vector3 PlayerOriginPos = Data->GetPlayerOriginPos();
	CNavAgent* Agent = Data->GetMonsterNavAgent();
	Vector3 FaceDir = Agent->GetCurrentFaceDir();
	Vector3 OriginDir = PlayerOriginPos - MyOriginPos;

	OriginDir.Normalize();

	Vector3 MyCurrentPos = m_Object->GetWorldPos();
	Vector3 PlayerCurrentPos = m_Object->GetScene()->GetPlayerObject()->GetWorldPos();
	
	Vector3 CurrentDir = PlayerCurrentPos - MyCurrentPos;
	CurrentDir.Normalize();

	Vector3 Cross = FaceDir.Cross(CurrentDir);

	m_RotateCW = true;
	m_SpinDegree = 0.f;
	if (Cross.y < 0)
	{
		m_RotateCW = false;
	}

	if (m_RotateCW)
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("RightSpin");
	else
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("LeftSpin");

	Vector3 SpinDir = MyOriginPos - PlayerCurrentPos;
	SpinDir.Normalize();

	float DotProduct = FaceDir.Dot(SpinDir);

	if (DotProduct < 0.995f && DotProduct > -0.995f)
	{
		m_SpinDegree = RadianToDegree(acosf(DotProduct));
	}

	else
	{
		if (m_RotateCW)
			m_SpinDegree = 180.f;
		else
			m_SpinDegree = -180.f;
	}

	return NodeResult::Node_True;
}

NodeResult CCrowBossSpinNode::OnUpdate(float DeltaTime)
{
	m_Owner->SetCurrentNode(this);

	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	//CMonsterNavAgent* Agent = Data->GetMonsterNavAgent();

	//CGameObject* Player = m_Object->GetScene()->GetPlayerObject();
	//CNavAgent* PlayerNavAgent = Player->FindObjectComponentFromType<CNavAgent>();

	//Vector3 MyFaceDir = Agent->GetCurrentFaceDir();
	//Vector3 PlayerFaceDir = PlayerNavAgent->GetCurrentFaceDir();

	//float DotProduct = MyFaceDir.Dot(PlayerFaceDir);
	//float Degree = DegreeToRadian(acosf(DotProduct));


	Vector3 MyOriginPos = Data->GetMyOriginPos();
	Vector3 PlayerOriginPos = Data->GetPlayerOriginPos();
	Vector3 OriginDir = PlayerOriginPos - MyOriginPos;
	OriginDir.Normalize();

	if (abs(m_AccRotation - m_SpinDegree) < 3.f)
	{
		if (m_AccSlidingTime > 1.1f)
		{
			m_AccRotation = 0.f;
			m_AccSlidingTime = 0.f;
			m_Owner->SetCurrentNode(nullptr);
			m_CallStart = false;

			m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("Run");

			return NodeResult::Node_True;
		}

		else
		{
			m_AccSlidingTime += DeltaTime;
			m_Object->AddWorldPos(OriginDir * 6.f * DeltaTime);
		}

		return NodeResult::Node_True;
	}

	m_Object->AddWorldPos(OriginDir * 6.f * DeltaTime);

	CNavAgent* Agent = Data->GetMonsterNavAgent();
	Vector3 FaceDir = Agent->GetCurrentFaceDir();

	if (m_SpinDegree > 0.f)
	{
		m_Object->AddWorldRotationY(180.f * DeltaTime);
		m_AccRotation += 180.f * DeltaTime;
	}

	else
	{
		m_Object->AddWorldRotationY(-180.f * DeltaTime);
		m_AccRotation += -180.f * DeltaTime;
	}
	

	return NodeResult::Node_True;
}

NodeResult CCrowBossSpinNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
