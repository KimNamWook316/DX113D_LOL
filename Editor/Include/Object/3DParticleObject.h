#pragma once
#include "GameObject\GameObject.h"
#include "Component/Arm.h"

class C3DParticleObject :
    public CGameObject
{
	friend class CScene;

protected:
	C3DParticleObject();
	C3DParticleObject(const C3DParticleObject& obj);
	virtual ~C3DParticleObject();
private :
	class CParticleComponent* m_ParticleComponent;
	class CCameraComponent* m_ParticleCamera;
	class CArm* m_ParticleArm;
private :
	bool m_IsCameraRotate;
	bool m_IsCameraZoom;
	float m_CameraRotateSpeed;
	float m_CameraZoomSpeed;
public :
	float GetCameraRotateSpeed() const
	{
		return m_CameraRotateSpeed;
	}
	bool IsCameraRotate() const
	{
		return m_IsCameraRotate;
	}
	bool IsCameraZoom() const
	{
		return m_IsCameraZoom; 
	}
public :
	void SetCameraZoom(bool Enable) 
	{
		m_IsCameraZoom = Enable;
	}
	void SetCameraRotate(bool Enable) 
	{
		m_IsCameraRotate = Enable;
	}
	void SetCameraRotateSpeed(float Speed)
	{
		m_CameraRotateSpeed = Speed;
	}
	void SetCameraZoomSpeed(float Speed)
	{
		m_CameraZoomSpeed = Speed;
	}
public:
	bool Init() override;
	void Update(float DeltaTime) override;
};

