
#include "CrowBossDirectionCheck.h"
#include "../GameStateComponent.h"
#include "../GameBehaviorTree.h"
#include "../CrowBossDataComponent.h"
#include "Scene/Scene.h"
#include "../MonsterNavAgent.h"

CCrowBossDirectionCheck::CCrowBossDirectionCheck()
{
	SetTypeID(typeid(CCrowBossDirectionCheck).hash_code());
}

CCrowBossDirectionCheck::CCrowBossDirectionCheck(const CCrowBossDirectionCheck& Node) :
	CConditionNode(Node)
{
}

CCrowBossDirectionCheck::~CCrowBossDirectionCheck()
{
}

NodeResult CCrowBossDirectionCheck::OnStart(float DeltaTime)
{
	//CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	//CMonsterNavAgent* Agent = Data->GetMonsterNavAgent();
	//CGameObject* Player = m_Object->GetScene()->GetPlayerObject();
	//CNavAgent* PlayerNavAgent = Player->FindObjectComponentFromType<CNavAgent>();

	//Vector3 MyFaceDir = Agent->GetCurrentFaceDir();
	//Vector3 PlayerFaceDir = PlayerNavAgent->GetCurrentFaceDir();

	//float DotProduct = MyFaceDir.Dot(PlayerFaceDir);
	//float Degree = DegreeToRadian(acosf(DotProduct));

	//if (Degree > -5.f && Degree < 5.f)
	//{
	//	Data->SetSpinAmount(0.f);
	//	return NodeResult::Node_True;
	//}
	//else
	//{
	//	Data->SetSpinAmount(Degree);
	//	return NodeResult::Node_False;
	//}

	return NodeResult::Node_True;
}

NodeResult CCrowBossDirectionCheck::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CCrowBossDirectionCheck::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}
