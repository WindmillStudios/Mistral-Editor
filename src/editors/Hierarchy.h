#pragma once

#include "Component.h"
#include "Editor.h"

namespace Vendaval::Editors
{
	class Hierarchy final : public Editor
	{
	  public:

		using Editor::Editor;

		// Getters
		[[nodiscard]] static const char* GetTypeName()
		{
			return "Hierarchy";
		}

		[[nodiscard]] static const char* GetTypeIcon()
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
} // namespace Vendaval::Editors