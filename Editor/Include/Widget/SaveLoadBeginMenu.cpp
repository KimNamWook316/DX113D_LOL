#include "SaveLoadBeginMenu.h"
#include "IMGUIMenuItem.h"
#include "../Window/BehaviorTreeMenuBar.h"
#include "../Window/ObjectCreateModal.h"
#include "IMGUIBeginMenu.h"
#include "../EditorUtil.h"
#include "../EditorManager.h"
#include "IMGUIPopUpModal.h"
#include "Engine.h"
#include "IMGUIManager.h"
#include "../Window/BehaviorTreeWindow.h"
#include "../Window/ObjectComponentWindow.h"
#include "../Window/SceneComponentHierarchyWindow.h"
#include "IMGUIManager.h"
#include "../Window/ObjectHierarchyWindow.h"
#include "GameObject/GameObject.h"
#include "Component/StateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"


CSaveLoadBeginMenu::CSaveLoadBeginMenu()
{
}

CSaveLoadBeginMenu::~CSaveLoadBeginMenu()
{
}

bool CSaveLoadBeginMenu::Init()
{
	if (!CIMGUIBeginMenu::Init())
		return false;

	// Scene
	m_SaveSceneMenu = AddMenuItem("Save Scene");
	m_SaveSceneMenu->SetClickCallBack(this, &CSaveLoadBeginMenu::OnSaveSceneMenuCallback);
	m_LoadSceneMenu = AddMenuItem("Load Scene");
	m_SaveSceneMenu->SetClickCallBack(this, &CSaveLoadBeginMenu::OnLoadSceneMenuCallback);

	// Object
	m_SaveObjectMenu = AddMenuItem("Save Object");
	m_SaveObjectMenu->SetClickCallBack(this, &CSaveLoadBeginMenu::OnSaveObjectMenuCallback);

	m_LoadObjectMenu = AddMenuItem("Load Object");
	m_LoadObjectMenu->SetClickCallBack(this, &CSaveLoadBeginMenu::OnLoadObjectMenuCallback);

	// Scene Component
	m_SaveSceneComponentMenu = AddMenuItem("Save SceneComponent");
	m_SaveSceneComponentMenu->SetClickCallBack(this, &CSaveLoadBeginMenu::OnSaveSceneComponentMenuCallback);
	m_LoadSceneComponentMenu = AddMenuItem("Load SceneComponent");
	m_LoadSceneComponentMenu->SetClickCallBack(this, &CSaveLoadBeginMenu::OnLoadSceneComponentMenuCallback);
	
	// Object Component
	m_SaveObjectComponentMenu = AddMenuItem("Save ObjectComponent");
	m_SaveObjectComponentMenu->SetClickCallBack(this, &CSaveLoadBeginMenu::OnSaveObjectComponentMenuCallback);
	m_LoadObjectComponentMenu = AddMenuItem("Load ObjectComponent");
	m_LoadObjectComponentMenu->SetClickCallBack(this, &CSaveLoadBeginMenu::OnLoadObjectComponentMenuCallback);

	return true;
}

void CSaveLoadBeginMenu::Render()
{
	CIMGUIBeginMenu::Render();
}

void CSaveLoadBeginMenu::OnSaveSceneMenuCallback()
{
}

void CSaveLoadBeginMenu::OnLoadSceneMenuCallback()
{
}

void CSaveLoadBeginMenu::OnSaveObjectMenuCallback()
{
	// CObjectComponentWindow* ComponentWindow = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);
	CObjectHierarchyWindow* ObjectWindow = (CObjectHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECT_HIERARCHY);

	if (ObjectWindow)
	{
		CGameObject* Object = ObjectWindow->GetSelectObject();

		if (!Object)
			return;

		TCHAR FileFullPath[MAX_PATH] = {};
		OPENFILENAME OpenFile = {};
		OpenFile.lStructSize = sizeof(OPENFILENAME);
		OpenFile.lpstrFile = FileFullPath;
		OpenFile.nMaxFile = MAX_PATH;
		OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;
		OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.GameObject File\0*.gobj");
		OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();

		if (GetSaveFileName(&OpenFile) != 0)
		{
			char FilePathMultibyte[MAX_PATH] = {};
			int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, 0, 0, 0, 0);
			WideCharToMultiByte(CP_ACP, 0, FileFullPath, -1, FilePathMultibyte, ConvertLength, 0, 0);

			Object->Save(FilePathMultibyte);

			// GameEngine 폴더에 저장하기
			// std::string ExtraFolderName = ENGINE_RESOURCE_OBJECT_PATH;
			// 
			// const PathInfo* EngineSequenceFolder = CPathManager::GetInst()->FindPath(ExtraFolderName);
			// 
			// // 파일 이름을 뽑아낸다.
			// char SavedFileName[MAX_PATH] = {};
			// char SavedExt[_MAX_EXT] = {};
			// _splitpath_s(FilePathMultibyte, nullptr, 0, nullptr, 0, SavedFileName, MAX_PATH, SavedExt, _MAX_EXT);
			// 
			// // 최종 GameEngine 경로를 만든다.
			// char SavedGameEnginePath[MAX_PATH] = {};
			// strcpy_s(SavedGameEnginePath, EngineSequenceFolder->PathMultibyte);
			// strcat_s(SavedGameEnginePath, SavedFileName);
			// strcat_s(SavedGameEnginePath, SavedExt);
			// 
			// // 현재 저장되는 경로와 다르다면, GameEngine 쪽에도 저장한다.
			// if (strcmp(EngineSequenceFolder->PathMultibyte, FilePathMultibyte) != 0)
			// {
			// 	Object->Save(SavedGameEnginePath);
			// }
		}
	}
}

void CSaveLoadBeginMenu::OnLoadObjectMenuCallback()
{
	TCHAR LoadFilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.lpstrFile = LoadFilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0*.GameObject File\0*.gobj");
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(OBJECT_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FilePathMultibyte[MAX_PATH] = {};
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, LoadFilePath, -1, FilePathMultibyte, ConvertLength, 0, 0);

		CGameObject* LoadedObject = CEditorManager::GetInst()->GetObjectHierarchyWindow()->GetObjectCreateModal()->OnCreateObject(FilePathMultibyte);

		if (!LoadedObject)
			return;

		CAnimationMeshComponent* Comp = LoadedObject->FindComponentFromType<CAnimationMeshComponent>();

		if (Comp)
		{
			CAnimationSequenceInstance* Instance = Comp->GetAnimationInstance();
			if (Instance)
				CEditorManager::GetInst()->SetChampionNotify(Instance, LoadedObject->GetName());
		}

		// Object Hierarchy GameObject 목록에 추가한다.
		//CEditorManager::GetInst()->GetObjectComponentWindow()->AddObjectComponent(LoadedObject->GetName());

		std::vector<CObjectComponent*> vecObjComp;
		LoadedObject->GetAllObjectComponentsPointer(vecObjComp);

		size_t Count = vecObjComp.size();

		for (size_t i = 0; i < Count; ++i)
		{
			CEditorManager::GetInst()->GetObjectComponentWindow()->AddObjectComponent(vecObjComp[i]->GetName());

			if (vecObjComp[i]->GetTypeID() == typeid(CNavAgent).hash_code())
			{
				LoadedObject->SetNavAgent((CNavAgent*)vecObjComp[i]);
				((CNavAgent*)vecObjComp[i])->SetUpdateComponent(LoadedObject->GetRootComponent());
			}
		}
	}
}

void CSaveLoadBeginMenu::OnSaveSceneComponentMenuCallback()
{
	CSceneComponentHierarchyWindow* SceneComponentWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);

	if (SceneComponentWindow)
	{
		SceneComponentWindow->OnSaveComponent();
	}
}

void CSaveLoadBeginMenu::OnLoadSceneComponentMenuCallback()
{
	CSceneComponentHierarchyWindow* SceneComponentWindow = (CSceneComponentHierarchyWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(SCENECOMPONENT_HIERARCHY);

	if (SceneComponentWindow)
	{
		SceneComponentWindow->OnLoadComponent();
	}
}

void CSaveLoadBeginMenu::OnSaveObjectComponentMenuCallback()
{
	CObjectComponentWindow* ObjList = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);

	if (ObjList)
	{
		ObjList->OnSaveComponent();
	}
}

void CSaveLoadBeginMenu::OnLoadObjectComponentMenuCallback()
{
	CObjectComponentWindow* ObjList = (CObjectComponentWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow(OBJECTCOMPONENT_LIST);

	if (ObjList)
	{
		ObjList->OnLoadComponent();
	}
}
