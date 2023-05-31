#ifndef __SNAIL_ENDIAN_H__
#define __SNAIL_ENDIAN_H__

#define SNAIL_LITTLE_ENDIAN 1
#define SNAIL_BIG_ENDIAN 2


#include <byteswap.h>
#include <stdint.h>

namespace snail
{

    template<class T>
    typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
    byteswap(T value) 
    {
        return (T)bswap_64((uint64_t)value);
    }

    template<class T>
    typename std::enable_if<sizeof(T) == sizeof(uint32_t), T>::type
    byteswap(T value) 
    {
        return (T)bswap_32((uint32_t)value);
    }

    template<class T>
    typename std::enable_if<sizeof(T) == sizeof(uint16_t), T>::type
    byteswap(T value) 
    {
        return (T)bswap_16((uint16_t)value);
    }

    #if BYTE_ORDER == BIG_ENDIAN
    #define SNAIL_BYTE_ORDER SNAIL_BIG_ENDIAN
    #else
    #define SNAIL_BYTE_ORDER SNAIL_LITTLE_ENDIAN
    #endif

    #if SNAIL_BYTE_ORDER == SNAIL_BIG_ENDIN
    template<class T>
    T byteswapOnLittleEndian(T t) 
    {
        return t;
    }

    template <class T>
    T byteswapOnBigEndian(T t) 
    {
        return byteswap(t);
    }

    #else
    template<class T>
    T byteswapOnLittleEndian(T t)
    {
        return byteswap(t);
    }

    template<class T>
    T byteswapOnBigEndian(T t)
    {
        return t;
    }
    #endif
}




#endif