#pragma once
#include "core/defines.hpp"

#pragma warning(push, 0)
	#include <spdlog/sinks/stdout_color_sinks.h>
	#include <spdlog/sinks/basic_file_sink.h>
	#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace tiny
{
	class LoggerSystem
	{
	public:
		static void initialize()
		{
			std::vector<spdlog::sink_ptr> sinks;
			sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
			sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("TinyEngine.log", true));

			sinks[0]->set_pattern("[%T] [%n] [%^%l%$]: %v");
			sinks[1]->set_pattern("[%D | %T] [%n] [%l]: %v");

			// Core Logger:
			m_Logger = std::make_shared<spdlog::logger>("Tiny", std::begin(sinks), std::end(sinks));
			spdlog::register_logger(m_Logger);

			m_Logger->set_level(spdlog::level::trace);
			m_Logger->flush_on(spdlog::level::trace);

			// Info Logger:
			m_InfoLogger = std::make_shared<spdlog::logger>("Info", sinks[0]);
			spdlog::register_logger(m_InfoLogger);

			m_InfoLogger->set_level(spdlog::level::trace);
			m_InfoLogger->flush_on(spdlog::level::trace);
		}

	public:
		static inline std::shared_ptr<spdlog::logger>& get_logger() { return m_Logger; }
		static inline std::shared_ptr<spdlog::logger>& get_info_logger() { return m_InfoLogger; }

	private:
		static inline std::shared_ptr<spdlog::logger> m_Logger;
		static inline std::shared_ptr<spdlog::logger> m_InfoLogger;
	};
}

#ifdef TINY_DEBUG
	#define TINY_LOG_CRITICAL(...)	{ ::tiny::LoggerSystem::get_logger()->critical(__VA_ARGS__); TINY_BREAKPOINT(); }
	#define TINY_LOG_ERROR(...)		::tiny::LoggerSystem::get_logger()->error(__VA_ARGS__)
	#define TINY_LOG_WARN(...)		::tiny::LoggerSystem::get_logger()->warn(__VA_ARGS__)
	#define TINY_LOG_TRACE(...)		::tiny::LoggerSystem::get_info_logger()->trace(__VA_ARGS__)
	#define TINY_LOG_DEBUG(...)		::tiny::LoggerSystem::get_logger()->debug(__VA_ARGS__)
	#define TINY_LOG_INFO(...)		::tiny::LoggerSystem::get_info_logger()->info(__VA_ARGS__)

#else
	#define TINY_LOG_CRITICAL(...)	{ ::tiny::LoggerSystem::get_logger()->critical(__VA_ARGS__); TINY_BREAKPOINT(); }
	#define TINY_LOG_ERROR(...)		::tiny::LoggerSystem::get_logger()->error(__VA_ARGS__)
	#define TINY_LOG_WARN(...)		{ }
	#define TINY_LOG_DEBUG(...)		{ }
	#define TINY_LOG_TRACE(...)		{ }
	#define TINY_LOG_INFO(...)		{ }
#endif