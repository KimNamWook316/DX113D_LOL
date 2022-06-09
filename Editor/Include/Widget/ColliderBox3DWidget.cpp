
#include "ColliderBox3DWidget.h"
#include "IMGUIText.h"
#include "IMGUICollapsingHeader.h"
#include "IMGUIInputFloat3.h"
#include "Component/ColliderBox3D.h"

CColliderBox3DWidget::CColliderBox3DWidget()
{
}

CColliderBox3DWidget::~CColliderBox3DWidget()
{
}

bool CColliderBox3DWidget::Init()
{
	CSceneComponentWidget::Init();

	m_ComponentTypeText->SetText("ColliderBox3D Component");

	CIMGUICollapsingHeader* Collapse = AddWidget<CIMGUICollapsingHeader>("ColliderBox3D Variables");

	m_OffsetInput = Collapse->AddWidget<CIMGUIInputFloat3>("Offset");
	m_OffsetInput->SetCallBack(this, &CColliderBox3DWidget::OnChangeOffset);

	return true;
}

void CColliderBox3DWidget::SetSceneComponent(CSceneComponent* Com)
{
	CSceneComponentWidget::SetSceneComponent(Com);

	Vector3 Offset = ((CColliderComponent*)m_Component.Get())->GetOffset();

	m_OffsetInput->SetX(Offset.x);
	m_OffsetInput->SetY(Offset.y);
	m_OffsetInput->SetZ(Offset.z);

}

void CColliderBox3DWidget::OnChangeOffset(const Vector3& Offset)
{
	((CColliderBox3D*)m_Component.Get())->SetOffset(Offset);
}
