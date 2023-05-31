#ifndef __SNAIL_NONCOPYABLE_H__
#define __SNAIL_NONCOPYABLE_H__

namespace snail
{
    class Noncopyable 
    {
    public:
        Noncopyable() = default;
        ~Noncopyable() = default;
        Noncopyable(const Noncopyable&) = delete;
        Noncopyable& operator=(const Noncopyable&) = delete;
    };
}

#endif