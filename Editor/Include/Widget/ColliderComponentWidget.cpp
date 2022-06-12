
#include "ColliderComponentWidget.h"
#include "IMGUIText.h"
#include "IMGUITree.h"
#include "IMGUIInputFloat3.h"
#include "Component/ColliderBox3D.h"

CColliderComponentWidget::CColliderComponentWidget()
{
}

CColliderComponentWidget::~CColliderComponentWidget()
{
}

bool CColliderComponentWidget::Init()
{
	CSceneComponentWidget::Init();

	m_ComponentTypeText->SetText("ColliderComponent Component");

	m_OffsetTree = AddWidget<CIMGUITree>("Collider Offset");

	m_OffsetInput = m_OffsetTree->AddWidget<CIMGUIInputFloat3>("Offset");
	m_OffsetInput->SetCallBack(this, &CColliderComponentWidget::OnChangeOffset);

	return true;
}

void CColliderComponentWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	Vector3 Offset = ((CColliderComponent*)m_Component.Get())->GetOffset();

	m_OffsetInput->SetX(Offset.x);
	m_OffsetInput->SetY(Offset.y);
	m_OffsetInput->SetZ(Offset.z);

}

void CColliderComponentWidget::OnChangeOffset(const Vector3& Offset)
{
	((CColliderComponent*)m_Component.Get())->SetOffset(Offset);
}
