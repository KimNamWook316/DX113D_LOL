// IMGUI
#include "IMGUITextInput.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "../EditorInfo.h"
#include "../EditorUtil.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIImage.h"
#include "IMGUISliderFloat.h"
#include "../Window/ObjectHierarchyWindow.h"
#include "IMGUIManager.h"
#include "IMGUIColor3.h"
#include "IMGUIColor4.h"
#include "IMGUIInputFloat.h"
#include "IMGUIComboBox.h"
#include "IMGUIDummy.h"
#include "IMGUITree.h"
// Window
#include "EffectEditor.h"
#include "../EditorManager.h"
#include "../Window/ResourceDisplayWindow.h"
// Object
#include "../Object/3DParticleObject.h"
// Engine
#include "Component/ParticleComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/Arm.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Render/RenderManager.h"
#include "Resource/Particle/ParticleManager.h"
#include "Scene/SceneResource.h"
#include "GameObject/SkyObject.h"

CEffectEditor::CEffectEditor() :
    m_StartEdit(false)
{
}

CEffectEditor::~CEffectEditor()
{
}

bool CEffectEditor::Init()
{
    // Save, Load
    m_SaveParticleBtn = AddWidget<CIMGUIButton>("Save Particle", 90.f, 20.f);
    m_SaveParticleBtn->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnSaveParticleClass);

    CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(105);

    m_LoadParticleBtn = AddWidget<CIMGUIButton>("Load Particle", 90.f, 20.f);
    m_LoadParticleBtn->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnLoadParticleClass);

    // Set Texture
    m_SetMaterialTextureButton = AddWidget<CIMGUIButton>("Set Texture", 90.f, 20.f);
    m_SetMaterialTextureButton->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnSetParticleTexture);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(105);

    m_StartEditBtn = AddWidget<CIMGUIButton>("Start Edit", 90.f, 20.f);
    m_StartEditBtn->SetClickCallback<CEffectEditor>(this, &CEffectEditor::SetStartEditing);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(205);

    m_RestartBtn = AddWidget<CIMGUIButton>("ReStart", 80.f, 20.f);
    m_RestartBtn->SetClickCallback<CEffectEditor>(this, &CEffectEditor::OnRestartParticleComponentButton);

    // Camera
    CIMGUITree* Tree = AddWidget<CIMGUITree>("Camera");

    // Rotate
    m_IsRotateEdit = Tree->AddWidget<CIMGUICheckBox>("Rotate", 80.f);
    m_IsRotateEdit->AddCheckInfo("Rotate");
    m_IsRotateEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsCameraRotateEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(100.f);

    m_IsRotateInv = Tree->AddWidget<CIMGUICheckBox>("Inv", 80.f);
    m_IsRotateInv->AddCheckInfo("Inv");
    m_IsRotateInv->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnCameraRotateInvEdit);


    m_RotateSpeedSliderBar = Tree->AddWidget<CIMGUISliderFloat>("Rotate Speed", 100.f, 20.f);
    m_RotateSpeedSliderBar->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnSetCameraRotateSpeed);
    m_RotateSpeedSliderBar->SetMin(10.f);
    m_RotateSpeedSliderBar->SetMax(90.f);

    // Zoom
    m_IsZoomEdit = Tree->AddWidget<CIMGUICheckBox>("Zoom", 80.f);
    m_IsZoomEdit->AddCheckInfo("Zoom");
    m_IsZoomEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsCameraZoomEdit);

    m_ZoomSpeedSliderBar = Tree->AddWidget<CIMGUISliderFloat>("Zoom Speed", 100.f, 20.f);
    m_ZoomSpeedSliderBar->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnSetCameraZoomSpeed);
    m_ZoomSpeedSliderBar->SetMin(0.2f);
    m_ZoomSpeedSliderBar->SetMax(50.f);

    m_CameraYOffsetBar = Tree->AddWidget<CIMGUISliderFloat>("Camera Y Offset", 100.f, 20.f);
    m_CameraYOffsetBar->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnSetCameraYOffset);
    m_CameraYOffsetBar->SetMin(-50.f);
    m_CameraYOffsetBar->SetMax(50.f);

    m_CameraXRotSlideBar = Tree->AddWidget<CIMGUISliderFloat>("Camera X Rot", 100.f, 20.f);
    m_CameraXRotSlideBar->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnSetCameraXRot);
    m_CameraXRotSlideBar->SetMin(-88.f);
    m_CameraXRotSlideBar->SetMax(88.f);

    // ParticleEffectRenderTarget
    m_ParticleRenderTarget = AddWidget<CIMGUIImage>("Render Target", 500.f, 400.f);
    m_ParticleRenderTarget->SetRenderTargetImage(true);
    m_ParticleRenderTarget->SetTexture(CRenderManager::GetInst()->GetParticleEffectRenderTarget());
    m_ParticleRenderTarget->SetBorderColor(10, 10, 255);
    m_ParticleRenderTarget->SetTableTitle("Render Target");

    // Particle Texture
    m_ParticleTexture = AddWidget<CIMGUIImage>("Particle Texture", 200.f, 200.f);
    // m_ParticleRenderTarget = AddWidget<CIMGUIImage>("Render Target", 500.f, 500.f);
    // m_ParticleRenderTarget->SetRenderTargetImage(true);
    // m_ParticleRenderTarget->SetTexture(CRenderManager::GetInst()->GetParticleEffectRenderTarget());
    m_ParticleTexture->SetBorderColor(10, 10, 255);
    m_ParticleTexture->SetTableTitle("Texture");

    // Material
    m_MaterialName = AddWidget<CIMGUITextInput>("Current Material", 150.f, 30.f);
    m_MaterialName->SetHideName(true);
    m_MaterialName->SetHintText("Current Material");
    m_MaterialName->SetDropCallBack<CEffectEditor>(this, &CEffectEditor::OnDropMaterialToParticle);

    // Movement
    Tree = AddWidget<CIMGUITree>("Movement");

    m_IsMoveEdit = Tree->AddWidget<CIMGUICheckBox>("Move", 80.f);
    m_IsMoveEdit->AddCheckInfo("Move");
    m_IsMoveEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsMoveEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(90.f);

    m_IsGravityEdit = Tree->AddWidget<CIMGUICheckBox>("Gravity", 80.f);
    m_IsGravityEdit->AddCheckInfo("Gravity");
    m_IsGravityEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsGravityEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(170.f);

    // m_IsRandomMoveEdit = Tree->AddWidget<CIMGUICheckBox>("Random", 80.f);
    // m_IsRandomMoveEdit->AddCheckInfo("Random");
    // m_IsRandomMoveEdit->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnIsRandomMoveEdit);

    // Line = Tree->AddWidget<CIMGUISameLine>("Line");
    // Line->SetOffsetX(260.f);

    m_IsPauseResumeToggle = Tree->AddWidget<CIMGUICheckBox>("Toggle", 80.f);
    m_IsPauseResumeToggle->AddCheckInfo("Toggle");
    m_IsPauseResumeToggle->SetCallBackLabel<CEffectEditor>(this, &CEffectEditor::OnPauseResumeToggle);
    m_IsPauseResumeToggle->SetCheck(0, true);

    // Spawn Time, Spawn Count
    Tree  = AddWidget<CIMGUITree>("Spawn Time, Count");

    m_SpawnTimeMaxEdit = Tree->AddWidget<CIMGUIInputFloat>("Spawn Time", 150.f);
    m_SpawnTimeMaxEdit->SetCallBack(this, &CEffectEditor::OnSpawnTimeMaxEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(270.f);

    m_SpawnCountMaxEdit = Tree->AddWidget<CIMGUIInputInt>("Spawn Max", 150.f);
    m_SpawnCountMaxEdit->SetCallBack(this, &CEffectEditor::OnSpawnCountMaxEdit);

    // Start Min, Start Max
    Tree = AddWidget<CIMGUITree>("Start Min, Max");

    m_StartMinEdit = Tree->AddWidget<CIMGUIInputFloat3>("StartMin", 150.f);
    m_StartMinEdit->SetCallBack(this, &CEffectEditor::OnStartMinEdit);

    m_StartMaxEdit = Tree->AddWidget<CIMGUIInputFloat3>("StartMax", 150.f);
    m_StartMaxEdit->SetCallBack(this, &CEffectEditor::OnStartMaxEdit);

    // Scale Min, Start Max
    Tree = AddWidget<CIMGUITree>("Scale Min, Max");

    m_ScaleMinEdit = Tree->AddWidget<CIMGUIInputFloat3>("ScaleMin", 150.f);
    m_ScaleMinEdit->SetCallBack(this, &CEffectEditor::OnScaleMinEdit);

    m_ScaleMaxEdit = Tree->AddWidget<CIMGUIInputFloat3>("ScaleMax", 150.f);
    m_ScaleMaxEdit->SetCallBack(this, &CEffectEditor::OnScaleMaxEdit);

    // LifeTime Min, Max
    Tree = AddWidget<CIMGUITree>("LifeTime Min, Max");

    m_LifeTimeMinEdit = Tree->AddWidget<CIMGUIInputFloat>("Life T Min", 150.f);
    m_LifeTimeMinEdit->SetCallBack(this, &CEffectEditor::OnLifeTimeMinEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(250.f);

    m_LifeTimeMaxEdit = Tree->AddWidget<CIMGUIInputFloat>("Life T Max", 150.f);
    m_LifeTimeMaxEdit->SetCallBack(this, &CEffectEditor::OnLifeTimeMaxEdit);

    // Speed Min, Max
    Tree = AddWidget<CIMGUITree>("Speed Min, Max");

    m_SpeedMinEdit = Tree->AddWidget<CIMGUIInputFloat>("Speed Min", 150.f);
    m_SpeedMinEdit->SetCallBack(this, &CEffectEditor::OnSpeedMinEdit);

    Line = Tree->AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(250.f);

    m_SpeedMaxEdit = Tree->AddWidget<CIMGUIInputFloat>("Speed Max", 150.f);
    m_SpeedMaxEdit->SetCallBack(this, &CEffectEditor::OnSpeedMaxEdit);

    // Color Min, Max
    Tree = AddWidget<CIMGUITree>("Color Min, Max");

    m_ColorMinEdit = Tree->AddWidget<CIMGUIColor4>("Color Min", 150.f);
    m_ColorMinEdit->SetCallBack(this, &CEffectEditor::OnColorMinEdit);

    m_ColorMaxEdit = Tree->AddWidget<CIMGUIColor4>("Color Max", 150.f);
    m_ColorMaxEdit->SetCallBack(this, &CEffectEditor::OnColorMaxEdit);

    // Move Dir, Angle
    Tree = AddWidget<CIMGUITree>("Move Angle, Dir");

    m_MoveDirEdit = Tree->AddWidget<CIMGUIInputFloat3>("Move Dir", 150.f);
    m_MoveDirEdit->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnMoveDirEdit);

    m_MoveAngleEdit = Tree->AddWidget<CIMGUIInputFloat3>("Move Angle", 150.f);
    m_MoveAngleEdit->SetCallBack<CEffectEditor>(this, &CEffectEditor::OnMoveAngleEdit);

    // m_GravityAccelEdit = AddWidget<CIMGUIInputFloat>("Gravity Accel", 100.f);
    // m_StartDelayEdit = AddWidget<CIMGUIInputFloat>("Start Delay T// ", 100.f);

    SetGameObjectReady();

	return true;
}

void CEffectEditor::OnSaveParticleObjectButton()
{
}

void CEffectEditor::OnLoadParticleObjectButton()
{
}

void CEffectEditor::OnRestartParticleComponentButton()
{
    // 아직 시작도 안했다면 동작 X
    if (!m_ParticleObject)
        return;

    if (!m_ParticleClass)
        return;

    // 기존에 존재하던 Particle Object 는 지워줘야 한다
    if (m_ParticleObject)
    {
        m_ParticleObject->Destroy();
    }

    BackUpParticleObjectInfo();

    m_ParticleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DParticleObject>("Particle Effect Base Ground");

    SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent()), m_ParticleClass);

    ReflectBackUpParticleObjectInfo();

    // IMGUI가 Particle 정보 반영하게 하기 
    SetIMGUIReflectParticle(m_ParticleClass);

    // IMGUI가 Paritlc Object 정보 반영하게 하기 
    SetIMGUIReflectObjectCamera();
}

void CEffectEditor::OnSpawnTimeMaxEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpawnTime(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->SetSpawnTime(Num);
}

// StartMin, Max , SpawnCountMax 의 경우, Particle Component 에서 Particle 의 상수 버퍼로 부터 정보를 바로 얻어와서 Post Update 에서 계산
// 따라서 Particle 의 상수 버퍼 정보를 바꿔주면 된다.
void CEffectEditor::OnStartMinEdit(const Vector3& Pos)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetStartMin(Pos);
    // m_ParticleComponent->GetParticle()->SetStartMin(Pos);
}

void CEffectEditor::OnStartMaxEdit(const Vector3& Pos)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetStartMax(Pos);
    // m_ParticleComponent->GetParticle()->SetStartMax(Pos);
}

void CEffectEditor::OnSpawnCountMaxEdit(int Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpawnCountMax(Num);
    // m_ParticleComponent->GetCBuffer()->SetSpawnCountMax(Num);
}

void CEffectEditor::OnScaleMinEdit(const Vector3& Scale)
{
    if (!m_ParticleClass)
        return;

    // m_ParticleClass->SetScaleMin(m_ScaleMinEdit->GetValue());
    m_ParticleClass->SetScaleMin(Scale);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetScaleMin(Scale);
}

void CEffectEditor::OnScaleMaxEdit(const Vector3& Scale)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetScaleMax(Scale);

    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetScaleMax(Scale);
    // m_ParticleComponent->GetCBuffer()->SetScaleMax(Scale);
}

void CEffectEditor::OnLifeTimeMinEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetLifeTimeMin(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetLifeTimeMin(Num);
    // m_ParticleComponent->GetCBuffer()->SetLifeTimeMin(Num);
}

void CEffectEditor::OnLifeTimeMaxEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetLifeTimeMax(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetLifeTimeMax(Num);
    // m_ParticleComponent->GetCBuffer()->SetLifeTimeMax(Num);
}

void CEffectEditor::OnSpeedMinEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpeedMin(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetSpeedMin(Num);
    // m_ParticleComponent->GetCBuffer()->SetSpeedMin(Num);
}

void CEffectEditor::OnSpeedMaxEdit(float Num)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetSpeedMax(Num);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetSpeedMax(Num);
    // m_ParticleComponent->GetCBuffer()->SetSpeedMax(Num);
}

void CEffectEditor::OnColorMinEdit(const Vector4& Color)
{
    if (!m_ParticleClass)
        return;

    // Alpha 값은 0으로 한다.

    m_ParticleClass->SetColorMin(Color.x, Color.y, Color.z, 1.f);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMin(Color.x, Color.y, Color.z, 1.f);
    // m_ParticleComponent->GetCBuffer()->SetColorMin(Color);
}

void CEffectEditor::OnColorMaxEdit(const Vector4& Color)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetColorMax(Color.x, Color.y, Color.z, 1.f);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetColorMax(Color.x, Color.y, Color.z, 1.f);
    // m_ParticleComponent->GetCBuffer()->SetColorMax(Color);
}


void CEffectEditor::OnIsMoveEdit(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetMove(Enable);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetMove(Enable);
    // m_ParticleComponent->GetCBuffer()->SetMove(Enable);
}

void CEffectEditor::OnIsGravityEdit(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetGravity(Enable);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetGravity(Enable);
    // m_ParticleComponent->GetCBuffer()->SetGravity(Enable);
}

// void CEffectEditor::OnIsRandomMoveEdit(const char*, bool Enable)
// {
//     if (!m_ParticleClass)
//         return;
// 
//     m_ParticleClass->SetApplyRandom(Enable); //
//     dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetApplyRandom(Enable);
//     // m_ParticleComponent->GetCBuffer()->SetApplyRandom(Enable);
// }

void CEffectEditor::OnPauseResumeToggle(const char*, bool Enable)
{
    if (!m_ParticleClass)
        return;

    m_ParticleObject->GetRootComponent()->Enable(Enable);
}

void CEffectEditor::OnIsCameraRotateEdit(const char*, bool Enable)
{
    m_ParticleObject->SetCameraRotate(Enable);
}

void CEffectEditor::OnCameraRotateInvEdit(const char*, bool Enable)
{
    m_ParticleObject->SetRotateInv(Enable);
}

void CEffectEditor::OnSetCameraRotateSpeed(float Speed)
{
    m_RotateSpeedSliderBar->SetValue(Speed);

    m_ParticleObject->SetCameraRotateSpeed(Speed);
}

void CEffectEditor::OnIsCameraZoomEdit(const char*, bool Enable)
{
    m_ParticleObject->SetCameraZoom(Enable);
}

void CEffectEditor::OnSetCameraZoomSpeed(float Speed)
{
    m_ZoomSpeedSliderBar->SetValue(Speed);

    m_ParticleObject->SetCameraZoomSpeed(Speed);
}

void CEffectEditor::OnSetCameraYOffset(float Offset)
{
    m_ParticleObject->SetYOffset(Offset);
}

void CEffectEditor::OnSetCameraXRot(float Rot)
{
    m_ParticleObject->SetRelativeRotationXPos(Rot);
}

void CEffectEditor::OnMoveDirEdit(const Vector3& Dir)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetMoveDir(Dir);
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetMoveDir(Dir);
}

void CEffectEditor::OnMoveAngleEdit(const Vector3& Angle)
{
    if (!m_ParticleClass)
        return;

    m_ParticleClass->SetMoveAngle(Angle);

    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetCBuffer()->SetMoveAngle(Angle);
}


void CEffectEditor::OnSetParticleTexture()
{
    TCHAR FilePath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle(); // handle to window that owns the dialog box
    OpenFile.lpstrFilter =
        TEXT("모든파일\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
    OpenFile.lpstrFile = FilePath;
    // buffer ! filename used to initialize the file name edit control -> 정상적으로 save,open할시 여기에 filePath 정보가 들어온다.
    OpenFile.nMaxFile = MAX_PATH; // size of buffer, pointed to by lpstrFile
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path; // Initial Directory

    if (GetOpenFileName(&OpenFile) != 0) // NonZero --> specifies file name, clicks ok button
    {
        int   TextureIndex = 0;
        TCHAR OriginFileName[MAX_PATH] = {};
        TCHAR FileName[MAX_PATH] = {};
        TCHAR Ext[_MAX_EXT] = {};
        _wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, MAX_PATH, Ext, _MAX_EXT);

        lstrcpy(OriginFileName, FileName);

        lstrcat(FileName, Ext);

        // Texture 저장용
        char ConvertFileName[MAX_PATH] = {};
        int  Length = WideCharToMultiByte(CP_ACP, 0, OriginFileName, -1, nullptr, 0, nullptr, nullptr);
        WideCharToMultiByte(CP_ACP, 0, OriginFileName, -1, ConvertFileName, Length, nullptr, nullptr);

        CMaterial* Material = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BasicParticleMaterial");

        if (!Material)
            return;

        // m_ParticleTexture->SetTextureFullPath(ConvertFileName, FilePath);
        m_ParticleTexture->SetTexture(ConvertFileName, FileName, PARTICLE_PATH);

        // 실제 Particle 이 사용하는 Material 의 Texture 교체
        Material->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FileName, PARTICLE_PATH);

        // 기존 세팅 정보를 그대로 반영한다.
        // OnReflectCurrentParticleSetting();
    }
}

void CEffectEditor::OnSaveParticleClass()
{
    if (!m_ParticleObject || !dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle())
        return;

    TCHAR FiileFullPath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};
    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
    OpenFile.lpstrFile = FiileFullPath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(PARTICLE_PATH)->Path;

    if (GetSaveFileName(&OpenFile) != 0)
    {
        char FileFullPathMultibyte[MAX_PATH] = {};
        char FileName[MAX_PATH] = {};
        char FileExt[_MAX_EXT] = {};

        int  ConvertLength = WideCharToMultiByte(CP_ACP, 0, FiileFullPath, -1, nullptr, 0, nullptr, nullptr);

        WideCharToMultiByte(CP_ACP, 0, FiileFullPath, -1, FileFullPathMultibyte, ConvertLength, nullptr, nullptr);

        _splitpath_s(FileFullPathMultibyte, nullptr, 0, nullptr, 0, FileName, MAX_PATH, FileExt, _MAX_EXT);

        _strupr_s(FileExt);

        // 확장자 .anim 이 아니라면 return;
        if (strcmp(FileExt, ".PRTC") != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("EXT Has To Be .prtc"), NULL, MB_OK);
            return;
        }

        m_ParticleClass->SaveFile(FileFullPathMultibyte);
    }
}

void CEffectEditor::OnLoadParticleClass()
{
   //  if (!m_ParticleObject || !dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle())
   //      return;

    TCHAR FilePath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};
    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(PARTICLE_PATH)->Path;

    if (GetOpenFileName(&OpenFile) != 0)
    {
        // 경로 추출
        char	Ext[_MAX_EXT] = {};

        char FilePathMultibyte[MAX_PATH] = {};
        char FileName[MAX_PATH] = {};

        int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

        _splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, FileName, MAX_PATH, Ext, _MAX_EXT);

        _strupr_s(Ext);

        // 확장자 .anim 이 아니라면 return;
        if (strcmp(Ext, ".PRTC") != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("확장자가 .prtc 이어야 합니다."), NULL, MB_OK);
            return;
        }

        // Edit State 처리 
        m_StartEdit = true;

        // Particle 파일 Load 하기
        // SAFE_DELETE(m_ParticleClass); --> Particle Manager 에서 알아서 관리해줄 것이다.
        m_ParticleClass = CResourceManager::GetInst()->CreateParticleEmpty<CParticle>();
        m_ParticleClass->LoadFile(FilePathMultibyte);

        // 이전 Particle Object 의 카메라 관련 정보 백업
        BackUpParticleObjectInfo();

         // 필요한 Object 목록 세팅 => Particle Object 도 세팅
        SetGameObjectReady();

        // Particle Object Enable 처리
        m_ParticleObject->Enable(true);
        m_ParticleObject->GetRootComponent()->Enable(true);

        // 백업 내용 반영
        ReflectBackUpParticleObjectInfo();

        // Particle Component 에 Particle 세팅
        SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent()), m_ParticleClass);

        // IMGUI Update
        SetIMGUIReflectParticle(m_ParticleClass);
        SetIMGUIReflectObjectCamera();

        // Particle Manager 의 Map 에 추가하기
        CResourceManager::GetInst()->GetParticleManager()->AddParticle(m_ParticleClass);

        // Resource Display Window 세팅하기
        CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedParticleResources();
    }
}

void CEffectEditor::OnSetBasicParticleMaterialSetting(CSceneComponent* Com)
{
    // 기본 Particle Setting, 현재 Component 에 Particle Setting 하기
    // 1) Particle Material 세팅
    CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterial<CMaterial>("BasicParticleMaterial");
    m_ParticleMaterial = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BasicParticleMaterial");
    m_ParticleMaterial->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));
    m_ParticleMaterial->SetShader("ParticleRenderShader");
    m_ParticleMaterial->SetRenderState("AlphaBlend");

    // 2) Particle 제작
    CSceneManager::GetInst()->GetScene()->GetResource()->CreateParticle("BasicParticle");
    m_ParticleClass = CSceneManager::GetInst()->GetScene()->GetResource()->FindParticle("BasicParticle");
    if (!m_ParticleClass)
        return;
    m_ParticleClass->SetMaterial(m_ParticleMaterial);

    SetBasicDefaultParticleInfos(m_ParticleClass);

    // 해당 정보들 UI 에 세팅하기
    SetParticleToParticleComponent(dynamic_cast<CParticleComponent*>(Com), "BasicParticle");

    // 기본 Z Pos 세팅
    Com->SetWorldPos(Com->GetWorldPos().x, Com->GetWorldPos().y, 10.f);

    // Resource Window Display
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedTextureResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();
}

void CEffectEditor::OnReflectCurrentParticleSetting()
{
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->Set2D(false);
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetSpawnCountMax(m_SpawnCountMaxEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetSpawnTime(m_SpawnTimeMaxEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetLifeTimeMin(m_LifeTimeMinEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetLifeTimeMax(m_LifeTimeMaxEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetScaleMin(m_ScaleMinEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetScaleMax(m_ScaleMaxEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetSpeedMin(m_SpeedMinEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetSpeedMax(m_SpeedMaxEdit->GetVal());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetMoveDir(m_MoveDirEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetStartMin(m_StartMinEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetStartMax(m_StartMaxEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetColorMin(m_ColorMinEdit->GetRGBA());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetColorMax(m_ColorMaxEdit->GetRGBA());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetMoveAngle(m_MoveAngleEdit->GetValue());
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetGravity(m_IsGravityEdit->GetCheck(0));
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetMove(m_IsMoveEdit->GetCheck(0));
    // dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->GetParticle()->SetApplyRandom(m_IsRandomMoveEdit->GetCheck(0));
}
void CEffectEditor::SetGameObjectReady()
{
    // SkyObject
   // m_SkyObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSkyObject>("Particle Effect Editor Sky");
   // m_SkyObject->GetRootComponent()->SetLayerName("ParticleEditorLayer");
   // m_SkyObject->SetScene(CSceneManager::GetInst()->GetScene());
   // m_SkyObject->Init();

    // Particle Object 생성
    if (m_ParticleObject)
    {
        m_ParticleObject->Destroy();
    }

    m_ParticleObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DParticleObject>("Particle Effect Base Ground");
    // 처음에는 Enable False 로 하여 보이지 않게 한다.
    m_ParticleObject->Enable(false);
    m_ParticleObject->GetRootComponent()->Enable(false);

    // 기존 Base Ground Object 는 지운다.
    if (!m_BaseGroundObject)
    {
        m_BaseGroundObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>("Particle Effect Base Ground");
        m_BaseGroundObject->CreateComponent<CSpriteComponent>("Simple Sprite");
        m_BaseGroundObject->GetRootComponent()->SetLayerName("ParticleEditorLayer");
        m_BaseGroundObject->SetPivot(0.5, 0.5, 0.5);
        m_BaseGroundObject->SetWorldScale(300.f, 300.f, 1.f);
        m_BaseGroundObject->AddWorldRotationX(90.f);
        m_BaseGroundObject->AddWorldPos(0.f, -30.f, 0.f);
        m_BaseGroundObject->ExcludeFromSceneSave();
    }
}

void CEffectEditor::SetBasicDefaultParticleInfos(CParticle* Particle)
{
    // 반드시 3D 로 세팅한다.
    Particle->Set2D(false);

    // Spawn Time, Count
    Particle->SetSpawnCountMax(1000);
    Particle->SetSpawnTime(0.05f);

    // Life Time
    Particle->SetLifeTimeMin(5.f);
    Particle->SetLifeTimeMax(10.f);

    // Scale
    Particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
    Particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));

    // Speed
    Particle->SetSpeedMin(10.f);
    Particle->SetSpeedMax(30.f);

    // Start Min,Max
    Particle->SetStartMin(Vector3(-30.f, -30.f, 0.f));
    Particle->SetStartMax(Vector3(30.f, 30.f, 0.f));

    // Color Min, Max
    Particle->SetColorMin(Vector4(0.2f, 0.1f, 0.8f, 1.f));
    Particle->SetColorMax(Vector4(0.6f, 0.8f, 0.8f, 1.f));

    // Move Dir
    Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f)); 
    // Move Angle
    Particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));

    // IsGravity
    // IsMove
    // IsRandomMove
    // IsPauseResume -> 무조건 Enable true 로 시작 세팅한다.
    Particle->SetGravity(true);
    Particle->SetMove(true);
    // Particle->SetApplyRandom(true);
    Particle->Enable(true);
}

void CEffectEditor::SetStartEditing()
{
    if (m_StartEdit)
        return;

    m_StartEdit = true;

    // 이미 Particle Object 가 만들어져 있다면 => Particle Editing 을 시작한 상태이므로
    m_ParticleObject->Enable(true);
    m_ParticleObject->GetRootComponent()->Enable(true);

    OnSetBasicParticleMaterialSetting(m_ParticleObject->GetRootComponent());

    SetIMGUIReflectParticle(m_ParticleClass);

    SetIMGUIReflectObjectCamera();

}

void CEffectEditor::OnDropMaterialToParticle(const std::string& InputName)
{
    if (!m_ParticleClass)
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("No Particle Set"), NULL, MB_OK);
        return;
    }

    // Key 값 형태로 Resource Manager 에서 Mateirla 을 먼저 찾는다.
    CMaterial* FoundMaterial = CResourceManager::GetInst()->FindMaterial(InputName);

    if (FoundMaterial)
    {
        ApplyNewMaterial(FoundMaterial);
        // 제대로 세팅되었다는 Message
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Set SuccessFully"), NULL, MB_OK);

        return;
    }

    // 그 다음 하드 디스크에서 찾는다.
    // Drop 한 Input 안에 있는 Texture 정보 불러오기 
    // Texture File 의 File 명을, Texture 를 저장하는 Key 값으로 사용할 것이다.
    std::string MaterialKeyName;

    std::optional<std::string> FoundResult = CEditorUtil::GetFullPathOfTargetFileNameInDir(MATERIAL_PATH,
        InputName, MaterialKeyName);

    // 찾지 못했다면 
    if (!FoundResult.has_value())
    {
        // New Texture Load Failure Message Box
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Load Failure From HardDisk"), NULL, MB_OK);
        return;
    }

    FoundMaterial = CResourceManager::GetInst()->LoadMaterialFullPathMultibyte(FoundResult.value().c_str());
    
    if (!FoundMaterial)
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Load Failure From HardDisk"), NULL, MB_OK);
        return;
    }
    
    ApplyNewMaterial(FoundMaterial);
  
    // 제대로 세팅되었다는 Message
    MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Set SuccessFully"), NULL, MB_OK);

}

void CEffectEditor::ApplyNewMaterial(CMaterial* FoundMaterial)
{
    if (!FoundMaterial)
        return;

    m_ParticleClass->SetMaterial(FoundMaterial);
    m_ParticleMaterial = FoundMaterial;

    // 해당 Material 의 Texture 를 불러와서 Image 에 세팅하기 => 첫번째 Texture 를 세팅해준다.
    if (FoundMaterial->GetTextureInfo().size() > 0)
        m_ParticleTexture->SetTexture(FoundMaterial->GetTexture());

    // Particle 의 Material 정보를 Particle Component 에 다시 반영한다.
    dynamic_cast<CParticleComponent*>(m_ParticleObject->GetRootComponent())->SetMaterial(m_ParticleClass->CloneMaterial());

    // 제대로 세팅되었다는 Message
    MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Material Set SuccessFully"), NULL, MB_OK);

    // Resource Window Display Update
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedTextureResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();
}

void CEffectEditor::BackUpParticleObjectInfo()
{
    if (!m_ParticleObject)
        return;

    m_BackUpStruct.IsCameraRotate = m_ParticleObject->IsCameraRotate();
    m_BackUpStruct.IsCameraRotateInv = m_ParticleObject->IsCameraRotateInv();
    m_BackUpStruct.IsCameraRotateSpeed = m_ParticleObject->GetCameraRotateSpeed();
    m_BackUpStruct.IsCameraZoom = m_ParticleObject->IsCameraZoom();
    m_BackUpStruct.CameraZoomSpeed = m_ParticleObject->GetCameraZoomSpeed();


    m_BackUpStruct.CameraYOffset = m_ParticleObject->GetCameraYOffset();
    m_BackUpStruct.CameraXRot = m_ParticleObject->GetCameraRelativeRotation().x;
    m_BackUpStruct.TargetDist = m_ParticleObject->GetArmComponent()->GetTargetDistance();
    m_BackUpStruct.WorldRotY = m_ParticleObject->GetCameraYOffset();
}

void CEffectEditor::ReflectBackUpParticleObjectInfo()
{
    if (!m_ParticleObject)
        return;

    m_ParticleObject->SetCameraRotate(m_BackUpStruct.IsCameraRotate);
    m_ParticleObject->SetRotateInv(m_BackUpStruct.IsCameraRotateInv);
    m_ParticleObject->SetCameraRotateSpeed(m_BackUpStruct.IsCameraRotateSpeed);
    m_ParticleObject->SetCameraZoom(m_BackUpStruct.IsCameraZoom);
    m_ParticleObject->SetCameraZoomSpeed(m_BackUpStruct.CameraZoomSpeed);

    m_ParticleObject->GetArmComponent()->SetOffset(0.f, m_BackUpStruct.CameraYOffset, 0.f);
    m_ParticleObject->GetArmComponent()->SetRelativeRotation(m_BackUpStruct.CameraXRot, 0.f, 0.f);
    m_ParticleObject->GetArmComponent()->SetTargetDistance(m_BackUpStruct.TargetDist);
    m_ParticleObject->GetArmComponent()->SetRelativeRotationY(m_BackUpStruct.WorldRotY);
}

void CEffectEditor::SetParticleToParticleComponent(CParticleComponent* Component,  const char* ParticleName)
{
    if (!Component)
        return;

    Component->SetParticle(ParticleName);
}

void CEffectEditor::SetParticleToParticleComponent(CParticleComponent* Component, CParticle* Particle)
{
    if (!Component)
        return;

    Component->SetParticle(Particle);
}

void CEffectEditor::SetIMGUIReflectParticle(CParticle* Particle)
{
    // 반드시 3D 로 세팅한다.
    Particle->Set2D(false);

    m_SpawnCountMaxEdit->SetVal(Particle->GetSpawnCountMax());
    m_SpawnTimeMaxEdit->SetVal(Particle->GetSpawnTime());

    m_LifeTimeMinEdit->SetVal(Particle->GetLifeTimeMin());
    m_LifeTimeMaxEdit->SetVal(Particle->GetLifeTimeMax());

    m_ScaleMinEdit->SetVal(Particle->GetScaleMin());
    m_ScaleMaxEdit->SetVal(Particle->GetScaleMax());

    m_SpeedMinEdit->SetVal(Particle->GetSpeedMin());
    m_SpeedMaxEdit->SetVal(Particle->GetSpeedMax());

    m_StartMinEdit->SetVal(Particle->GetStartMin());
    m_StartMaxEdit->SetVal(Particle->GetStartMax());

    m_ColorMinEdit->SetRGBA(Particle->GetColorMin());
    m_ColorMaxEdit->SetRGBA(Particle->GetColorMax());

    m_MoveDirEdit->SetVal(Particle->GetMoveDir());
    m_MoveAngleEdit->SetVal(Particle->GetMoveAngle());

    m_IsGravityEdit->SetCheck(0, Particle->GetGravity());
    m_IsMoveEdit->SetCheck(0, Particle->GetMove());
    // m_IsRandomMoveEdit->SetCheck(0, Particle->GetApplyRandom() == 1 ? true : false);
    m_IsPauseResumeToggle->SetCheck(0, true);
}

void CEffectEditor::SetIMGUIReflectObjectCamera()
{
    // 새로운 Particle Object 에 이전에 세팅되었던 Camera 관련 정보를 반영해준다.

    m_IsRotateEdit->SetCheck(0, m_ParticleObject->IsCameraRotate());
    m_IsRotateInv->SetCheck(0, m_ParticleObject->IsCameraRotateInv());
    m_RotateSpeedSliderBar->SetValue(m_ParticleObject->GetCameraRotateSpeed());
    m_IsZoomEdit->SetCheck(0, m_ParticleObject->IsCameraZoom());
    m_ZoomSpeedSliderBar->SetValue(m_ParticleObject->GetCameraZoomSpeed());

    m_CameraYOffsetBar->SetValue(m_ParticleObject->GetCameraYOffset());
    m_CameraXRotSlideBar->SetValue(m_ParticleObject->GetCameraRelativeRotation().x);
}
