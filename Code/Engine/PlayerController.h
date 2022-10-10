#pragma once

#include "Transform.h"

namespace sad
{
	class PlayerController
	{
	public:
		PlayerController(Transform* t)
		{
			transform = t;
		}
		~PlayerController();

		void Update();

	private:
		Transform* transform;
	};
}