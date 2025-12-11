#include "Editor.h"

#include <format>
#include <map>
#include <ranges>

#include "Color.h"
#include "editors/FileExplorer.h"
#include "editors/Hierarchy.h"
#include "editors/Inspector.h"
#include "editors/Scene.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Mistral.h"
#include "Random.h"

static std::map<std::string, std::shared_ptr<Vendaval::Editor>, std::less<>> editors;
static std::vector<std::string> createList;
static std::vector<std::string> destroyList;

Vendaval::Editor::Editor() :
	mId(GenerateUUID()),
	mDockSpaceId(0)
{
}

Vendaval::Editor::Editor(const uint32_t dockSpaceId) :
	mId(GenerateUUID()),
	mDockSpaceId(dockSpaceId)
{
}

void Vendaval::Editor::RenderInWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Vec2::Zero);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, Vec2::Zero);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_DockingSeparatorSize, 0);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Color4(.15f).ToUInt32());

	if (mDockSpaceId != 0)
	{
		ImGui::SetNextWindowDockID(mDockSpaceId);
		mDockSpaceId = 0;
	}

	ImGui::SetNextWindowSize({200.f, 200.f}, ImGuiCond_FirstUseEver);
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
	ImGuiID currentDockNodeId = ImGui::GetWindowDockID();
	ImGui::BeginChild("Header", Vec2(0, headerHeight),
					  ImGuiChildFlags_AlwaysUseWindowPadding);
	if (ImGui::Button(ICON_FA_BARS " " ICON_FA_CARET_DOWN))
	{
		ImGui::OpenPopup("EditorSelectorPopup");
	}
	if (ImGui::BeginPopup("EditorSelectorPopup"))
	{
		if (ImGui::MenuItem((std::string(Editors::Scene::GetTypeIcon()) + " " +
							 std::string(Editors::Scene::GetTypeName()))
								.c_str()))
		{
			CreateEditor<Editors::Scene>(currentDockNodeId);
			DestroyEditor(GetId());
		}
		if (ImGui::MenuItem((std::string(Editors::Inspector::GetTypeIcon()) + " " +
							 std::string(Editors::Inspector::GetTypeName()))
								.c_str()))
		{
			CreateEditor<Editors::Inspector>(currentDockNodeId);
			DestroyEditor(GetId());
		}
		if (ImGui::MenuItem((std::string(Editors::Hierarchy::GetTypeIcon()) + " " +
							 std::string(Editors::Hierarchy::GetTypeName()))
								.c_str()))
		{
			CreateEditor<Editors::Hierarchy>(currentDockNodeId);
			DestroyEditor(GetId());
		}
		if (ImGui::MenuItem((std::string(Editors::FileExplorer::GetTypeIcon()) + " " +
							 std::string(Editors::FileExplorer::GetTypeName()))
								.c_str()))
		{
			CreateEditor<Editors::FileExplorer>(currentDockNodeId);
			DestroyEditor(GetId());
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
	if (!createList.empty())
	{
		for (const auto& editorId : createList)
		{
			editors[editorId]->CreateEvent();
		}
		createList.clear();
	}
}

void Vendaval::EditorsDestroyEventCallback()
{
	if (!destroyList.empty())
	{
		for (const auto& editorId : destroyList)
		{
			editors[editorId]->DestroyEvent();
			editors.erase(editorId);
		}
		destroyList.clear();
	}
}

void Vendaval::EditorsRenderEventCallback()
{
	ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, Color4::Transparent.ToUInt32());
	GenerateDefaultLayout();
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

void Vendaval::GenerateDefaultLayout()
{
	if (const auto dockSpaceId = ImGui::GetID("DockSpace");
		ImGui::DockBuilderGetNode(dockSpaceId) == nullptr)
	{
		CreateEditor<Editors::Scene>();
		CreateEditor<Editors::Hierarchy>();
		CreateEditor<Editors::Inspector>();

		ImGui::DockBuilderRemoveNode(dockSpaceId);
		ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetMainViewport()->Size);

		ImGuiID mainId = dockSpaceId;
		ImGuiID leftTopId =
			ImGui::DockBuilderSplitNode(mainId, ImGuiDir_Left, .2f, nullptr, &mainId);
		ImGuiID leftBottomId = ImGui::DockBuilderSplitNode(leftTopId, ImGuiDir_Down, .5f,
														   nullptr, &leftTopId);

		ImGui::DockBuilderDockWindow("Scene", mainId);
		ImGui::DockBuilderDockWindow("Inspector", leftTopId);
		ImGui::DockBuilderDockWindow("Hierarchy", leftBottomId);

		ImGui::DockBuilderFinish(dockSpaceId);
	}
}
