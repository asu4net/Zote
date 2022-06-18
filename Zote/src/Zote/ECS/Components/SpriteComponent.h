#pragma once
#include "Core.h"
#include "BaseComponent.h"

#include "Rendering/Shader.h"
#include "Rendering/Sprite.h"
#include "Rendering/Texture.h"

#include "Utils/Memory.h"
#include "Utils/CustomTypes.h"

namespace Zote
{
	struct ZOTE_API SpriteComponent : public BaseComponent
	{
		Ref<Sprite> sprite;
		Ref<Texture> texture;
		Ref<Shader> shader;

		SpriteComponent(cstr path)
		{
			sprite = MakeRef<Sprite>();
			shader = MakeRef<Shader>(sprite->GetVertPath(), sprite->GetFragPath());
			texture = MakeRef<Texture>(path);
			texture->Load();
		}
	};
}