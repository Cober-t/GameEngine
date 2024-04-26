#ifndef LOG_H
#define LOG_H

#include <string>
#include "Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Cober {

	class Log
	{
	public:
		static enum LOG_LEVELS { TRACE = 0, DEBUG, INFO, WARN, ERR, CRITICAL };

		struct LogMessage
		{
			int line, level;
			
			spdlog::details::log_msg_buffer msgBuffer;
			uint32_t thread_id;
			std::string function, fileName, message, loggerName;

			std::string GetFormatedMessage()
			{
				return "[" + function + " - " + fileName + ": " + std::to_string(line) + "] " + std::to_string(level) + " " + message;
			}

			operator std::string()
			{
				return GetFormatedMessage();
			}
		};

	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		static std::vector<LogMessage> GetMessages();
		static void FormatMessages();


	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
		static std::vector<LogMessage> s_LogMessages;
	};
}


template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}


// Core log macros
#define LOG_CORE_TRACE(...)    SPDLOG_LOGGER_TRACE(::Cober::Log::GetCoreLogger(), __VA_ARGS__); 	::Cober::Log::FormatMessages();
#define LOG_CORE_INFO(...)     SPDLOG_LOGGER_INFO(::Cober::Log::GetCoreLogger(), __VA_ARGS__); 		::Cober::Log::FormatMessages();
#define LOG_CORE_WARNING(...)  SPDLOG_LOGGER_WARN(::Cober::Log::GetCoreLogger(), __VA_ARGS__); 		::Cober::Log::FormatMessages();
#define LOG_CORE_ERROR(...)    SPDLOG_LOGGER_ERROR(::Cober::Log::GetCoreLogger(), __VA_ARGS__); 	::Cober::Log::FormatMessages();
#define LOG_CORE_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::Cober::Log::GetCoreLogger(), __VA_ARGS__); 	::Cober::Log::FormatMessages();

// Client log macros
#define LOG_TRACE(...)         SPDLOG_LOGGER_TRACE(::Cober::Log::GetClientLogger(), __VA_ARGS__);	::Cober::Log::FormatMessages();
#define LOG_INFO(...)          SPDLOG_LOGGER_INFO(::Cober::Log::GetClientLogger(), __VA_ARGS__);	::Cober::Log::FormatMessages();
#define LOG_WARNING(...)       SPDLOG_LOGGER_WARN(::Cober::Log::GetClientLogger(), __VA_ARGS__);	::Cober::Log::FormatMessages();
#define LOG_ERROR(...)         SPDLOG_LOGGER_ERROR(::Cober::Log::GetClientLogger(), __VA_ARGS__);	::Cober::Log::FormatMessages();
#define LOG_CRITICAL(...)      SPDLOG_LOGGER_CRITICAL(::Cober::Log::GetClientLogger(), __VA_ARGS__);::Cober::Log::FormatMessages();


// #define LOG_CORE_TRACE(...)    ::Cober::Log::GetCoreLogger()->trace(__VA_ARGS__)
// #define LOG_CORE_INFO(...)     ::Cober::Log::GetCoreLogger()->info(__VA_ARGS__)
// #define LOG_CORE_WARNING(...)  ::Cober::Log::GetCoreLogger()->warn(__VA_ARGS__)
// #define LOG_CORE_ERROR(...)    ::Cober::Log::GetCoreLogger()->error(__VA_ARGS__)
// #define LOG_CORE_CRITICAL(...) ::Cober::Log::GetCoreLogger()->critical(__VA_ARGS__)

// // Client log macros
// #define LOG_TRACE(...)         ::Cober::Log::GetClientLogger()->trace(__VA_ARGS__)
// #define LOG_INFO(...)          ::Cober::Log::GetClientLogger()->info(__VA_ARGS__)
// #define LOG_WARNING(...)       ::Cober::Log::GetClientLogger()->warn(__VA_ARGS__)
// #define LOG_ERROR(...)         ::Cober::Log::GetClientLogger()->error(__VA_ARGS__)
// #define LOG_CRITICAL(...)      ::Cober::Log::GetClientLogger()->critical(__VA_ARGS__)


#endif