#pragma once

#include "Transform.h"

namespace sad
{
	class PlayerController
	{
	public:
		PlayerController(Transform* t)
		{
			m_Transform = t;
		}
		~PlayerController();

		void Update();

	private:
		Transform* m_Transform;
	};
}