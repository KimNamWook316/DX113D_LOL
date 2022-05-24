
#include "BehaviorTreeMenuBar.h"
#include "IMGUIBeginMenu.h"
#include "../EditorUtil.h"
#include "IMGUIPopUpModal.h"

CBehaviorTreeMenuBar::CBehaviorTreeMenuBar()	:
	m_NodeEditorModal(nullptr)
{
}

CBehaviorTreeMenuBar::~CBehaviorTreeMenuBar()
{
}

bool CBehaviorTreeMenuBar::Init()
{
	if (!CIMGUIWindow::Init())
		return false;


	return true;
}

void CBehaviorTreeMenuBar::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("BehaviorTree"))
		{
			if (!m_NodeEditorModal)
			{
				m_NodeEditorModal = AddWidget<CIMGUIPopUpModal>("NodeEditorModal");
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	//CEditorUtil::ShowDemo();
}
