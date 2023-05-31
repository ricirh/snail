#ifndef __SNAIL_IOMANAGER_H__
#define __SNAIL_IOMANAGER_H__

#include "scheduler.h"
#include "thread.h"
#include "timer.h"
#include "fiber.h"

namespace snail
{
    class IOManager : public Scheduler, public TimerManager
    {
    public:
        typedef std::shared_ptr<IOManager> ptr;
        typedef RWMutex MutexType;

        enum Event
        {
            NONE = 0x0,
            READ = 0x1,
            WRITE = 0x4,
        };

    private:
        struct FdContext
        {
            typedef Mutex MutexType;
            struct EventContext
            {
                Scheduler* scheduler = nullptr;
                Fiber::ptr fiber;
                std::function<void()> cb;
            };

            EventContext& getContext(Event event);
            void resetContext(EventContext& ctx);
            void triggerEvent(Event event);

            EventContext read;
            EventContext write;
            int fd = 0;
            Event m_events = NONE;
            MutexType m_mutex;
        };
    
    public:
        IOManager(size_t threads = 1, bool use_caller = true, const std::string& name = "");
        ~IOManager();

        int addEvent(int fd, Event event, std::function<void()> cb = nullptr);
        bool delEvent(int fd, Event event);
        bool cancelEvent(int fd, Event event);

        bool cancelAll(int fd);

        static IOManager* GetThis();

    protected:
        void tickle() override;
        bool stopping() override;
        bool stopping(uint64_t& timeout);
        void idle() override;
        
        void contextResize(size_t size);

        void OnTimerInsertAtFront() override;
    private:
        int m_epfd = 0;
        int m_tickleFds[2];
        
        std::atomic<size_t> m_pendingEventCount = {0};
        MutexType m_mutex;
        std::vector<FdContext*> m_fdContexts;


    };
}

#endif