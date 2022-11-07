#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

/**
 * @brief
 * nlohmann-json based entt-deserialization
*/
class NJSONInputArchive
{
private:
    nlohmann::json m_Root;
    nlohmann::json m_Current;

    int m_RootIndex = -1;
    int m_CurrentIndex = 0;

public:
    NJSONInputArchive(const std::string& json_string)
        :m_Root(nlohmann::json::parse(json_string))
    {}

    void next_root()
    {
        m_RootIndex++;
        if (m_RootIndex >= m_Root.size())
        {
            core::Log(ELogType::Error, "m_RootIndex exceeded or equal to size of m_Root. No idea what this means");
            return;
        }
        m_Current = m_Root[m_RootIndex];
        m_CurrentIndex = 0;
    }

    void operator()(std::underlying_type_t<entt::entity>& s)
    {
        next_root();
        int size = m_Current[0].get<int>();
        m_CurrentIndex++;
        s = static_cast<std::underlying_type_t<entt::entity>>(size); // pass amount to entt
    }

    void operator()(entt::entity& entity)
    {
        uint32_t ent = m_Current[m_CurrentIndex].get<uint32_t>();
        entity = entt::entity(ent);
        m_CurrentIndex++;
    }

    template<typename T>
    void operator()(entt::entity& ent, T& t)
    {
        nlohmann::json component_data = m_Current[m_CurrentIndex * 2];

        auto comp = component_data.get<T>();
        t = comp;

        uint32_t _ent = m_Current[m_CurrentIndex * 2 - 1];
        ent = entt::entity(_ent); // last element is the entity-id
        m_CurrentIndex++;
    }
};