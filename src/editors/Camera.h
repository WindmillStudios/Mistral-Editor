#pragma once

#include "Editor.h"

namespace Vendaval::Editors
{
	class Camera final : public Editor
	{
	  public:

		// Getters
		[[nodiscard]] const char* GetTypeName() const override
		{
			return "Camera";
		}

		[[nodiscard]] const char* GetTypeIcon() const override
		{
			return ICON_FA_CAMERA;
		}

		// Events
		void RenderEvent() override;
	};
} // namespace Vendaval