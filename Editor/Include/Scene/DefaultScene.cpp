#include "DefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Material/Material.h"

CDefaultScene::CDefaultScene()
{
    SetTypeID<CDefaultScene>();
}

CDefaultScene::~CDefaultScene()
{
}

bool CDefaultScene::Init()
{
    CreateMaterial();

    /*
    if (!LoadAnimationSequence2D())
        return false;
    */


    // Animation Editor  Test ¿ë Object, Mesh
   // m_Scene->GetResource()->LoadMesh(Mesh_Type::Animation, "PlayerMesh",
   //     TEXT("singed_attack1.msh"), MESH_PATH);
   // 
   // CMesh* Mesh = m_Scene->GetResource()->FindMesh("PlayerMesh");
   // 
   // m_Scene->GetResource()->LoadSkeleton("PlayerSkeleton",
   //     TEXT("singed_attack1.bne"), MESH_PATH);
   // 
   // m_Scene->GetResource()->SetMeshSkeleton("PlayerMesh", "PlayerSkeleton");
   // 
   // m_Scene->GetResource()->LoadAnimationSequence(true, "PlayerIdle",
   //     TEXT("singed_attack1.sqc"), MESH_PATH);
   // 
   // m_Scene->GetResource()->LoadAnimationSequence(false, "PlayerAttack",
   //     TEXT("singed_attack1.sqc"), MESH_PATH);
   // 
   // m_Scene->GetResource()->LoadAnimationSequence(true, "PlayerWalk",
   //     TEXT("singed_attack1.sqc"), MESH_PATH);
   // 
    return true;
}

bool CDefaultScene::LoadAnimationSequence2D()
{
    m_Scene->GetResource()->LoadSequence2D("PlayerIdle.sqc");
    m_Scene->GetResource()->LoadSequence2D("PlayerRun.sqc");

    return true;
}

bool CDefaultScene::CreateMaterial()
{
    m_Scene->GetResource()->CreateMaterial<CMaterial>("TileMap");
    CMaterial* Material = m_Scene->GetResource()->FindMaterial("TileMap");

    //Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "DiabloTile", TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

    Material->SetShader("TileMapShader");
    Material->SetRenderState("AlphaBlend");

    return true;
}
