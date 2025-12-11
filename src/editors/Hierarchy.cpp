#include "Hierarchy.h"

#include <imgui.h>

#include "Component.h"
// #include "game/ComponentTest.h"
#include "Inspector.h"
#include "misc/cpp/imgui_stdlib.h"

void Vendaval::Editors::Hierarchy::CreateEvent()
{
	// Mistral::CreateComponent<ComponentTest>();
}

void Vendaval::Editors::Hierarchy::RenderEvent()
{
	for (const auto& component : Mistral::GetComponentsView())
	{
		if (!component->GetParent())
		{
			RenderTreeItem(component.get());
		}
	}

	if (IsKeyPressed(KEY_F2) && GetEditor<Inspector>()->GetSelectedComponent() &&
		ImGui::IsWindowFocused())
	{
		mRenaming = true;
	}
}

void Vendaval::Editors::Hierarchy::RenderTreeItem(Mistral::Component* component)
{
	const auto inspectorEditor = GetEditor<Inspector>();

	ImGui::PushID(component->GetId().c_str());

	if (mRenaming && component == inspectorEditor->GetSelectedComponent())
	{
		auto componentName = component->GetName();
		ImGui::SetKeyboardFocusHere();
		const bool closeRename = ImGui::InputText(
			"##name", &componentName,
			ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
		component->SetName(componentName);
		if (closeRename || ImGui::IsItemDeactivated())
		{
			mRenaming = false;
		}
	}
	else
	{
		ImGui::Selectable(component->GetName().c_str(),
						  inspectorEditor->GetSelectedComponent() == component);
	}
	RenderContextMenu(component);

	if (ImGui::IsItemClicked() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		inspectorEditor->SetSelectedComponent(component);
	}

	if (component->HasChildren())
	{
		ImGui::Indent();
		for (const auto child : component->GetChildren())
		{
			RenderTreeItem(child);
		}
		ImGui::Unindent();
	}

	ImGui::PopID();
}

void Vendaval::Editors::Hierarchy::RenderContextMenu(Mistral::Component* component)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Add component"))
		{
			// component->CreateChild<ComponentTest>();
		}

		const auto menuItemDeleteString = "Remove " + component->GetName();
		bool remove = false;
		if (ImGui::MenuItem(menuItemDeleteString.c_str()))
		{
			remove = true;
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Rename"))
		{
			mRenaming = true;
		}

		ImGui::EndPopup();

		if (remove)
		{
			component->GetParent()->RemoveChild(component);
		}
	}
}