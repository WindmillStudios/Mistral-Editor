#include "Editor.h"

#include <map>
#include <ranges>

#include "Color.h"
#include "extras/IconsFontAwesome6.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Mistral.h"
#include "Random.h"

static std::map<std::string, std::shared_ptr<Vendaval::Editor>, std::less<>> editors;
static std::vector<std::string> createList;
static std::vector<std::string> destroyList;

Vendaval::Editor::Editor():
	mId(GenerateUUID())
{
}

void Vendaval::Editor::RenderInWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2::Zero);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Vec2::Zero);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_DockingSeparatorSize, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Color4(.15f).ToUInt32());

	ImGui::Begin(mName.c_str(), nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2(5.f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, Vec2(5.f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.f);

	ImGui::PushStyleColor(ImGuiCol_Button, Color4(.15).ToUInt32());
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Color4(.2).ToUInt32());
	ImGui::PushStyleColor(ImGuiCol_Border, Color4(.3).ToUInt32());

	const auto headerHeight = ImGui::CalcTextSize("A").y +
							  ImGui::GetStyle().FramePadding.y * 2.f +
							  ImGui::GetStyle().WindowPadding.y * 2.f;
	ImGui::BeginChild("Header", Vec2(0, headerHeight),
					  ImGuiChildFlags_AlwaysUseWindowPadding);
	if (ImGui::Button(ICON_FA_BARS " " ICON_FA_CARET_DOWN))
	{
		ImGui::OpenPopup("EditorSelectorPopup");
	}
	if (ImGui::BeginPopup("EditorSelectorPopup"))
	{
		if (ImGui::MenuItem(ICON_FA_CUBE " Scene"))
		{
		}
		if (ImGui::MenuItem(ICON_FA_MAGNIFYING_GLASS " Inspector"))
		{
		}
		if (ImGui::MenuItem(ICON_FA_FOLDER_TREE " Hyerarchy"))
		{
		}
		if (ImGui::MenuItem(ICON_FA_CAMERA " Rendering"))
		{
		}
		ImGui::EndPopup();
	}
	ImGui::EndChild();
	ImGui::PopStyleVar(8);
	ImGui::PopStyleColor(4);

	RenderEvent();

	ImGui::End();
}

std::string Vendaval::Editor::GetId() const
{
	return mId;
}

std::string Vendaval::Editor::GetName() const
{
	return mName;
}

void Vendaval::Editor::SetName(const std::string& name)
{
	mName = name;
}

void Vendaval::RegisterComponent(std::shared_ptr<Editor> editor)
{
	createList.emplace_back(editor->GetId());

	auto counter = 1u;
	while (std::ranges::find_if(editors, [&](const auto& pair) {
			   return pair.second->GetName() == editor->GetName();
		   }) != editors.cend())
	{
		editor->SetName(std::string(editor->GetTypeName()) + " (" +
						std::to_string(counter++) + ")");
	}

	editors.try_emplace(editor->GetId(), std::move(editor));
}

void Vendaval::DestroyEditor(const std::shared_ptr<const Editor>& editor)
{
	destroyList.emplace_back(editor->GetId());
}

void Vendaval::DestroyEditor(const std::string& editorId)
{
	destroyList.emplace_back(editorId);
}

Vendaval::Editor& Vendaval::GetEditor(const std::string& editorName)
{
	for (const auto& editor : editors | std::views::values)
	{
		if (editor->GetName() == editorName)
		{
			return *editor;
		}
	}
	throw std::runtime_error("Editor (" + editorName + ") not found");
}

std::map<std::string, std::shared_ptr<Vendaval::Editor>, std::less<>> Vendaval::
	GetEditorsList()
{
	return editors;
}

void Vendaval::EditorsCreateEventCallback()
{
	for (const auto& editorId : createList)
	{
		editors[editorId]->CreateEvent();
	}
	createList.clear();
}

void Vendaval::EditorsDestroyEventCallback()
{
	for (const auto& editorId : destroyList)
	{
		editors[editorId]->DestroyEvent();
	}
	destroyList.clear();
}

void Vendaval::EditorsRenderEventCallback()
{
	ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, Color4::Transparent.ToUInt32());
	ImGui::DockSpaceOverViewport(ImGui::GetID("DockSpace"), ImGui::GetMainViewport(),
								 ImGuiDockNodeFlags_NoTabBar);
	ImGui::PopStyleColor();

	for (const auto& editor : editors | std::views::values)
	{
		editor->RenderInWindow();
	}
}

void Vendaval::EditorsPreRenderEventCallback()
{
	for (const auto& editor : editors | std::views::values)
	{
		editor->PreRenderEvent();
	}
}

void Vendaval::EditorsPostRenderEventCallback()
{
	for (const auto& editor : editors | std::views::values)
	{
		editor->PostRenderEvent();
	}
}