#pragma once

#include "Editor.h"
#include "Component.h"

namespace Vendaval::Editors
{
	class Hyerarchy final : public Editor
	{
	  public:

		// Getters
		[[nodiscard]] const char* GetTypeName() const override
		{
			return "Hyerarchy";
		}

		[[nodiscard]] const char* GetTypeIcon() const override
		{
			return ICON_FA_FOLDER_TREE;
		}

		// Events
		void CreateEvent() override;

		void RenderEvent() override;

	  private:

		void RenderTreeItem(Mistral::Component* component);

		void RenderContextMenu(Mistral::Component* component);

		bool mRenaming = false;
	};
} // namespace Vendaval