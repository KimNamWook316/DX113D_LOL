#pragma once
#include "GameObject\GameObject.h"

class C3DParticleCamera :
    public CGameObject
{
	friend class CCameraManager;
	friend class CScene;
protected:
	C3DParticleCamera();
	C3DParticleCamera(const C3DParticleCamera& com);
	virtual ~C3DParticleCamera();

protected:
	float	m_TargetDistance;
	Vector3	m_Offset;
	CSharedPtr<class CCameraComponent> m_Camera;
	CSharedPtr<class CGameObject> m_TargetObject;
	bool m_Rotate;
	bool m_Zoom;
public:
	float GetTargetDistance()	const
	{
		return m_TargetDistance;
	}

	const Vector3& GetOffset()	const
	{
		return m_Offset;
	}
	CCameraComponent* GetCamera() const
	{
		return m_Camera;
	}
public:
	void SetTargetObject(CGameObject* Object)
	{
		m_TargetObject = Object;
	}
	void SetOffset(const Vector3& Offset)
	{
		m_Offset = Offset;
	}
	void SetOffset(float x, float y, float z)
	{
		Vector3 Offset(x, y, z);

		m_Offset = Offset;
	}
	void SetTargetDistance(float Distance)
	{
		m_TargetDistance = Distance;
	}
	void SetCameraZoomEnable(bool Enable)
	{
		m_Zoom = Enable;
	}
	void SetCameraRotateEnable(bool Enable)
	{
		m_Rotate = Enable;
	}
public :
	void SetCameraSeeFromUpToDown();
	void SetCameraSeeFront();
	void SetCameraSeeDiagonal();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

