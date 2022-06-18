#pragma once
#include "Core.h"
#include "BaseComponent.h"

#include "Utils/Math.h"

namespace Zote
{
	struct ZOTE_API TransformComponent : public BaseComponent
	{
		TransformComponent() {}

		TransformComponent(const TransformComponent& other) = default;

		void SetPosition(const vec3& newPos) { position = newPos; }
		const vec3& GetPosition() const { return position; }

		void SetScale(const vec3& newScale) { scale = newScale; }
		const vec3& GetScale() const { return scale; }

		void SetRotation(const quat& newRot) { rotation = newRot; UpdateAxis(); }
		const quat& GetRotation() const { return rotation; }

		const vec3& GetForward() const { return forward; }
		const vec3& GetRight() const { return right; }
		const vec3& GetUp() const { return up; }

		void RotateLocal(const float& degrees, const vec3& axis)
		{
			rotation = rotation * glm::normalize(glm::angleAxis(degrees, axis));
			rotation = glm::normalize(rotation);
			UpdateAxis();
		}

		void RotateGlobal(const float& degrees, const vec3& axis)
		{
			rotation = glm::normalize(glm::angleAxis(degrees, axis)) * rotation;
			rotation = glm::normalize(rotation);
			UpdateAxis();
		}

	private:

		vec3 position = { 0, 0, 0 };
		quat rotation = { 1, 0, 0, 0 };
		vec3 scale = { 1, 1, 1 };

		vec3 right = { 1, 0, 0 };
		vec3 up = { 0, 1, 0 };
		vec3 forward = { 0, 0, 1 };

		void UpdateAxis()
		{
			mat4 axis = mat4(1.0f);

			axis = axis * glm::toMat4(rotation);

			forward = axis * vec4(0, 0, 1, 0);
			right = -axis * vec4(1, 0, 0, 0);
			up = axis * vec4(0, 1, 0, 0);
		}
	};
}