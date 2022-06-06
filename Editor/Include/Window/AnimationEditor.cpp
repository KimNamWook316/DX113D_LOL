// IMGUI
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUICheckBox.h"
#include "IMGUIDummy.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIProgressBar.h"
#include "IMGUIChild.h"
#include "IMGUIText.h"
#include "IMGUITable.h"
#include "IMGUITextInput.h"
#include "IMGUISliderFloat.h"
#include "IMGUISliderInt.h"
// Engine
#include "AnimationEditor.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Animation/AnimationSequenceInstance.h"
#include "../EditorUtil.h"
#include "Engine.h"
#include "Device.h"
#include "../Object/Anim3DObject.h"
#include "Render/RenderManager.h"
#include "Resource/ResourceManager.h"
#include "../AnimationInstanceConvertThread.h"

CAnimationEditor::CAnimationEditor() :
	m_RenderTargetSet(false)
{
}

CAnimationEditor::~CAnimationEditor()
{
	// 이 Animation 은 실제 3DTestObject 에서 얻어온 Animation 이다. 
	// SAFE_DELETE(m_Animation);


	// Delete 하여 얻어온 BoneKeyFrame 정보를 지워준다.
	// while (!m_StackDeleteFrame.empty())
	// {
	// 	BoneKeyFrame* BoneFrame = m_StackDeleteFrame.top().second;
	// 	m_StackDeleteFrame.pop();
	// 
	// 	for (int i = 0; i < BoneFrame->vecKeyFrame.size(); ++i)
	// 	{
	// 		SAFE_DELETE(BoneFrame->vecKeyFrame[i]);
	// 	}
	// 	/*
	// 	for (int i = 0; i < BoneFrame->vecKeyFrame.size(); ++i)
	// 	{
	// 		--m_vecKeyFrame[i]->iRefCount;
	// 
	// 		if (m_vecKeyFrame[i]->iRefCount == 0)
	// 			delete	m_vecKeyFrame[i];
	// 	}
	// 	*/
	// }
}

bool CAnimationEditor::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	// Anim Name
	// CIMGUILabel* Label = AddWidget<CIMGUILabel>("Anim Name", 100.f, 30.f);
	// Label->SetColor(0, 0, 255);
	// Label->SetAlign(0.5f, 0.0f);

	m_CurAnimComboBox = AddWidget<CIMGUIComboBox>("Anim List Combo Box", 300.f, 30.f);
	m_CurAnimComboBox->SetHideName(true);
	m_CurAnimComboBox->SetSelectCallback<CAnimationEditor>(this, &CAnimationEditor::OnClickAnimationSequence);

	// 별도 Render Target
	m_AnimationRenderTarget = AddWidget<CIMGUIImage>("Render Target", 400.f, 400.f);
	m_AnimationRenderTarget->SetRenderTargetImage(true);
	m_AnimationRenderTarget->SetBorderColor(10, 10, 255);
	m_AnimationRenderTarget->SetTableTitle("Render Target");

	// Clip Info
	m_AnimInfoTable = AddWidget<CIMGUITable>("AnimTable", 600.f, 200.f);

	// Frame Slider 
	m_FrameSlider = AddWidget<CIMGUISliderInt>("Frame Slider", 90.f, 30.f);
	m_FrameSlider->SetCallBack<CAnimationEditor>(this, &CAnimationEditor::OnAnimationSliderIntCallback);

	m_FrameInput = AddWidget<CIMGUITextInput>("Frame Input", 90.f, 30.f);
	m_FrameInput->SetTextType(ImGuiText_Type::Int);
	m_FrameInput->SetCallback<CAnimationEditor>(this, &CAnimationEditor::OnAnimationFrameInputCallback);

	// Edit  -------------------------------------------------------
	// Play Scale 조정
	m_PlayScaleInput = AddWidget<CIMGUITextInput>("Play Scale Input", 50.f, 30.f);
	m_PlayScaleInput->SetHideName(true);
	m_PlayScaleInput->SetTextType(ImGuiText_Type::Float);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(60.f);

	m_PlayScaleEditBtn = AddWidget<CIMGUIButton>("Edit Scale", 90.f, 30.f);
	m_PlayScaleEditBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimPlayScale);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(155.f);

	// Play Time 조정
	m_PlayTimeInput = AddWidget<CIMGUITextInput>("Play Time Input", 50.f, 30.f);
	m_PlayTimeInput->SetHideName(true);
	m_PlayTimeInput->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(210.f);

	m_PlayTimeEditBtn = AddWidget<CIMGUIButton>("Edit Time", 90.f, 30.f);
	m_PlayTimeEditBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimPlayTime);

	// Animation Instance Key Name 수정 기능
	m_EditAnimSeqDataKeyName = AddWidget<CIMGUITextInput>("Edit Sequence Key", 50.f, 30.f);
	m_EditAnimSeqDataKeyName->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(60.f);

	m_EditAnimKeyBtn = AddWidget<CIMGUIButton>("Edit Key", 90.f, 30.f);
	m_EditAnimKeyBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnEditAnimSequenceKey);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	CIMGUIText* HelpText = AddWidget<CIMGUIText>("Anim Key Name Edit Help", 90.f, 30.f);
	HelpText->SetText("ex) 'EditIdle' --> 기존의 m_Animation->AddAnimation('ZedIdle', 'Idle') 으로 인해 만들어진 m_mapAnimationSequence['Idle'] = 'ZedIdle' 을 \n m_mapAnimationSequence['EditIdle'] = 'ZedIdle' 로 Key 값 수정 ");
	HelpText->SetIsHelpMode(true);

	// 각종 체크 박스들 
	m_DeltaTimeCheckBtn = AddWidget<CIMGUICheckBox>("Engine Play", 90.f, 30.f);
	m_DeltaTimeCheckBtn->AddCheckInfo("Engine Play");
	m_DeltaTimeCheckBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnSetPlayEngineDeltaTime);
	// m_DeltaTimeCheckBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnSetPlayEngineDeltaTime);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(110.f);

	m_AnimationCheckBtn = AddWidget<CIMGUICheckBox>("Object Play", 90.f, 30.f);
	m_AnimationCheckBtn->AddCheckInfo("Anim Play");
	m_AnimationCheckBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnPlayAnimation);
	// m_AnimationCheckBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnPlayAnimation);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(205.f);

	m_RotationCheckBtn = AddWidget<CIMGUICheckBox>("Camera Rotation", 90.f, 30.f);
	m_RotationCheckBtn->AddCheckInfo("Rotate");
	m_RotationCheckBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnRotateAnimationCamera);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(280.f);

	m_ZoomEnableBtn = AddWidget<CIMGUICheckBox>("Camera Zoom In", 90.f, 30.f);
	m_ZoomEnableBtn->AddCheckInfo("Zoom");
	m_ZoomEnableBtn->SetCallBackLabel<CAnimationEditor>(this, &CAnimationEditor::OnZoomAnimationCamera);

	// Sequence Make Widgets ----------------------------------------------------------------------------------------------------------------
	HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	HelpText->SetText("ex)  'ZedIdle' -- > pair('ZedIdle', 'ZedIdle.sqc') 형태로 \n SceneResource, ResourceManager의 m_mapSequence 에 저장");
	HelpText->SetIsHelpMode(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	HelpText->SetText("ex) 'Idle' --> m_Animation->AddAnimation('ZedIdle', 'Idle') ? \n ZedIdle Key로 ResourceManager 의 mapSequence 에 저장된 Animation Sequence 를 \n 'Idle' 이라는 이름의 Key값으로 AnimationInstance 에 정보 추가");
	HelpText->SetIsHelpMode(true);

	// Seq Name Inputs
	m_NewAnimSeqName = AddWidget<CIMGUITextInput>("Sequence Name", 90.f, 30.f);
	m_NewAnimSeqName->SetHideName(true);
	m_NewAnimSeqName->SetHintText("Map Key");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_NewAnimSeqDataKeyName = AddWidget<CIMGUITextInput>("Sequence  Key", 90.f, 30.f);
	m_NewAnimSeqDataKeyName->SetHideName(true);
	m_NewAnimSeqDataKeyName->SetHintText("Anim Key");

	// Sequence Related Btns
	HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	HelpText->SetText("같은 경로에 .msh , .bne, .fbm(폴더) 가 존재해야 한다. \n ex) Idle1.sqc 를 Add 하려면, Idle1.msh, Idle1.bne, Idle1.fbm이 같은 경로 내에 존재해야 한다.");
	HelpText->SetIsHelpMode(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(195.f);

	// Animation Related Btns
	HelpText = AddWidget<CIMGUIText>("Instance Save Btn Help Text", 90.f, 30.f);
	HelpText->SetText("Bin\\Animation 폴더에 .anim 파일 확장자로 저장해야 한다.");
	HelpText->SetIsHelpMode(true);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(290.f);

	// Animation Related Btns
	HelpText = AddWidget<CIMGUIText>("Instance Load Btn Help Text", 90.f, 30.f);
	HelpText->SetText(".anim 파일을 Load 하려면, MESH_PATH (Bin//Mesh) 경로에 관련 .msh , .bne, .fbm(폴더) 가 존재해야 한다. \n ex) Alistar.anim 를 Load 하려면, \n MESH_PATH 에 Alistar_Idle.sqc, Alistar_Idle.msh, Alistar_Idle.fbm 등, \n Alistar Animation 과 관련된 파일들이 하나는 존재햐야 한다.");
	HelpText->SetIsHelpMode(true);

	m_AnimSequenceAddBtn = AddWidget<CIMGUIButton>("Add Seq", 90.f, 30.f);
	m_AnimSequenceAddBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnAddAnimationSequence);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(100.f);

	m_DeleteAnimSequenceBtn = AddWidget<CIMGUIButton>("Delete Seq", 90.f, 30.f);
	m_DeleteAnimSequenceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnDeleteAnimationSequenceData);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(195.f);

	m_SaveAnimationInstanceBtn = AddWidget<CIMGUIButton>("Save Instance", 90.f, 30.f);
	m_SaveAnimationInstanceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnSaveAnimationInstance);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(290.f);

	m_LoadAnimationInstanceBtn = AddWidget<CIMGUIButton>("Load Instance", 90.f, 30.f);
	m_LoadAnimationInstanceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnLoadAnimationInstance);

	// Table Key 값 정보 세팅
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameRange);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameLength);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameMode);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::PlayTime);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameTime);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::PlayScale);

	// Animation Instance Convert Widgets ----------------------------------------------------------------------------------------------------------------

	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 200.f, 30.f);

	// m_AnimInstanceConvertThread = new CAnimationInstanceConvertThread;
	// m_AnimInstanceConvertThread->Init();
	// m_AnimInstanceConvertThread->Start();
	// m_AnimInstanceConvertThread->SetLoadingCallBack(this, &CAnimationEditor::OnAnimInstanceConvertLoading);

	// Common Name
	m_CommonAnimSeqName = AddWidget<CIMGUITextInput>("Common Sqc Name", 150.f, 20.f);
	m_CommonAnimSeqName->SetHideName(true);
	m_CommonAnimSeqName->SetHintText("Common Sqc Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(165.f);

	m_SelectAnimInstanceFolderPath = AddWidget<CIMGUIButton>("Select Dir", 90.f, 20.f);
	m_SelectAnimInstanceFolderPath->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnClickSetAnimSeqSrcDirButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(260.f);

	m_ConvertAnimInstanceBtn = AddWidget<CIMGUIButton>("Make Inst", 90.f, 20.f);
	m_ConvertAnimInstanceBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnConvertSequencesIntoAnimationInstance);

	// Folder Path
	m_AnimSeqcSrcFolderPath = AddWidget<CIMGUITextInput>("Sqc Folder Path", 350.f, 20.f);
	m_AnimSeqcSrcFolderPath->ReadOnly(true);
	m_AnimSeqcSrcFolderPath->SetHideName(true);
	m_AnimSeqcSrcFolderPath->SetHintText("Set .sqc Folder Path");

	// Convert
	m_AnimInstanceProgressBar = AddWidget<CIMGUIProgressBar>("", 350.f, 0.f);

	m_AnimInstanceConvertLog = AddWidget<CIMGUIChild>("Log", 200.f, 200.f);
	m_AnimInstanceConvertLog->EnableBorder(true);

	return true;
}

void CAnimationEditor::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	// Animation Slider 증가
	if (m_Animation && m_Animation->GetCurrentAnimation())
	{
		int NextAnimationIdx = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetCurrentFrameIdx();
		OnAnimationSliderIntCallback(NextAnimationIdx);
	}

	// 별도 RenderTarget 세팅 (처음 한번만)
	if (!m_RenderTargetSet)
	{
		m_AnimationRenderTarget->SetTexture(CRenderManager::GetInst()->GetAnimationRenderTarget());
		// m_AnimationRenderTarget->SetTexture(CResourceManager::GetInst()->FindTexture("FinalScreen"));
		m_RenderTargetSet = true;
	}
}

void CAnimationEditor::OnClearExistingAnimationSeqInfos()
{
	if (!m_Animation)
		return;

	m_Animation->ClearAnimationSequenceFromAnimationEditor();

	// Combo Box Clear
	m_CurAnimComboBox->Clear();

	// Table Claer
	m_AnimInfoTable->ClearContents();
}

void CAnimationEditor::OnCreateSample3DObject()
{
	if (m_3DTestObject)
		return;

	m_3DTestObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CAnim3DObject>("3DObject");

	// 3DTest Object의 Animation 정보를 가져온다.
	m_Animation = dynamic_cast<CAnimationMeshComponent*>(m_3DTestObject->GetRootComponent())->CreateBasicAnimationInstance();
}

void CAnimationEditor::OnPlayAnimation(const char* Lable, bool Enable)
{
	if (!m_3DTestObject)
		return;

	CAnimationMeshComponent* RootMeshComponent = dynamic_cast<CAnimationMeshComponent*>(m_3DTestObject->GetRootComponent());

	if (!RootMeshComponent)
		return;

	bool IsAnimPlay = RootMeshComponent->GetAnimationInstance()->IsPlay();

	// if (IsAnimPlay)
	if (!Enable)
		RootMeshComponent->GetAnimationInstance()->Stop();
	else
		RootMeshComponent->GetAnimationInstance()->Play();
}

void CAnimationEditor::OnRotateAnimationCamera(const char*, bool Enable)
{
	if (!m_3DTestObject)
		return;

	// if (IsAnimPlay)
	if (Enable)
		m_3DTestObject->SetCameraRot(true);
	else
		m_3DTestObject->SetCameraRot(false);
}

void CAnimationEditor::OnZoomAnimationCamera(const char*, bool Enable)
{
	if (!m_3DTestObject)
		return;

	// if (IsAnimPlay)
	if (Enable)
		m_3DTestObject->SetCameraZoom(true);
	else
		m_3DTestObject->SetCameraZoom(false);
}

void CAnimationEditor::OnSaveAnimationInstance()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	TCHAR FiileFullPath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
	OpenFile.lpstrFile = FiileFullPath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

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
		if (strcmp(FileExt, ".ANIM") != 0)
		{
			MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT("EXT Has To Be .anim"), NULL, MB_OK);
			return ;
		}

		m_Animation->SetSavedFileName(FileName);

		m_Animation->SaveAnimationFullPath(FileFullPathMultibyte);
	}
}

void CAnimationEditor::OnLoadAnimationInstance()
{
	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("All Files\0*.*\0.Animation File\0*.anim");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char	Ext[_MAX_EXT] = {};

		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, nullptr, 0, Ext, _MAX_EXT);

		_strupr_s(Ext);

		// 확장자 .anim 이 아니라면 return;
		if (strcmp(Ext, ".ANIM") != 0)
			return;

		if (!m_Animation)
		{
			// ex) singed_spell2.sqc 를 선택했다면
			// 같은 폴더 목록 내에서 singed_spell2.msh / singed_spell2.bne 를 Load 하여 세팅한다.
			// singed_spell2.msh -> singed_spell2_mesh 라는 이름으로
			// singed_spell2.bne -> singed_spell2_skeleton 이라는 이름으로
			OnCreateSample3DObject();
		}
		else
		{
			// 기존 Animation List에 보여지던 , 즉, 현재 Animation에 Added 되었던 모든 Sequence 정보를 지워준다
			OnClearExistingAnimationSeqInfos();
		}

		m_Animation->LoadAnimationFullPath(FilePathMultibyte);

		// CurrentAnimation 이 없다면,
		if (!m_Animation->GetCurrentAnimation())
			return;

		// const char* CurSeqFileName = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetSequenceFileNameMultibyte();
		// if (!LoadElementsForSqcLoading(CurSeqFileName))
		// 	return;

		std::pair<bool, std::string> LoadResult = CResourceManager::GetInst()->LoadMeshTextureBoneInfo(m_Animation);

		if (LoadResult.first == false)
			return;

		// 해당 이름을 3d Test Object 에서 가져와서 사용할 것이다.
		m_3DTestObjectMeshName = LoadResult.second;

		// Set Material, Mesh Info
		SetMeshMaterialReadyForAnimation();

		// CameraObject 생성하기
		// CEditorManager::GetInst()->CreateCameraObject();

		// Animation Key Name 조정을 한다. (혹시나 이름이 잘못 저장되어 있을 수도 있으므로 )
		// m_Animation->AdjustSequence2DKeyName();

		// ComboBox 에 모든 Seq 내용을 추가해준다
		m_CurAnimComboBox->Clear();

		int Size = m_Animation->GetAnimationCount();
		std::vector<std::string> vecSeqNames;
		vecSeqNames.reserve(Size);

		m_Animation->GatherSequenceNames(vecSeqNames);

		for (int i = 0; i < Size; i++)
		{
			m_CurAnimComboBox->AddItem(vecSeqNames[i]);
		}

		// Current Animation 정보로 세팅한다.
		int CurAnimIdx = m_Animation->GetCurrentAnimationOrder();
		if (CurAnimIdx == -1)
			return;

		m_CurAnimComboBox->SetSelectIndex(CurAnimIdx);

		// 현재 Select 된 Animation 의 최소, 최대 Frame 으로 Frame Slider 범위 세팅
		m_FrameSlider->SetMin(m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetStartFrame());
		m_FrameSlider->SetMax(m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetEndFrame());

		// Table 정보 세팅
		OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());

		// 현재 Scene에 모든 Sequence 내용을 추가한다.
		// m_Animation->AddAnimationSequenceToSceneResource();

		// 현재 Scene의 정보를 m_Scene으로 지정해준다
		m_Animation->SetScene(CSceneManager::GetInst()->GetScene());

		// Animation Play Scale, Time
		OnRefreshScaleAndTimeInputInfo();

		// Animation을 시작한다..
		m_Animation->Play();
	}
}

void CAnimationEditor::OnAnimationFrameInputCallback()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// 범위를 넘어가면 조정해준다.
	int InputFrame = m_FrameInput->GetValueInt();
	int StFrame = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetStartFrame();
	int EdFrame = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetEndFrame();

	if (InputFrame < StFrame)
		InputFrame = EdFrame;
	else if (InputFrame > EdFrame)
		InputFrame = StFrame;

	m_FrameInput->SetInt(InputFrame);

	// Input 의 Frame 으로 Animation Frame 정보를 세팅해준다.
	// m_Animation->SetCurrentAnimationFrameIdx(InputFrame);
	m_Animation->SetEditorStopTargetFrame(InputFrame);

}

void CAnimationEditor::OnAnimationSliderIntCallback(int CurrentFrame)
{
	m_FrameSlider->SetValue(CurrentFrame);
}

void CAnimationEditor::OnApplyAnimationSlider(CAnimationSequence* Sequence)
{
	if (!Sequence)
		return;

	m_FrameSlider->SetMin(Sequence->GetStartFrame());
	m_FrameSlider->SetMax(Sequence->GetEndFrame());
}

bool CAnimationEditor::LoadElementsForSqcLoading(const char* SqcFileName)
{
	// 만약 Mesh Load 과정에서 필요한 Texture가 없다면 
	// ex) FBX Convert 이후, singed_spell2.sqc 가 있다면, 같은 경로내에 singed_spell2.fbm 이라는 디렉토리가 존재해야 한다.
	// 만약 해당 Folder 가 존재하지 않는다면, Mesh를 Load 하더라도 Texture 가 없다고 뜰 것이다
	char TextFolderExt[10] = ".fbm";
	char TextFolderFullPath[MAX_PATH] = {};

	// RESOURCE_MESH_PATH 에 있는 정보로부터 불러올 것이다.
	char MeshFileFullPath[MAX_PATH] = {};
	
	const PathInfo* Path = CPathManager::GetInst()->FindPath(MESH_PATH);
	
	if (Path)
		strcpy_s(MeshFileFullPath, Path->PathMultibyte);

	strcpy_s(TextFolderFullPath, MeshFileFullPath);
	strcat_s(TextFolderFullPath, SqcFileName);
	strcat_s(TextFolderFullPath, TextFolderExt);

	std::filesystem::path MeshTextureFolderPath(TextFolderFullPath);

	if (!std::filesystem::exists(MeshTextureFolderPath))
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT(".fbm Folder Does Not Exist"), NULL, MB_OK);
		return false;
	}

	// ex) singed_spell2.sqc 를 선택했다면
	// 같은 폴더 목록 내에서 singed_spell2.msh / singed_spell2.bne 를 Load 하여 세팅한다.
	// singed_spell2.msh -> singed_spell2_mesh 라는 이름으로
	// singed_spell2.bne -> singed_spell2_skeleton 이라는 이름으로

	// Load Mesh
	std::string LoadedMeshName = SqcFileName;
	// LoadedMeshName.append("_mesh");

	// 해당 이름을 3d Test Object 에서 가져와서 사용할 것이다.
	m_3DTestObjectMeshName = LoadedMeshName;

	char MeshExt[10] = ".msh";

	char MshFileName[MAX_PATH] = {};
	TCHAR MshTCHARFileName[MAX_PATH] = {};

	strcpy_s(MshFileName, SqcFileName);
	strcat_s(MshFileName, MeshExt);

	int ConvertLength = MultiByteToWideChar(CP_ACP, 0, MshFileName, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, MshFileName, -1, MshTCHARFileName, ConvertLength);

	if (!CSceneManager::GetInst()->GetScene()->GetResource()->LoadMesh(Mesh_Type::Animation, LoadedMeshName,
		MshTCHARFileName, MESH_PATH))
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT(".msh Load Failure"), NULL, MB_OK);
		return false;
	}

	// Bne (Skeleton) Load
	char BneExt[10] = ".bne";

	std::string LoadedBneName = SqcFileName;
	LoadedBneName.append("_skeleton");

	char BneFileName[MAX_PATH] = {};
	TCHAR BneTCHARFileName[MAX_PATH] = {};

	strcpy_s(BneFileName, SqcFileName);
	strcat_s(BneFileName, BneExt);

	ConvertLength = MultiByteToWideChar(CP_ACP, 0, BneFileName, -1, 0, 0);
	MultiByteToWideChar(CP_ACP, 0, BneFileName, -1, BneTCHARFileName, ConvertLength);

	if (!CSceneManager::GetInst()->GetScene()->GetResource()->LoadSkeleton(LoadedBneName, BneTCHARFileName, MESH_PATH))
	{
		MessageBox(CEngine::GetInst()->GetWindowHandle(), TEXT(".bne Load Failure"), NULL, MB_OK);
		return false;
	}

	// Mesh 에 해당 Skeleton 세팅
	CSceneManager::GetInst()->GetScene()->GetResource()->SetMeshSkeleton(LoadedMeshName, LoadedBneName); //


	return true;
}

void CAnimationEditor::SetMeshMaterialReadyForAnimation()
{
	m_3DTestObject->SetMeshAndMaterialInfo();
	m_Animation->Start();
}

void CAnimationEditor::OnEditAnimPlayTime()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// if (m_PlayTimeInput->Empty())
	//	return;

	m_Animation->GetCurrentAnimation()->GetAnimationSequence()->SetPlayTime(m_PlayTimeInput->GetValueFloat());
	m_Animation->GetCurrentAnimation()->SetPlayTime(m_PlayTimeInput->GetValueFloat());

	OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());
}

void CAnimationEditor::OnEditAnimPlayScale()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// if (m_PlayScaleInput->Empty())
	//	return;

	m_Animation->GetCurrentAnimation()->GetAnimationSequence()->SetPlayScale(m_PlayScaleInput->GetValueFloat());
	m_Animation->GetCurrentAnimation()->SetPlayScale(m_PlayScaleInput->GetValueFloat());

	OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());
}

void CAnimationEditor::OnEditAnimSequenceKey()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	// 기존 Animation 이 저장된 Key 값

	// New Key 값
	// EditCurrentSequenceKeyName
	if (!m_Animation->EditCurrentSequenceKeyName(m_EditAnimSeqDataKeyName->GetTextUTF8(), m_CurAnimComboBox->GetSelectItem()))
	{
		assert(false);
		return;
	}

	// 바꿔준 이름의 Animation 을  Current Animation 으로 세팅한다.
	m_Animation->SetCurrentAnimation(m_EditAnimSeqDataKeyName->GetTextUTF8());

	// Combo Box 내용 Refresh
	OnRefreshAnimationComboBox();
}

void CAnimationEditor::OnDeleteAnimationSequenceData()
{
	if (!m_Animation)
		return;

	m_Animation->DeleteCurrentAnimation();

	// Combo Box 내용 Refresh
	OnRefreshAnimationComboBox();
}

void CAnimationEditor::OnClickSetAnimSeqSrcDirButton()
{
	if (m_CommonAnimSeqName->Empty())
		return;

	TCHAR Buf[MAX_PATH] = {};

	BROWSEINFO Bi = {};

	Bi.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	Bi.lpszTitle = TEXT("하나의 Instance 로 만들 Seq 파일들이 있는 폴더 선택");
	Bi.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_VALIDATE;
	Bi.lpfn = BrowseCallbackProc;
	//	Bi.lParam = (LPARAM)InitialPath;

	LPITEMIDLIST ItemIDList = ::SHBrowseForFolder(&Bi);

	if (::SHGetPathFromIDList(ItemIDList, Buf))
	{
		int length = WideCharToMultiByte(CP_ACP, 0, Buf, -1, nullptr, 0, nullptr, 0);
		WideCharToMultiByte(CP_ACP, 0, Buf, -1, m_SelectedSeqSrcsDirPath, length, nullptr, 0);
		// strcat_s(m_SrcDirFullPath, "\\");

		// ExtractAnimationSequenceFilesFullPath(m_SelectedSeqSrcsDirPath, m_vecAnimationSeqFilesFullPath);
		std::vector<std::string> vecSeqFilesFullPath;

		CEditorUtil::GetAllFileFullPathInDir(m_SelectedSeqSrcsDirPath, vecSeqFilesFullPath, ".sqc");
		CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, m_vecAnimationSeqFilesFullPath, m_CommonAnimSeqName->GetTextUTF8());

		// 하나도 찾아내지 못했다면.
		if (m_vecAnimationSeqFilesFullPath.size() == 0)
			return;

		m_AnimSeqcSrcFolderPath->SetText(m_SelectedSeqSrcsDirPath);
	}
}

// m_ConvertAnimInstanceBtn 의 Callback
void CAnimationEditor::OnConvertSequencesIntoAnimationInstance()
{
	if (m_AnimSeqcSrcFolderPath->GetName() == "")
		return;
	
	// 로그창 클리어
	m_AnimInstanceConvertLog->ClearWidget();

	CIMGUIText* StartText = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("Text");
	StartText->SetText("Convert Start...");


	// 모은 모든 녀석들로 Mesh Load 하고 
	size_t Size = m_vecAnimationSeqFilesFullPath.size();

	// 쓰레드에 수행 요청
	for (size_t i = 0; i < Size; ++i)
	{
		m_AnimInstanceConvertThread->AddWork(m_vecAnimationSeqFilesFullPath[i]);
	}
}

void CAnimationEditor::OnAnimInstanceConvertLoading(const LoadingMessage& msg)
{
	m_AnimInstanceProgressBar->SetPercent(msg.Percent);

	CIMGUIText* Text = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("Text");

	std::string LoadingMsg = "Add Success : " + msg.Message;

	Text->SetText(LoadingMsg.c_str());

	if (msg.Complete)
	{
		Text = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("OK");
		Text->SetText("Complete!");
		MessageBox(nullptr, TEXT("Instance Create 완료"), TEXT("완료"), MB_OK);
	}
}

void CAnimationEditor::AddSequenceToDummyAnimationInstance(const char* FileFullPath)
{
	char FileFullPathCopy[MAX_PATH] = {};

	strcpy_s(FileFullPathCopy, FileFullPath);

	char SqcFileName[MAX_PATH] = {};
	char SqcExt[_MAX_EXT] = {};

	_splitpath_s(FileFullPathCopy, nullptr, 0, nullptr, 0, SqcFileName, MAX_PATH, SqcExt, _MAX_EXT);
	
	_strupr_s(SqcExt);
	
	// 확장자 .sqc 이 아니라면 return;
	if (strcmp(SqcExt, ".SQC") != 0)
		return;

	if (!m_Animation)
	{
		std::pair<bool, std::string> LoadResult = CResourceManager::GetInst()->LoadMeshTextureBoneInfo(SqcFileName);
	
		if (!LoadResult.first)
			return;
	
		m_3DTestObjectMeshName = LoadResult.second;
	
		// Create Object
		OnCreateSample3DObject();
	
		// Set Material, Mesh Info
		SetMeshMaterialReadyForAnimation();
	}
	
	// 이름 중복 X --> Key 이름 중복되는 Sequence 는 추가 X 
	// ex) AnimationInstance --> ("ZedIdle", "Idle"); --> "ZedIdle" 이라는 Key 를 지닌 또 다른 Sqc 파일을 로드하면 안된다.
	if (m_Animation->FindAnimation(m_NewAnimSeqDataKeyName->GetTextUTF8()))
		return;
	
	// Animation Seq Load
	std::string FSequenceName;
	
	// Load 한 파일 본연의 이름을 가져와서 세팅한다.
	bool ResultLoadSeq = CSceneManager::GetInst()->GetScene()->GetResource()->LoadAnimationSequenceFullPathMultibyte(true,
		m_NewAnimSeqName->GetTextUTF8(), FileFullPath);
	
	if (!ResultLoadSeq)
		return;
	
	CAnimationSequence* LoadedSequence = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence(m_NewAnimSeqName->GetTextUTF8());
	
	LoadedSequence->SetAnimationFullPathMultibyte(FileFullPath);
	LoadedSequence->SetAnimationFileNameMultibyte(SqcFileName);
	
	// 현재 
	m_Animation->AddAnimation(m_NewAnimSeqName->GetTextUTF8(), m_NewAnimSeqDataKeyName->GetTextUTF8());
	
	// Combo Box 정보 갱신
	OnRefreshAnimationComboBox();
	
	OnRefreshAnimationClipTable(LoadedSequence);
	
	OnRefreshScaleAndTimeInputInfo();
	
	// Frame Slider 의 최대 최소 값 세팅하기
	m_FrameSlider->SetMin(LoadedSequence->GetStartFrame());
	m_FrameSlider->SetMax(LoadedSequence->GetEndFrame());
}

/*
void CAnimationEditor::OnDeleteAnimFrame()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	int DeleteFrameIdx = m_DeleteFrameInput->GetValueInt();

	// if (m_PlayScaleInput->Empty())
	//	return;
	BoneKeyFrame* DeleteBoneFrame = m_Animation->GetCurrentAnimation()->GetAnimationSequence()->DeleteAnimationFrame(DeleteFrameIdx);

	m_StackDeleteFrame.push(std::make_pair(DeleteFrameIdx, DeleteBoneFrame));

	// Frame Slider
	OnRefreshFrameSliderInfo(m_Animation->GetCurrentAnimation()->GetAnimationSequence());

	// Anim Table
	OnRefreshAnimationClipTable(m_Animation->GetCurrentAnimation()->GetAnimationSequence());
}
*/

void CAnimationEditor::OnSetPlayEngineDeltaTime(const char* Lable, bool Enable)
{
	// bool IsEnginePlay = CEngine::GetInst()->IsPlay();

	// if (IsEnginePlay)
	if (!Enable)
		CEngine::GetInst()->SetPlay(false);
	else
		CEngine::GetInst()->SetPlay(true);
}

void CAnimationEditor::OnAddAnimationSequence()
{
	// Add 할 Animation 의 이름을 입력해야 한다
	if (m_NewAnimSeqName->Empty() || m_NewAnimSeqDataKeyName->Empty())
		return;

	TCHAR LoadFilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = LoadFilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.Scene File\0*.scn");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MESH_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char	Ext[_MAX_EXT] = {};
		char FilePathMultibyte[MAX_PATH] = {};
		char SqcFileName[MAX_PATH] = {};

		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		_splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SqcFileName, MAX_PATH, Ext, _MAX_EXT);
		
		_strupr_s(Ext);
		
		// 확장자 .sqc 이 아니라면 return;
		if (strcmp(Ext, ".SQC") != 0)
			return;
		
		if (!m_Animation)
		{
			// f (!LoadElementsForSqcLoading(SqcFileName))
			// 	return;
		
			std::pair<bool, std::string> LoadResult = CResourceManager::GetInst()->LoadMeshTextureBoneInfo(SqcFileName);
		
			if (!LoadResult.first)
				return;
		
			m_3DTestObjectMeshName = LoadResult.second;
		
			// Create Object
			OnCreateSample3DObject();
		
			// Set Material, Mesh Info
			SetMeshMaterialReadyForAnimation();
		}
		
		// 이름 중복 X --> Key 이름 중복되는 Sequence 는 추가 X 
		// ex) AnimationInstance --> ("ZedIdle", "Idle"); --> "ZedIdle" 이라는 Key 를 지닌 또 다른 Sqc 파일을 로드하면 안된다.
		if (m_Animation->FindAnimation(m_NewAnimSeqDataKeyName->GetTextUTF8()))
			return;
		
		// Animation Seq Load
		std::string FSequenceName;
		
		// Load 한 파일 본연의 이름을 가져와서 세팅한다.
		bool ResultLoadSeq = CSceneManager::GetInst()->GetScene()->GetResource()->LoadAnimationSequenceFullPathMultibyte(true,
			m_NewAnimSeqName->GetTextUTF8(), FilePathMultibyte);
		
		if (!ResultLoadSeq)
			return;
		
		CAnimationSequence* LoadedSequence = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence(m_NewAnimSeqName->GetTextUTF8());
		
		LoadedSequence->SetAnimationFullPathMultibyte(FilePathMultibyte);
		LoadedSequence->SetAnimationFileNameMultibyte(SqcFileName);
		
		// 현재 
		m_Animation->AddAnimation(m_NewAnimSeqName->GetTextUTF8(), m_NewAnimSeqDataKeyName->GetTextUTF8());
		
		// Combo Box 정보 갱신
		OnRefreshAnimationComboBox();
		
		OnRefreshAnimationClipTable(LoadedSequence);
		
		OnRefreshScaleAndTimeInputInfo();
		
		// Frame Slider 의 최대 최소 값 세팅하기
		m_FrameSlider->SetMin(LoadedSequence->GetStartFrame());
		m_FrameSlider->SetMax(LoadedSequence->GetEndFrame());
	}
}

void CAnimationEditor::OnClickAnimationSequence(int Index, const char* Name)
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	m_CurAnimComboBox->SetSelectIndex(Index);

	const std::string& SelectSequenceName = m_CurAnimComboBox->GetSelectItem();

	CAnimationSequenceData* SequenceData = m_Animation->FindAnimation(SelectSequenceName);

	if (!SequenceData)
	{
		return;
	}

	// Table 정보 갱신
	OnRefreshAnimationClipTable(SequenceData->GetAnimationSequence());

	// 클릭한 Animation 으로 Current Animation 세팅
	m_Animation->SetCurrentAnimation(Name);
}

void CAnimationEditor::OnRefreshAnimationClipTable(CAnimationSequence* Sequence)
{
	if (!Sequence)
	{
		return;
	}
	m_AnimInfoTable->ClearContents();

	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameRange, Sequence->GetStartFrame());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameRange, Sequence->GetEndFrame());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameLength, Sequence->GetFrameLength());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameMode, Sequence->GetFrameMode());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::PlayTime, Sequence->GetPlayTime());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::FrameTime, Sequence->GetFrameTime());
	m_AnimInfoTable->AddData(AnimationClipInfoKeys::PlayScale, Sequence->GetPlayScale());
}

void CAnimationEditor::OnRefreshFrameSliderInfo(CAnimationSequence* Sequence)
{
	if (!Sequence)
		return;

	m_FrameSlider->SetMin(Sequence->GetStartFrame());
	m_FrameSlider->SetMax(Sequence->GetEndFrame());
}

void CAnimationEditor::OnRefreshAnimationComboBox()
{
	if (!m_Animation)
		return;

	// 맨 처음 Sequence를 추가하는 상황이라면
	int SeletedIdx = m_CurAnimComboBox->GetSelectIndex();

	m_CurAnimComboBox->Clear();

	std::vector<std::string> vecCurKeyNames;

	m_Animation->GatherSequenceNames(vecCurKeyNames);

	for (const auto& name : vecCurKeyNames)
		m_CurAnimComboBox->AddItem(name);

	if (m_CurAnimComboBox->Empty())
		return;

	if (SeletedIdx < m_CurAnimComboBox->GetItemCount())
	{
		// 처음 Animation 을 추가하는 상황이라면
		if (SeletedIdx == -1)
			m_CurAnimComboBox->SetSelectIndex(0);
		else
			m_CurAnimComboBox->SetSelectIndex(SeletedIdx);
	}
	else
		m_CurAnimComboBox->SetSelectIndex(m_CurAnimComboBox->GetItemCount() - 1);

}

void CAnimationEditor::OnRefreshScaleAndTimeInputInfo()
{
	if (!m_Animation || !m_Animation->GetCurrentAnimation())
		return;

	m_PlayScaleInput->SetFloat(m_Animation->GetCurrentAnimation()->GetAnimationPlayScale());
	m_PlayTimeInput->SetFloat(m_Animation->GetCurrentAnimation()->GetAnimationTime());
}
