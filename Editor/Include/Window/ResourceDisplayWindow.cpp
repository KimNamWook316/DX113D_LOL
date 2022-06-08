// IMGUI
#include "IMGUIWidgetList.h"
#include "IMGUIText.h"
// Window
#include "ResourceDisplayWindow.h"

CResourceDisplayWindow::CResourceDisplayWindow()
{
}

CResourceDisplayWindow::~CResourceDisplayWindow()
{
}

bool CResourceDisplayWindow::Init()
{
	m_TextureList = AddWidget<CIMGUIWidgetList>("Set Texture", 100.f, 20.f);
	m_TextureList->SetApplyHideEffect(true);

	CIMGUIText* AddedText = m_TextureList->AddWidget<CIMGUIText>("Sample Text");
	AddedText->SetText("Hello");

	m_vecResourceTapList.push_back(m_TextureList);

	GatherLoadedTextureResources();
	GatherLoadedMaterialResources();

	return true;
}

void CResourceDisplayWindow::GatherLoadedTextureResources()
{
	// Texture Manager 에 Load 된 Texture 목록만 세팅할 것이다.
}

void CResourceDisplayWindow::GatherLoadedMaterialResources()
{
}

