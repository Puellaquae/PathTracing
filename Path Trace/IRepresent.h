#pragma once
#include <functional>
#include "Common.h"

namespace RayTrace
{
	struct IRepresent
	{
		IRender* render;
		std::function<void(IRepresent*)> onUpdate = nullptr;
		
		virtual void Run() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void Stop() = 0;
	};
}
