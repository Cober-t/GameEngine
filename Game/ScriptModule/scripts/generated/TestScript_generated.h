#ifndef TEST_SCRIPT_GENERATED_H
#define TEST_SCRIPT_GENERATED_H

#define ENTT_STANDARD_CPP
#include <entt/entt.hpp>

#include <imgui.h>
#include <map>

// #include "../TestScript.h"

using namespace entt::literals;

namespace Cober {

    namespace ReflectTestScript {

        // bool initialized = false;
        
        // std::vector<entt::id_type> ids = 
        // {
        //     "TestScript::m_ExampleFloatProperty"_hs,
        //     "TestScript::m_ExampleIntProperty"_hs
        // };

        // std::map<entt::id_type, const char*> debugNames;
        // std::map<std::string, entt::id_type> stringToMap;
        

        // void InitScripts(Entity* entity)
        // {
        //     if (initialized) return;

        //     initialized = true;
            
        //     // debugNames = {
        //     //     {entt::type_id<TestScript>().hash(), "TestScript"},
        //     //     {ids[0], "m_ExampleFloatProperty"},
        //     //     {ids[1], "m_ExampleIntProperty"}
        //     // };

        //     // stringToMap = {
        //     //     {"TestScript", entt::type_id<TestScript>().hash() }
        //     // };

        //     // auto testScriptFactory = entt::meta<TestScript>()
        //     //     .data<&TestScript::m_ExampleFloatProperty, entt::as_ref_t>(ids[0])
        //     //     .data<&TestScript::m_ExampleIntProperty, entt::as_ref_t>(ids[1]);

        //     // entity->GetComponent<TestScript>().entity = entity;
        //     // entity->GetComponent<TestScript>().OnStart();
        // }
    }
}

#endif