#pragma once

#include "GameInfo.h"

enum class EditMode
{
	Scene,
	Sprite,
	TileMap
};

class CEditorManager
{
private:
	EditMode	m_EditMode;
	class CDragObject* m_DragObj;
	//class CSpriteWindow* m_SpriteWindow;
	//class CDetailWindow* m_DetailWindow;
	//class CEditorMenu* m_EditorMenu;
	//class CObjectHierarchy* m_ObjectHierarchy;
	//class CTileMapWindow* m_TileMapWindow;
	class CFileBrowser* m_FileBrowser;
	class CFileBrowserTree* m_FileBrowserTree;
	class CObjectHierarchyWindow* m_ObjectHierarchyWindow;
	class CSceneComponentHierarchyWindow* m_ComponentHierarchyWindow;
	class CObjectComponentWindow* m_ObjectComponentWindow;
	class CAnimationEditor* m_AnimationEditor;
	class CEffectEditor* m_EffectEditor;
	class CInspectorWindow* m_InspectorWindow;
	class CToolWindow* m_ToolWindow;
	class CBehaviorTreeMenuBar* m_BehaviorTreeMenuBar;
	class CBaseMenuBar* m_BaseMenuBar;
	class CMaterialEditor* m_MaterialEditor;
	class CResourceDisplayWindow* m_ResourceDisplayWindow;
	class C3DCameraObject* m_CameraObject;

	bool				m_MousePush;
	float				m_CameraMoveSpeed;

public:
	bool GetLButtonPush()	const
	{
		return m_MousePush;
	}
	class CDragObject* GetDragObj()	const
	{
		return m_DragObj;
	}
	class CObjectHierarchyWindow* GetObjectHierarchyWindow()	const
	{
		return m_ObjectHierarchyWindow;
	}
	class CObjectComponentWindow* GetObjectComponentWindow()	const
	{
		return m_ObjectComponentWindow;
	}
	class CSceneComponentHierarchyWindow* GetComponentHierarchyWindow()	const
	{
		return m_ComponentHierarchyWindow;
	}
	class CEffectEditor* GetEffectEditor() const
	{
		return m_EffectEditor;
	}
	class CMaterialEditor* GetMaterialEditor() const
	{
		return m_MaterialEditor;
	}
	class CResourceDisplayWindow* GetResourceDisplayWindow() const
	{
		return m_ResourceDisplayWindow;
	}
	class CAnimationEditor* GetAnimationEditor()	const
	{
		return m_AnimationEditor;
	}
	class C3DCameraObject* Get3DCameraObject()	const
	{
		return m_CameraObject;
	}
	EditMode GetEditMode()
	{
		return m_EditMode;
	}

	void SetEditMode(EditMode Mode);

public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

public:
	void MouseLButtonDown(float DeltaTime);
	void MouseLButtonPush(float DeltaTime);
	void MouseLButtonUp(float DeltaTime);

	void KeyboardUp(float DeltaTime);
	void KeyboardDown(float DeltaTime);
	void KeyboardLeft(float DeltaTime);
	void KeyboardRight(float DeltaTime);

public:
	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);
	class CComponent* CreateComponent(class CGameObject* Obj, size_t Type);
	void CreateAnimInstance(class CSpriteComponent* Sprite, size_t Type);
	
public:
	void SetChampionNotify(class CAnimationSequenceInstance* Instance, const std::string& ChampionName);

private:
	void CreateEditorCamera();
	void ReadChampionNotify();
	void ReadChampionSkillInfo();

	DECLARE_SINGLE(CEditorManager)
};

