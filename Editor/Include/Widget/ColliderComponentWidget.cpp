
#include "ColliderComponentWidget.h"
#include "IMGUIText.h"
#include "IMGUITree.h"
#include "IMGUIInputFloat3.h"
#include "Component/ColliderBox3D.h"
#include "IMGUISeperator.h"
#include "IMGUIComboBox.h"

CColliderComponentWidget::CColliderComponentWidget()
{
}

CColliderComponentWidget::~CColliderComponentWidget()
{
}

bool CColliderComponentWidget::Init()
{
	CSceneComponentWidget::Init();

	m_ComponentTypeText->SetText("ColliderComponent Widget");

	m_ColliderComponentTree = AddWidget<CIMGUITree>("Collider Info");

	m_OffsetInput = m_ColliderComponentTree->AddWidget<CIMGUIInputFloat3>("Offset");
	m_CollisionProfileCombo = m_ColliderComponentTree->AddWidget<CIMGUIComboBox>("Profile");
	m_CollisionProfileCombo->SetHideName(true);

	//m_CollisionProfileCombo->AddItem()

	m_OffsetInput->SetCallBack(this, &CColliderComponentWidget::OnChangeOffset);
	m_CollisionProfileCombo->SetSelectCallback(this, &CColliderComponentWidget::OnChangeProfile);

	AddWidget<CIMGUISeperator>("Sep");

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

void CColliderComponentWidget::OnChangeProfile(int Index, const char* Label)
{
	((CColliderComponent*)m_Component.Get())->SetCollisionProfile(Label);
}
