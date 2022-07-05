#pragma once

#include "GameObject/GameObject.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/Arm.h"

class CAlistar :
    public CGameObject
{
    friend class CScene;

protected:
    CAlistar();
    CAlistar(const CAlistar& obj);
    virtual ~CAlistar();

private:
    CSharedPtr<CAnimationMeshComponent>       m_Mesh;
    CSharedPtr<CArm>                        m_Arm;
    CSharedPtr<CCameraComponent>        m_Camera;


    Vector3     m_Velocity;
    class CAlistarAnimation* m_Animation;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CAlistar* Clone();

private:
    void MoveFront(float DeltaTime);
};

