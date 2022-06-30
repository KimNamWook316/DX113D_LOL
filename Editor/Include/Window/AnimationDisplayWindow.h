#pragma once
#include "IMGUIWindow.h"

class CAnimationDisplayWindow :
    public CIMGUIWindow
{
    friend class CEditorManager;
public:
    CAnimationDisplayWindow();
    ~CAnimationDisplayWindow();
public:
    virtual bool Init() override;
	// Animation Render Target
private:
    class CIMGUIImage* m_AnimationRenderTarget;
	// Anim Instance Make
private:
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
private:
	// FullPath 목록 모아둔 Vector 
	std::vector<std::string> m_vecAnimationSeqFilesFullPath;

	// FileName 목록 모아둔 Vector
	// std::vector<std::string> m_vecAnimationSeqFileNames;

	char m_SelectedSeqSrcsDirPath[MAX_PATH];

	class CAnimationSequenceInstance* m_DummyAnimation;

private :
	void OnAnimInstanceConvertLoading(const LoadingMessage& msg);
	void OnClickSetAnimSeqSrcDirButton();
	void OnConvertSequencesIntoAnimationInstance();
private :
	void AddSequenceToDummyAnimationInstance(const char* FileFullPath);
	bool CheckSavedFileNameDuplicated();
	bool CheckGatheredSeqFilesIncludeCommonName();
};

