#pragma once
#include "GameObject\GameObject.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/Arm.h"

class CAnim3DObject :
    public CGameObject
{
    friend class CScene;
    friend class CAnimationEditor;
protected:
    CAnim3DObject();
    CAnim3DObject(const CAnim3DObject& obj);
    virtual ~CAnim3DObject();

private:
    CSharedPtr<CAnimationMeshComponent>    m_Mesh;
    CSharedPtr<CArm>                    m_Arm;
    CSharedPtr<CCameraComponent>        m_Camera;
    Vector3     m_Velocity;
    class CAnimationSequenceInstance* m_Animation;
    bool m_IsCameraRot;
public :
    void SetCameraRot(bool Enable)
    {
        m_IsCameraRot = Enable;
    }
public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CAnim3DObject* Clone();

private:
    void MoveFront(float DeltaTime);
    void MoveBack(float DeltaTime);
    void RotationYInv(float DeltaTime);
    void RotationY(float DeltaTime);
    void Attack(float DeltaTime);

public:
    CCameraComponent* GetCamera() const
    {
        return m_Camera;
    }
};

