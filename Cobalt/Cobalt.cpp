#include <../Cobalt.h>
#include <../WinURL.h>
#include <../Memory.h>

namespace Cobalt
{
	cobalt::cobalt()
	{
		Memory::FortniteUtilConsole();
		printf("Cobalt Made By Ryse 404\n", BUILD_ID);
#ifdef _PROD
		printf("Starting Cobalt...\n");
#endif
		printf("Cobalt Developed On: %s at %s\n", __DATE__, __TIME__);

		m_pCurl = new Curl;
	}

	cobalt::~cobalt()
	{
		if (m_pCurl) {
			delete m_pCurl;
		}
	}
}