
#include "NavigationThread3D.h"
#include "../Component/NavMeshComponent.h"
#include "Navigation3DManager.h"
#include "../Component/NavMeshComponent.h"

CNavigationThread3D::CNavigationThread3D()
{
	SetLoop(true);

	//m_NavMeshComponent = new CNavMeshComponent;

	m_ExitEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

CNavigationThread3D::~CNavigationThread3D()
{
	m_Loop = false;

	WaitForSingleObject(m_ExitEvent, INFINITE);

	CloseHandle(m_ExitEvent);

	//SAFE_DELETE(m_NavMeshComponent);
}


void CNavigationThread3D::SetNavMeshComponent(CNavMeshComponent* NavMeshComp)
{
	m_NavMeshComponent = NavMeshComp;
}

void CNavigationThread3D::SetNavigationManager(CNavigation3DManager* Manager)
{
	m_NavManager = Manager;
}

void CNavigationThread3D::Run()
{
	while (m_Loop)
	{
		if (!m_WorkQueue.empty())
		{
			NavWorkData Work = m_WorkQueue.front();
			m_WorkQueue.pop();

			NavResultData	Result;

			m_NavMeshComponent->FindPath(Work.OwnerComponent->GetWorldPos(), Work.End, Result.vecPath);

			Result.Callback = Work.Callback;

			m_NavManager->AddNavResult(Result);
		}
	}

	SetEvent(m_ExitEvent);
}
