#pragma once
#include "IMGUIWindow.h"

class CMaterialEditor :
    public CIMGUIWindow
{
	friend class CEditorManager;
	// friend class CAnimationInstanceConvertThread;
public:
	CMaterialEditor();
	~CMaterialEditor();
private :
	class CMaterial* m_SelectedMaterial;
private :
	class CIMGUIText* m_SelectedMaterialName;  // 현재 선택된 Material 이름
	class CIMGUITextInput* m_NewMaterialName; // 동시에 파일 이름으로 저장될 것이다.

	class CIMGUIButton* m_CreateMaterialBtn;
	class CIMGUIButton* m_SaveMaterialBtn;
	class CIMGUIButton* m_LoadMaterialBtn;

	class CIMGUIListBox* m_MadeMaterialList;

	// Shader 를 Drag, Drop 할 수 있는 장소

	// Texture 목록을 보여주는 장소
	class CIMGUITableElemList* m_TextureInfoTable;

	// Drop 해서 Set Texture 하기

	// Drop 해서 Add Texture 하기 

	// Drop 해서 Image 보여주는 Image Widget


	// 남욱씨가 말한 내용들 Setting 하는 장소
	class CIMGUICheckBox* m_OutLineCheck;
	class CIMGUIColor4* m_OutLineColor;
	class CIMGUIInputFloat* m_OutLineThickNess;
public:
	virtual bool Init();
public :
	void SetSelectMaterial(class CMaterial* Material)
	{
		m_SelectedMaterial = Material;
	}
	void OnSaveMaterialCallback();
	void OnLoadMaterialCallback();
	void OnCreateMaterialCallback();
};

