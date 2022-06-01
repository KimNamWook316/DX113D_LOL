#include "GameObjectWidget.h"
#include "GameObject/GameObject.h"
#include "IMGUITextInput.h"
#include "IMGUIText.h"
#include "IMGUIButton.h"
#include "IMGUICheckBox.h"
#include "IMGUISameLine.h"
#include "IMGUISeperator.h"
#include "IMGUIDummy.h"
#include "Component/StaticMeshComponent.h"
#include "Component/LightComponent.h"
#include "Component/ParticleComponent.h"
#include "../Widget/StaticMeshComponentWidget.h"
#include "../Widget/LightComponentWidget.h"
#include "../Widget/ObjectComponentWidget.h"
#include "../Widget/ParticleComponentWidget.h"
#include "../Window/ObjectHierarchyWindow.h"
#include "IMGUIManager.h"
#include "../EditorInfo.h"

CGameObjectWidget::CGameObjectWidget()	:
	m_NameInput(nullptr),
	m_RenameButton(nullptr),
	m_EnableCheckBox(nullptr)
{
}

CGameObjectWidget::~CGameObjectWidget()
{
}

bool CGameObjectWidget::Init()
{
	// AddWidget
	CIMGUIText* Text = AddWidget<CIMGUIText>("Text");
	Text->SetText("GameObject Name");

	AddWidget<CIMGUISameLine>("Line");

	m_EnableCheckBox = AddWidget<CIMGUICheckBox>("ObjEnable");
	m_EnableCheckBox->AddCheckInfo("Enable");

	m_NameInput = AddWidget<CIMGUITextInput>("Name");
	AddWidget<CIMGUISameLine>("Line");
	m_RenameButton = AddWidget<CIMGUIButton>("Rename", 0.f, 0.f);

	AddWidget<CIMGUISeperator>("Sep");

	// CallBack
	m_RenameButton->SetClickCallback(this, &CGameObjectWidget::OnClickRenameButton);
	m_EnableCheckBox->SetCallBackIdx(this, &CGameObjectWidget::OnCheckEnableCheckBox);

	return true;
}

void CGameObjectWidget::ClearComponentWidget()
{
	auto iter = m_SceneComponentWidgetList.begin();
	auto iterEnd = m_SceneComponentWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(*iter);
	}

	m_SceneComponentWidgetList.clear();

	auto iterO = m_ObjectComponentWidgetList.begin();
	auto iterOEnd = m_ObjectComponentWidgetList.end();

	for (; iterO != iterOEnd; ++iterO)
	{
		SAFE_DELETE(*iterO);
	}

	m_ObjectComponentWidgetList.clear();
	
	mVecChild.resize(7);
}

void CGameObjectWidget::SetGameObject(CGameObject* Obj)
{
	m_Object = Obj;

	if (Obj)
	{
		m_EnableCheckBox->SetCheck(0, m_Object->IsEnable());
		m_NameInput->SetText(m_Object->GetName().c_str());

		// Component Pointer들 받아와 Type에 맞는 Widget들 생성
		std::vector<CSceneComponent*> vecSceneComp;
		m_Object->GetAllSceneComponentsPointer(vecSceneComp);

		size_t Size = vecSceneComp.size();
		for (size_t i = 0; i < Size; ++i)
		{
			CreateSceneComponentWidget(vecSceneComp[i]);
		}

		std::vector<CObjectComponent*> vecObjComp;
		m_Object->GetAllObjectComponentsPointer(vecObjComp);

		Size = vecObjComp.size();
		for (size_t i = 0; i < Size; ++i)
		{
			CreateObjectComponentWidget(vecObjComp[i]);
		}
	}
}

void CGameObjectWidget::CreateSceneComponentWidget(CSceneComponent* Com)
{
	size_t TypeID = Com->GetTypeID();

	CSceneComponentWidget* Widget = nullptr;

	// TODO : 컴포넌트별 위젯 추가
	if (TypeID == typeid(CStaticMeshComponent).hash_code())
	{
	 	Widget = AddWidget<CStaticMeshComponentWidget>("StaticMeshWidget");
	}
	else if (TypeID == typeid(CLightComponent).hash_code())
	{
	 	Widget = AddWidget<CLightComponentWidget>("LightWidget");
	}
	else if (TypeID == typeid(CParticleComponent).hash_code())
	{
		Widget = AddWidget<CParticleComponentWidget>("ParticleWidget");
	}
	else
	{
	 	Widget = AddWidget<CSceneComponentWidget>("SceneWidget");
	}

	// Component 넣어주면서 내부 Widget들 생성
	Widget->SetSceneComponent(Com);

	m_SceneComponentWidgetList.push_back(Widget);
}

void CGameObjectWidget::CreateObjectComponentWidget(CObjectComponent* Com)
{
	size_t TypeID = Com->GetTypeID();

	// TODO : 컴포넌트별 위젯 추가
}

void CGameObjectWidget::OnClickRenameButton()
{
	m_Object->SetName(m_NameInput->GetTextMultibyte());

	// Hierachy 갱신
	CObjectHierarchyWindow* Hierachy = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);
	Hierachy->OnRenameObject(m_NameInput->GetTextMultibyte());
}

void CGameObjectWidget::OnCheckEnableCheckBox(int Idx, bool Check)
{
	m_Object->Enable(Check);

	auto iter = m_SceneComponentWidgetList.begin();
	auto iterEnd = m_SceneComponentWidgetList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->OnGameObjectEnable(Check);
	}

	auto iterO = m_ObjectComponentWidgetList.begin();
	auto iterOEnd = m_ObjectComponentWidgetList.end();

	for (; iterO != iterOEnd; ++iterO)
	{
		(*iterO)->OnGameObjectEnable(Check);
	}
}
