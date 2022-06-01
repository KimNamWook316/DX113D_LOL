#pragma once

#include "IMGUIWindow.h"
#include "IMGUIListBox.h"

class CObjectComponentWindow :
    public CIMGUIWindow
{
public:
	CObjectComponentWindow();
	virtual ~CObjectComponentWindow();

private:
	class CIMGUIButton*					m_ComponentCreatePopUpButton;
	class CIMGUIButton*					m_ComponentDeleteButton;
	class CObjectComponentCreateModal*	m_ComponentCreateModal;
	class CIMGUIListBox*				m_ComponentListBox;
	int m_SelectIndex;
	std::string m_SelectLabel;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	void OnRenameComponent(const std::string& NewName, const std::string& PrevName);
	// Component 생성 팝업창 띄워주는 콜백
	void OnCreateComponentPopUp();
	std::string GetComponentNameInput()	const;
	int AddObjectComponent(const std::string& Name);
	void OnSelectComponent(int Index, const char* Label);
	void OnDeleteComponent();

public:
	int GetSelectIndex()	const
	{
		return m_SelectIndex;
	}

	std::string GetSelectLabel()	const
	{
		return m_SelectLabel;
	}



public:
	template <typename T>
	void SetSelectCallback(int Index, void(T::*Func)(int, const char*))
	{
		m_ComponentListBox->SetSelectCallback(this, &CObjectComponentWindow::OnSelectComponent);
	}
	// List에서 Select된 Component 선형 탐색
	//void FindSelectComponent(class CIMGUITree* RootNode);
};

