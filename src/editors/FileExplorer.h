#pragma once

#include "Editor.h"

namespace Vendaval::Editors
{
	class FileExplorer final : public Editor
	{
	public:

		using Editor::Editor;

		// Getters
		[[nodiscard]] static const char* GetTypeName()
		{
			return "FileExplorer";
		}

		[[nodiscard]] static const char* GetTypeIcon()
		{
			return ICON_FA_FOLDER;
		}

		// Events
		void RenderEvent() override;
	};
} // namespace Vendaval::Editors
