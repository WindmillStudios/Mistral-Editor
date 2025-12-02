#include "imgui.h"
#include "Mistral.h"
#include "Quaternion.h"
#include "raylib.h"
#include "rlImGui.h"
#include "Scene.h"

Vendaval::Editors::Scene::Scene():
	mViewportTexture(),
	mCamera()
{
	mCamera.up = Vec3(0.f, 1.f, 0.f);
	mCamera.position = Vec3(10.f);
	mCamera.fovy = 60.f;
	mCamera.projection = CAMERA_PERSPECTIVE;
}

Camera3D* Vendaval::Editors::Scene::GetCamera()
{
	return &mCamera;
}

RenderTexture2D* Vendaval::Editors::Scene::GetViewportTexture()
{
	return &mViewportTexture;
}

void Vendaval::Editors::Scene::RenderEvent()
{
	mViewportPanelSize = ImGui::GetContentRegionAvail();
	const Rectangle sourceRect = {0.0f, 0.0f, static_cast<float>(mViewportTexture.texture.width),
								  -static_cast<float>(mViewportTexture.texture.height)};
	rlImGuiImageRect(&mViewportTexture.texture, static_cast<int>(mViewportPanelSize.x), static_cast<int>(mViewportPanelSize.y), sourceRect);

	static constexpr auto cameraMoveButton = MOUSE_BUTTON_RIGHT;
	if (ImGui::IsWindowHovered())
	{
		if (IsMouseButtonPressed(cameraMoveButton))
		{
			HideCursor();
		}
		else if (IsMouseButtonReleased(cameraMoveButton))
		{
			ShowCursor();
		}

		if (IsMouseButtonDown(cameraMoveButton))
		{
			// Rotate camera
			constexpr float sensitivity = 0.005f;
			const Vec3 worldUp = {0.0f, 1.0f, 0.0f};
			const auto mouseDelta = Vec2(GetMouseDelta());
			const float yawAngle = -mouseDelta.x * sensitivity;
			const float pitchAngle = -mouseDelta.y * sensitivity;
			auto cameraLookAt = (Vec3(mCamera.target) - Vec3(mCamera.position)).Normalized();
			const Vec3 cameraRight = cameraLookAt.Cross(worldUp).Normalized();
			const Quat yawRotation = Quat::FromAxisAngle(worldUp, yawAngle);
			const Quat pitchRotation = Quat::FromAxisAngle(cameraRight, pitchAngle);
			cameraLookAt = pitchRotation * yawRotation * cameraLookAt;
			mCamera.target = Vec3(mCamera.position) + cameraLookAt;
			auto cameraInput = Vec3(IsKeyDown(KEY_D) - IsKeyDown(KEY_A), IsKeyDown(KEY_E) - IsKeyDown(KEY_Q), IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
			cameraInput = cameraInput.Normalized();
			constexpr float moveSpeed = 5.0f;
			const float frameMoveAmount = moveSpeed * GetFrameTime();
			const Vec3 cameraUp = cameraRight.Cross(cameraLookAt).Normalized();
			Vec3 worldMovement = (cameraLookAt * cameraInput.z) + (cameraRight * cameraInput.x) + (cameraUp * cameraInput.y);
			worldMovement *= frameMoveAmount;
			mCamera.position = Vec3(mCamera.position) + worldMovement;
			mCamera.target = Vec3(mCamera.target) + worldMovement;
			SetMousePosition(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() / 2, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() / 2);
		}
	}
}

void Vendaval::Editors::Scene::PostRenderEvent()
{
	if (static_cast<int>(mViewportPanelSize.x) != mViewportTexture.texture.width ||
		static_cast<int>(mViewportPanelSize.y) != mViewportTexture.texture.height)
	{
		UnloadRenderTexture(mViewportTexture);
		mViewportTexture = LoadRenderTexture(static_cast<int>(mViewportPanelSize.x), static_cast<int>(mViewportPanelSize.y));
	}
}
