#pragma once

#include <vector>

#include <imgui.h>
#include <glm/glm.hpp>

namespace cap
{
	class GizmoSystem
	{
	public:
		void Render();

		std::vector<glm::vec3> UpdateGizmos(float* modelMatrix, bool transformDecomposition);

		static ImGuiWindowFlags s_GameWindowFlags;

	private:
		int m_LastGizmoUsed;
	};
}
