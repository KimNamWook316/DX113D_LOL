#pragma once
#include "IMGUIWindow.h"
#include "../EditorInfo.h"
#include <filesystem>


class CAnimationEditor :
	public CIMGUIWindow
{
	friend class CEditorManager;
	// friend class CAnimationInstanceConvertThread;
public:
	CAnimationEditor();
	~CAnimationEditor();

private:
	// Anim Combo Box
	class CIMGUIComboBox* m_CurAnimComboBox;
	// Anim Clip Table
	class CIMGUITable* m_AnimInfoTable;
	// Frame 조정
	class CIMGUITextInput* m_FrameInput;
	class CIMGUISliderInt* m_FrameSlider;
	// class CIMGUIButton* m_FrameDeleteBtn;
	// class CIMGUITextInput* m_DeleteFrameInput;
	// Animation Seq 추가
	class CIMGUIButton* m_AnimSequenceAddBtn;
	class CIMGUIButton* m_CreateSample3DBtn;
	// Animation Seq 지우기
	class CIMGUIButton* m_DeleteAnimSequenceBtn;
	// Animation Play 여부
	class CIMGUICheckBox* m_DeltaTimeCheckBtn;
	class CIMGUICheckBox* m_AnimationCheckBtn;
	class CIMGUICheckBox* m_RotationCheckBtn;
	class CIMGUICheckBox* m_ZoomEnableBtn;
	class CIMGUICheckBox* m_LoopEnableBtn;
	// New Anim Names
	class CIMGUITextInput* m_NewAnimSeqName;
	class CIMGUITextInput* m_NewAnimSeqDataKeyName;
	class CIMGUITextInput* m_EditAnimSeqDataKeyName;
	class CIMGUIButton* m_EditAnimKeyBtn;
	// Save Load
	class CIMGUIButton* m_SaveAnimationInstanceBtn;
	class CIMGUIButton* m_LoadAnimationInstanceBtn;
	// Play Scale, Time
	class CIMGUITextInput* m_PlayScaleInput;
	class CIMGUIButton* m_PlayScaleEditBtn;
	class CIMGUITextInput* m_PlayTimeInput;
	class CIMGUIButton* m_PlayTimeEditBtn;
	// Render Target
	bool m_RenderTargetSet;
	class CIMGUIImage* m_AnimationRenderTarget;
private:
	class CAnim3DObject* m_3DTestObject;
	std::string m_3DTestObjectMeshName;
	class CAnimationSequenceInstance* m_Animation;
	// --------------------------------------------------------------------
private :
	class CAnimationInstanceConvertThread* m_AnimInstanceConvertThread;
	// 지정된 Folder
	class CIMGUITextInput* m_AnimSeqcSrcFolderPath;
	// 공통 File Name
	class CIMGUITextInput* m_CommonAnimSeqName;
	// Convert Btn
	class CIMGUIButton* m_ConvertAnimInstanceBtn;
	// Folder 지정
	class CIMGUIButton* m_SelectAnimInstanceFolderPath;
	// Log
	class CIMGUIChild* m_AnimInstanceConvertLog;
	// ProgressBar
	class CIMGUIProgressBar* m_AnimInstanceProgressBar;
	// 저장할 Animation File Name
	class CIMGUITextInput* m_SavedAnimFileName;
private :
	std::vector<std::string> m_vecAnimationSeqFilesFullPath;
	char m_SelectedSeqSrcsDirPath[MAX_PATH];
	// Animation Instance 를 File형태로 저장하기 위해 임시적으로 사용하는 Dummy Animation Instance
	class CAnimationSequenceInstance* m_DummyAnimation;
public:
	const std::string& Get3DTestObjectMeshName() const
	{
		return m_3DTestObjectMeshName;
	}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private:
	// Refresh 
	void OnRefreshAnimationClipTable(class CAnimationSequence* SequenceData);
	void OnRefreshFrameSliderInfo(class CAnimationSequence* SequenceData);
	void OnRefreshAnimationComboBox();
	void OnRefreshScaleAndTimeInputInfo();
	void OnClearExistingAnimationSeqInfos();
	void OnAddAnimationSequence();
	void OnClickAnimationSequence(int, const char*);
	void OnAnimationSliderIntCallback(int);
	void OnAnimationFrameInputCallback();
	// Play
	void OnCreateSample3DObject();
	void OnSetPlayEngineDeltaTime(const char*, bool);
	void OnPlayAnimation(const char*, bool);
	void OnRotateAnimationCamera(const char*, bool);
	void OnZoomAnimationCamera(const char*, bool);
	// Loop
	void OnLoopAnimation(const char*, bool);
	// Save Load
	void OnSaveAnimationInstance();
	void OnLoadAnimationInstance();
	void OnApplyAnimationSlider(CAnimationSequence* Sequence);
	bool LoadElementsForSqcLoading(const char* SqcFileName);
	void SetMeshMaterialReadyForAnimation();
	// Edit
	void OnEditAnimPlayTime();
	void OnEditAnimPlayScale();
	void OnEditAnimSequenceKey();
	void OnDeleteAnimationSequenceData(); // 특정 Animation 지우기
	// Animation Sequence Delete -> 현재 코드상 BoneKeyFrame 을 하나 제거하는 형태이다.
	// 그런데 이렇게 하면 Animation 이 이상하게 동작하게 된다. 아마도 
	// void OnDeleteAnimFrame();

	// Convert Animation Instance  Functions
private :
	void OnClickSetAnimSeqSrcDirButton();
	void OnConvertSequencesIntoAnimationInstance();
	void OnAnimInstanceConvertLoading(const LoadingMessage& msg);
	// Helper Functions
private :
	void AddSequenceToDummyAnimationInstance(const char* FileFullPath);
};

