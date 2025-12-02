#include <imgui.h>

#include "Camera.h"
#include "Mistral.h"

void Vendaval::Editors::Camera::RenderEvent()
{
	ImGui::SeparatorText("Camera");
	if (Mistral::GetActiveCamera())
	{
		ImGui::SliderFloat3("Position", &Mistral::GetActiveCamera()->position.x, -20.f,
							20.f);
		ImGui::SliderFloat3("Target", &Mistral::GetActiveCamera()->target.x, -20.f, 20.f);
		ImGui::SliderFloat("Fov", &Mistral::GetActiveCamera()->fovy, 1.f, 179.f);
	}
}