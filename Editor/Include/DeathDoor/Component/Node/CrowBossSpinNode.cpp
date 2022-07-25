
#include "CrowBossSpinNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../CrowBossDataComponent.h"
#include "../MonsterNavAgent.h"
#include "Scene/Scene.h"

CCrowBossSpinNode::CCrowBossSpinNode()	:
	m_AccRotation(0.f),
	m_Degree(0.f),
	m_AccSlidingTime(0.f)
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
	Vector3 OriginDir = PlayerOriginPos - MyOriginPos;
	OriginDir.Normalize();

	Vector3 MyCurrentPos = m_Object->GetWorldPos();
	Vector3 PlayerCurrentPos = m_Object->GetScene()->GetPlayerObject()->GetWorldPos();
	
	Vector3 CurrentDir = PlayerCurrentPos - MyCurrentPos;
	CurrentDir.Normalize();

	Vector3 Cross = OriginDir.Cross(CurrentDir);
	bool CW = true;

	if (Cross.y < 0)
	{
		CW = false;
	}

	if (CW)
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("RightSpin");
	else
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("LeftSpin");

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
	Vector3 Dir = PlayerOriginPos - MyOriginPos;
	Dir.Normalize();

	if (m_AccRotation >= 180.f)
	{
		if (m_AccSlidingTime > 1.2f)
		{
			m_AccRotation = 0.f;
			m_AccSlidingTime = 0.f;
			m_Owner->SetCurrentNode(nullptr);
			m_CallStart = false;
			return NodeResult::Node_True;
		}

		else
		{
			m_AccSlidingTime += DeltaTime;
			m_Object->AddWorldPos(Dir * 6.f * DeltaTime);
		}

		return NodeResult::Node_True;
	}

	m_Object->AddWorldPos(Dir * 6.f * DeltaTime);

	if (m_Degree < 0.f)
	{
		m_Object->AddWorldRotationY(-180.f * DeltaTime);
		m_AccRotation += -180.f * DeltaTime;
	}

	else
	{
		m_Object->AddWorldRotationY(180.f * DeltaTime);
		m_AccRotation += 180.f * DeltaTime;
	}
	

	return NodeResult::Node_True;
}

NodeResult CCrowBossSpinNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}
