#pragma once

#include "Mistral.h"

namespace Vendaval
{
	class VendavalRenderPipeline final : public Mistral::IRenderPipeline
	{
	  public:

		~VendavalRenderPipeline() override = default;

		void Initialize() override;

		void RenderEvent() override;

	  private:

		static void InitFonts();

		static void RenderMainMenuBar();
	};
} // namespace Vendaval
