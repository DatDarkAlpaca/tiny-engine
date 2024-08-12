#pragma once

#if defined(_DEBUG) && !defined(TINY_DEBUG)
	#define TINY_DEBUG
#elif defined(NDEBUG) && !defined(TINY_RELEASE)
	#define TINY_RELEASE
#endif

#ifdef TINY_PLATFORM_WIN32
	#define TINY_BREAKPOINT() __debugbreak()
#elif defined(__linux__)
	#define TINY_PLATFORM_LINUX
	#define TINY_BREAKPOINT() __builtin_trap()
#elif defined(__APPLE__)
	#define TINY_PLATFORM_APPLE
	#define TINY_BREAKPOINT() __builtin_trap()
#elif defined(__ANDROID__)
	#define TINY_PLATFORM_ANDROID
	#define TINY_BREAKPOINT() __builtin_trap()
#else
	#error Unsupported platform
#endif