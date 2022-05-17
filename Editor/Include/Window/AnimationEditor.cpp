#include "AnimationEditor.h"
#include "IMGUIButton.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUILabel.h"
#include "IMGUIListBox.h"
#include "IMGUISameLine.h"
#include "IMGUIText.h"
#include "IMGUITable.h"
#include "IMGUITextInput.h"
#include "../EditorManager.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "IMGUISliderFloat.h"
#include "Animation/AnimationSequenceInstance.h"

CAnimationEditor::CAnimationEditor()
{
}

CAnimationEditor::~CAnimationEditor()
{
}

bool CAnimationEditor::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	// Anim Name
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Anim Name", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(150.f);

	m_CurAnimComboBox = AddWidget<CIMGUIComboBox>("Anim List Combo Box", 300.f, 30.f);
	m_CurAnimComboBox->SetHideName(true);
	m_CurAnimComboBox->SetSelectCallback<CAnimationEditor>(this, &CAnimationEditor::OnClickAnimationSequence);

	// Animation Clip + Table
	Label = AddWidget<CIMGUILabel>("Anim Clip Info", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	m_AnimInfoTable = AddWidget<CIMGUITable>("AnimTable", 600.f, 200.f);

	// Frame Slide 
	Label = AddWidget<CIMGUILabel>("Frame Slide", 100.f, 30.f);
	Label->SetColor(0, 0, 255);
	Label->SetAlign(0.5f, 0.0f);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(150.f);

	m_FrameSlider = AddWidget<CIMGUISliderFloat>("Frame Slider", 100.f, 30.f);

	// Btns
	m_AnimSeqLoadBtn = AddWidget<CIMGUIButton>("AddSequences", 100.f, 30.f);
	m_AnimSeqLoadBtn->SetClickCallback<CAnimationEditor>(this, &CAnimationEditor::OnAddAnimationSequence);

	m_NewAnimSeqName = AddWidget<CIMGUITextInput>("New Anim Name", 200.f, 30.f);;


	// Animation Instance 객체 생성
	m_Animation = new CAnimationSequenceInstance;

	/*
	if (m_Skeleton)
	m_Animation->SetSkeleton(m_Skeleton);
	*/

	if (!m_Animation->Init())
	{
		SAFE_DELETE(m_Animation);
		return false;
	}

	// Table Key 값 정보 세팅
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameRange);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameLength);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameMode);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::PlayTime);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::FrameTime);
	m_AnimInfoTable->MakeKey(AnimationClipInfoKeys::PlayScale);


	return true;
}

void CAnimationEditor::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CAnimationEditor::OnAddAnimationSequence()
{
	// Add 할 Animation 의 이름을 입력해야 한다
	if (m_NewAnimSeqName->Empty())
		return;

	// 이름 중복 X
	if (m_Animation->FindAnimation(m_NewAnimSeqName->GetTextUTF8()))
		return;

	TCHAR LoadFilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = LoadFilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.Scene File\0*.scn");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		// Animation Seq Load
		std::string SequenceName;
		bool ResultLoadSeq = CSceneManager::GetInst()->GetScene()->GetResource()->LoadAnimationSequenceFullPathMultibyteSetOriginFileName(true,
			SequenceName, FilePathMultibyte);

		if (!ResultLoadSeq)
			return;

		CAnimationSequence* LoadedSequence = CSceneManager::GetInst()->GetScene()->GetResource()->FindAnimationSequence(SequenceName);
		
		if (!m_Animation)
		{
			m_Animation = new CAnimationSequenceInstance;

			if (!m_Animation->Init())
			{
				SAFE_DELETE(m_Animation);
				return;
			}
		/*
			if (m_Skeleton)
			m_Animation->SetSkeleton(m_Skeleton);
			*/

			// Table 목록에 Animation Key 목록 추가
			// MakeKey
		}

		m_Animation->AddAnimation(m_NewAnimSeqName->GetTextUTF8(), LoadedSequence->GetName());

		// Combo Box 정보 갱신
		std::vector<std::string> vecSequenceNames = {};
		m_Animation->GetAnimationSequenceNames(vecSequenceNames);

		m_CurAnimComboBox->Clear();

		for (const auto& name : vecSequenceNames)
			m_CurAnimComboBox->AddItem(name);

		// Load 한 Sequence 로 내용 Setting 되기
		m_CurAnimComboBox->SetSelectIndex(m_CurAnimComboBox->GetItemCount() - 1);

		OnRefreshAnimationClipTable(LoadedSequence);
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

	OnRefreshAnimationClipTable(SequenceData->GetAnimationSequence());

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
