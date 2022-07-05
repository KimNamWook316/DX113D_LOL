
#include "NavMeshComponentWidget.h"
#include "Component/NavMeshComponent.h"
#include "Resource/Mesh/NavMesh.h"
#include "IMGUITextInput.h"
#include "Engine.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUITree.h"
#include "IMGUIButton.h"
#include "IMGUICheckBox.h"

CNavMeshComponentWidget::CNavMeshComponentWidget()
{
}

CNavMeshComponentWidget::~CNavMeshComponentWidget()
{
}

bool CNavMeshComponentWidget::Init()
{
	CSceneComponentWidget::Init();

	// AddWidget
	m_ComponentTypeText->SetText("NavMesh Component");

	// 최상위 트리
	CIMGUITree* m_RootTree = AddWidget<CIMGUITree>("NavMesh Mesh Variables");

	m_MeshName = m_RootTree->AddWidget<CIMGUITextInput>("Mesh Name");
	m_LoadMeshButton = m_RootTree->AddWidget<CIMGUIButton>("Load", 0.f, 0.f);
	m_DebugRenderCheckBox = m_RootTree->AddWidget<CIMGUICheckBox>("Debug Render");
	m_DebugRenderCheckBox->AddCheckInfo("Debug Render");

	// CallBack
	m_LoadMeshButton->SetClickCallback(this, &CNavMeshComponentWidget::OnClickLoadMesh);

	m_DebugRenderCheckBox->SetCallBackIdx<CNavMeshComponentWidget>(this, &CNavMeshComponentWidget::OnClickDebugRender);

	return true;
}

void CNavMeshComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	CNavMeshComponent* MeshCom = (CNavMeshComponent*)m_Component;

	if (MeshCom->GetNavMesh())
	{
		RefreshMeshWidget(MeshCom->GetNavMesh());
	}

	if (MeshCom->GetDebugRender())
		m_DebugRenderCheckBox->SetCheck(0, true);
	else
		m_DebugRenderCheckBox->SetCheck(0, false);
}

void CNavMeshComponentWidget::OnClickLoadMesh()
{
	if (m_NameInput->Empty())
	{
		MessageBox(nullptr, TEXT("Enter the Name"), TEXT("Fail"), MB_OK);
		return;
	}

	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("Obj File\0*.obj\0");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MESH_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		std::string MeshName;
		MeshName = m_MeshName->GetTextMultibyte();
		if (!CSceneManager::GetInst()->GetScene()->GetResource()->LoadMeshFullPath(Mesh_Type::Nav, MeshName, FilePath))
		{
			MessageBox(nullptr, TEXT("메쉬 로드 실패"), TEXT("실패"), MB_OK);
			return;
		}

		CMesh* Mesh = CSceneManager::GetInst()->GetScene()->GetResource()->FindMesh(MeshName);
		Mesh->SetName(MeshName);

		CNavMeshComponent* NavMeshComp = static_cast<CNavMeshComponent*>(m_Component);

		NavMeshComp->SetNavMesh((CNavMesh*)Mesh);

		RefreshMeshWidget(Mesh);


		CNavMesh* NavMesh = (CNavMesh*)Mesh;

		NavMesh->CreateNavMesh(MeshName);


		m_MeshName->ClearText();

		CSceneManager::GetInst()->GetScene()->GetNavigation3DManager()->SetNavMeshData(NavMeshComp);

		NavMeshComp->GetTransform()->ForceUpdateMat();
	}
}

void CNavMeshComponentWidget::OnClickDebugRender(int Idx, bool Check)
{
	if (Check)
	{
		static_cast<CNavMeshComponent*>(m_Component)->SetDebugRender(true);
		m_Component->SetLayerName("Default");
	}
	else
		static_cast<CNavMeshComponent*>(m_Component)->SetDebugRender(false);
}

void CNavMeshComponentWidget::RefreshMeshWidget(CMesh* Mesh)
{
	CNavMeshComponent* MeshCom = (CNavMeshComponent*)m_Component;

	m_MeshName->SetText(MeshCom->GetNavMesh()->GetName().c_str());
}
