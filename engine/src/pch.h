#pragma once

// std libs
#include <set>
#include <random>
#include <limits>
#include <vector>
#include <string>
#include <bitset>
#include <cassert>
#include <memory>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>

// export api
#ifdef GE_EXPORT
    #ifdef _MSC_VER
        #define GE_API __declspec(dllexport)
    #else
        #define GE_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define GE_API __declspec(dllimport)
    #else
        #define GE_API
    #endif
#endif

#if defined(_DEBUG)
	#define GE_LOG(level, message, ...) \
        fprintf(stderr, "[%s] ", level); \
        fprintf(stderr, message, ##__VA_ARGS__); \
        fprintf(stderr, "\n");

	#define GE_WARNING(message, ...) GE_LOG("warning", message, ##__VA_ARGS__)
	#define GE_ERROR(message, ...) GE_LOG("error", message, ##__VA_ARGS__)
	#define GE_INFO(message, ...) GE_LOG("info", message, ##__VA_ARGS__)
#else
	#define GE_ERROR	
	#define GE_WARNING
    #define GE_INFO
#endif

#define GE_ASSERT assert

#if defined(__clang__) || defined(__gcc__)
    #define GE_STATIC_ASSERT _Static_assert
#else
    #define FUSE_STATIC_ASSERT static_assert
#endif

#if defined(__clang__) || defined(__gcc__)
    #define GE_INLINE __attribute__((always_inline)) inline
    #define GE_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
    #define GE_INLINE __forceinline
    #define GE_NOINLINE __declspec(noinline)
#else
    #define GE_INLINE inline
    #define GE_NOINLINE
#endif

// free allocated memory and sets it to null pointer
#define GE_DELETE(m) if (m != NULL) { delete (m); m = NULL; }

// attach lamda function to function call
#define GE_BIND(f) [this](auto&&... args)->decltype(auto) \
{ return this->f(std::forward<decltype(args)>(args)...); }

#define INVALID_ID 0

namespace ge {
	using uuid64 = size_t;
	GE_API uuid64 uuid();
}
