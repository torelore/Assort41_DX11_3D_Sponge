#include "Dupli_Can.h"
#include "Component/ColliderOBB3D.h"
#include "Component/StaticMeshComponent.h"
#include "Component/RigidBody.h"
#include "Scene/NavigationManager3D.h"
#include "Scene/Scene.h"
#include "Hammer.h"
#include "Duplicatotron.h"

CDupli_Can::CDupli_Can()	: 
    m_FallTime(0.f),
    m_DefyingGravity(true),
    m_CountHammer(0),
    m_SpawnOn(true)
{
	SetTypeID<CDupli_Can>();

	m_ObjectTypeName = "Dupli_Can";
}

CDupli_Can::CDupli_Can(const CDupli_Can& Obj)
    : CMonster(Obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
	m_Cube = (CColliderOBB3D*)FindComponent("Cube");
	m_Rigid = (CRigidBody*)FindComponent("Rigid"); 
}

CDupli_Can::~CDupli_Can()
{
}

void CDupli_Can::Start()
{
	CMonster::Start();
}

bool CDupli_Can::Init()
{
	CMonster::Init();

    if (!m_Scene)
    {
        return false;
    }

    m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");
    m_Cube = CreateComponent<CColliderOBB3D>("Cube");
    m_Rigid = CreateComponent<CRigidBody>("Rigid");

	m_Mesh->SetMesh("DuplicatotronCan");

	SetRootComponent(m_Mesh);

    m_Mesh->AddChild(m_Cube);
    m_Mesh->AddChild(m_Rigid);

    m_Cube->SetBoxHalfSize(50.f, 50.f, 50.f);
    m_Cube->SetRelativePosition(0.f, 0.f,0.f);

    CDuplicatotron* Duplicatotron = (CDuplicatotron*)m_Scene->FindObject("Duplicatotron");

	Vector3 DuplicatotronPos = Duplicatotron->GetWorldPos();

	m_Mesh->SetWorldPosition(DuplicatotronPos.x, DuplicatotronPos.y + 200.f, DuplicatotronPos.z + 400.f);

    m_Rigid->SetGravity(true);

    return true;
}

void CDupli_Can::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

    CNavigationManager3D* Nav = (CNavigationManager3D*)m_Scene->GetNavigationManager();

    float Y = Nav->GetHeight(GetWorldPos());

    // ���߿� ���� ��
    if (m_DefyingGravity)
    {
        m_Rigid->SetGround(false);
        //m_Rigid->AddForce(0, 1.f);
        m_Rigid->SetVelocityY(350.f);
        AddWorldPosition(GetWorldAxis(AXIS_Z) * 300.f * DeltaTime);
    }

    float PosY = GetWorldPos().y;

    if (GetWorldPos().y >= 1200.f)
    {
        m_DefyingGravity = false;
        m_Rigid->SetVelocityY(-200.f);
        AddWorldPosition(GetWorldAxis(AXIS_Z) * 300.f * DeltaTime);
        m_Rigid->AddForce(0, 70.f);
    }

    // ���� ����
    if (Y != FLT_MAX && GetWorldPos().y - Y < m_Mesh->GetMeshSize().y / 2.f && m_Mesh)
    {
        if (m_CountHammer == 2)
            m_SpawnOn = true;

        SetWorldPositionY(Y + m_Mesh->GetMeshSize().y / 2.f);
        m_Rigid->SetGround(true);
        m_Rigid->SetVelocityY(0.f);
        m_Rigid->AddForce(0, 0.f);

        if(m_SpawnOn)
            SpawnHammer();

        if (m_CountHammer >= 2)
            m_SpawnOn = false;


        m_Mesh->Destroy();
    }

   
    
}

void CDupli_Can::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

CDupli_Can* CDupli_Can::Clone() const
{
    return new CDupli_Can(*this);
}

void CDupli_Can::Save(FILE* File)
{
	CMonster::Save(File);
}

void CDupli_Can::Load(FILE* File)
{
	CMonster::Load(File);
}

void CDupli_Can::SpawnHammer()
{
	if (m_Mesh)
	{
        CHammer* Hammer = m_Scene->CreateObject<CHammer>("Hammer");
        Hammer->SetWorldPosition(GetWorldPos());

        ++m_CountHammer;
	}
}