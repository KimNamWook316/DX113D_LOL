
#include "EyeLaserTracking.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CEyeLaserTracking::CEyeLaserTracking()	:
	m_Player(nullptr)
{
}

CEyeLaserTracking::CEyeLaserTracking(const CEyeLaserTracking& Node)	:
	CActionNode(Node)
{
}

CEyeLaserTracking::~CEyeLaserTracking()
{
}

NodeResult CEyeLaserTracking::OnStart(float DeltaTime)
{
	m_CallStart = true;

	m_Player = CSceneManager::GetInst()->GetScene()->GetPlayerObject();

	return NodeResult::Node_True;
}

NodeResult CEyeLaserTracking::OnUpdate(float DeltaTime)
{
	// 원래 레이저가 바라보는 방향은 플레이어가 입장하는 문을 바라보는 방향
	Vector3 LaserRot = m_Object->GetWorldRot();

	Vector3 PlayerPos = m_Player->GetWorldPos();
	Vector3 LaserPos = m_Object->GetWorldPos();

	Vector3 Dir = PlayerPos - LaserPos;
	Dir.Normalize();

	float Rad = Dir.Dot(LaserRot);
	float Degree = RadianToDegree(acosf(Rad));

	Vector3 CrossVector = Dir.Cross(LaserRot);

	// 
	if (CrossVector.y < 0.f)
	{

	}

	else
	{

	}


	return NodeResult::Node_True;
}

NodeResult CEyeLaserTracking::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CEyeLaserTracking::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}
