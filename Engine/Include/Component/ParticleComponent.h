#pragma once

#include "SceneComponent.h"
#include "../Resource/Particle/Particle.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/ParticleUpdateShader.h"
#include "../Resource/Shader/ParticleRenderShader.h"
#include "../Resource/Shader/ParticleConstantBuffer.h"
#include "../Resource/Material/Material.h"

class CParticleComponent :
    public CSceneComponent
{
	friend class CGameObject;

protected:
	CParticleComponent();
	CParticleComponent(const CParticleComponent& com);
	virtual ~CParticleComponent();

protected:
	CSharedPtr<CParticle>					m_Particle;
	std::vector<class CStructuredBuffer*>	m_vecStructuredBuffer;
	CSharedPtr<CMesh>						m_Mesh;
	CSharedPtr<CMaterial>					m_Material;
	CSharedPtr<CParticleUpdateShader>		m_UpdateShader;
	CParticleConstantBuffer*				m_CBuffer;
	ParticleInfo							m_Info;
	ParticleInfoShared						m_InfoShared;
	float									m_SpawnTime;
	float									m_SpawnTimeMax;
	// BillBoard
	bool  m_BillBoardEffect;
	// Move
	float m_ParticleMoveAccTime;
	float m_ParticleMoveSpeed;
	float m_ParticleMoveInitSpeed;
	ParticleSpeedChangeMethod m_SpeedChangeMethod;
	Vector3 m_ParticleNextMovePos;
	Vector3 m_ParticleMoveDir;
	Vector3 m_ParticleRotOffset;
	// 지수 함수 형태로 사용할 밑
	float m_ParticleMoveSpeedBottom;
	// Bazier
	float m_BazierMoveTargetDist;
	float m_BazierMoveCurDist;
	bool m_BazierMoveEffect;
	std::queue<Vector3> m_queueBazierMovePos;
	// 등장 Delay Time -> ex. 0.5초 후에 해당 위치에 나타가게 하기
	float m_InitActiveDelayTime;
private :
	std::string m_ParticleName;
public:
	void SetParticle(const std::string& Name);
	void SetParticle(CParticle* Particle);
	void SetSpawnTime(float Time);
	// Particle 입자가 아니라, Particle Component 의 움직임 효과
	void SetComponentSpeedChangeMethod(ParticleSpeedChangeMethod Method)
	{
		m_SpeedChangeMethod = Method;
	}
private :
	void ApplyBillBoardEffect();
	void ApplyBazierMove(float DeltaTime);
	// Bazier 방식으로 특정 방향을 따라가게 세팅한다.
public :
	// D1 , 즉, 시작점은 WorldPos 가 될 것이다.
	void SetBazierTargetPos(const Vector3& D2, const Vector3& D3, const Vector3& D4, int DetailNum);
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void RenderParticleEffectEditor();
	virtual void PostRender();
	virtual CParticleComponent* Clone();
public :
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	// Editor 에서 Test 용으로 특정 Button을 누르면 처음부터 다시 생성하게 하는 기능
public :
	void ResetParticleStructuredBufferInfo();
private :
	virtual bool SaveOnly(FILE* File);
	virtual bool LoadOnly(FILE* File);
public:
	float GetInitActiveDelayTime() const
	{
		return m_InitActiveDelayTime;
	}
	bool IsBazierMoveEnable() const
	{
		return m_BazierMoveEffect;
	}
	CParticleConstantBuffer* GetCBuffer() const
	{
		return m_CBuffer;
	}
	CParticle* GetParticle() const
	{
		return m_Particle;
	}
	const std::string& GetParticleName() const
	{
		return m_ParticleName;
	}
	CMaterial* GetMaterial() const
	{
		return m_Material;
	}
public:
	void SetGravityEffect(bool Enable)
	{
		m_ParticleMoveSpeed = Enable;
	}
	void SetParticleMoveSpeed(float Speed)
	{
		m_ParticleMoveSpeed = Speed;
	}
	void SetParticleClassFileName(const std::string& ParticleFileName)
	{
		m_ParticleName = ParticleFileName;
	}
	void SetMaterial(class CMaterial* Material)
	{
		m_Material = Material;
	}
	void SetBillBoardEffect(bool Enable)
	{
		m_BillBoardEffect = Enable;
	}
	void SetBazierMoveEffect(bool Enable)
	{
		m_BazierMoveEffect = Enable;
	}
	void SetInitActiveDelayTime(float Time)
	{
		m_InitActiveDelayTime = Time;
	}
};

