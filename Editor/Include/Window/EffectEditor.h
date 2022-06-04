#pragma once
#include "IMGUIWindow.h"

// Particle 을 제작하는 Editor
class CEffectEditor :
    public CIMGUIWindow
{
    friend class CEditorManager;
public:
    CEffectEditor();
    ~CEffectEditor();
private:
    class CIMGUIButton* m_OpenPopUpScreenBtn;
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

    class CIMGUICheckBox* m_IsMoveEdit;
    class CIMGUICheckBox* m_IsGravityEdit;
    class CIMGUICheckBox* m_IsRandomMoveEdit;
    // class CIMGUICheckBox* m_Is3DEdit;

    class CIMGUIInputFloat3* m_MoveDirEdit;
    class CIMGUIInputFloat3* m_MoveAngleEdit;

    // Camera 관련 세팅
    // class CIMGUICheckBox* m_CameraZoom;
    // class CIMGUICheckBox* m_CameraLookDownFromUp;
    // class CIMGUICheckBox* m_CameraRotate;

    // Unity 참고 이후 추가
    class CIMGUIInputFloat* m_GravityAccelEdit;
    class CIMGUIInputFloat* m_StartDelayEdit;
    class CIMGUIButton* m_SetMaterialTextureButton;
    class CIMGUIButton* m_RestartButton;

    // Render Target
    class CIMGUIImage* m_ParticleTexture;
    class CIMGUIImage* m_ParticleRenderTarget;
private :
    // class CParticleComponent* m_ParticleComponent;
    // class CStaticMeshComponent* m_BaseGroundComponent;
    // class CStaticMeshComponent* m_SkyComponent;
    class CGameObject* m_ParticleObject;
    class CGameObject* m_BaseGroundObject;
    class CGameObject* m_SkyObject;
public :
    virtual bool Init() override;
private :
    void SetComInfos();
    void OnCreateParticlePopUp();
private:
    void OnSaveParticleObjectButton();
    void OnLoadParticleObjectButton();

    void OnSpawnTimeMaxEdit(float Num);

    void OnStartMinEdit(const Vector3&);
    void OnStartMaxEdit(const Vector3&);

    void OnSpawnCountMaxEdit(int Num);

    void OnScaleMinEdit(const Vector3&);
    void OnScaleMaxEdit(const Vector3&);

    void OnLifeTimeMinEdit(float Num);
    void OnLifeTimeMaxEdit(float Num);

    void OnSpeedMinEdit(float Num);
    void OnSpeedMaxEdit(float Num);

    void OnColorMinEdit(const Vector4& Color);
    void OnColorMaxEdit(const Vector4& Color);

    void OnIsMoveEdit(const char*, bool);
    void OnIsGravityEdit(const char*, bool);
    void OnIsRandomMoveEdit(const char*, bool);

    void OnMoveDirEdit(const Vector3& Dir);
    void OnMoveAngleEdit(const Vector3& Angle);

    void OnSetParticleTexture();

    // void OnGravityAccelEdit();
    // void OnStartDelayEdit();
private:
    // Particle 기본 세팅
    void OnSetParticleMaterialSetting(class CSceneComponent* Com);
    void OnReflectCurrentParticleSetting();
};

