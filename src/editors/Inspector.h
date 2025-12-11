#pragma once

#include "Component.h"
#include "Editor.h"

namespace Vendaval::Editors
{
	class Inspector final : public Editor
	{
	  public:

		using Editor::Editor;

		// Getters
		[[nodiscard]] static const char* GetTypeName()
		{
			return "Inspector";
		}

		[[nodiscard]] static const char* GetTypeIcon()
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