#include "EditorManager.h"
#include "Engine.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/DefaultScene.h"
#include "Scene/CameraManager.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Render/RenderManager.h"
// Component
#include "Component/CameraComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/StateComponent.h"
#include "Component/LandScape.h"
#include "Component/StateComponent.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Component/BuildingComponent.h"
// Window
#include "Window/ObjectHierarchyWindow.h"
#include "Window/SceneComponentHierarchyWindow.h"
#include "Window/ObjectComponentWindow.h"
#include "Window/FileBrowser.h"
#include "Window/FileBrowserTree.h"
#include "Window/AnimationEditor.h"
#include "Window/FBXConvertWindow.h"
#include "Window/InspectorWindow.h"
#include "Window/EffectEditor.h"
#include "Window/ToolWindow.h"
#include "Window/BehaviorTreeMenuBar.h"
#include "Window/BaseMenuBar.h"
#include "Window/MaterialEditor.h"
#include "Window/ResourceDisplayWindow.h"
// Object
#include "Object/DragObject.h"
#include "Object/SpriteEditObject.h"
#include "Object/Player2D.h"
#include "Object/3DCameraObject.h"

#include "Component/State/StateManager.h"

#include <sstream>

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
	CEngine::GetInst()->EnableEditMode();

	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
		1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

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
	CInput::GetInst()->CreateKey("SkillQ", 'Q');
	CInput::GetInst()->CreateKey("SkillW", 'W');
	CInput::GetInst()->CreateKey("SkillE", 'E');
	CInput::GetInst()->CreateKey("SkillR", 'R');
	CInput::GetInst()->CreateKey("SpellD", 'D');
	CInput::GetInst()->CreateKey("SpellF", 'F');

	m_CameraObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<C3DCameraObject>("EditorCamera");
	m_CameraObject->SetNoDestroyOnSceneChange(true);

	CSceneManager::GetInst()->SetCreateSceneModeFunction<CEditorManager>(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectFunction<CEditorManager>(this, &CEditorManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentFunction<CEditorManager>(this, &CEditorManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceFunction<CEditorManager>(this, &CEditorManager::CreateAnimInstance);

	CResourceManager::GetInst()->LoadTexture(DIRECTORY_IMAGE, TEXT("Directory.png"));
	CResourceManager::GetInst()->LoadTexture(FILE_IMAGE, TEXT("FileImage.png"));

	m_ObjectHierarchyWindow = CIMGUIManager::GetInst()->AddWindow<CObjectHierarchyWindow>(OBJECT_HIERARCHY);
	m_ComponentHierarchyWindow = CIMGUIManager::GetInst()->AddWindow<CSceneComponentHierarchyWindow>(SCENECOMPONENT_HIERARCHY);
	m_ObjectComponentWindow = CIMGUIManager::GetInst()->AddWindow<CObjectComponentWindow>(OBJECTCOMPONENT_LIST);

	m_InspectorWindow = CIMGUIManager::GetInst()->AddWindow<CInspectorWindow>(INSPECTOR);

	m_FileBrowserTree = CIMGUIManager::GetInst()->AddWindow<CFileBrowserTree>(FILE_BROWSERTREE);
	m_FileBrowser = CIMGUIManager::GetInst()->AddWindow<CFileBrowser>(FILE_BROWSER);
	
	m_AnimationEditor = CIMGUIManager::GetInst()->AddWindow<CAnimationEditor>(ANIMATION_EDITOR);
	m_AnimationEditor->Close();

	m_EffectEditor = CIMGUIManager::GetInst()->AddWindow<CEffectEditor>(PARTICLE_EDITOR);
	m_EffectEditor->Close();

	m_ToolWindow = CIMGUIManager::GetInst()->AddWindow<CToolWindow>(TOOL);

	m_FBXConvertWindow = CIMGUIManager::GetInst()->AddWindow<CFBXConvertWindow>(FBX_CONVERTOR);

	m_BaseMenuBar = CIMGUIManager::GetInst()->AddWindow<CBaseMenuBar>("BehaviorTree");
	
	m_MaterialEditor = CIMGUIManager::GetInst()->AddWindow<CMaterialEditor>("MaterialEditor");
	m_MaterialEditor->Close();
	
	m_ResourceDisplayWindow = CIMGUIManager::GetInst()->AddWindow<CResourceDisplayWindow>("Resources");
	m_ResourceDisplayWindow->Close();

	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);
	// ���� ���澾 Behavior TreeMenu Bar
	// m_BehaviorTreeMenuBar = CIMGUIManager::GetInst()->AddWindow<CBehaviorTreeMenuBar>("BehaviorTree");

	ReadChampionSkillInfo();
	ReadChampionNotify();

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
	else if (Type == typeid(CParticleComponent).hash_code())
	{
		CParticleComponent* Component = Obj->LoadComponent<CParticleComponent>();
		// Component->EnableEditMode(true);
		return Component;
	}
	else if (Type == typeid(CAnimationMeshComponent).hash_code())
	{
		CAnimationMeshComponent* Component = Obj->LoadComponent<CAnimationMeshComponent>();
		// Component->EnableEditMode(true);
		return Component;
	}
	else if (Type == typeid(CLandScape).hash_code())
	{
		CLandScape* Component = Obj->LoadComponent<CLandScape>();
		CSceneManager::GetInst()->GetScene()->GetNavigation3DManager()->SetLandScape(Component);
		// Component->EnableEditMode(true);
		return Component;
	}

	else if (Type == typeid(CStateComponent).hash_code())
	{
		CStateComponent* Component = Obj->LoadObjectComponent<CStateComponent>();
		return Component;
	}

	else if (Type == typeid(CNavAgent).hash_code())
	{
		CNavAgent* Component = Obj->LoadObjectComponent<CNavAgent>();
		return Component;
	}

	else if (Type == typeid(CColliderBox3D).hash_code())
	{
		CColliderBox3D* Component = Obj->LoadComponent<CColliderBox3D>();
		// Component->EnableEditMode(true);
		return Component;
	}

	else if (Type == typeid(CColliderSphere).hash_code())
	{
		CColliderSphere* Component = Obj->LoadComponent<CColliderSphere>();
		// Component->EnableEditMode(true);
		return Component;
	}

	else if (Type == typeid(CBuildingComponent).hash_code())
	{
		CBuildingComponent* Component = Obj->LoadComponent<CBuildingComponent>();
		// Component->EnableEditMode(true);
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

void CEditorManager::SetChampionNotify(CAnimationSequenceInstance* Instance, const std::string& ChampionName)
{
	CExcelData* Data = CResourceManager::GetInst()->FindCSV("AnimationNotify");

	if (!Data)
		return;

	CStateManager* StateManager = CSceneManager::GetInst()->GetStateManager();
	
	// TODO : è�Ǿ�� ��ų�� �߰��ɶ����� ���⿡ Notify �߰�
	if (ChampionName.find("Alistar") != std::string::npos)
	{
		Row* row = Data->GetRow("Alistar");

		size_t Count = row->size();

		for (size_t i = 0; i < Count; ++i)
		{
			std::stringstream ss;

			ss << (*row)[i];

			int Frame = 0;

			ss >> Frame;
			
			// Q Skill
			if (i == 0)
			{
				Instance->AddNotifyParam<CStateManager>("Alistar_SkillQ", "AlistarQAirborne", Frame, StateManager, &CStateManager::CheckAirborneTarget);

				std::string StrRange = CResourceManager::GetInst()->FindCSV("SkillInfo")->FindData("Alistar", "QRange");
				int Range = 0;
				ss.clear();

				ss << StrRange;
				ss >> Range;
				
				Instance->SetNotifyParamRange("Alistar_SkillQ", "AlistarQAirborne", Range);
			}
			// W Skill
			if (i == 1)
				Instance->AddNotifyParam<CStateManager>("Airborne", "AlistarQAirborne", Frame, StateManager, &CStateManager::FindKnockBackTarget);
		}

	}
}

void CEditorManager::SetChampionInfo(class CGameObject* Object, const std::string& ChampionName)
{
	CExcelData* Data = CResourceManager::GetInst()->FindCSV("LoLChampionInfo");

	if (!Data)
		return;

	CStateManager* StateManager = CSceneManager::GetInst()->GetStateManager();

	// TODO : è�Ǿ�� ��ų�� �߰��ɶ����� ���⿡ Notify �߰�
	if (ChampionName.find("Alistar") != std::string::npos)
	{
		Row* row = Data->GetRow("Alistar");

		size_t Count = row->size();

		for (size_t i = 0; i < Count; ++i)
		{
			std::stringstream ss;

			ss << (*row)[i];

			int Info = 0;

			ss >> Info;

			// TODO : LoLChampionInfo.csv���� �о���� �׸� �þ �� ���� �߰����ֱ�

			// Move Speed
			if (i == 0)
				Object->SetChampionMoveSpeed((float)Info);
			// Attack
			else if (i == 1)
				Object->SetChampionAttack(Info);

			else if (i == 2)
				Object->SetChampionAttackSpeed(Info);

			else if (i == 3)
			{

			}

			else if (i == 4)
			{

			}
			else if (i == 5)
			{

			}

			else if (i == 6)
			{

			}

			else if (i == 7)
			{
				Object->SetChampionHP(Info);
			}

			else if (i == 8)
			{

			}
		}

	}
}

void CEditorManager::ReadChampionNotify()
{
	CResourceManager::GetInst()->LoadCSV("AnimationNotify.csv");
}

void CEditorManager::ReadChampionSkillInfo()
{
	CResourceManager::GetInst()->LoadCSV("SkillInfo.csv");
}
