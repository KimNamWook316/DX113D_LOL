// IMGUI Widget
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
// Components
#include "Component/StaticMeshComponent.h"
#include "Resource/Particle/Particle.h"
#include "Engine.h"
#include "EngineUtil.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "Component/ParticleComponent.h"
#include "Component/SceneComponent.h"
#include "Resource/Particle/Particle.h"
// Editor Windows
#include "ParticleComponentWidget.h"
#include "../Window/ObjectHierarchyWindow.h"
#include "../EditorInfo.h"
#include "../EditorUtil.h"
#include "../Object/3DCameraObject.h"
#include "../EditorManager.h"
#include "../EditorManager.h"
#include "../Window/ResourceDisplayWindow.h"


CParticleComponentWidget::CParticleComponentWidget()
{
}

CParticleComponentWidget::~CParticleComponentWidget()
{
}

bool CParticleComponentWidget::Init()
{
    CSceneComponentWidget::Init();

    // AddWidget
    m_ComponentTypeText->SetText("Particle Component");

    // 최상위 트리
    CIMGUITree* m_RootTree = AddWidget<CIMGUITree>("Particle Variables");

    m_LoadedParticleName = m_RootTree->AddWidget<CIMGUITextInput>("Loaded Particle(Drop)", 150.f, 30.f);
    m_LoadedParticleName->SetHintText("Particle Name");
    m_LoadedParticleName->SetDropCallBack<CParticleComponentWidget>(this, &CParticleComponentWidget::OnDropParticleToParticleWidget);
    
    m_LoadedParticleFileName = m_RootTree->AddWidget<CIMGUITextInput>("Particle File Name", 150.f, 30.f);
    m_LoadedParticleFileName->SetHintText("FileName");

    m_LoadParticleButton = m_RootTree->AddWidget<CIMGUIButton>("Load Particle", 150.f, 20.f);
    m_LoadParticleButton->SetClickCallback<CParticleComponentWidget>(this, &CParticleComponentWidget::OnLoadParticleClass);

   
    // Camera 세팅
    // OnSetCameraSetting();

    /*
    // Particle Texture
    m_ParticleTexture = m_RootTree->AddWidget<CIMGUIImage>("Render Target", 200.f, 200.f);
    m_ParticleTexture->SetBorderColor(10, 10, 255);

    m_SpawnTimeMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Spawn Time", 200.f);
    m_SpawnTimeMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnSpawnTimeMaxEdit);

    m_StartMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("StartMin", 200.f);
    m_StartMinEdit->SetCallBack(this, &CParticleComponentWidget::OnStartMinEdit);

    m_StartMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("StartMax", 200.f);
    m_StartMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnStartMaxEdit);

    m_SpawnCountMaxEdit = m_RootTree->AddWidget<CIMGUIInputInt>("Spawn Max", 200.f);
    m_SpawnCountMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnSpawnCountMaxEdit);

    m_ScaleMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("ScaleMin", 200.f);
    m_ScaleMinEdit->SetCallBack(this, &CParticleComponentWidget::OnScaleMinEdit);

    m_ScaleMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("ScaleMax", 200.f);
    m_ScaleMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnScaleMaxEdit);

    m_LifeTimeMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Life T Min", 200.f);
    m_LifeTimeMinEdit->SetCallBack(this, &CParticleComponentWidget::OnLifeTimeMinEdit);

    m_LifeTimeMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Life T Max", 200.f);
    m_LifeTimeMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnLifeTimeMaxEdit);

    m_SpeedMinEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Speed Min", 200.f);
    m_SpeedMinEdit->SetCallBack(this, &CParticleComponentWidget::OnSpeedMinEdit);

    m_SpeedMaxEdit = m_RootTree->AddWidget<CIMGUIInputFloat>("Speed Max", 200.f);
    m_SpeedMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnSpeedMaxEdit);

    m_ColorMinEdit = m_RootTree->AddWidget<CIMGUIColor4>("Color Min", 200.f);
    m_ColorMinEdit->SetCallBack(this, &CParticleComponentWidget::OnColorMinEdit);

    m_ColorMaxEdit = m_RootTree->AddWidget<CIMGUIColor4>("Color Max", 200.f);
    m_ColorMaxEdit->SetCallBack(this, &CParticleComponentWidget::OnColorMaxEdit);

    m_IsMoveEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Move", 100.f);
    m_IsMoveEdit->AddCheckInfo("Move");
    m_IsMoveEdit->SetCallBackLabel<CParticleComponentWidget>(this, &CParticleComponentWidget::OnIsMoveEdit);

    m_IsGravityEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Gravity", 100.f);
    m_IsGravityEdit->AddCheckInfo("Gravity");
    m_IsGravityEdit->SetCallBackLabel<CParticleComponentWidget>(this, &CParticleComponentWidget::OnIsGravityEdit);

    m_IsRandomMoveEdit = m_RootTree->AddWidget<CIMGUICheckBox>("Random", 100.f);
    m_IsRandomMoveEdit->AddCheckInfo("Random");
    m_IsRandomMoveEdit->SetCallBackLabel<CParticleComponentWidget>(this, &CParticleComponentWidget::OnIsRandomMoveEdit);

    m_MoveDirEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("Move Dir", 200.f);
    m_MoveDirEdit->SetCallBack<CParticleComponentWidget>(this, &CParticleComponentWidget::OnMoveDirEdit);

    m_MoveAngleEdit = m_RootTree->AddWidget<CIMGUIInputFloat3>("Move Angle", 200.f);
    m_MoveAngleEdit->SetCallBack<CParticleComponentWidget>(this, &CParticleComponentWidget::OnMoveAngleEdit);

    m_SetMaterialTextureButton = m_RootTree->AddWidget<CIMGUIButton>("Set Texture", 100.f, 30.f);
    m_SetMaterialTextureButton->SetClickCallback<CParticleComponentWidget>(this, &CParticleComponentWidget::OnSetParticleTexture);

   */

	return true;
}

void CParticleComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CParticleComponent* MeshCom = (CParticleComponent*)m_Component;

    // 처음에는 Particle 이 세팅되어 있지 않으므로, Diable 처리하여 Update X
    // m_Component->Enable(false);
    // m_Component->GetGameObject()->Enable(false);

    // Material , Particle 세팅
    // OnSetParticleMaterialSetting(m_Component);

    CParticle* ParticleClass = dynamic_cast<CParticleComponent*>(m_Component)->GetParticle();
    if (ParticleClass)
        ParticleLoadSuccessCallback(ParticleClass);
}

void CParticleComponentWidget::OnSaveParticleObjectButton()
{
}
   
void CParticleComponentWidget::OnLoadParticleObjectButton()
{
}
void CParticleComponentWidget::OnLoadParticleClass()
{
    // Bin//ParticleClass 폴더로부터 Load 되게 세팅하기
    const PathInfo* ParticleClassPathInfo = CPathManager::GetInst()->FindPath(PARTICLE_PATH);
    CEngineUtil::CheckAndMakeDirectory(ParticleClassPathInfo);

    TCHAR FileFullPath[MAX_PATH] = {};

    OPENFILENAME OpenFile = {};
    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Particle File\0*.prtc");
    OpenFile.lpstrFile = FileFullPath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = ParticleClassPathInfo->Path;

    if (GetOpenFileName(&OpenFile) != 0)
    {
        // 경로 추출
        std::string FileExt;
        std::string FileName;

        const char* FilePathMultibyte = CEditorUtil::ChangeTCHARTextToMultibyte(FileFullPath);
        char FilePathMultibyteCopy[MAX_PATH] = {};
        strcpy_s(FilePathMultibyteCopy, FilePathMultibyte);

        // 현재 Load하는 Directory가  Bin//ParticleClass 인지 확인하기 => 아니라면, Load
        std::string PathInfoBeforeFileName;
        CEngineUtil::GetPathInfoBeforeFileName(FilePathMultibyteCopy, PathInfoBeforeFileName);

        if (strcmp(ParticleClassPathInfo->PathMultibyte, PathInfoBeforeFileName.c_str()) != 0)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Particle 의 경우 Bin/ParticleClass 로부터 Load 해야 한다."), NULL, MB_OK);
            return;
        }

        CEditorUtil::ExtractFileNameAndExtFromPath(FilePathMultibyteCopy, FileName, FileExt);

        // 확장자 검사
        std::transform(FileExt.begin(), FileExt.end(), FileExt.begin(), [](char c) {return std::toupper(c); });

        if (FileExt != ".PRTC")
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("확장자가 .prtc 이어야 합니다."), NULL, MB_OK);
            return;
        }

        // SAFE_DELETE(m_ParticleClass); --> Particle Manager 에서 알아서 관리해줄 것이다.
        CParticle* LoadedParticle = CResourceManager::GetInst()->CreateParticleEmpty<CParticle>();
        bool LoadResult = LoadedParticle->LoadFile(FilePathMultibyteCopy);

        if (!LoadResult)
        {
            MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Particle Class Load Failure While Loading"), NULL, MB_OK);
            return;
        }

        ParticleLoadSuccessCallback(LoadedParticle);
    }

}
void CParticleComponentWidget::OnDropParticleToParticleWidget(const std::string& InputName)
{
    // Key 값 형태로 Resource Manager 에서 Particle을 먼저 찾는다.
    CParticle* FoundParticle = CResourceManager::GetInst()->FindParticle(InputName);

    if (FoundParticle)
    {
        ParticleLoadSuccessCallback(FoundParticle);
        return;
    }

    // 그래도 없다면, 하드디스크에서 찾는다.
    std::string ParticleKeyName;

    // Bin//ParticleClass 에 접근
    std::optional<std::string> FoundResult = CEditorUtil::GetFullPathOfTargetFileNameInDir(PARTICLE_PATH,
        InputName, ParticleKeyName);

    // 찾지 못했다면 
    if (!FoundResult.has_value())
    {
        // New Texture Load Failure Message Box
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Particle Class Load Failure From Bin/ParticleClass"), NULL, MB_OK);

        // m_LoadedParticleName Text Input 내용을 되돌려 놓는다
        m_LoadedParticleName->ResetText();

        return;
    }

    // 파일의 확장자가 .prtc 인지 확인한다.
    const std::string& FullPathMultibyte = FoundResult.value();
    std::string ExtractedFilName;
    std::string ExtractedExt;
    CEditorUtil::ExtractFileNameAndExtFromPath(FullPathMultibyte, ExtractedFilName, ExtractedExt);

    // 대문자로 변환 이후, Ext 확인
    std::transform(ExtractedExt.begin(), ExtractedExt.end(), ExtractedExt.begin(), ::toupper);
    if (ExtractedExt != ".PRTC")
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("확장자가 .prtc 이어야 합니다."), NULL, MB_OK);

        // m_LoadedParticleName Text Input 내용을 되돌려 놓는다
        m_LoadedParticleName->ResetText();

        return;
    }

    // SAFE_DELETE(m_ParticleClass); --> Particle Manager 에서 알아서 관리해줄 것이다.
    FoundParticle = CResourceManager::GetInst()->CreateParticleEmpty<CParticle>();
    bool LoadResult = FoundParticle->LoadFile(FoundResult.value().c_str());

    if (!LoadResult)
    {
        MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("Particle Class Load Failure From Bin/ParticleClass"), NULL, MB_OK);

        // m_LoadedParticleName Text Input 내용을 되돌려 놓는다
        m_LoadedParticleName->ResetText();

        return;
    }

    ParticleLoadSuccessCallback(FoundParticle);
}
void CParticleComponentWidget::ParticleLoadSuccessCallback(CParticle* LoadedParticle)
{
    if (!LoadedParticle)
        return;

    m_ParticleClass = LoadedParticle;

    SetParticleClassToParticleComponent(LoadedParticle);

    // Particle Manager 의 Map 에 추가하기
    CResourceManager::GetInst()->GetParticleManager()->AddParticle(m_ParticleClass);

    // Resource Display Window 세팅하기
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedTextureResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedMaterialResources();
    CEditorManager::GetInst()->GetResourceDisplayWindow()->RefreshLoadedParticleResources();
}
void CParticleComponentWidget::SetParticleClassToParticleComponent(CParticle* Particle)
{
    if (!Particle)
        return;

    dynamic_cast<CParticleComponent*>(m_Component)->SetParticle(Particle);

    // Name 세팅
    m_LoadedParticleName->ClearText();
    m_LoadedParticleName->SetText(Particle->GetName().c_str());

    // FileName 세팅하기 
    std::string LoadedParticleName = dynamic_cast<CParticleComponent*>(m_Component)->GetParticleName();
    
    // .prtc 를 붙이기
    if (LoadedParticleName.find(".prtc") == std::string::npos)
        LoadedParticleName.append(".prtc");

    m_LoadedParticleFileName->ClearText();
    m_LoadedParticleFileName->SetText(LoadedParticleName.c_str());

}
void CParticleComponentWidget::ReflectParticleToIMGUI()
{
    if (!m_Component || m_ParticleClass)
        return;

}
/*
void CParticleComponentWidget::OnSpawnTimeMaxEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component)->SetSpawnTime(Num);
}

void CParticleComponentWidget::OnStartMinEdit(const Vector3& Pos)
{
   dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetStartMin(Pos);
}

void CParticleComponentWidget::OnStartMaxEdit(const Vector3& Pos)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetStartMax(Pos);
}

void CParticleComponentWidget::OnSpawnCountMaxEdit(int Num)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetSpawnCountMax(Num);
}

void CParticleComponentWidget::OnScaleMinEdit(const Vector3& Scale)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetScaleMin(Scale);
}

void CParticleComponentWidget::OnScaleMaxEdit(const Vector3& Scale)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetScaleMax(Scale);
}

void CParticleComponentWidget::OnLifeTimeMinEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetLifeTimeMin(Num);
}

void CParticleComponentWidget::OnLifeTimeMaxEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetLifeTimeMax(Num);
}

void CParticleComponentWidget::OnSpeedMinEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetSpeedMin(Num);
}

void CParticleComponentWidget::OnSpeedMaxEdit(float Num)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetSpeedMax(Num);
}

void CParticleComponentWidget::OnColorMinEdit(const Vector4& Color)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetColorMin(Color);
}

void CParticleComponentWidget::OnColorMaxEdit(const Vector4& Color)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetColorMax(Color);
}


void CParticleComponentWidget::OnIsMoveEdit(const char*, bool Enable)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetMove(Enable);
}

void CParticleComponentWidget::OnIsGravityEdit(const char*, bool Enable)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetGravity(Enable);
}

void CParticleComponentWidget::OnIsRandomMoveEdit(const char*, bool Enable)
{
    // dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetApplyRandom(Enable);
}

void CParticleComponentWidget::OnMoveDirEdit(const Vector3& Dir)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetMoveDir(Dir);
}

void CParticleComponentWidget::OnMoveAngleEdit(const Vector3& Angle)
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetCBuffer()->SetMoveAngle(Angle);
}


void CParticleComponentWidget::OnSetParticleTexture()
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

        // 바뀐 Particle 정보를 Clone 해서 다시 세팅한다.
        dynamic_cast<CParticleComponent*>(m_Component)->SetParticle("BasicParticle");

        // 기존 세팅 정보를 그대로 반영한다.
        OnReflectCurrentParticleSetting();
    }
}
*/

/*
void CParticleComponentWidget::OnSetParticleMaterialSetting(CSceneComponent* Com)
{
    // 기본 Particle Setting, 현재 Component 에 Particle Setting 하기
    // 1) Particle Material 세팅
    CSceneManager::GetInst()->GetScene()->GetResource()->CreateMaterial<CMaterial>("BasicParticleMaterial");
    CMaterial* Material = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BasicParticleMaterial");
    Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));
    Material->SetShader("ParticleRenderShader");
    Material->SetRenderState("AlphaBlend");
    Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Bubble", TEXT("Particle/Bubbles99px.png"));

    // 2) Particle 제작
    CSceneManager::GetInst()->GetScene()->GetResource()->CreateParticle("BasicParticle");
    CParticle* Particle = CSceneManager::GetInst()->GetScene()->GetResource()->FindParticle("BasicParticle");
    Material = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("BasicParticleMaterial");
    Particle->SetMaterial(Material);

    // 반드시 3D 로 세팅한다.
    Particle->Set2D(false);

    Particle->SetSpawnCountMax(1000);
    m_SpawnCountMaxEdit->SetVal(Particle->GetSpawnCountMax());

    Particle->SetLifeTimeMin(70.f);
    m_LifeTimeMinEdit->SetVal(Particle->GetLifeTimeMin());

    Particle->SetLifeTimeMax(80.f);
    m_LifeTimeMaxEdit->SetVal(Particle->GetLifeTimeMax());

    Particle->SetScaleMin(Vector3(20.f, 20.f, 1.f));
    m_ScaleMinEdit->SetVal(Particle->GetScaleMin());

    Particle->SetScaleMax(Vector3(50.f, 50.f, 1.f));
    m_ScaleMaxEdit->SetVal(Particle->GetScaleMax());

    Particle->SetSpeedMin(10.f);
    m_SpeedMinEdit->SetVal(Particle->GetSpeedMin());

    Particle->SetSpeedMax(30.f);
    m_SpeedMaxEdit->SetVal(Particle->GetSpeedMax());

    Particle->SetMoveDir(Vector3(0.f, 1.f, 1.f)); // Z 축 방향으로도 이동할 수 있게 세팅한다.
    m_MoveDirEdit->SetVal(Particle->GetMoveDir());
    
    Particle->SetStartMin(Vector3(-30.f, -30.f, 0.f));
    m_StartMinEdit->SetVal(Particle->GetStartMin());

    Particle->SetStartMax(Vector3(30.f, 30.f, 0.f));
    m_StartMaxEdit->SetVal(Particle->GetStartMax());

    Particle->SetColorMin(Vector4(0.2f, 0.1f, 0.8f, 1.f));
    m_ColorMinEdit->SetRGBA(Particle->GetColorMin());

    Particle->SetColorMax(Vector4(0.2f, 0.1f, 0.8f, 1.f));
    m_ColorMaxEdit->SetRGBA(Particle->GetColorMax());

    Particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));
    m_MoveAngleEdit->SetVal(Particle->GetMoveAngle());

    Particle->SetGravity(true);
    Particle->SetMove(true);

    // 해당 정보들 UI 에 세팅하기

    // Particle 세팅
    dynamic_cast<CParticleComponent*>(Com)->SetParticle("BasicParticle");
    dynamic_cast<CParticleComponent*>(Com)->SetBillBoardEffect(true);
    dynamic_cast<CParticleComponent*>(Com)->SetSpawnTime(0.05f);
    m_SpawnTimeMaxEdit->SetVal(0.05f);

    // 기본 Z Pos 세팅
    Com->SetWorldPos(Com->GetWorldPos().x, Com->GetWorldPos().y, 10.f);

}
*/

/*
void CParticleComponentWidget::OnReflectCurrentParticleSetting()
{
    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->Set2D(false);

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetSpawnCountMax(m_SpawnCountMaxEdit->GetVal());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetLifeTimeMin(m_LifeTimeMinEdit->GetVal());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetLifeTimeMax(m_LifeTimeMaxEdit->GetVal());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetScaleMin(m_ScaleMinEdit->GetValue());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetScaleMax(m_ScaleMaxEdit->GetValue());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetSpeedMin(m_SpeedMinEdit->GetVal());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetSpeedMax(m_SpeedMaxEdit->GetVal());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetMoveDir(m_MoveDirEdit->GetValue());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetStartMin(m_StartMinEdit->GetValue());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetStartMax(m_StartMaxEdit->GetValue());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetColorMin(m_ColorMinEdit->GetRGBA());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetColorMax(m_ColorMaxEdit->GetRGBA());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetMoveAngle(m_MoveAngleEdit->GetValue());

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetGravity(m_IsGravityEdit->GetCheck(0));

    dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetMove(m_IsMoveEdit->GetCheck(0));

    // dynamic_cast<CParticleComponent*>(m_Component)->GetParticle()->SetApplyRandom(m_IsRandomMoveEdit->GetCheck(0));
}
*/

