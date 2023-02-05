#include <iostream>
#include "../snail/log.h"
#include "../snail/util.h"

int main(int argc, char** argv)
{
	snail::Logger::ptr logger(new snail::Logger);
	logger->addAppender(snail::LogAppender::ptr(new snail::StdoutLogAppender));
	//snail::LogEvent::ptr event(new snail::LogEvent(__FILE__, __LINE__, 0, snail::GetThreadId(), snail::GetFiberId(), time(0)));
	//event->getSS() << "hello snail log";
	//logger->log(snail::LogLevel::DEBUG, event);
	snail::FileLogAppender::ptr file_appender(new snail::FileLogAppender("./log.txt"));
	file_appender->setLevel(snail::LogLevel::ERROR);

	snail::LogFormatter::ptr fmt(new snail::LogFormatter("%d%T%m\n"));
	file_appender->setFormatter(fmt);
	logger->addAppender(file_appender);
	SNAIL_LOG_INFO(logger) << "test macro";
	SNAIL_LOG_ERROR(logger) << "test macro error";

	SNAIL_LOG_FMT_ERROR(logger, "test macro fmt error \s", "aa");

	auto l = snail::LoggerMgr::GetInstance()->getLogger("xx");
	SNAIL_LOG_INFO(l) << "xxx";
	return 0;
}