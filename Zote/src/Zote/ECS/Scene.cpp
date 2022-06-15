#include "Scene.h"
#include "Entity.h"
#include "Rendering/Renderer.h"

namespace Zote
{
	Scene::Scene(Window& window)
	{
		mainCamera = new Entity();
		*mainCamera = CreateEntity();
		mainCamera->AddComponent<CameraComponent>();

		TransformComponent& mainCam_t = mainCamera->GetComponent<TransformComponent>();

		mainCam_t.position = defaultCameraPos;
		mainCam_t.rotation = defaultCameraRot;

		renderer = new Renderer(mainCamera);

		//Suscribe to DrawMesh to WindowUpdate
		window.OnRenderFrame.AddListener(new Delegate<OnRenderFrameArgs>(this, &Scene::OnRenderFrame));
	}

	void Scene::OnRenderFrame(OnRenderFrameArgs args)
	{
		DrawAxisGizmos();
		DrawMeshes(args);
		HandleScripts(args);
	}

	Entity Scene::CreateEntity()
	{
		return { registry.create(), this };
	}
	Scene::~Scene()
	{
		delete renderer;
		delete mainCamera;
	}
	void Scene::DrawMeshes(OnRenderFrameArgs args)
	{
		auto group = registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			MeshComponent& mesh = group.get<MeshComponent>(entity);
			TransformComponent& transform = group.get<TransformComponent>(entity);
			renderer->DrawMesh(mesh, transform, args.aspect);
		}
	}
	void Scene::HandleScripts(OnRenderFrameArgs args)
	{
		auto view = registry.view<ScriptComponent>();
		for (auto entity : view)
		{
			ScriptComponent& scriptComponent = view.get<ScriptComponent>(entity);

			if (!scriptComponent.enabled)
				return;

			for (auto script : scriptComponent.scripts)
			{
				if ((!script->enabled))
					continue;

				if (!script->started)
				{
					script->Start();
					script->started = true;
				}

				script->Update(args.deltaTime);
			}
		}
	}
	void Scene::DrawAxisGizmos()
	{
		auto view = registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transformComponent = view.get<TransformComponent>(entity);
			renderer->DrawAxisGizmos(transformComponent);
		}
	}
}