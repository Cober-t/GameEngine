#include <pch.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/dup_filter_sink.h>
#include <iostream>
#include <sstream>

#include "Log.h"

namespace Cober {

	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;
	std::vector<Log::LogMessage> Log::s_LogMessages;
	Ref<spdlog::sinks::ringbuffer_sink_mt> s_BufferMessages;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		s_BufferMessages = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(5);

		logSinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Engine.log", true));
		logSinks.push_back(std::make_shared<spdlog::sinks::dup_filter_sink_st>(std::chrono::seconds(1)));
		logSinks.push_back(s_BufferMessages);

		logSinks[0]->set_pattern("%^[%! %s - %#] %n: %v%$");

		s_CoreLogger = std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}


	void Log::FormatMessages()
	{
		auto rawMsg = s_BufferMessages->last_raw().back();
		if (s_LogMessages.empty() || s_LogMessages.back().msgBuffer.time != rawMsg.time)
		{
			LogMessage logMessage;
			logMessage.msgBuffer = rawMsg;

			std::string formattedString(rawMsg.payload.data(), rawMsg.payload.size());
			logMessage.message = formattedString;

			std::string funcName = rawMsg.source.funcname;
			logMessage.function = funcName.substr(funcName.find_last_of("::") + 1, funcName.length());;

			std::string fileName = rawMsg.source.filename;
			logMessage.fileName = fileName.substr(fileName.find_last_of("\\") + 1, fileName.find_last_of("."));

			logMessage.line = rawMsg.source.line;
			logMessage.level = rawMsg.level;

			s_LogMessages.push_back(logMessage);
		}
	}
}