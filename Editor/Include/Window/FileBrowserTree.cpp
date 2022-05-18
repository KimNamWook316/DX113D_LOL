
#include "FileBrowserTree.h"
#include "IMGUIText.h"
#include "IMGUITree.h"
#include "IMGUIDummy.h"
#include "../EditorUtil.h"
#include "PathManager.h"
#include "FileBrowser.h"
#include "IMGUIManager.h"

CFileBrowserTree::CFileBrowserTree()
{
	m_CurrentPath = ROOT_PATH;
}

CFileBrowserTree::~CFileBrowserTree()
{
}

bool CFileBrowserTree::Init()
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(m_CurrentPath);
	std::string FullPath = Info->PathMultibyte;

	m_CurrentPathText = AddWidget<CIMGUIText>(m_CurrentPath);
	m_CurrentPathText->SetText(FullPath.c_str());

	CIMGUIDummy* Dummy = AddWidget<CIMGUIDummy>("Dummy", 20.f, 10.f);

	m_Root = AddWidget<CIMGUITree>("Root");
	std::vector<std::string>	Tmp; // 사용하지 않을 벡터

	CEditorUtil::GetAllFilenames(m_CurrentPath, Tmp, m_vecCurrentPathDir);

	size_t Count = m_vecCurrentPathDir.size();
	for (size_t i = 0; i < Count; ++i)
	{
		CIMGUITree* ChildTreeNode = m_Root->AddChild(m_vecCurrentPathDir[i]);
		ChildTreeNode->AddOpenCallback<CFileBrowserTree>(this, &CFileBrowserTree::OnOpenBrowserTree);
		ChildTreeNode->AddSelectCallback<CFileBrowserTree>(this, &CFileBrowserTree::OnShowFileBrowser);
	}

	return true;
}

void CFileBrowserTree::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CFileBrowserTree::OnOpenBrowserTree(CIMGUITree* Tree)
{
	if (Tree->GetChildCount() > 0)
		return;

	// open한 디렉토리의 경로안에 있는 디렉토리 목록들을 다시 받아와서 Tree의 Child로 넣어준다
	const PathInfo* Info = CPathManager::GetInst()->FindPath(Tree->GetName());

	if (Info)
	{
		m_CurrentFullPath = Info->PathMultibyte;
		m_CurrentPath = Tree->GetName();
	}

	else
	{
		std::string NewFullPath;
		// 클릭한 TreeNode 내부까지의 경로가 PathManager에 없는 경로라면 거기까지의 풀 경로를 새로 만들어줘야 한다
		GetFullPath(Tree->GetName(), NewFullPath);
	}

	std::vector<std::string> vecFileName;
	std::vector<std::string> vecDirName;

	CEditorUtil::GetAllFilenamesFullPath(m_CurrentFullPath, vecFileName, vecDirName);

	size_t Count = vecDirName.size();

	for (size_t i = 0; i < Count; ++i)
	{
		CIMGUITree* ChildTreeNode = Tree->AddChild(vecDirName[i]);
		ChildTreeNode->AddOpenCallback<CFileBrowserTree>(this, &CFileBrowserTree::OnOpenBrowserTree);
		ChildTreeNode->AddSelectCallback<CFileBrowserTree>(this, &CFileBrowserTree::OnShowFileBrowser);
	}
}

void CFileBrowserTree::OnShowFileBrowser(CIMGUITree* Tree)
{
	CFileBrowser* FileBrowser = (CFileBrowser*)CIMGUIManager::GetInst()->FindIMGUIWindow(FILE_BROWSER);

	std::string NewPath = Tree->GetName();

	const PathInfo* Info = CPathManager::GetInst()->FindPath(NewPath);


	// FileBrowser창 Path Update
	if (!Info)
	{
		const std::string OriginPath = FileBrowser->GetInitialPath();

		const PathInfo* PrevPathInfo = CPathManager::GetInst()->FindPath(OriginPath);
		std::string NewFullPath = PrevPathInfo->PathMultibyte + NewPath + "\\";

		FileBrowser->SetInitialFullPath(NewFullPath);
		return;
	}

	else
	{
		FileBrowser->SetInitialPath(NewPath);
	}
}

void CFileBrowserTree::GetFullPath(const std::string& Path, std::string& NewFullPath)
{
	const PathInfo* Info = CPathManager::GetInst()->FindPath(ROOT_PATH);

	//std::vector<
}

