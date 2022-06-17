// IMGUI
#include "IMGUITextInput.h"
#include "IMGUIInputFloat.h"
#include "IMGUIInputInt.h"
#include "IMGUIInputFloat3.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUIChild.h"
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
#include "IMGUIProgressBar.h"
#include "IMGUITree.h"
// Window
#include "AnimationEditor.h"
#include "../EditorManager.h"
#include "../Window/ResourceDisplayWindow.h"
#include "AnimationDisplayWindow.h"
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
#include "AnimationEditor.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Animation/AnimationSequenceInstance.h"
#include "../EditorUtil.h"
#include "Device.h"
#include "../Object/Anim3DObject.h"
#include "Render/RenderManager.h"
#include "Resource/ResourceManager.h"
#include "../AnimationInstanceConvertThread.h"

CAnimationDisplayWindow::CAnimationDisplayWindow()
{
}

CAnimationDisplayWindow::~CAnimationDisplayWindow()
{
	SAFE_DELETE(m_DummyAnimation);
}

bool CAnimationDisplayWindow::Init()
{
    m_AnimationRenderTarget = AddWidget<CIMGUIImage>("Render Target", 400.f, 400.f);
    m_AnimationRenderTarget->SetRenderTargetImage(true);
    m_AnimationRenderTarget->SetTexture(CRenderManager::GetInst()->GetAnimationRenderTarget());
    m_AnimationRenderTarget->SetBorderColor(10, 10, 255);
    m_AnimationRenderTarget->SetTableTitle("Render Target");

    // Anim Instance Making
	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 100.f, 20.f);
	CIMGUIText* Text = AddWidget<CIMGUIText>("Create Instance", 100.f, 30.f);
	Text->SetText("Create Instance");

	// Common Name
	m_SavedAnimFileName = AddWidget<CIMGUITextInput>("Saved File Name", 150.f, 20.f);
	m_SavedAnimFileName->SetHideName(true);
	m_SavedAnimFileName->SetHintText("Saved File Name");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(160.f);

	CIMGUIText* HelpText = AddWidget<CIMGUIText>("Anim Seq Load Btn Help Text", 90.f, 30.f);
	HelpText->SetText("ZedIdle 이라고 하면, Bin/Animation 폴더에 ZedIdle.anim 이름으로 Animation Instance 저장");
	HelpText->SetIsHelpMode(true);

	m_CommonAnimSeqName = AddWidget<CIMGUITextInput>("Common Sqc Name", 150.f, 20.f);
	m_CommonAnimSeqName->SetHideName(true);
	m_CommonAnimSeqName->SetHintText("Common Sqc Name");

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(165.f);

	m_SelectAnimInstanceFolderPath = AddWidget<CIMGUIButton>("Select Dir", 90.f, 20.f);
	m_SelectAnimInstanceFolderPath->SetClickCallback<CAnimationDisplayWindow>(this, &CAnimationDisplayWindow::OnClickSetAnimSeqSrcDirButton);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(260.f);

	m_ConvertAnimInstanceBtn = AddWidget<CIMGUIButton>("Make Inst", 90.f, 20.f);
	m_ConvertAnimInstanceBtn->SetClickCallback<CAnimationDisplayWindow>(this, &CAnimationDisplayWindow::OnConvertSequencesIntoAnimationInstance);

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



void CAnimationDisplayWindow::OnClickSetAnimSeqSrcDirButton()
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

		// 모든 .sqc 확장자 파일 이름들을 모은다. FullPath 들을 모아둘 것이다.
		std::vector<std::string> vecSeqFilesFullPath;

		// 먼저 해당 .sqc 확장자를 지닌, FullPath 목록들을 vector 형태로 모아둬야 한다.
		// CEditorUtil::GetAllFileNamesInDir(m_SelectedSeqSrcsDirPath, vecSeqFilesFullPath, ".sqc");
		CEditorUtil::GetAllFileFullPathInDir(m_SelectedSeqSrcsDirPath, vecSeqFilesFullPath, ".sqc");

		// vecAllAnimationSeqFilesFullPath 에 특정 문자를 포함하는 (대문자, 소문자 형태 모두) .sqc 파일들을 모두 모아놓을 것이다.
		std::vector<std::string> vecAllAnimationSeqFilesFullPath;
		vecAllAnimationSeqFilesFullPath.reserve(100);

		m_vecAnimationSeqFilesFullPath.clear();
		m_vecAnimationSeqFilesFullPath.reserve(100);

		// 1. Origin 입력 내용 그대로의 파일 이름을 포함하는 파일 들을 모은다
		// ex) "zed" => 모든 "zed~~" 형태의 파일 이름들 추출

		// CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, m_vecAnimationSeqFilesFullPath, m_CommonAnimSeqName->GetTextUTF8());
		CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, m_CommonAnimSeqName->GetTextUTF8());

		// 모든 .sqc 파일들을 m_vecAnimationSeqFilesFullPath 에 모아두기 
		size_t AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

		for (size_t i = 0; i < AllSeqFilesCnt; ++i)
		{
			// 중복 제거
			if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
				continue;

			m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
		}

		// 2. 각각 소문자, 대문자
		//    모두 소문자, 모두 대문자
		std::vector<std::string> vecEachUpperSeqFilesFullPath;
		std::vector<std::string> vecEachLowerSeqFilesFullPath;
		std::string AllUpper;
		std::string AllLower;

		CEditorUtil::GetAllKindsOfTransformedStringVersions(m_CommonAnimSeqName->GetTextUTF8(), vecEachLowerSeqFilesFullPath,
			vecEachUpperSeqFilesFullPath, AllUpper, AllLower);

		// 각각 대문자 목록들 모아두기
		size_t EachUpperTotSize = vecEachUpperSeqFilesFullPath.size();

		for (size_t i = 0; i < EachUpperTotSize; ++i)
		{
			// FilterSpecificNameIncludedFilePath 에서는 vecAllAnimationSeqFilesFullPath 을 비워준다.
			CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, vecEachUpperSeqFilesFullPath[i].c_str());

			AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

			for (size_t i = 0; i < AllSeqFilesCnt; ++i)
			{
				if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
					continue;

				m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
			}
		}

		// 각각 소문자 목록들 모아두기
		size_t EachLowerTotSize = vecEachLowerSeqFilesFullPath.size();

		for (size_t i = 0; i < EachLowerTotSize; ++i)
		{
			// FilterSpecificNameIncludedFilePath 에서는 vecAllAnimationSeqFilesFullPath 을 비워준다.
			CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, vecEachLowerSeqFilesFullPath[i].c_str());

			AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

			for (size_t i = 0; i < AllSeqFilesCnt; ++i)
			{
				if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
					continue;

				m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
			}
		}


		// All 대문자 목록들 모아두기
		CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, AllUpper.c_str());

		AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

		for (size_t i = 0; i < AllSeqFilesCnt; ++i)
		{
			if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
				continue;

			m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
		}

		// All 소문자 목록들 모아두기
		CEditorUtil::FilterSpecificNameIncludedFilePath(vecSeqFilesFullPath, vecAllAnimationSeqFilesFullPath, AllLower.c_str());

		AllSeqFilesCnt = vecAllAnimationSeqFilesFullPath.size();

		for (size_t i = 0; i < AllSeqFilesCnt; ++i)
		{
			if (std::find(m_vecAnimationSeqFilesFullPath.begin(), m_vecAnimationSeqFilesFullPath.end(), vecAllAnimationSeqFilesFullPath[i]) != m_vecAnimationSeqFilesFullPath.end())
				continue;

			m_vecAnimationSeqFilesFullPath.push_back(std::move(vecAllAnimationSeqFilesFullPath[i]));
		}


		// 하나도 찾아내지 못했다면.
		if (m_vecAnimationSeqFilesFullPath.size() == 0)
		{
			MessageBox(nullptr, TEXT("No .sqc Files Found"), TEXT(".sqc File 찾기 실패"), MB_OK);
			return;
		}

		m_AnimSeqcSrcFolderPath->SetText(m_SelectedSeqSrcsDirPath);
	}
}

// m_ConvertAnimInstanceBtn 의 Callback
void CAnimationDisplayWindow::OnConvertSequencesIntoAnimationInstance()
{
	//m_SavedAnimFileName : Instance 를 만든 이후, .anim 파일로 저장할 때의 파일 이름
	if (m_AnimSeqcSrcFolderPath->Empty() || m_SavedAnimFileName->Empty())
		return;

	if (m_vecAnimationSeqFilesFullPath.size() <= 0)
	{
		MessageBox(nullptr, TEXT("해당 Folder 에 해당 Common 이름을 포함한 .sqc 파일 존재 X"), TEXT("Common Name Error"), MB_OK);
		return;
	}
	
	// 다시 한번, 현재 모아둔 m_vecAnimationSeqFilesFullPath 에서, Common Name 이 포함되어 있는지 한번 더 검사한다.
	// 첫번째 File 로만 검사할 것이다.
	if (m_vecAnimationSeqFilesFullPath.size() > 0)
	{
		bool CommonNameIncluded = false;

		// && m_vecAnimationSeqFilesFullPath[0].find(m_CommonAnimSeqName->GetTextUTF8()) == std::string::npos
		std::vector<std::string> vecEachUpperSeqFilesFullPath;
		std::vector<std::string> vecEachLowerSeqFilesFullPath;
		std::string AllUpper;
		std::string AllLower;

		CEditorUtil::GetAllKindsOfTransformedStringVersions(m_CommonAnimSeqName->GetTextUTF8(), vecEachLowerSeqFilesFullPath,
			vecEachUpperSeqFilesFullPath, AllUpper, AllLower);

		size_t EachUpperTotalSize = vecEachUpperSeqFilesFullPath.size();

		for (size_t i = 0; i < EachUpperTotalSize; ++i)
		{
			// 만약에 하나라도 포함한다면, 현재 Folder 경로와 Common Name 이 잘 짝지어진 것이다.
			if (m_vecAnimationSeqFilesFullPath[0].find(vecEachUpperSeqFilesFullPath[i]) != std::string::npos)
			{
				CommonNameIncluded = true;
				break;
			}
		}

		if (!CommonNameIncluded)
		{
			size_t EachLowerTotalSize = vecEachLowerSeqFilesFullPath.size();

			for (size_t i = 0; i < EachLowerTotalSize; ++i)
			{
				// 만약에 하나라도 포함한다면, 현재 Folder 경로와 Common Name 이 잘 짝지어진 것이다.
				if (m_vecAnimationSeqFilesFullPath[0].find(vecEachLowerSeqFilesFullPath[i]) != std::string::npos)
				{
					CommonNameIncluded = true;
					break;
				}
			}
		}

		if (!CommonNameIncluded)
		{
			// 만약에 하나라도 포함한다면, 현재 Folder 경로와 Common Name 이 잘 짝지어진 것이다.
			if (m_vecAnimationSeqFilesFullPath[0].find(AllUpper) != std::string::npos)
				CommonNameIncluded = true;
		}

		if (CommonNameIncluded)
		{
			// 만약에 하나라도 포함한다면, 현재 Folder 경로와 Common Name 이 잘 짝지어진 것이다.
			if (m_vecAnimationSeqFilesFullPath[0].find(AllLower) != std::string::npos)
				CommonNameIncluded = true;
		}

		// 현재 Folder 내 특정 이름을 포함하는 File 들을 모아놨는데
		// 해당 File 들 중, 첫번째 파일이, Common Name 을 Include 하지 않는다는 의미는
		// 중간에 Common NAme 을 바꾸는 등의 변화가 있었다는 것이다.
		if (!CommonNameIncluded)
		{
			MessageBox(nullptr, TEXT("Folder 경로와, Common 이름을 확인하세요. (중간에 Common 이름을 변경했는지 확인)"), TEXT("Common Name Error"), MB_OK);
			return;
		}
	}

	// 로그창 클리어
	m_AnimInstanceConvertLog->ClearWidget();

	CIMGUIText* StartText = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("Text");
	StartText->SetText("Convert Start...");


	// 모은 모든 녀석들로 Mesh Load 하고 
	size_t Size = m_vecAnimationSeqFilesFullPath.size();

	CIMGUIText* Text = nullptr;

	for (size_t i = 0; i < Size; ++i)
	{
		AddSequenceToDummyAnimationInstance(m_vecAnimationSeqFilesFullPath[i].c_str());

		// File 이름 목록에 추가
		Text = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("Text");

		Text->SetText(CEditorUtil::FilterFileName(m_vecAnimationSeqFilesFullPath[i]).c_str());
	}

	m_AnimInstanceProgressBar->SetPercent(100.f);

	Text = m_AnimInstanceConvertLog->AddWidget<CIMGUIText>("OK");
	Text->SetText("Complete!");
	MessageBox(nullptr, TEXT("Instance Create 완료"), TEXT("완료"), MB_OK);

	// Dummy Animation Intance 를 저장한다.
	if (!m_DummyAnimation)
		return;

	const PathInfo* Path = CPathManager::GetInst()->FindPath(ANIMATION_PATH);

	char SavedFileNameMultibyte[MAX_PATH] = {};

	if (Path)
		strcpy_s(SavedFileNameMultibyte, Path->PathMultibyte);

	strcat_s(SavedFileNameMultibyte, m_SavedAnimFileName->GetTextUTF8());
	strcat_s(SavedFileNameMultibyte, ".anim");

	m_DummyAnimation->SetSavedFileName(m_SavedAnimFileName->GetTextUTF8());
	m_DummyAnimation->SaveAnimationFullPath(SavedFileNameMultibyte);

	SAFE_DELETE(m_DummyAnimation);
}

void CAnimationDisplayWindow::OnAnimInstanceConvertLoading(const LoadingMessage& msg)
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

		// Dummy Animation Intance 를 저장한다.
		if (!m_DummyAnimation)
			return;

		m_DummyAnimation->SaveAnimationFullPath(m_SelectedSeqSrcsDirPath);

		SAFE_DELETE(m_DummyAnimation);
	}
}

void CAnimationDisplayWindow::AddSequenceToDummyAnimationInstance(const char* FileFullPath)
{
	if (!m_DummyAnimation)
	{
		m_DummyAnimation = new CAnimationSequenceInstance;

		if (!m_DummyAnimation->Init())
		{
			SAFE_DELETE(m_DummyAnimation);
			return;
		}
	}

	char FileFullPathCopy[MAX_PATH] = {};

	strcpy_s(FileFullPathCopy, FileFullPath);

	char SqcFileName[MAX_PATH] = {};
	char SqcExt[_MAX_EXT] = {};

	_splitpath_s(FileFullPathCopy, nullptr, 0, nullptr, 0, SqcFileName, MAX_PATH, SqcExt, _MAX_EXT);

	_strupr_s(SqcExt);

	// 확장자 .sqc 이 아니라면 return;
	if (strcmp(SqcExt, ".SQC") != 0)
		return;


	// FileFullPath 에서 File 이름으로 Key 값을 지정해줄 것이다.
	// 중복 방지 
	if (m_DummyAnimation->FindAnimation(SqcFileName))
		return;

	// Animation Seq Load
	std::string FSequenceName;

	// Load 한 파일 본연의 이름을 가져와서 세팅한다.
	bool ResultLoadSeq = CSceneManager::GetInst()->GetScene()->GetResource()->LoadAnimationSequenceFullPathMultibyte(true,
		SqcFileName, FileFullPath);

	if (!ResultLoadSeq)
		return;

	CAnimationSequence* LoadedSequence = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence(SqcFileName);

	LoadedSequence->SetAnimationFullPathMultibyte(FileFullPath);
	LoadedSequence->SetAnimationFileNameMultibyte(SqcFileName);

	// ex) File 이름이 Single_Idle.sqc 라면
	// Resource Manager, Animation Instance 에 모두
	// 동일하게 Single_Idle 이라는 Key 값으로 해당 정보를 세팅할 것이다.
	m_DummyAnimation->AddAnimation(SqcFileName, SqcFileName);
}
