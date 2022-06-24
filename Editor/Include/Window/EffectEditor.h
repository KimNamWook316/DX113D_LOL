#pragma once
#include "IMGUIWindow.h"

struct Particle3DObjectBackUpStruct
{
    bool IsCameraRotate;
    bool IsCameraRotateInv;
    bool IsCameraRotateSpeed;
    bool IsCameraZoom;
    float CameraZoomSpeed;
    float CameraYOffset;
    float CameraXRot;
    float TargetDist;
    float WorldRotY;
};

// Particle 을 제작하는 Editor
class CEffectEditor :
    public CIMGUIWindow
{
    friend class CEditorManager;
public:
    CEffectEditor();
    ~CEffectEditor();
private:
    class CIMGUIButton* m_SaveParticleBtn;
    class CIMGUIButton* m_LoadParticleBtn;
    class CIMGUIButton* m_StartEditBtn;
    class CIMGUIButton* m_RestartBtn;
private :
    class CIMGUITextInput* m_CurrentParticleName;
private:
    class CIMGUIInputFloat* m_SpawnTimeMaxEdit;
    // class CIMGUIButton* m_RestartButton;

    class CIMGUIInputFloat3* m_StartMinEdit;
    class CIMGUIInputFloat3* m_StartMaxEdit;

    class CIMGUIInputInt* m_SpawnCountMaxEdit;
    // class CIMGUIButton* m_RestartButton;

    class CIMGUIInputFloat3* m_ScaleMinEdit;
    class CIMGUIInputFloat3* m_ScaleMaxEdit;

    class CIMGUIInputFloat* m_LifeTimeMinEdit;
    class CIMGUIInputFloat* m_LifeTimeMaxEdit;

    class CIMGUIInputFloat* m_SpeedMinEdit;
    class CIMGUIInputFloat* m_SpeedMaxEdit;

    class CIMGUIColor4* m_ColorMinEdit;
    class CIMGUIColor4* m_ColorMaxEdit;

    class CIMGUIInputFloat* m_AlphaMinEdit;
    class CIMGUIInputFloat* m_AlphaMaxEdit;
    class CIMGUIButton* m_AlphaBlendEnableButton;

    // Material
    class CIMGUITextInput* m_CurrentMaterialName;
    class CIMGUITextInput* m_LoadedMaterialFileName;
    class CIMGUIButton*    m_MaterialLoadButton;

    // Ground Texture
    class CIMGUISliderFloat* m_GroundTextureScale;

    class CIMGUICheckBox* m_IsMoveEdit;
    class CIMGUICheckBox* m_IsGravityEdit;
    class CIMGUICheckBox* m_IsPauseResumeToggle;

    // Camera Related
    class CIMGUICheckBox* m_IsRotateEdit;
    class CIMGUICheckBox* m_IsRotateInv;
    class CIMGUISliderFloat* m_RotateSpeedSliderBar;
    class CIMGUISliderFloat* m_YRotateSliderBar;
    class CIMGUICheckBox* m_IsZoomEdit;
    class CIMGUISliderFloat* m_ZoomSpeedSliderBar;

    // Bounce Effect
    class CIMGUICheckBox* m_IsBounce;
    class CIMGUISliderFloat* m_BounceResistance; // 마찰력

    // Ring Generate
    class CIMGUICheckBox* m_IsGenerateRing;
    class CIMGUISliderFloat* m_GenerateRingRadius;
    class CIMGUICheckBox* m_IsLoopGenerateRing; 

    // Circle Generate
    class CIMGUICheckBox* m_IsGenerateCircle;
    class CIMGUISliderFloat* m_GenerateCircleRadius;

    // 카메라의 Y 축 위치 (위로 갈지, 아래로 갈지 조정)
    class CIMGUISliderFloat* m_CameraYOffsetBar;

    // 카메라의 X 축 기준 회전 조정 (위를 볼지, 아래를 볼지)
    class CIMGUISliderFloat* m_CameraXRotSlideBar;

    // Move Dir, Angle
    class CIMGUIInputFloat3* m_MoveDirEdit;
    class CIMGUIInputFloat3* m_MoveAngleEdit;

    // Particle 이 사용하는 Material Texture
    class CIMGUIImage* m_ParticleTexture;
private :
    class CParticle* m_ParticleClass;
    class CMaterial* m_ParticleMaterial;
private :
    Particle3DObjectBackUpStruct m_BackUpStruct;
    bool m_StartEdit;
private :
    class C3DParticleObject* m_ParticleObject;
    class CGameObject* m_ParticleSampleObject;
    class CGameObject* m_BaseGroundObject;
    class CGameObject* m_SkyObject;
public :
    virtual bool Init() override;
private:
    void OnSaveParticleObjectButton();
    void OnLoadParticleObjectButton();
    void OnRestartParticleComponentButton();

    // Material Load By Btn
    void OnLoadParticleMaterialCallback();

    // Base Ground Size
    void OnEditBaseGroundSize(float Speed);

    // Bounce
    void OnIsBounceEdit(const char*, bool);
    void OnEditBounceResistance(float Speed);

    // Generate Ring
    void OnIsGenerateRingEdit(const char*, bool);
    void OnIsLoopGenerateRingEdit(const char*, bool);
    void OnEditGenerateRingRadius(float Radius);

    // Generate Circle
    void OnIsGenerateCircleEdit(const char*, bool);
    void OnEditGenerateCircleRadius(float Radius);

    // Spawn Time, Count
    void OnSpawnTimeMaxEdit(float Num);
    void OnSpawnCountMaxEdit(int Num);

    // StartMin,Max
    void OnStartMinEdit(const Vector3&);
    void OnStartMaxEdit(const Vector3&);

    // Scale Min, Max
    void OnScaleMinEdit(const Vector3&);
    void OnScaleMaxEdit(const Vector3&);

    // LifeTime Min, Max
    void OnLifeTimeMinEdit(float Num);
    void OnLifeTimeMaxEdit(float Num);

    // Speed Min, Max
    void OnSpeedMinEdit(float Num);
    void OnSpeedMaxEdit(float Num);

    // Color Min, Max
    void OnColorMinEdit(const Vector4& Color);
    void OnColorMaxEdit(const Vector4& Color);

    // Alpha Min, Max
    void OnAlphaMinEdit(float Alpha);
    void OnAlphaMaxEdit(float Alpha);

    // 현재 Material에 Alpha Blend 적용하기
    void OnSetAlphaBlendToMaterialCallback();

    // MoveMent
    void OnIsMoveEdit(const char*, bool);
    void OnIsGravityEdit(const char*, bool);
    void OnPauseResumeToggle(const char*, bool);

    // Camera
    void OnIsCameraRotateEdit(const char*, bool);
    void OnCameraRotateInvEdit(const char*, bool);
    void OnSetCameraRotateSpeed(float Speed);
    void OnSetCameraYAxisRotate(float Speed);
    void OnIsCameraZoomEdit(const char*, bool);
    void OnSetCameraZoomSpeed(float Speed);
    void OnSetCameraYOffset(float Offset);
    void OnSetCameraXRot(float Rot);

    // Move Dir, Angle
    void OnMoveDirEdit(const Vector3& Dir);
    void OnMoveAngleEdit(const Vector3& Angle);

    // Save, Load
    void OnSaveParticleClass();
    void OnLoadParticleClass();
private:
    // Particle 기본 세팅
    void OnSetBasicParticleMaterialSetting(class CSceneComponent* Com);
    void OnReflectCurrentParticleSetting();

    // Base Ground Object 준비시키기
    void SetGameObjectReady();

    // 기본 Particle 정보 세팅하기
    void SetBasicDefaultParticleInfos(class CParticle* Particle);

    // Material Setting
    void OnDropMaterialToParticle(const std::string& InputName);
    void ApplyNewMaterial(class CMaterial* Material);

    // Particle Object 백업, 
    void BackUpParticleObjectInfo();
    void ReflectBackUpParticleObjectInfo();

    // Edit 시작 하기 (기본 설정 세팅)
    void SetStartEditing();

    // Particle Component 에 Particle 세팅하기 .
    void SetParticleToParticleComponent(class CParticleComponent* Component, const char* ParticleName);
    void SetParticleToParticleComponent(class CParticleComponent* Component, CParticle* Particle);

    // IMGUI가 Particle 정보 반영하게 하기 
    void SetIMGUIReflectParticle(class CParticle* Particle);

    // IMGUI가 Paritlc Object 정보 반영하게 하기 
    void SetIMGUIReflectObjectCamera();
};

