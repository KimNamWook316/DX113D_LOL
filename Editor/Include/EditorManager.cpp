#include "EditorManager.h"
#include "Engine.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/DefaultScene.h"
#include "Scene/CameraManager.h"
#include "Component/CameraComponent.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "Object/DragObject.h"
#include "Render/RenderManager.h"
#include "Object/SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Object/Player2D.h"
#include "Engine.h"

#include "Window/ObjectHierarchyWindow.h"
#include "Window/SceneComponentHierarchyWindow.h"
#include "Window/ObjectComponentWindow.h"
#include "Window/FileBrowser.h"
#include "Window/FileBrowserTree.h"
#include "Window/AnimationEditor.h"
#include "Window/FBXConvertWindow.h"
#include "Window/InspectorWindow.h"
#include "Window/ToolWindow.h"

#include "Object/3DCameraObject.h"

DEFINITION_SINGLE(CEditorManager)

CEditorManager::CEditorManager() :
	m_EditMode(EditMode::Scene),
	m_DragObj(nullptr),
	m_CameraMoveSpeed(1000.f),
	m_CameraObject(nullptr)
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

void CEditorManager::SetEditMode(EditMode Mode)
{
	m_EditMode = Mode;

	switch (m_EditMode)
	{
	case EditMode::Scene:
		break;
	case EditMode::Sprite:
		if (m_DragObj)
			m_DragObj->Destroy();
		m_DragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
		m_DragObj->SetWorldScale(0.f, 0.f, 1.f);
		break;
	case EditMode::TileMap:
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}
		break;
	}

	if (m_EditMode == EditMode::Sprite)
	{
	}
}

bool CEditorManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
		1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}


	CSceneManager::GetInst()->SetCreateSceneModeFunction<CEditorManager>(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectFunction<CEditorManager>(this, &CEditorManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentFunction<CEditorManager>(this, &CEditorManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceFunction<CEditorManager>(this, &CEditorManager::CreateAnimInstance);

	CResourceManager::GetInst()->LoadTexture(DIRECTORY_IMAGE, TEXT("Directory.png"));
	CResourceManager::GetInst()->LoadTexture(FILE_IMAGE, TEXT("FileImage.png"));

	//m_FileBrowser = CIMGUIManager::GetInst()->AddWindow<CFileBrowser>(FILE_BROWSER);
	m_ObjectHierarchyWindow = CIMGUIManager::GetInst()->AddWindow<CObjectHierarchyWindow>(OBJECT_HIERARCHY);
	m_ComponentHierarchyWindow = CIMGUIManager::GetInst()->AddWindow<CSceneComponentHierarchyWindow>(SCENECOMPONENT_HIERARCHY);
	m_ObjectComponentWindow = CIMGUIManager::GetInst()->AddWindow<CObjectComponentWindow>(OBJECTCOMPONENT_LIST);
	//m_FileBrowserTree = CIMGUIManager::GetInst()->AddWindow<CFileBrowserTree>(FILE_BROWSERTREE);
	m_InspectorWindow = CIMGUIManager::GetInst()->AddWindow<CInspectorWindow>(INSPECTOR);
	m_ToolWindow = CIMGUIManager::GetInst()->AddWindow<CToolWindow>(TOOL);
	//m_AnimationEditor = CIMGUIManager::GetInst()->AddWindow<CAnimationEditor>(ANIMATION_EDITOR);

	//CFBXConvertWindow* win = CIMGUIManager::GetInst()->AddWindow<CFBXConvertWindow>(FBX_CONVERTOR);

	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);

	CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Down, this, &CEditorManager::MouseLButtonDown);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Push, this, &CEditorManager::MouseLButtonPush);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Up, this, &CEditorManager::MouseLButtonUp);


	CInput::GetInst()->CreateKey("Up", VK_UP);
	CInput::GetInst()->CreateKey("Down", VK_DOWN);
	CInput::GetInst()->CreateKey("Left", VK_LEFT);
	CInput::GetInst()->CreateKey("Right", VK_RIGHT);

	CInput::GetInst()->SetKeyCallback("Up", KeyState_Push, this, &CEditorManager::KeyboardUp);
	CInput::GetInst()->SetKeyCallback("Down", KeyState_Push, this, &CEditorManager::KeyboardDown);
	CInput::GetInst()->SetKeyCallback("Left", KeyState_Push, this, &CEditorManager::KeyboardLeft);
	CInput::GetInst()->SetKeyCallback("Right", KeyState_Push, this, &CEditorManager::KeyboardRight);

	CInput::GetInst()->CreateKey("MoveUp", 'W');
	CInput::GetInst()->CreateKey("MoveDown", 'S');
	CInput::GetInst()->CreateKey("RotationZInv", 'A');
	CInput::GetInst()->CreateKey("RotationZ", 'D');

	m_CameraObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DCameraObject>("EditorCamera");

	return true;
}

void CEditorManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CDefaultScene>();
}

int CEditorManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CEditorManager::MouseLButtonDown(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->SetStartPos(CInput::GetInst()->GetMousePos());
	}
}

void CEditorManager::MouseLButtonPush(float DeltaTime)
{
	m_MousePush = true;

	if (m_DragObj)
	{
		m_DragObj->SetEndPos(CInput::GetInst()->GetMousePos());
	}
}

void CEditorManager::MouseLButtonUp(float DeltaTime)
{
	m_MousePush = false;
}

void CEditorManager::KeyboardUp(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(0.f, 1.f, 0.f);

	else
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		Camera->AddWorldPos(Vector3(0.f, m_CameraMoveSpeed * DeltaTime, 0.f));
	}
}

void CEditorManager::KeyboardDown(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(0.f, -1.f, 0.f);

	else
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		Camera->AddWorldPos(Vector3(0.f, -m_CameraMoveSpeed * DeltaTime, 0.f));
	}
}

void CEditorManager::KeyboardLeft(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(-1.f, 0.f, 0.f);

	else
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		Camera->AddWorldPos(Vector3(-m_CameraMoveSpeed * DeltaTime, 0.f, 0.f));
	}
}

void CEditorManager::KeyboardRight(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(1.f, 0.f, 0.f);

	else
	{
		CCameraComponent* Camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		Camera->AddWorldPos(Vector3(m_CameraMoveSpeed * DeltaTime, 0.f, 0.f));
	}
}

void CEditorManager::CreateSceneMode(CScene* Scene, size_t Type)
{
	if (Type == typeid(CDefaultScene).hash_code())
	{
		Scene->LoadSceneMode<CDefaultScene>();
	}
}

CGameObject* CEditorManager::CreateObject(CScene* Scene, size_t Type)
{
	if (Type == typeid(CGameObject).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CGameObject>();

		return Obj;
	}

	else if (Type == typeid(CDragObject).hash_code())
	{
		CDragObject* Obj = Scene->LoadGameObject<CDragObject>();

		return Obj;
	}

	else if (Type == typeid(CSpriteEditObject).hash_code())
	{
		CSpriteEditObject* Obj = Scene->LoadGameObject<CSpriteEditObject>();

		return Obj;
	}

	else if (Type == typeid(CPlayer2D).hash_code())
	{
		CPlayer2D* Obj = Scene->LoadGameObject<CPlayer2D>();

		return Obj;
	}

	return nullptr;
}

CComponent* CEditorManager::CreateComponent(CGameObject* Obj, size_t Type)
{
	if (Type == typeid(CSceneComponent).hash_code())
	{
		CComponent* Component = Obj->LoadComponent<CSceneComponent>();

		return Component;
	}

	else if (Type == typeid(CSpriteComponent).hash_code())
	{
		CComponent* Component = Obj->LoadComponent<CSpriteComponent>();

		return Component;
	}

	else if (Type == typeid(CStaticMeshComponent).hash_code())
	{
		CComponent* Component = Obj->LoadComponent<CStaticMeshComponent>();

		return Component;
	}

	else if (Type == typeid(CTileMapComponent).hash_code())
	{
		CTileMapComponent* Component = Obj->LoadComponent<CTileMapComponent>();

		Component->EnableEditMode(true);

		return Component;
	}

	return nullptr;
}

void CEditorManager::CreateAnimInstance(CSpriteComponent* Sprite, size_t Type)
{
	if (Type == typeid(CAnimationSequence2DInstance).hash_code())
	{
		Sprite->LoadAnimationInstance<CAnimationSequence2DInstance>();
	}
}

void CEditorManager::CreateEditorCamera()
{
}

