#pragma once
#include "IMGUIPopUpModal.h"
class CSceneComponentCreateModal :
    public CIMGUIPopUpModal
{
public:
	CSceneComponentCreateModal();
	virtual ~CSceneComponentCreateModal();

private:

	class CIMGUIComboBox* m_ComponentCombo;
	class CIMGUITextInput* m_NameTextInput;
	class CIMGUIButton* m_ComponentCreateButton;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	std::string GetComponentNameInput()	const;

	// Component 생성 팝업창내에서 Create버튼 누르면 호출되는 콜백
	void OnCreateComponent();

	// Load Object 시에 호출하는 함수 -> 이미 Load 된 GameObject 의 Root Component 로 Widget 및 트리 구성
	void OnLoadComponent(class CGameObject* SelectObject);
	void OnLoadComponentRecursive(class CGameObject* SelectObject);
	void OnLoadChildComponentRecursive(class CSceneComponent* ParentComp, class CIMGUITree* ParentTreeNode);

	// 부모 Component가 있다면 부모 Component로 하여금 나를 자식 목록에서 지우도록 하는 함수
	void DisconnetParent();
	// 새로운 부모 밑으로 들어간다면 새로운 부모 Component로 하여금 나를 자식 목록에서 추가하도록 하는 함수
	void SetParent();
};

