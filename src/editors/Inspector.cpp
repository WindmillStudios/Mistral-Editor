#include "imgui.h"
#include "Inspector.h"

Mistral::Component* Vendaval::Editors::Inspector::GetSelectedComponent() const
{
	return mSelectedComponent;
}

void Vendaval::Editors::Inspector::SetSelectedComponent(Mistral::Component* component)
{
	mSelectedComponent = component;
}

void Vendaval::Editors::Inspector::RenderEvent()
{
	if (!mSelectedComponent)
	{
		return;
	}

	ImGui::SeparatorText("Spatial");

	auto position = mSelectedComponent->GetSpatial().GetLocalPosition();
	if (ImGui::SliderFloat3("Position", &position.x, -10.f, 10.f))
	{
		mSelectedComponent->GetSpatial().SetPosition(position);
	}

	Vec3 rotation = mSelectedComponent->GetSpatial().GetLocalRotation().ToEuler() * 180.f / PI;
	bool rotationChanged = false;
	rotationChanged |= ImGui::DragFloat("Roll (X)", &rotation.x, 0.5f, -360.0f, 360.0f, "%.2f deg");
	rotationChanged |= ImGui::DragFloat("Pitch (Y)", &rotation.y, 0.5f, -89.9f, 89.9f, "%.2f deg");
	rotationChanged |= ImGui::DragFloat("Yaw (Z)", &rotation.z, 0.5f, -360.0f, 360.0f, "%.2f deg");
	if (rotationChanged)
	{
		mSelectedComponent->GetSpatial().SetRotation(Quat::FromEuler(rotation * PI / 180.0f));
	}

	auto scale = mSelectedComponent->GetSpatial().GetLocalScale();
	if (ImGui::SliderFloat3("Scale", &scale.x, 0.f, 10.f))
	{
		mSelectedComponent->GetSpatial().SetScale(scale);
	}
}