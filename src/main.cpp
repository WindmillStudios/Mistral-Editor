#include "VendavalRenderPipeline.h"

int main()
{
	Mistral::StartApplication("Vendaval", std::make_unique<Vendaval::VendavalRenderPipeline>());

	return 0;
}