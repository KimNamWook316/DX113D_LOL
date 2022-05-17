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


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	// Component ���� �˾�â ����ִ� �ݹ�
	void OnCreateComponentPopUp();
	std::string GetComponentNameInput()	const;
	int AddObjectComponent(const std::string& Name);
	void OnSelectComponent(int Index, const char* Label);
	void OnDeleteComponent();

public:
	template <typename T>
	void SetSelectCallback(int Index, void(T::*Func)(int, const char*))
	{
		m_ComponentListBox->SetSelectCallback(this, &CObjectComponentWindow::OnSelectComponent);
	}
	// List���� Select�� Component ���� Ž��
	//void FindSelectComponent(class CIMGUITree* RootNode);
};

