#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

namespace sad
{
	/**
	 * @brief Serializes entt-snapshots into nlohmann-json
	*/
	class LevelSnapshotOutput
	{
	public:
		LevelSnapshotOutput()
			: m_Root(nlohmann::json::array())
		{}

		/**
		 * @brief 
		 * // new element for serialization. giving you the amount of elements that is going to be
		   // pumped into operator()(entt::entity ent) or operator()(entt::entity ent, const T &t)
		 * @param size 
		*/
		void operator()(std::underlying_type_t<entt::entity> size)
		{
			int a = 0;
			if (!m_Current.empty())
			{
				m_Root.push_back(m_Current);
			}
			m_Current = nlohmann::json::array();
			m_Current.push_back(size); // first element of each array keeps the amount of elements. 
		}

		/**
		 * @brief 
		 * persist entity ids
		 * @param entity 
		*/
		void operator()(entt::entity entity)
		{
			// Here it is assumed that no custom entt-type is chosen
			m_Current.push_back(static_cast<uint32_t>(entity));
		}

		template<typename T>
		void operator()(entt::entity ent, const T& t)
		{
			m_Current.push_back(static_cast<uint32_t>(ent)); // persist the entity id of the following component
			nlohmann::json json = t;
			m_Current.push_back(json);
		}

		void Close()
		{
			if (!m_Current.empty())
			{
				m_Root.push_back(m_Current);
			}
		}

		/**
		 * @brief 
		 * create a json as string
		 * @return 
		*/
		const std::string AsString()
		{
			return m_Root.dump();
		}

	private:
		nlohmann::json m_Root;
		nlohmann::json m_Current;
	};
}