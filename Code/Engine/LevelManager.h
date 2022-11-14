#pragma once

#include "Level.h"

namespace sad
{
	class LevelManager
	{
	public:
		/**
		 * @brief Deserializes entities of the last saved level from a JSON file
		 * @note SaveFile00 is the default location, saveFile default value is 1
		*/
		static Level* ImportLevel(int saveFile = 1);

		/**
		 * @brief Serializes entities of the level into a json save file
		 * @note saveFile default value is 1
		*/
		bool static ExportLevel(int saveFile = 1);
	};
}