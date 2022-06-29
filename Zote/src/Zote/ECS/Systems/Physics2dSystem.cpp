#include "Physics2dSystem.h"

#include <entt.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>

#include "ECS/Entity.h"
#include "ECS/Scene.h"
#include "ECS/Components/CameraComponent.h"

namespace Zote
{
	void Physics2dSystem::UpdateFixture(Rigidbody2DComponent& rb)
	{
		b2FixtureDef fixtureDef;

		switch (rb.m_shape)
		{
		case Rigidbody2DComponent::Shape::circle: 
			fixtureDef.shape = &rb.m_circle;
			break;

		default: 
			fixtureDef.shape = &rb.m_box; 
			break;
		}

		fixtureDef.density = rb.m_density;
		fixtureDef.friction = rb.m_friction;
		fixtureDef.isSensor = rb.m_isTrigger;

		if (rb.m_fixture != nullptr)
		{
			rb.m_body->DestroyFixture(rb.m_fixture);
			rb.m_fixture = nullptr;
		}

		rb.m_fixture = rb.m_body->CreateFixture(&fixtureDef);
		rb.m_fixtureUpdated = true;
	}

	void Physics2dSystem::UpdateBodyDef(Rigidbody2DComponent& rb, TransformComponent& transform)
	{
		b2BodyDef bodyDef;
		bodyDef.type = rb.m_mode == Rigidbody2DComponent::Mode::dynamic ? 
			b2_dynamicBody : b2_staticBody;

		bodyDef.position.Set(transform.GetPosition().x, transform.GetPosition().y);
		bodyDef.angle = glm::eulerAngles(transform.GetRotation()).z;
		bodyDef.gravityScale = rb.m_gScale;

		if (rb.m_body != nullptr)
		{
			m_world->DestroyBody(rb.m_body);
			rb.m_body = nullptr;
		}

		rb.m_body = m_world->CreateBody(&bodyDef);
		
		switch (rb.m_shape)
		{
			case Rigidbody2DComponent::Shape::circle:
				vec3 pos = transform.GetPosition();
				rb.m_circle.m_p.Set(pos.x, pos.y + -1.f);
				rb.m_circle.m_radius = rb.m_radius;			
				break;

			default: //Shape::box:
				rb.m_box.SetAsBox((rb.m_colliderSize.x / 2.f) * transform.GetScale().x,
					(rb.m_colliderSize.y / 2.f) * transform.GetScale().y);		
				break;
		}

		rb.m_bodyUpdated = true;
		UpdateFixture(rb);
	}

	Physics2dSystem::Physics2dSystem(Scene* scene)
		: m_scene(scene)
	{
		m_world = MakeRef<b2World>(b2Vec2(m_worldGravity.x, m_worldGravity.y));
	}

	void Physics2dSystem::Handle2dPhysics()
	{
		auto view = m_scene->registry.view<Rigidbody2DComponent>();
		auto& camera = m_scene->GetMainCamera().GetComponent<CameraComponent>();
		auto& cameraTransform = m_scene->GetMainCamera().GetComponent<TransformComponent>();

		m_world->Step(m_timeStep, m_velocityIterations, m_positionIterations);

		for (auto entity : view)
		{
			auto& rb = view.get<Rigidbody2DComponent>(entity);

			if (!rb.enabled)
				continue;

			auto& transform = rb.GetEntity()->GetComponent<TransformComponent>();
			
			//Keep data updated
			if (!rb.m_bodyUpdated)
				UpdateBodyDef(rb, transform);

			else if (!rb.m_fixtureUpdated)
				UpdateFixture(rb);

			if (!rb.m_HasGizmos)
			{
				rb.m_boxGizmos = MakeRef<BoxGizmos>(rb.GetColliderSize());
				//rb.circleGizmos = MakeRef<CircleGizmos>();
				rb.m_HasGizmos = true;
			}
			
			rb.m_boxGizmos->Render(camera.GetProjection(), camera.GetView(cameraTransform), transform.GetModel());
			//rb.circleGizmos->Render(camera.GetProjection(), camera.GetView(cameraTransform), transform.GetModel());

			//Apply physics to transforms
			b2Vec2 position = rb.m_body->GetPosition();
			transform.SetPosition({ position.x, position.y, transform.GetPosition().z });
			
			float angle = rb.m_body->GetAngle();
			float selfAngle = glm::eulerAngles(transform.GetRotation()).z;
			
			float dif = std::abs(std::abs(angle) - std::abs(selfAngle));

			if (dif > m_rotationOffset)
				transform.RotateGlobal(angle, VEC3_FORWARD);
		}
	}
}