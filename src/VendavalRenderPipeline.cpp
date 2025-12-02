#include "VendavalRenderPipeline.h"

#include "EditorResources.h"
#include "editors/Camera.h"
#include "editors/Hyerarchy.h"
#include "editors/Inspector.h"
#include "editors/Scene.h"
#include "extras/FA6FreeSolidFontData.h"
#include "extras/IconsFontAwesome6.h"
#include "game/SpatialTest.h"

namespace
{
	bool ShowHelpers = false;
} // namespace

void Vendaval::VendavalRenderPipeline::Initialize()
{
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = "layout.ini";

	InitFonts();

	CreateEditor<Editors::Scene>();
	CreateEditor<Editors::Scene>();
	CreateEditor<Editors::Scene>();
	CreateEditor<Editors::Scene>();
	CreateEditor<Editors::Hyerarchy>();
	CreateEditor<Editors::Camera>();
	CreateEditor<Editors::Inspector>();
}

void Vendaval::VendavalRenderPipeline::RenderEvent()
{
	// 3D Environment rendering
	for (auto& [id, view] : GetEditorsList())
	{
		const auto sceneEditor = dynamic_cast<Editors::Scene*>(view.get());

		if (!sceneEditor)
		{
			continue;
		}

		Mistral::SetActiveCamera(sceneEditor->GetCamera());

		BeginTextureMode(*sceneEditor->GetViewportTexture());

		ClearBackground(RAYWHITE);

		if (const auto camera = Mistral::GetActiveCamera())
		{
			BeginMode3D(*camera);

			Mistral::ComponentRender3DEventCallback();

			EndMode3D();
		}

		Mistral::ComponentRender2DEventCallback();

		EndTextureMode();
	}

	// GUI rendering
	{
		EditorsCreateEventCallback();
		EditorsPreRenderEventCallback();

		BeginDrawing();

		ClearBackground(DARKGRAY);

		rlImGuiBegin();

		RenderMainMenuBar();

		Mistral::ComponentRenderGUIEventCallback();
		EditorsRenderEventCallback();

		if (IsKeyPressed(KEY_F1))
		{
			ShowHelpers = !ShowHelpers;
		}

		if (ShowHelpers)
		{
			ImGui::Begin("StyleEditor");
			ImGui::ShowStyleEditor();
			ImGui::End();
			ImGui::ShowStackToolWindow();
			ImGui::ShowDemoWindow();
		}

		rlImGuiEnd();

		EndDrawing();

		EditorsPostRenderEventCallback();
		EditorsDestroyEventCallback();
	}
}

void Vendaval::VendavalRenderPipeline::InitFonts()
{
	const auto& io = ImGui::GetIO();
	ImFontConfig config;
	constexpr float fontSize = 18.f;
	static constexpr ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
	io.Fonts->Clear();
	std::cout << RESOURCES::FONTS::JETBRAINSMONO_REGULAR << std::endl;
	io.Fonts->AddFontFromFileTTF(RESOURCES::FONTS::JETBRAINSMONO_REGULAR.string().c_str(),
								 fontSize);

	config.MergeMode = true;
	config.PixelSnapH = true;
	config.GlyphOffset.x = 1.f;
	config.GlyphOffset.y = 1.f;
	config.FontDataOwnedByAtlas = false;
	config.GlyphMaxAdvanceX = std::numeric_limits<float>::max();
	config.RasterizerMultiply = 1.0f;
	config.OversampleH = 2;
	config.OversampleV = 1;
	config.GlyphRanges = iconsRanges;
	io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data,
											 fa_solid_900_compressed_size, fontSize * .8f,
											 &config, iconsRanges);
}

void Vendaval::VendavalRenderPipeline::RenderMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(ICON_FA_FILE " File"))
		{
			if (ImGui::MenuItem("Test"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(ICON_FA_BARS_PROGRESS " Edit"))
		{
			if (ImGui::MenuItem("Test"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(ICON_FA_WINDOW_RESTORE " Window"))
		{
			if (ImGui::MenuItem("Test"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(ICON_FA_CIRCLE_QUESTION " Help"))
		{
			ImGui::MenuItem("Show helpers", "F1", &ShowHelpers);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}