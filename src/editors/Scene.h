#pragma once

#include "Editor.h"
#include "raylib.h"
#include "Vector.h"

namespace Vendaval::Editors
{
	class Scene final : public Editor
	{
	  public:

		Scene();

		// Getters
		[[nodiscard]] const char* GetTypeName() const override
		{
			return "Scene";
		}

		[[nodiscard]] const char* GetTypeIcon() const override
		{
			return ICON_FA_CUBE;
		}

		[[nodiscard]] Camera3D* GetCamera();

		[[nodiscard]] RenderTexture2D* GetViewportTexture();

		// Events
		void RenderEvent() override;

		void PostRenderEvent() override;

		RenderTexture2D mViewportTexture;
		Vec2 mViewportPanelSize;
		Camera3D mCamera;
	};
} // namespace Vendaval