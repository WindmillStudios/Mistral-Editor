#include "VendavalRenderPipeline.h"

int main()
{
	Mistral::StartApplication("Mistral Engine | Core: 0.0.1 | Editor: 0.0.1",
	                          std::make_unique<Vendaval::VendavalRenderPipeline>());

	return 0;
}