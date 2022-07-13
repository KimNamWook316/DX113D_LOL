
#include "Lockstone3TriggerBoxAction.h"
#include "Component/ColliderBox3D.h"
#include "../EyeLaserComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CLockstone3TriggerBoxAction::CLockstone3TriggerBoxAction()
{
	SetTypeID(typeid(CLockstone3TriggerBoxAction).hash_code());
}

CLockstone3TriggerBoxAction::CLockstone3TriggerBoxAction(const CLockstone3TriggerBoxAction& Node) :
	CActionNode(Node)
{
}

CLockstone3TriggerBoxAction::~CLockstone3TriggerBoxAction()
{
}

NodeResult CLockstone3TriggerBoxAction::OnStart(float DeltaTime)
{
	CGameObject* EyeLaser = CSceneManager::GetInst()->GetScene()->FindObject("EyeLaserTop");

	if (EyeLaser)
	{
		CColliderBox3D* TriggerBox = m_Object->FindComponentFromType<CColliderBox3D>();

		if (TriggerBox)
			TriggerBox->Enable(false);

		CGameObject* EyeLaser = CSceneManager::GetInst()->GetScene()->FindObject("EyeLaserTop");
		CEyeLaserComponent* EyeLaserComp = EyeLaser->FindComponentFromType<CEyeLaserComponent>();

		if (EyeLaserComp)
		{
			EyeLaserComp->AddTriggerCount();
		}


		m_Object->SetHit(false);
	}

	return NodeResult::Node_True;
}

NodeResult CLockstone3TriggerBoxAction::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CLockstone3TriggerBoxAction::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CLockstone3TriggerBoxAction::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
