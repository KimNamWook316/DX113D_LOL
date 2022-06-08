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
	class CIMGUITextInput* m_NewMaterialName; // 동시에 파일 이름으로 저장될 것이다.
	class CIMGUIButton* m_CreateMaterialBtn;

	class CIMGUITextInput* m_SelectedMaterialName;  // 현재 선택된 Material 이름

	// class CIMGUIListBox* m_MadeMaterialList;

	// 해당 Material 의 정보를 보여주는 Info Table
	class CIMGUITable* m_MtrlInfoTable;

	// Texture 목록을 보여주는 장소
	class CIMGUITableElemList* m_TextureInfoTable;

	// 남욱씨가 말한 내용들 Setting 하는 장소
	class CIMGUICheckBox* m_OutLineCheck;
	class CIMGUIColor3* m_OutLineColor;
	class CIMGUIInputFloat* m_OutLineThickNess;

	// Shader 를 Drag, Drop 할 수 있는 장소
	class CIMGUITextInput* m_ShaderSetInput;

	// Drop 해서 Set Texture 하기
	class CIMGUITextInput* m_AddTextureInput; // 동시에 파일 이름으로 저장될 것이다.
	class CIMGUIButton* m_AddTextureBtn;

	// Drop 해서 Add Texture 하기 
	class CIMGUIInputInt* m_TextureIndex; // 동시에 파일 이름으로 저장될 것이다.
	class CIMGUITextInput* m_SetTextureInput; // 동시에 파일 이름으로 저장될 것이다.
	class CIMGUIButton* m_SetTextureBtn;

	// Drop 해서 Image 보여주는 Image Widget
	// 각종 Btn 들
	class CIMGUIButton* m_SaveMaterialBtn;
	class CIMGUIButton* m_LoadMaterialBtn;
public:
	virtual bool Init();
public :
	void SetSelectMaterial(class CMaterial* Material)
	{
		m_SelectedMaterial = Material;
	}
private :
	void OnCreateMaterialCallback();

	void OnDropAndCreateMaterialCallback(const std::string&);

	void OnIsOutLineEdit(const char*, bool Enable);
	void OnSetOutLineColor(const Vector3& Color);
	void OnSetOutLineThickNess(float ThickNess);

	void OnSetTextureBtn();
	void OnAddTextureBtn();

	void OnSaveMaterial();
	void OnLoadMaterial();

	void RefreshMaterialDisplayInfo(class CMaterial* Material);
};

