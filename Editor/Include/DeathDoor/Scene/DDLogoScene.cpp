#include "DDLogoScene.h"
#include "../UI/UIManager.h"
#include "Widget/Button.h"
#include "Render/RenderManager.h"
#include "../Thread/DDSceneLoadingThread.h"
#include "Scene/SceneManager.h"

CDDLogoScene::CDDLogoScene()
{
	SetTypeID<CDDLogoScene>();
}

CDDLogoScene::~CDDLogoScene()
{
}

bool CDDLogoScene::Init()
{
	CSceneMode::Init();

	return true;
}

void CDDLogoScene::Start()
{
	CSceneMode::Start();

	m_NextSceneLoadingThread = new CDDSceneLoadingThread;
	m_NextSceneLoadingThread->Init();
	m_NextSceneLoadingThread->Load<CDDLogoScene>(this, "MapScene/MushroomDungeon/Sequencial/Mushroom_Init.scn", &CDDLogoScene::OnLoadEnd);
}

void CDDLogoScene::SetStartCallBack(CButton* Button)
{
	m_StartButton = Button;

	Button->ButtonEnable(false);
	Button->SetClickCallback(this, &CDDLogoScene::OnClickStartButton);
}

void CDDLogoScene::OnClickStartButton()
{
	CRenderManager::GetInst()->SetFadeEndCallBack(this, &CDDLogoScene::OnFadeOutEnd);
	CRenderManager::GetInst()->StartFadeEffect(FadeEffecType::FADE_OUT, true);
}

void CDDLogoScene::OnLoadEnd()
{
	m_StartButton->ButtonEnable(true);
}

void CDDLogoScene::OnFadeOutEnd()
{
	// Scene Change
	CSceneManager::GetInst()->ChangeNextScene();
}
