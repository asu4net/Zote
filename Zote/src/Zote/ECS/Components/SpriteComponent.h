#pragma once
#include "Core.h"
#include "BaseComponent.h"

#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/Mesh.h"

#include "Utils/Memory.h"
#include "Utils/CustomTypes.h"
#include "Utils/GlobalStrings.h"

namespace Zote
{
	struct ZOTE_API SpriteComponent : public BaseComponent
	{
		Color color = { 1.f, 1.f, 1.f, 1.f };
		Ref<Mesh> mesh;
		Ref<Texture> texture;
		Ref<Shader> shader;
		
		bool HasTexture() { return std::string(m_path) != ""; }

		SpriteComponent(cstr path = "")
		{
			//Quad data...
			float vertices[] = {
				// x	 y	   u	 v
				-0.5f, -0.5f, 1.0f, 0.0f, // 0
				 0.5f, -0.5f, 0.0f, 0.0f, // 1
				 0.5f,  0.5f, 0.0f, 1.0f, // 2
				-0.5f,	0.5f, 1.0f, 1.0f, // 3 
			};

			unsigned int indices[] = {
				0, 1, 2, //triangle 1
				2, 3, 0  //triangle 2
			};

			mesh = MakeRef<Mesh>(ZOTE_2D_MESH, true, vertices, indices, 16, 6);
			shader = MakeRef<Shader>(SHADER_VERT_SPRITE, SHADER_FRAG_SPRITE);
			
			if (std::string(path) == std::string(""))
				return;

			AddTexture(path);
		}

		void AddTexture(cstr path)
		{
			m_path = path;
			texture = MakeRef<Texture>(path);
			texture->Load();
		}

	private:
		cstr m_path = "";
	};
}