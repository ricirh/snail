#include "../snail/snail.h"
#include <unistd.h>
snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

int count = 0;
//snail::RWMutex s_mutex;
snail::Mutex s_mutex;

void func1()
{
	
	SNAIL_LOG_INFO(g_logger) << "name: " << snail::Thread::GetName()
		<< " this.name: " << snail::Thread::GetThis()->GetName()
		<< " id: " << snail::GetThreadId()
		<< " this.id: " << snail::Thread::GetThis()->getId();

	for (int i = 0; i < 100000; ++i)
	{
		//snail::RWMutex::WriteLock lock(s_mutex);
		snail::Mutex::Lock lock(s_mutex);
		++count;
	}

}

void func2()
{

}


int main(int argc, char** argv)
{
	SNAIL_LOG_INFO(g_logger) << "thread test begin" << std::endl;
	std::vector<snail::Thread::ptr> thrs;
	for (int i = 0; i < 5; ++i)
	{
		snail::Thread::ptr thr(new snail::Thread(&func1, "name_" + std::to_string(i)));
		thrs.push_back(thr);
	}

	for (int i = 0; i < 5; ++i)
	{
		thrs[i]->join();
	}
	SNAIL_LOG_INFO(g_logger) << "thread test end" << std::endl;
	SNAIL_LOG_INFO(g_logger) << "count =" << count << std::endl;
}