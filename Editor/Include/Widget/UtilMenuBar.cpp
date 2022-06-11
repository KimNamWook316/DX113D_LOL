#include "UtilMenuBar.h"
#include "../Window/FBXConvertWindow.h"
#include "../Window/FileBrowser.h"
#include "../Window/FileBrowserTree.h"
#include "IMGUIMenuItem.h"
#include "../EditorManager.h"

CUtilMenuBar::CUtilMenuBar()
{
}

CUtilMenuBar::~CUtilMenuBar()
{
}

bool CUtilMenuBar::Init()
{
	// Scene
	m_FBXConvertWindow = AddMenuItem("FBX Convert");
	m_FBXConvertWindow->SetClickCallBack(this, &CUtilMenuBar::OnToggleFBXConvertWindow);

	// File Browser
	m_FileBrowserTree = AddMenuItem("File Browse");
	m_FileBrowserTree->SetClickCallBack(this, &CUtilMenuBar::OnToggleFileBrowser);

	// Object
	m_FileBrowser = AddMenuItem("Tree File Browse");
	m_FileBrowser->SetClickCallBack(this, &CUtilMenuBar::OnToggleFileBrowserTree);

	return true;
}

void CUtilMenuBar::Render()
{
	CIMGUIBeginMenu::Render();
}

void CUtilMenuBar::OnToggleFBXConvertWindow()
{
	bool IsOpen = CEditorManager::GetInst()->GetFBXConvertWindow()->IsOpen();

	if (IsOpen)
	{
		CEditorManager::GetInst()->GetFBXConvertWindow()->Close();
	}
	else
	{
		CEditorManager::GetInst()->GetFBXConvertWindow()->Open();
	}
}

void CUtilMenuBar::OnToggleFileBrowser()
{
	bool IsOpen = CEditorManager::GetInst()->GetFileBrowser()->IsOpen();

	if (IsOpen)
	{
		CEditorManager::GetInst()->GetFileBrowser()->Close();
	}
	else
	{
		CEditorManager::GetInst()->GetFileBrowser()->Open();
	}
}

void CUtilMenuBar::OnToggleFileBrowserTree()
{
	bool IsOpen = CEditorManager::GetInst()->GetFileBrowserTree()->IsOpen();

	if (IsOpen)
	{
		CEditorManager::GetInst()->GetFileBrowserTree()->Close();
	}
	else
	{
		CEditorManager::GetInst()->GetFileBrowserTree()->Open();
	}
}

