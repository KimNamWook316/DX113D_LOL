
#include "MonsterDataComponent.h"
#include "GameObject/GameObject.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderBox3D.h"
#include "../Component/GameStateComponent.h"
#include "MonsterNavAgent.h"
#include "Scene/Scene.h"
#include "../Component/PlayerDataComponent.h"
#include "Component/PaperBurnComponent.h"
#include "../DDUtil.h"
#include "PlayerNormalAttackCheckCollider.h"
#include "../Scene/DDSceneMode.h"

CMonsterDataComponent::CMonsterDataComponent() :
	m_AnimMesh(nullptr),
	m_HitBox(nullptr),
	m_HitEffectFlag(0),
	m_HitEffectTimer(0.f),
	m_HitEffectMax(0.15f),
	m_IsCombat(false),
	m_LookPlayer(false),
	m_CurMoveSpeed(0.f),
	m_DeathColorStart(Vector4::Red),
	m_DeathColorEnd(Vector4::White),
	m_LeftLookPlayer(false),
	m_RightLookPlayer(false),
	m_CurRotSpeed(0.f),
	m_AttackCoolDelayTimeMax(0.5f)
{
	SetTypeID<CMonsterDataComponent>();

	m_ComponentType = Component_Type::ObjectComponent;
}

CMonsterDataComponent::CMonsterDataComponent(const CMonsterDataComponent& com)	:
	CObjectDataComponent(com)
{
}

CMonsterDataComponent::~CMonsterDataComponent()
{
}

void CMonsterDataComponent::Start()
{
	m_MonsterNavAgent = m_Object->FindObjectComponentFromType<CMonsterNavAgent>();
	m_State = m_Object->FindObjectComponentFromType<CGameStateComponent>();

	std::vector<CAnimationMeshComponent*> vecAnim;
	std::vector<CColliderBox3D*> vecCollider;

	m_Object->FindAllSceneComponentFromType<CAnimationMeshComponent>(vecAnim);
	m_Object->FindAllSceneComponentFromType<CColliderBox3D>(vecCollider);

	if (vecAnim.size() == 1)
	{
		m_AnimMesh = vecAnim[0];
	}
	else
	{
		size_t Size = vecAnim.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if ("AnimComp" == vecAnim[i]->GetName())
			{
				m_AnimMesh = vecAnim[i];
				break;
			}
		}
	}

	if (vecCollider.size() == 1)
	{
		m_HitBox = vecCollider[0];
	}
	else
	{
		m_HitBox = vecCollider[0];
		size_t Size = vecCollider.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if ("HitBox" == vecCollider[i]->GetName())
			{
				m_HitBox = vecCollider[i];
				break;
			}
		}
	}

	// Hit Effect를 위한 원래 컬러 저장
	if (m_AnimMesh)
	{
		m_MeshMatSize = m_AnimMesh->GetMaterialSlotCount();
		m_vecOriginDiffuse.resize(m_MeshMatSize);
		m_vecOriginAmbient.resize(m_MeshMatSize);
		m_vecOriginSpecluar.resize(m_MeshMatSize);
		m_vecOriginEmissive.resize(m_MeshMatSize);

		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_vecOriginDiffuse[i] = m_AnimMesh->GetMaterial(i)->GetBaseColor();
			m_vecOriginAmbient[i] = m_AnimMesh->GetMaterial(i)->GetAmbientColor();
			m_vecOriginSpecluar[i] = m_AnimMesh->GetMaterial(i)->GetSpecularColor();
			m_vecOriginEmissive[i] = m_AnimMesh->GetMaterial(i)->GetEmissiveColor();
		}
	}

	// Melee Attack Collider CallBack
	m_MeleeAttackCollider = (CColliderBox3D*)m_Object->FindComponent("MeleeAttackCollider");

	if (m_MeleeAttackCollider)
	{
		m_MeleeAttackCollider->AddCollisionCallback(Collision_State::Begin, this, &CMonsterDataComponent::OnHitMeleeAttack);
	}
	
	// PaperBurn 및 Death
	m_PaperBurn = m_Object->FindComponentFromType<CPaperBurnComponent>();
	
	if (m_PaperBurn)
	{
		m_PaperBurn->SetFinishCallback(this, &CMonsterDataComponent::OnDeadPaperBurnEnd);
	}

	// Blood Particle
	m_BloodParticle = (CParticleComponent*)m_Object->FindComponent("BloodParticle");

	if (m_BloodParticle)
	{
		m_BloodParticle->Enable(false);
	}

	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();
	AnimInst->AddNotify("Death", "DeathStart", 0, this, &CMonsterDataComponent::OnDeadAnimStart);
	AnimInst->SetEndFunction("Death", this, &CMonsterDataComponent::OnDeadAnimEnd);

	// CutScene 관련 ( Enter Trigger, CutScene Cam, Collider CallBack)
	m_PlayerEnterZoneTrigger = (CColliderBox3D*)m_Object->FindComponent("PlayerEnterTrigger");
	m_CutSceneCam = m_Object->FindComponentFromType<CCameraComponent>();

	if (m_PlayerEnterZoneTrigger)
	{
		m_PlayerEnterZoneTrigger->AddCollisionCallback(Collision_State::Begin, this, &CMonsterDataComponent::OnPlayerEnterZone);
	}

	// MonsterNavAgent 관련
	m_MonsterNavAgent = m_Object->FindObjectComponentFromType<CMonsterNavAgent>();

	// Player Data를 미리 들고 있는다
	CGameObject* Player = m_Scene->GetPlayerObject();

	if (Player)
	{
		m_PlayerData = Player->FindObjectComponentFromType<CPlayerDataComponent>();
	}
}

void CMonsterDataComponent::Update(float DeltaTime)
{
	CObjectDataComponent::Update(DeltaTime);

	if (m_HitEffectStart)
	{
		ActiveHitEffect(DeltaTime);
	}

	if (m_LookPlayer)
	{
		LookPlayer(DeltaTime);
	}
	else
	{
		if (m_RightLookPlayer)
		{
			// 오른쪽으로 돌아야 하는데 왼쪽으로 도는 설정도 true 라면 오류
			if (m_LeftLookPlayer)
				assert(false);

			RightLookPlayer(DeltaTime);
		}
		else  if (m_LeftLookPlayer)
		{
			// 오른쪽으로 돌아야 하는데 왼쪽으로 도는 설정도 true 라면 오류
			if (m_RightLookPlayer)
				assert(false);

			LeftLookPlayer(DeltaTime);
		}
	}

	if (m_MoveZEnableMaxTime > 0.f)
	{
		m_MoveZ = true;
		m_MoveZEnableMaxTime -= DeltaTime;

		if (m_MoveZEnableMaxTime < 0.f)
		{
			m_MoveZ = false;
			m_MoveZEnableMaxTime = 0.f;
		}
	}

	if (m_MoveZ)
	{
		MoveZ(DeltaTime);
	}
}

void CMonsterDataComponent::PostUpdate(float DeltaTime)
{
	CObjectDataComponent::PostUpdate(DeltaTime);

	if (m_AttackCoolTimeEnable)
	{
		m_AttackCoolDelayTime -= DeltaTime;

		if (m_AttackCoolDelayTime < 0.f)
		{
			m_AttackCoolTimeEnable = false;
		}
	}
}

CMonsterDataComponent* CMonsterDataComponent::Clone()
{
	return new CMonsterDataComponent(*this);
}

void CMonsterDataComponent::LookPlayer(float DeltaTime)
{
	// 플레이어 방향으로 회전 범위만큼 회전
	float AnglePlayer = GetAnglePlayer();

	CGameObject* MyObj = m_Object;

	float RotSpeed = m_Data.RotateSpeedPerSec;

	// 만약 m_CurRotSpeed 를 별도로 세팅한 상태라면
	if (m_CurRotSpeed != 0.f)
	{
		RotSpeed = m_CurRotSpeed;
	}

	// (OBJ) 순간적으로 미세하게 떨리는 오류
	// if (abs(AnglePlayer) < m_Data.RotateSpeedPerSec * DeltaTime)
	if (abs(AnglePlayer) < 3.f)
	{
		// MyObj->AddWorldRotationY(AnglePlayer * DeltaTime);
	}
	else 
	{
		bool IsLeft = IsPlayerLeftBasedInLookDir();

		if (IsLeft)
		{
			MyObj->AddWorldRotationY(RotSpeed * DeltaTime);
		}
		else
		{
			MyObj->AddWorldRotationY(-1.f * RotSpeed * DeltaTime);
		}
	}
}

void CMonsterDataComponent::RightLookPlayer(float DeltaTime)
{
	m_Object->AddWorldRotationY(-1.f * m_Data.RotateSpeedPerSec * DeltaTime);
}

void CMonsterDataComponent::LeftLookPlayer(float DeltaTime)
{
	m_Object->AddWorldRotationY(m_Data.RotateSpeedPerSec * DeltaTime);
}

void CMonsterDataComponent::MoveZ(float DeltaTime)
{
	m_Object->AddWorldPosByLocalAxis(AXIS::AXIS_Z, -m_CurMoveSpeed * DeltaTime);
}

void CMonsterDataComponent::ChangeColorBossDeath(float DeltaTime)
{
	if (m_DeathColorChangeStart)
	{
		m_DeathTimer += DeltaTime;

		// 컬러 선형 보간
		Vector4 Color = CDDUtil::LerpColor(m_DeathColorStart, m_DeathColorEnd, m_DeathTimer, m_DeathColorChangeTimeMax);

		for (size_t i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->SetMetallic(false, 0);

			// Base
			// m_AnimMesh->SetBaseColor(Color, i);

			// Ambient
			// m_AnimMesh->SetAmbientColor(Color, i);

			// Specular
			m_AnimMesh->SetSpecularColor(Vector4::Black, (int)i);

			// Emmisive
			m_AnimMesh->SetEmissiveColor(Color, (int)i);
		}

		if (m_DeathTimer >= m_DeathColorChangeTimeMax)
		{
			m_DeathColorChangeStart = false;
		}
	}
}

void CMonsterDataComponent::OnEndAnimPostAttackDelayOff()
{
	m_PostAttackDelaying = false;

	SetCurrentNodeNull();
}

void CMonsterDataComponent::OnDeadPaperBurnEnd()
{
	// TODO : Monster Death 관련 -> 차후 Object Pool 몬스터에 대한 처리 필요
	m_Object->Destroy();

	CDDSceneMode* SceneMode = dynamic_cast<CDDSceneMode*>(m_Scene->GetSceneMode());

	if (SceneMode)
	{
		SceneMode->OnDieMonster();
	}
}

void CMonsterDataComponent::OnDeadAnimStart()
{
	m_HitBox->Enable(false);

	if (m_MeleeAttackCollider)
	{
		m_MeleeAttackCollider->Enable(false);
	}

	// DeathChangeColor() 를 사용하는 경우
	m_DeathColorChangeStart = true;

	CGameObject* Player = m_Object->GetScene()->GetPlayerObject();

	// 플레이어의 공격에 맞은 오브젝트들의 DataComponent를 모아놓은 m_CollisionObjDataList에서 본인을 지운다
	CPlayerNormalAttackCheckCollider* AttackCollider = Player->FindComponentFromType<CPlayerNormalAttackCheckCollider>();
	AttackCollider->DeleteObjectData(this);

}

void CMonsterDataComponent::OnDeadAnimEnd()
{
	// Death 애니메이션이 끝나면 PaperBurn을 켠다.
	m_PaperBurn->StartPaperBurn();
}

void CMonsterDataComponent::OnPlayerEnterZone(const CollisionResult& Result)
{
	if (m_PlayerEnterZoneTrigger)
	{
		m_PlayerEnterZoneTrigger->Enable(false);
	}

	m_PlayerEnterZone = true;

 //	if (m_CutSceneCam)
 //	{
 //		m_Scene->GetCameraManager()->KeepCamera();
 //		m_Scene->GetCameraManager()->SetCurrentCamera(m_CutSceneCam);
 //	}
 }

void CMonsterDataComponent::OnStartCutScene()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	// 플레이어 입력 받지 않게 하고, 애니메이션 강제 변화
	if (PlayerObj)
	{
		CGameStateComponent* PlayerState = PlayerObj->FindComponentFromType<CGameStateComponent>();
		CAnimationSequenceInstance* PlayerAnim = PlayerObj->FindComponentFromType<CAnimationMeshComponent>()->GetAnimationInstance();

		PlayerAnim->ChangeAnimation("PlayerIdle");
		PlayerState->SetTreeUpdate(false);
	}

	m_IsCutScenePlaying = true;

	// 컷신 카메라에 이동 동선이 있는지
	bool IsCutSceneMove = m_CutSceneCam->GetMoveDataCount() > 0 ? true : false;

	if (IsCutSceneMove)
	{
		m_CutSceneCam->StartMove();
	}
	else
	{
		m_Scene->GetCameraManager()->KeepCamera();
		m_Scene->GetCameraManager()->SetCurrentCamera(m_CutSceneCam);
	}
}

void CMonsterDataComponent::OnEndCutScene()
{
	m_IsCutScenePlaying = false;

	// 전투 상태 on
	OnCombatStart();

	if (m_CutSceneCam)
	{
		m_Scene->GetCameraManager()->ReturnCamera();
	}

	// 플레이어 조작 On
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (PlayerObj)
	{
		CGameStateComponent* PlayerState = PlayerObj->FindComponentFromType<CGameStateComponent>();

		PlayerState->SetTreeUpdate(true);
	}
}

void CMonsterDataComponent::SetIsHit(bool Hit)
{
	CObjectDataComponent::SetIsHit(Hit);

	if (Hit)
	{
		if (!m_HitEffectStart)
		{
			m_HitEffectFlag = 0;
			m_HitEffectTimer = 0.f;
			m_HitEffectStart = true;
		}
	}
}

void CMonsterDataComponent::SetCurrentNodeNull()
{
	m_State->GetBehaviorTree()->SetCurrentNode(nullptr);
}

void CMonsterDataComponent::OnActiveMeleeAttackCollider()
{
	m_MeleeAttackCollider->Enable(true);
}

void CMonsterDataComponent::OnInActiveMeleeAttackCollider()
{
	m_MeleeAttackCollider->Enable(false);

	// Player Hit False 처리
	if (m_PlayerData)
	{
		m_PlayerData->SetIsHit(false);
	}
}

void CMonsterDataComponent::OnHitMeleeAttack(const CollisionResult& Result)
{
	CGameObject* Player = m_Scene->GetPlayerObject();

	if (m_PlayerData)
	{
		m_PlayerData->DecreaseHP(1);
		m_PlayerData->SetIsHit(true);
	}
}

void CMonsterDataComponent::OnEndAnimPostAttackDelayOn()
{
	SetCurrentNodeNull();

	m_PostAttackDelaying = true;
}

float CMonsterDataComponent::GetAnglePlayer()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (!PlayerObj)
	{
		return 0.f;
	}

	Vector3 PlayerPos = PlayerObj->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	Vector3 ToPlayer = PlayerPos - MyPos;

	Vector3 MyAxisZ = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;

	float Angle = ToPlayer.Angle(MyAxisZ);
	return Angle;
}

bool CMonsterDataComponent::IsPlayerLeftBasedInLookDir()
{
	/*
	Vector3 vToPlayer = ToPlayer();
	// Vector3 vWorldLook = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 vLook = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 vWorldLook = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 vWorldRight = m_Object->GetWorldAxis(AXIS::AXIS_X) * -1.f;

	// Monster 의 오른쪽에 Player 방향의 벡터를 투영
	float ProjToMonsterWorldRight = vToPlayer.Dot(vWorldRight);

	if (ProjToMonsterWorldRight < 0.f)
		return true;
	else
		return false;
		*/

	Vector3 vToPlayer = ToPlayer();
	Vector3 vLook = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;

	Vector3 vCross = vToPlayer.Cross(vLook);

	if (vCross.y < 0.f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CMonsterDataComponent::OnEnableLookPlayer()
{
	m_LookPlayer = true;
}

void CMonsterDataComponent::OnDisableLookPlayer()
{
	m_LookPlayer = false;
}

void CMonsterDataComponent::OnEnableRightLookPlayer()
{
	m_RightLookPlayer = true;
}

void CMonsterDataComponent::OnDisableRightLookPlayer()
{
	m_RightLookPlayer = false;
}

void CMonsterDataComponent::OnEnableLeftLookPlayer()
{
	m_LeftLookPlayer = true;
}

void CMonsterDataComponent::OnDisableLeftLookPlayer()
{
	m_LeftLookPlayer = false;
}

void CMonsterDataComponent::OnEnableMoveZ()
{
	m_MoveZ = true;
}

void CMonsterDataComponent::OnDisableMoveZ()
{
	m_MoveZ = false;
}

bool CMonsterDataComponent::IsPlayerInMeleeAttackRange()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (!PlayerObj)
	{
		return false;
	}

	Vector3 MyPos = m_Object->GetWorldPos();
	Vector3 PlayerPos = PlayerObj->GetWorldPos();
	float Dist = MyPos.Distance(PlayerPos);

	if (Dist <= m_Data.MeleeAttackRange)
	{
		return true;
	}

	return false;
}

Vector3 CMonsterDataComponent::ToPlayer()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (!PlayerObj)
	{
		return Vector3::Zero;
	}

	Vector3 PlayerPos = PlayerObj->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	Vector3 ToP = PlayerPos - MyPos;
	ToP.Normalize();
	return ToP;
}

float CMonsterDataComponent::DistToPlayer()
{
	CGameObject* PlayerObj = m_Scene->GetPlayerObject();

	if (!PlayerObj)
	{
		return -1.f;
	}

	Vector3 MyPos = m_Object->GetWorldPos();
	Vector3 PlayerPos = PlayerObj->GetWorldPos();
	float Dist = MyPos.Distance(PlayerPos);

	return MyPos.Distance(PlayerPos);;
}

bool CMonsterDataComponent::Save(FILE* File)
{
	CObjectDataComponent::Save(File);

	return true;
}

bool CMonsterDataComponent::Load(FILE* File)
{
	CObjectDataComponent::Load(File);

	return true;
}

bool CMonsterDataComponent::SaveOnly(FILE* File)
{
	CObjectDataComponent::SaveOnly(File);

	return true;
}

bool CMonsterDataComponent::LoadOnly(FILE* File)
{
	CObjectDataComponent::LoadOnly(File);

	return true;
}

void CMonsterDataComponent::ActiveHitEffect(float DeltaTime)
{
	m_HitEffectTimer += DeltaTime;

	// 조건이 되었을 때 컬러 체인지를 한 번만 수행
	if ((m_HitEffectFlag & HIT_EFFECT_FIRST) == 0 && ((m_HitEffectTimer <= m_HitEffectMax / 5.f)))
	{
		m_HitEffectFlag |= HIT_EFFECT_FIRST;
		ChangeHitColor(HIT_EFFECT_FIRST);
		return;
	}
	else if ((m_HitEffectFlag & HIT_EFFECT_SECOND) == 0 && (m_HitEffectTimer >= ((m_HitEffectMax * 2.f / 5.f))))
	{
		m_HitEffectFlag |= HIT_EFFECT_SECOND;
		ChangeHitColor(HIT_EFFECT_SECOND);
		return;
	}
	else if ((m_HitEffectFlag & HIT_EFFECT_THIRD) == 0  && (m_HitEffectTimer >= ((m_HitEffectMax * 3.f / 5.f))))
	{
		m_HitEffectFlag |= HIT_EFFECT_THIRD;
		ChangeHitColor(HIT_EFFECT_THIRD);
		return;
	}
	else if ((m_HitEffectFlag & HIT_EFFECT_FOURTH) == 0 && m_HitEffectTimer >= ((m_HitEffectMax * 4.f / 5.f)))
	{
		m_HitEffectFlag |= HIT_EFFECT_FOURTH;
		ChangeHitColor(HIT_EFFECT_FOURTH);
		return;
	}
	else if ((m_HitEffectFlag & HIT_EFFECT_FIFTH) == 0 && m_HitEffectTimer >= m_HitEffectMax)
	{
		m_HitEffectFlag |= HIT_EFFECT_FIFTH;
		ChangeHitColor(HIT_EFFECT_FIFTH);
		return;
	}
	else if (m_HitEffectTimer >= m_HitEffectMax + (m_HitEffectMax / 5.f))
	{
		m_HitEffectStart = false;
		m_HitEffectTimer = 0.f;
		m_HitEffectFlag = 0;

		// 원래 컬러로 돌아온다.
		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->GetMaterial(i)->SetBaseColor(m_vecOriginDiffuse[i]);
			m_AnimMesh->GetMaterial(i)->SetAmbientColor(m_vecOriginAmbient[i]);
			m_AnimMesh->GetMaterial(i)->SetSpecularColor(m_vecOriginSpecluar[i]);
			m_AnimMesh->GetMaterial(i)->SetEmissiveColor(m_vecOriginEmissive[i]);
		}
	}
}

void CMonsterDataComponent::ChangeHitColor(int EffectNum)
{
	// 흰
	if (EffectNum == HIT_EFFECT_FIRST)
	{
		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->GetMaterial(i)->SetEmissiveColor(Vector4::White);
			m_AnimMesh->GetMaterial(i)->SetBaseColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetAmbientColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetSpecularColor(Vector4::Black);
		}
	}
	// 검
	else if (EffectNum == HIT_EFFECT_SECOND || EffectNum == HIT_EFFECT_FOURTH)
	{
		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->GetMaterial(i)->SetEmissiveColor(Vector4::Black); //
			m_AnimMesh->GetMaterial(i)->SetBaseColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetAmbientColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetSpecularColor(Vector4::Black);
		}
	}
	// 빨
	else if (EffectNum == HIT_EFFECT_THIRD || EffectNum == HIT_EFFECT_FIFTH)
	{
		for (int i = 0; i < m_MeshMatSize; ++i)
		{
			m_AnimMesh->GetMaterial(i)->SetEmissiveColor(Vector4::Red);
			m_AnimMesh->GetMaterial(i)->SetBaseColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetAmbientColor(Vector4::Black);
			m_AnimMesh->GetMaterial(i)->SetSpecularColor(Vector4::Black);
		}
	}
}

CMonsterNavAgent* CMonsterDataComponent::GetMonsterNavAgent() const
{
	return m_MonsterNavAgent;
}

CColliderBox3D* CMonsterDataComponent::GetHitBox() const
{
	return m_HitBox;
}
