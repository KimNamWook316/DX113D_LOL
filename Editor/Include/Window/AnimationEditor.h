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
	class CIMGUISliderFloat* m_FrameSlider;
	
	class CIMGUIButton* m_AnimSeqLoadBtn;
	class CIMGUITextInput* m_NewAnimSeqName;
private :
	class CAnimationSequenceInstance* m_Animation;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private :
	void OnAddAnimationSequence();
	void OnClickAnimationSequence(int, const char*);
	void OnRefreshAnimationClipTable(class CAnimationSequence* SequenceData);
};

