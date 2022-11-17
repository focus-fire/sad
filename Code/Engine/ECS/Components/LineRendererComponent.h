#pragma once

#include <algorithm>

#include <Core/Memory.h>
#include <Engine/LineRenderer.h>

namespace sad::ecs
{
	struct LineRendererComponent
	{
		core::Pointer<LineRenderer> m_LineRenderer;
	};
}
