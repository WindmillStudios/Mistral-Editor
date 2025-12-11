#pragma once

#include "Editor.h"
#include "raylib.h"
#include "Vector.h"

namespace Vendaval::Editors
{
	class Scene final : public Editor
	{
	  public:

		using Editor::Editor;

		// Getters
		[[nodiscard]] static const char* GetTypeName()
		{
			return "Scene";
		}

		[[nodiscard]] static const char* GetTypeIcon()
		{
			return ICON_FA_CUBE;
		}

		[[nodiscard]] Camera3D* GetCamera();

		[[nodiscard]] RenderTexture2D* GetViewportTexture();

		// Events
		void CreateEvent() override;

		void RenderEvent() override;

		void PostRenderEvent() override;

		RenderTexture2D mViewportTexture;
		Vec2 mViewportPanelSize;
		Camera3D mCamera;
	};
} // namespace Vendaval::Editors