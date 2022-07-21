
#include "Lockstone3TriggerBoxAction.h"
#include "Component/ColliderBox3D.h"
#include "../EyeLaserComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/StaticMeshComponent.h"

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
	CGameObject* Laser = CSceneManager::GetInst()->GetScene()->FindObject("LaserGenerator");

	if (Laser)
	{
		CEyeLaserComponent* EyeLaserComp = Laser->FindComponentFromType<CEyeLaserComponent>();
		CColliderBox3D* TriggerBox = m_Object->FindComponentFromType<CColliderBox3D>();

		if (TriggerBox)
			TriggerBox->Enable(false);

		if (EyeLaserComp)
		{
			EyeLaserComp->AddTriggerCount();
			CStaticMeshComponent* Comp = m_Object->FindComponentFromType<CStaticMeshComponent>();
			Comp->SetEmissiveColor(Vector4(0.f, 0.f, 0.f, 1.f));
		}


		//m_Object->SetHit(false);
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
