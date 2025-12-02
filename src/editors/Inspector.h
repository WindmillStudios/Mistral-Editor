#pragma once

#include "Component.h"
#include "Editor.h"

namespace Vendaval::Editors
{
	class Inspector final : public Editor
	{
	  public:

		// Getters
		[[nodiscard]] const char* GetTypeName() const override
		{
			return "Inspector";
		}

		[[nodiscard]] const char* GetTypeIcon() const override
		{
			return ICON_FA_MAGNIFYING_GLASS;
		}

		[[nodiscard]] Mistral::Component* GetSelectedComponent() const;

		// Setters
		void SetSelectedComponent(Mistral::Component* component);

		// Events
		void RenderEvent() override;

	  private:

		Mistral::Component* mSelectedComponent = nullptr;
	};
} // namespace Vendaval::Editors