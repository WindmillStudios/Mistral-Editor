#pragma once

#include <map>
#include <memory>
#include <string>

#include "extras/IconsFontAwesome6.h"

namespace Vendaval
{
	class Editor;

	// Editors management
	template <typename T, typename... Args>
	T* CreateEditor(Args&&... args)
	{
		std::shared_ptr<T> editor = std::make_shared<T>(std::forward<Args>(args)...);
		editor->SetName(editor->GetTypeName());
		auto ptr = editor.get();
		RegisterComponent(editor);
		return ptr;
	}

	void RegisterComponent(std::shared_ptr<Editor> editor);

	void DestroyEditor(const std::shared_ptr<const Editor>& editor);

	void DestroyEditor(const std::string& editorId);

	std::map<std::string, std::shared_ptr<Editor>, std::less<>> GetEditorsList();

	template <typename T>
	T* GetEditor()
	{
		for (auto& [id, view] : GetEditorsList())
		{
			if (auto ptr = dynamic_cast<T*>(view.get()))
			{
				return ptr;
			}
		}
		return nullptr;
	}
	Editor& GetEditor(const std::string& editorName);

	// Event callbacks
	void EditorsCreateEventCallback();

	void EditorsDestroyEventCallback();

	void EditorsRenderEventCallback();

	void EditorsPreRenderEventCallback();

	void EditorsPostRenderEventCallback();

	class Editor
	{
	  public:

		explicit Editor();

		virtual ~Editor() = default;

		void RenderInWindow();

		// Getters
		[[nodiscard]] virtual const char* GetTypeName() const = 0;

		[[nodiscard]] virtual const char* GetTypeIcon() const = 0;

		[[nodiscard]] std::string GetId() const;

		[[nodiscard]] std::string GetName() const;

		// Setters
		void SetName(const std::string& name);

		// Events
		virtual void CreateEvent()
		{
		}

		virtual void DestroyEvent()
		{
		}

		virtual void UpdateEvent()
		{
		}

		virtual void RenderEvent()
		{
		}

		virtual void PreRenderEvent()
		{
		}

		virtual void PostRenderEvent()
		{
		}

	  private:

		const std::string mId;
		std::string mName;
	};
} // namespace Vendaval