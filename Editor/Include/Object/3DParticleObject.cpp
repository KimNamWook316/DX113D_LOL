#include "3DParticleObject.h"
#include "Component/ParticleComponent.h"
#include "Component/CameraComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Input.h"

C3DParticleObject::C3DParticleObject() :
    m_IsCameraRotate(false),
    m_IsCameraZoom(false),
    m_CameraRotateSpeed(30.f),
    m_CameraZoomSpeed(0.5f)
{
}

C3DParticleObject::C3DParticleObject(const C3DParticleObject& obj)
{
}

C3DParticleObject::~C3DParticleObject()
{
}

bool C3DParticleObject::Init()
{
    if (!CGameObject::Init())
        return false;

    // Root 
    m_ParticleComponent = CreateComponent<CParticleComponent>("Particle Sprite");
    m_ParticleComponent->SetLayerName("ParticleEditorLayer");

    // m_ParticleComponent->SetWorldScale(20.f, 20.f, 20.f); // m_ParticleComponent->SetRelativeScale(0.02f, 0.02f, 0.02f);
    m_ParticleComponent->SetRelativeScale(0.02f, 0.02f, 0.02f);

    // Arm, Camera
    m_ParticleArm = CreateComponent<CArm>("Arm");

    m_ParticleCamera = CreateComponent<CCameraComponent>("Particle Camera");

    m_ParticleComponent->AddChild(m_ParticleArm);
    m_ParticleComponent->AddChild(m_ParticleCamera);

    m_ParticleCamera->SetInheritRotX(true);
    m_ParticleCamera->SetInheritRotY(true);
    m_ParticleCamera->SetInheritRotZ(true);

    m_ParticleArm->AddChild(m_ParticleCamera);

    // Camera Setting
    CSceneManager::GetInst()->GetScene()->GetCameraManager()->SetParticleEditorCamera(m_ParticleCamera);


    m_ParticleCamera->SetCameraType(Camera_Type::Camera3D);
    m_ParticleCamera->SetViewAngle(27.f);

    // m_ParticleCamera->AddRelativePos(0.f, 50.f, -50.f);  // m_ParticleCamera->SetRelativeRotationX(45.f);

    m_ParticleArm->SetOffset(0.f, 2.f, 0.f);
    m_ParticleArm->SetRelativeRotation(45.f, 0.f, 0.f);
    m_ParticleArm->SetTargetDistance(150.f);

    /*
    
    m_ParticleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>("Particle Object");
    m_ParticleObject->CreateComponent<CParticleComponent>("Particle Sprite");
    m_ParticleObject->GetRootComponent()->SetLayerName("ParticleEditorLayer");

    CCameraComponent* ParticleCamera = m_ParticleObject->CreateComponent<CCameraComponent>("Particle Camera");
    m_ParticleObject->GetRootComponent()->AddChild(ParticleCamera);
    
    // Camera Setting
    CSceneManager::GetInst()->GetScene()->GetCameraManager()->SetParticleEditorCamera(ParticleCamera);

    ParticleCamera->SetCameraType(Camera_Type::Camera3D);
    ParticleCamera->SetViewAngle(27.f);

    ParticleCamera->AddRelativePos(0.f, 30.f, -100.f);
    ParticleCamera->SetRelativeRotationX(45.f);*/


	return true;
}

void C3DParticleObject::Update(float DeltaTime)
{
    if (CInput::GetInst()->GetWheelDir() && m_IsCameraZoom)
    {
        float Length = m_ParticleArm->GetTargetDistance() +
            CInput::GetInst()->GetWheelDir() * m_CameraZoomSpeed;

        m_ParticleArm->SetTargetDistance(Length);
    }

    if (m_IsCameraRotate)
    {
        m_ParticleArm->AddRelativeRotationY(m_CameraRotateSpeed * DeltaTime);
    }
}
