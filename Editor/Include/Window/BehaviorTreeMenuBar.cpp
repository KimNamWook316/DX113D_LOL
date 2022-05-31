
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
	if (!m_TreeEditorWindow)
	{
		GraphEditorDelegate Delegate;

		size_t Count = Delegate.GetTemplateCount();

		for (size_t i = 0; i < Count; ++i)
		{
			GraphEditor::Template tmp = Delegate.GetTemplate(i);
			if (tmp.mInputNames)
			{
				SAFE_DELETE_ARRAY(tmp.mInputNames);
			}

			if (tmp.mOutputNames)
			{
				SAFE_DELETE_ARRAY(tmp.mOutputNames);
			}
		}

		Delegate.m_IsCleared = true;
	}
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
								m_TreeEditorWindow->GetStateComponent()->SetTreeUpdate(false);
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
