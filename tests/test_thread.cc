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
	while(true)
	{
		SNAIL_LOG_INFO(g_logger) << "xxxxxxxxxxxxxx";
	}
}
	

void func3()
{
	while(true)
	{
		SNAIL_LOG_INFO(g_logger) << "==============";
	}
}


int main(int argc, char** argv)
{
	SNAIL_LOG_INFO(g_logger) << "thread test begin" << std::endl;
	
	YAML::Node root = YAML::LoadFile("/home/liufx/workspace/snail/bin/conf/log2.yml");
	snail::Config::LoadFromYaml(root);

	std::vector<snail::Thread::ptr> thrs;
	for (int i = 0; i < 2; ++i)
	{
		snail::Thread::ptr thr(new snail::Thread(&func2, "name_" + std::to_string(i * 2)));
		snail::Thread::ptr thr2(new snail::Thread(&func3, "name_" + std::to_string(i  * 2 + 1)));
		thrs.push_back(thr);
		thrs.push_back(thr2);
	}

	for (size_t i = 0; i < thrs.size(); ++i)
	{
		thrs[i]->join();
	}
	SNAIL_LOG_INFO(g_logger) << "thread test end" << std::endl;
	SNAIL_LOG_INFO(g_logger) << "count =" << count << std::endl;
}