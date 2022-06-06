#include "InspectorWindow.h"
#include "../Widget/GameObjectWidget.h"
#include "IMGUISeperator.h"
#include "IMGUIText.h"

CInspectorWindow::CInspectorWindow()	:
	m_ObjWidget(nullptr)
{
}

CInspectorWindow::~CInspectorWindow()
{
}

bool CInspectorWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUIText* Text = AddWidget<CIMGUIText>("Text");
	Text->SetText("Inspector");
	AddWidget<CIMGUISeperator>("Sep");

	m_ObjWidget = AddWidget<CGameObjectWidget>("GameObjectInspector");
	m_ObjWidget->SetRender(false);

	return true;
}

void CInspectorWindow::OnSelectGameObject(CGameObject* Obj)
{
	// 현재 선택된 게임오브젝트가 다시 선택된 경우
	if (m_ObjWidget->GetGameObject() == Obj)
	{
		return;
	}

	// Object Widget에 GameObject 세팅하면서 Inspector 갱신 
	m_ObjWidget->SetRender(true);
	m_ObjWidget->ClearComponentWidget();
	m_ObjWidget->SetGameObject(Obj);
}

void CInspectorWindow::OnCreateSceneComponent(CSceneComponent* Com)
{
	m_ObjWidget->CreateSceneComponentWidget(Com);
}

void CInspectorWindow::OnCreateObjectComponent(CObjectComponent* Com)
{
	m_ObjWidget->CreateObjectComponentWidget(Com);
}

void CInspectorWindow::OnClearGameObject()
{
	m_ObjWidget->SetRender(false);
	m_ObjWidget->ClearComponentWidget();
	m_ObjWidget->SetGameObject(nullptr);
}

void CInspectorWindow::OnDeleteSceneComponent(CSceneComponent* Com)
{
	m_ObjWidget->DeleteSceneComponentWidget(Com);
}

void CInspectorWindow::OnDeleteGameObject()
{
	DeleteWidget("GameObjectInspector");
	m_ObjWidget = nullptr;
}
