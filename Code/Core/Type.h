#pragma once

#include <string>
#include <typeinfo>

#ifdef __GNUG__
#include <memory>
#include <cxxabi.h>

/**
 * @brief Demangles a typeid on GCC before extracting type name information 
*/
inline std::string_view Demangle(const char * typeName)
{
    int status = -1;

    std::unique_ptr<char, void(*)(void *)> result 
    {
        abi::__cxa_demangle(typeName, NULL, NULL, &status),
        std::free
    };

    return (status == 0) ? result.get() : typeName;
}
#else
/**
 * @brief Passes string through demangling if the compiler isn't GCC
*/
inline std::string Demangle(const char* typeName) { return typeName; }
#endif

/**
 * @brief Extracts the demangled typeinfo for a type
*/
template <typename T>
inline std::string_view Typeof() { return Demangle(typeid(T).name()); }
