#pragma once
#include <../Hooks.h>

namespace Cobalt
{
	class Curl
	{
	private:
		Hook* mpCurlEasySetoptHook;

	public:
		Curl();
		~Curl();
	};
}