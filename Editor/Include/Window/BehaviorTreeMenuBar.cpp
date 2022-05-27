
#include "BehaviorTreeMenuBar.h"
#include "IMGUIBeginMenu.h"
#include "../EditorUtil.h"
#include "IMGUIPopUpModal.h"
#include "IMGUIManager.h"
#include "BehaviorTreeWindow.h"
#include "ObjectComponentWindow.h"
#include "IMGUIManager.h"
#include "ObjectHierarchyWindow.h"
#include "GameObject/GameObject.h"
#include "Component/StateComponent.h"
#include "Component/AnimationMeshComponent.h"

CBehaviorTreeMenuBar::CBehaviorTreeMenuBar()	:
	m_TreeEditorWindow(nullptr)
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
	//CIMGUIWindow::Update(DeltaTime);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("BehaviorTree"))
		{
			if (ImGui::MenuItem("Open Tree Editor"))
			{
				CObjectComponentWindow* ComponentWindow = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);
				CObjectHierarchyWindow* ObjectWindow = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

				if (ComponentWindow && ObjectWindow)
				{
					CGameObject* Obj = ObjectWindow->GetSelectObject();

					if (Obj)
					{
						CComponent* Component = Obj->FindObjectComponentFromType<CStateComponent>();
						CAnimationMeshComponent* AnimMeshComp = Obj->FindComponentFromType<CAnimationMeshComponent>();


						// Object Component Window에서 StateComponent가 선택됐을때만 Behavior Tree Window 열어주기
						if (Component && AnimMeshComp)
						{
							if (!m_TreeEditorWindow)
							{
								m_TreeEditorWindow = CIMGUIManager::GetInst()->AddWindow<CBehaviorTreeWindow>(BEHAVIORTREE_WINDOW);
								m_TreeEditorWindow->Open();
								m_TreeEditorWindow->SetStateComponent((CStateComponent*)Component);
								((CStateComponent*)Component)->SetAnimationMeshComponent(AnimMeshComp);
							}

							else
							{
								m_TreeEditorWindow->Open();
							}
						}
					}
				}

			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//CEditorUtil::ShowDemo();
}
