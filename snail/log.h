#ifndef __SNAIL_LOG_H__
#define __SNAIL_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdarg>
#include <map>
#include "util.h"
#include "singleton.h"
#include "thread.h"

#define SNAIL_LOG_LEVEL(logger, level) \
	if(logger->getLevel() <= level) \
		snail::LogEventWrap(snail::LogEvent::ptr(new snail::LogEvent(logger, level, \
			__FILE__, __LINE__, 0, snail::GetThreadId(), \
			snail::GetFiberId(), time(0)))).getSS()

#define SNAIL_LOG_DEBUG(logger) SNAIL_LOG_LEVEL(logger, snail::LogLevel::DEBUG)
#define SNAIL_LOG_INFO(logger) SNAIL_LOG_LEVEL(logger, snail::LogLevel::INFO)
#define SNAIL_LOG_WARN(logger) SNAIL_LOG_LEVEL(logger, snail::LogLevel::WARN)
#define SNAIL_LOG_ERROR(logger) SNAIL_LOG_LEVEL(logger, snail::LogLevel::ERROR)
#define SNAIL_LOG_FATAL(logger) SNAIL_LOG_LEVEL(logger, snail::LogLevel::FATAL)

#define SNAIL_LOG_FMT_LEVEL(logger, level, fmt, ...) \
	if(logger->getLevel() <= level) \
		snail::LogEventWrap(snail::LogEvent::ptr(new snail::LogEvent(logger, level, \
							__FILE__, __LINE__, 0, snail::GetThreadId(), \
			snail::GetFiberId(), time(0)))).getEvent()->format(fmt, __VA_ARGS__)

#define SNAIL_LOG_FMT_DEBUG(logger, fmt, ...) SNAIL_LOG_FMT_LEVEL(logger, snail::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define SNAIL_LOG_FMT_INFO(logger, fmt, ...) SNAIL_LOG_FMT_LEVEL(logger, snail::LogLevel::INFO, fmt, __VA_ARGS__)
#define SNAIL_LOG_FMT_WARN(logger, fmt, ...) SNAIL_LOG_FMT_LEVEL(logger, snail::LogLevel::WARN, fmt, __VA_ARGS__)
#define SNAIL_LOG_FMT_ERROR(logger, fmt, ...) SNAIL_LOG_FMT_LEVEL(logger, snail::LogLevel::ERROR, fmt, __VA_ARGS__)
#define SNAIL_LOG_FMT_FATAL(logger, fmt, ...) SNAIL_LOG_FMT_LEVEL(logger, snail::LogLevel::FATAL, fmt, __VA_ARGS__)

#define SNAIL_LOG_ROOT() snail::LoggerMgr::GetInstance()->getRoot()
#define SNAIL_LOG_NAME(name) snail::LoggerMgr::GetInstance()->getLogger(name)

//日志输出器
namespace snail
{
	class Logger;
	class LoggerManager;

	class LogLevel
	{
	public:
		enum Level
		{
			UNKNOW = 0,
			DEBUG = 1,
			INFO = 2,
			WARN = 3,
			ERROR = 4,
			FATAL = 5
		};

		static const char* ToString(LogLevel::Level level);
		static LogLevel::Level FromString(const std::string& str);
	};

	class LogEvent
	{
	public:
		typedef std::shared_ptr<LogEvent> ptr;
		LogEvent(std::shared_ptr<Logger> looger, LogLevel::Level level
				, const char* file, int32_t line, uint32_t elapse
				, uint32_t thread_id, uint32_t fiber_id, uint64_t time);

		const char* getFile() const { return m_file; }
		int32_t getLine() const { return m_line; }
		uint32_t getElapse() const { return m_elapse; }
		uint32_t getThreadId() const { return m_threadId; }
		uint32_t getFiberId() const { return m_fiberId; }
		uint64_t getTime() const { return m_time; }
		const std::string& getThreadName() const { return m_threadName; }
		std::string getContent() const { return m_ss.str(); }
		std::shared_ptr<Logger> getLogger() const { return m_logger; }
		LogLevel::Level getLevel() const { return m_level; }
		std::stringstream& getSS() { return m_ss; }

		void format(const char* fmt, ...);
		void format(const char* fmt, va_list al);
	private:
		const char* m_file = nullptr;   //文件名
		int32_t m_line = 0;			   	//行号
		uint32_t m_elapse = 0;			//程序启动到现在的毫秒数
		uint32_t m_threadId = 0;		//线程id
		uint32_t m_fiberId = 0;		   	//协程id
		uint64_t m_time;				//时间戳
		std::string m_threadName;		// 线程名称
		std::stringstream m_ss;			// 日志内容流
		std::shared_ptr<Logger> m_logger; // 日志器
		LogLevel::Level m_level;		// 日志等级
	};

	class LogEventWrap
	{
	public:
		LogEventWrap(LogEvent::ptr e);
		~LogEventWrap();

		std::stringstream& getSS();
		LogEvent::ptr getEvent() const { return m_event; }
	private:
		LogEvent::ptr m_event;
	};

	//日志格式器
	class LogFormatter
	{
	public:
		typedef std::shared_ptr<LogFormatter> ptr;

		LogFormatter(const std::string& pattern);
	
		std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
		std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
		class FormatItem
		{
		public:
			typedef std::shared_ptr<FormatItem> ptr;
			virtual ~FormatItem() {}
			virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
		};

		void init();
		bool isError() const { return m_error; }
		const std::string getPattern() const{ return m_pattern; }

	private:
		std::string m_pattern;
		std::vector<FormatItem::ptr> m_items;
		bool m_error = false;
	};

	//日志输出地
	class LogAppender
	{
		friend class Logger;
	public:
		typedef std::shared_ptr<LogAppender> ptr;
		typedef NullMutex MutexType;
		virtual ~LogAppender() {}

		virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
		void setFormatter(LogFormatter::ptr val);
		void setLevel(LogLevel::Level level);
		LogFormatter::ptr getFormatter();
		LogLevel::Level getLevel() const { return m_level; }



		virtual std::string toYamlString() = 0;
	protected:
		LogLevel::Level m_level = LogLevel::DEBUG;
		LogFormatter::ptr m_formatter;
		bool m_hasFormatter = false;
		MutexType m_mutex;
	};

	//日志器
	class Logger : public std::enable_shared_from_this<Logger>
	{
		friend class LoggerManager;
	public:
		typedef std::shared_ptr<Logger> ptr;
		typedef NullMutex MutexType;

		Logger(const std::string& name = "root");

		void log(LogLevel::Level level, LogEvent::ptr event);
		void debug(LogEvent::ptr event);
		void info(LogEvent::ptr event);
		void warn(LogEvent::ptr event);
		void error(LogEvent::ptr event);
		void fatal(LogEvent::ptr event);

		void addAppender(LogAppender::ptr appender);
		void delAppender(LogAppender::ptr appender);
		void clearAppenders();
		LogLevel::Level getLevel() const { return m_level; }
		void setLevel(LogLevel::Level val) { m_level = val; }

		const std::string& getName() const { return m_name; }

		void setFormatter(LogFormatter::ptr val);
		void setFormatter(const std::string& val);
		LogFormatter::ptr getFormatter();

		std::string toYamlString();
	private:
		std::string m_name;			//日志名称
		LogLevel::Level m_level;	//日志级别
		std::list<LogAppender::ptr> m_appenders;			//Appender集合
		LogFormatter::ptr m_formatter;
		/// 主日志器
		Logger::ptr m_root;
		MutexType m_mutex;
	};


	class StdoutLogAppender : public LogAppender
	{

	public:
		typedef std::shared_ptr<StdoutLogAppender> ptr;
		virtual void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
		std::string toYamlString() override;
	};

	class FileLogAppender : public LogAppender 
	{

	public:
		typedef std::shared_ptr<FileLogAppender> ptr;
		FileLogAppender(const std::string& filename);
		virtual void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
	
		bool reopen();
		std::string toYamlString() override;
	private:
		std::string m_filename;
		std::ofstream m_filestream;
	};

	class LoggerManager
	{
	public:
		typedef NullMutex MutexType;

		LoggerManager();
		Logger::ptr getLogger(const std::string& name);

		void init();
		Logger::ptr getRoot() const { return m_root; }

		std::string toYamlString();
	private:
		MutexType m_mutex;
		std::map<std::string, Logger::ptr> m_loggers;
		Logger::ptr m_root;
	};

	typedef snail::Singleton<LoggerManager> LoggerMgr;
	
}



#endif
