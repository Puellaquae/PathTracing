#pragma once
#include <Common.h>

namespace RayTrace
{
	struct IRepresent
	{
		IRender* render;
		
		virtual void Run() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void Stop() = 0;
	};
}
