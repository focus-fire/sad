#pragma once
#include <stdlib.h>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace sad
{
	class JsonManager
	{
	public:
		JsonManager();

		static json GetJson(std::string path);

		/**
		 * @brief 
		 * Usage GetJsonContent<expectedDataType>(Json, key value)
		 * @sample
		 * std::string jsonContent = sad::JsonManager::GetJsonContent<json>(sad::JsonManager::GetJson(jsonPath), "entities").dump();
		 * @tparam T 
		 * json, int, or string
		 * @param Json 
		 * @param key 
		 * @return 
		*/
		template <typename T>
		inline static T GetJsonContent(const json& Json, std::string key)
		{
			return Json[key];
		};
	};
}