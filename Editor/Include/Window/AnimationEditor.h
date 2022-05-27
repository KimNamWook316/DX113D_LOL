#pragma once
#include "IMGUIWindow.h"
#include <filesystem>

namespace AnimationClipInfoKeys
{
	const std::string FrameRange = "Frame Range";
	const std::string FrameLength = "Frame Length";
	const std::string FrameMode = "Frame Mode"; // 24, 30, 60
	const std::string PlayTime = "Play Time";
	const std::string FrameTime = "Frame Time";
	const std::string PlayScale = "Play Scale";
};

class CAnimationEditor :
    public CIMGUIWindow
{
	friend class CEditorManager;
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
private :
	class CAnim3DObject* m_3DTestObject;
	std::string m_3DTestObjectMeshName;
	class CAnimationSequenceInstance* m_Animation;
	class std::stack<std::pair<int, class BoneKeyFrame*>> m_StackDeleteFrame;
public :
	const std::string& Get3DTestObjectMeshName() const
	{
		return m_3DTestObjectMeshName;
	}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private :
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
	// Save Load
	void OnSaveAnimationInstance();
	void OnLoadAnimationInstance();
	void OnApplyAnimationSlider(CAnimationSequence* Sequence);
	bool LoadElementsForSqcLoading(const char* SqcFileName);
	// Edit
	void OnEditAnimPlayTime();
	void OnEditAnimPlayScale();
	void OnEditAnimSequenceKey();
	void OnDeleteAnimationSequenceData(); // 특정 Animation 지우기
	// Animation Sequence Delete -> 현재 코드상 BoneKeyFrame 을 하나 제거하는 형태이다.
	// 그런데 이렇게 하면 Animation 이 이상하게 동작하게 된다. 아마도 
	// void OnDeleteAnimFrame();
};

