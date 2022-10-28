#pragma once

#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

// nlohmann-json based entt-(de)serialization
class NJSONOutputArchive
{
public:
    NJSONOutputArchive()
    {
        m_Root = nlohmann::json::array();
    };

    // new element for serialization. giving you the amount of elements that is going to be
    // pumped into operator()(entt::entity ent) or operator()(entt::entity ent, const T &t)
    void operator()(std::underlying_type_t<entt::entity> size)
    {
        int a = 0;
        if (!m_Current.empty()) {
            m_Root.push_back(m_Current);
        }
        m_Current = nlohmann::json::array();
        m_Current.push_back(size); // first element of each array keeps the amount of elements. 
    }

    // persist entity ids
    void operator()(entt::entity entity)
    {
        // Here it is assumed that no custom entt-type is chosen
        m_Current.push_back((uint32_t)entity);
    }

    // persist components
    // ent is the entity and t a component that is attached to it
    // in json we first push the entity-id and then convert the component
    // to json just by assigning:  'nlohmann:json json=t'
    // For this to work all used component musst have following in its body:
    // NLOHMANN_DEFINE_TYPE_INTRUSIVE([component_name], fields,....)
    // e.g.
    // struct Transform {
    //     float x;
    //     float y;
    // 
    //     NLOHMANN_DEFINE_TYPE_INTRUSIVE(Transform, x,y)
    // };
    //
    template<typename T>
    void operator()(entt::entity ent, const T& t)
    {
        m_Current.push_back((uint32_t)ent); // persist the entity id of the following component

        // auto factory = entt::type_id<T>();
        // std::string component_name = std::string(factory.name()); 
        // current.push_back(component_name);

        nlohmann::json json = t;
        m_Current.push_back(json);
    }

    void Close()
    {
        if (!m_Current.empty()) {
            m_Root.push_back(m_Current);
        }
    }


    // create a json as string
    const std::string AsString()
    {
        std::string output = m_Root.dump();
        return output;
    }

    // create bson-data
    const std::vector<uint8_t> AsBson()
    {
        std::vector<std::uint8_t> as_bson = nlohmann::json::to_bson(m_Root);
        return as_bson;
    }

private:
    nlohmann::json m_Root;
    nlohmann::json m_Current;
};

class NJSONInputArchive
{
private:
    nlohmann::json m_Root;
    nlohmann::json m_Current;

    int m_RootIndex = -1;
    int m_CurrentIndex = 0;

public:
    NJSONInputArchive(const std::string& json_string)
    {
        m_Root = nlohmann::json::parse(json_string);
    };

    ~NJSONInputArchive(){}

    void next_root()
    {
        m_RootIndex++;
        if (m_RootIndex >= m_Root.size())
        {
            // ERROR
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
        s = (std::underlying_type_t<entt::entity>)size; // pass amount to entt
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