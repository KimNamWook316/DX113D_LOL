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
// Window
#include "MaterialEditor.h"

CMaterialEditor::CMaterialEditor()
{
}

CMaterialEditor::~CMaterialEditor()
{
}

bool CMaterialEditor::Init()
{
	// class CIMGUIText* m_SelectedMaterialName;  // 현재 선택된 Material 이름
	// class CIMGUITextInput* m_NewMaterialName; // 동시에 파일 이름으로 저장될 것이다.
	
	m_NewMaterialName = AddWidget<CIMGUITextInput>("New Material Name", 150.f, 20.f);
	m_NewMaterialName->SetHideName(true);


	// class CIMGUIButton* m_CreateMaterialBtn;
	// class CIMGUIButton* m_SaveMaterialBtn;
	// class CIMGUIButton* m_LoadMaterialBtn;
	// 
	// class CIMGUIListBox* m_MadeMaterialList;
	// 
	// // Shader 를 Drag, Drop 할 수 있는 장소
	// 
	// // Texture 목록을 보여주는 장소
	// class CIMGUITableElemList* m_TextureInfoTable;
	// 
	// // Drop 해서 Set Texture 하기
	// 
	// // Drop 해서 Add Texture 하기 
	// 
	// // Drop 해서 Image 보여주는 Image Widget
	// 
	// 
	// // 남욱씨가 말한 내용들 Setting 하는 장소
	// class CIMGUICheckBox* m_OutLineCheck;
	// class CIMGUIColor4* m_OutLineColor;
	// class CIMGUIInputFloat* m_OutLineThickNess;

	return true;
}

void CMaterialEditor::OnSaveMaterialCallback()
{
}

void CMaterialEditor::OnLoadMaterialCallback()
{
}

void CMaterialEditor::OnCreateMaterialCallback()
{
}
