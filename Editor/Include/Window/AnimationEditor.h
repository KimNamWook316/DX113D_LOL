#pragma once
#include "IMGUIWindow.h"

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
	class CIMGUIComboBox* m_CurAnimComboBox;

	class CIMGUITable* m_AnimInfoTable;

	class CIMGUITextInput* m_FrameInput;
	class CIMGUIButton* m_FrameDelBtn;
	class CIMGUIButton* m_FramePopDelBtn;
	class CIMGUISliderInt* m_FrameSlider;
	
	class CIMGUIButton* m_AnimSeqLoadBtn;
	class CIMGUIButton* m_CreateSample3DBtn;
	class CIMGUIButton* m_DeltaTimePlayBtn;
	class CIMGUIButton* m_AnimationPlayBtn;
	class CIMGUITextInput* m_NewAnimSeqName;
	class CIMGUITextInput* m_NewAnimSeqDataKeyName;
	class CIMGUIButton* m_SaveAnimationInstanceBtn;
	class CIMGUIButton* m_LoadAnimationInstanceBtn;
private :
	class CAnim3DObject* m_3DTestObject;
	class CAnimationSequenceInstance* m_Animation;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private :
	void OnClearExistingAnimationSeqInfos();
private :
	void OnAddAnimationSequence();
	void OnClickAnimationSequence(int, const char*);
	void OnRefreshAnimationClipTable(class CAnimationSequence* SequenceData);
	void OnAnimationSliderIntCallback(int);
	void OnAnimationFrameInputCallback();
	void OnCreateSample3DObject();
	void OnSetPlayEngineDeltaTime();
	void OnPlayAnimation();
	void OnSaveAnimationInstance();
	void OnLoadAnimationInstance();
	void OnApplyAnimationSlider(CAnimationSequence* Sequence);
};

