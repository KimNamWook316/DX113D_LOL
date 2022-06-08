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


	return true;
}

