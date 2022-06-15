#include "DataMenu.h"
#include "IMGUIMenuItem.h"
#include "../EditorManager.h"
#include "../Window/CollisionProfileEditor.h"

CDataMenu::CDataMenu()	:
	m_CollisionProfileToggle(nullptr)
{
}

CDataMenu::~CDataMenu()
{
}

bool CDataMenu::Init()
{
	m_CollisionProfileToggle = AddMenuItem("Collision Profile");
	m_CollisionProfileToggle->SetClickCallBack(this, &CDataMenu::OnToggleCollisionProfileToggle);

	return true;
}

void CDataMenu::OnToggleCollisionProfileToggle()
{
	bool IsOpen = CEditorManager::GetInst()->GetCollisionProfileEditor()->IsOpen();

	if (IsOpen)
	{
		CEditorManager::GetInst()->GetCollisionProfileEditor()->Close();
	}
	else
	{
		CEditorManager::GetInst()->GetCollisionProfileEditor()->Open();
	}
}
