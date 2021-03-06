#include "Scene.h"
#include "Entity.h"
#include "Components/CameraComponent.h"
#include "Components/PBody2DComponent.h"
#include "Components/ScriptComponent.h"

namespace Zote
{
	vec3 Scene::MainCamStart = { 0, 0, -6 };

	Scene::Scene(Ref<Window> window)
		: m_window(window)
	{
		mainCamera = new Entity();
		*mainCamera = CreateEntity();
		mainCamera->AddComponent<CameraComponent>(window);
		mainCamera->GetComponent<TransformComponent>().SetPosition(MainCamStart);

		m_scriptSystem = MakeRef<ScriptSystem>(this);
		m_meshSystem = MakeRef<MeshSystem>(this);
		m_spriteSystem = MakeRef<SpriteSystem>(this);
		m_transformGizmoSystem = MakeRef<TransformGizmoSystem>(this);
		m_Physic2DSystem = MakeRef<Physic2DSystem>(this);
		m_CollisionCheckSystem = MakeRef<Collision2DCheckSystem>(this);
		m_Box2DGizmoSystem = MakeRef<Box2DGizmoSystem>(this);

		window->OnRenderFrame.AddListener(new Delegate<OnRenderFrameArgs>(this, &Scene::OnRenderFrame));
	}

	void Scene::OnRenderFrame(OnRenderFrameArgs args)
	{
		m_Physic2DSystem->m_frameRate = args.frameRate;
#ifdef ZOTE_DEBUG
		m_transformGizmoSystem->HandleGizmos();
#endif	
		m_meshSystem->HandleMeshes();
		m_spriteSystem->HandleSprites();
		m_scriptSystem->HandleScripts(args.deltaTime);
		m_Physic2DSystem->Handle2dPhysics();
		m_CollisionCheckSystem->Check2DCollisions();
#ifdef ZOTE_DEBUG
		m_Box2DGizmoSystem->HandleBox2DGizmos();
#endif	
	}

	//TOFIX: This updates the PBody every new component added after it...
	void Scene::OnComponentAdded(entt::entity entity)
	{
		if (auto* tryPb = registry.try_get<PBody2DComponent>(entity))
		{
			auto& pb = registry.get<PBody2DComponent>(entity);
			m_Physic2DSystem->UpdateBodyDef(entity);

			pb.m_physicsSystem = m_Physic2DSystem;
			pb.entity = entity;
		}
	}

	Entity Scene::CreateEntity(str name, str tag)
	{
		entt::entity entity = registry.create();
		return { entity, this, name, tag };
	}

	Entity Scene::CreateScriptableEntity(Script* script, str name, str tag)
	{
		Entity se = CreateEntity(name, tag);
		auto& sc = se.AddComponent<ScriptComponent>();
		sc.AddScript(script);
		return se;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entt::entity id = entity.GetId();
		if (registry.valid(id))
		{
			OnDestroyEntity.Invoke({ &entity });
			registry.destroy(id);
		}			
	}
	Scene::~Scene()
	{
		delete mainCamera;
	}
}