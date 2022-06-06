#pragma once

#include "IMGUIPopUpModal.h"

class CObjectCreateModal :
    public CIMGUIPopUpModal
{
public:
	CObjectCreateModal();
	virtual ~CObjectCreateModal();

private:
	//class CIMGUIPopUpModal* m_ObjectCreatePopUp;
	class CIMGUIComboBox* m_ObjectCombo;
	class CIMGUITextInput* m_NameTextInput;
	class CIMGUIButton* m_ObjectCreateButton;
	class CIMGUICheckBox* m_ObjectTypeCheckBox;
	class CIMGUICheckBox* m_EnemyCheckBox;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	std::string GetObjectNameInput()	const;

	// 오브젝트 생성 팝업창내에서 Create버튼 누르면 호출되는 콜백
	void OnCreateObject();

	// Load Object 시에, Type Index 를 받아서 Object 를 만들어내는 기능
	class CGameObject* OnCreateObject(const char* FullPathMultibyte);

	void SetObjectType(int Index, bool Boolean);
};

