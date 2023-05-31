#include "../snail/bytearray.h"
#include "../snail/log.h"
#include "../snail/macro.h"

static snail::Logger::ptr g_logger = SNAIL_LOG_ROOT();

void test()
{

/*
    std::vector<int8_t> vec;
    for(int i = 0; i < 1; ++i)
    { \
        vec.push_back(rand());
    } \
    snail::ByteArray::ptr ba(new snail::ByteArray(1));
    for(auto &i : vec)
    {
        ba->writeFuint8(i);
    }
    ba->setPosition(0);
    for(size_t i = 0; i < vec.size(); ++i)
    {
        int32_t v = ba->readFint8();
        SNAIL_ASSERT(v == vec[i]);
    }
    SNAIL_ASSERT(ba->getReadSize() == 0);
    SNAIL_LOG_INFO(g_logger) << "writeFuint8" "/" "readFint8" 
        " (" "int8_t" " ) len=" << 1
        << "base_len=" << 1;
*/

#define XX(type, len, write_fun, read_fun, base_len) \
    { \
        std::vector<type> vec; \
        for(int i = 0; i < len; ++i) \
        { \
            vec.push_back(rand()); \
        } \
        snail::ByteArray::ptr ba(new snail::ByteArray(base_len)); \
        for(auto &i : vec) \
        { \
            ba->write_fun(i); \
        } \
        ba->setPosition(0); \
        for(size_t i = 0; i < vec.size(); ++i) \
        { \
            type v = ba->read_fun(); \
            SNAIL_ASSERT(v == vec[i]); \
        } \
        SNAIL_ASSERT(ba->getReadSize() == 0); \
        SNAIL_LOG_INFO(g_logger) << #write_fun "/"  #read_fun \
            " (" #type " ) len=" << len \
            << ", base_len=" << base_len \
            << " size=" << ba->getSize(); \
    }

    XX(int8_t, 100, writeFuint8, readFint8, 1);
    XX(uint8_t, 100, writeFuint8, readFuint8, 1);
    XX(int16_t, 100, writeFint16, readFint16, 1);
    XX(uint16_t, 100, writeFuint16, readFuint16, 1);
    XX(int32_t, 100, writeFint32, readFint32, 1);
    XX(uint32_t, 100, writeFuint32, readFuint32, 1);
    XX(int64_t, 100, writeFint64, readFint64, 1);
    XX(uint64_t, 100, writeFuint64, readFuint64, 1);

    XX(int32_t, 100, writeInt32, readInt32, 1);
    XX(uint32_t, 100, writeUint32, readUint32, 1);
    XX(int64_t, 100, writeInt64, readInt64, 1);
    XX(uint64_t, 100, writeUint64, readUint64, 1);


#undef XX


#define XX(type, len, write_fun, read_fun, base_len) \
    { \
        std::vector<type> vec; \
        for(int i = 0; i < len; ++i) \
        { \
            vec.push_back(rand()); \
        } \
        snail::ByteArray::ptr ba(new snail::ByteArray(base_len)); \
        for(auto &i : vec) \
        { \
            ba->write_fun(i); \
        } \
        ba->setPosition(0); \
        for(size_t i = 0; i < vec.size(); ++i) \
        { \
            type v = ba->read_fun(); \
            SNAIL_ASSERT(v == vec[i]); \
        } \
        SNAIL_ASSERT(ba->getReadSize() == 0); \
        SNAIL_LOG_INFO(g_logger) << #write_fun "/"  #read_fun \
            " (" #type " ) len=" << len \
            << ", base_len=" << base_len \
            << " size=" << ba->getSize(); \
        ba->setPosition(0); \
        SNAIL_ASSERT(ba->writeToFile("/tmp/" #type "_" #len "-" #read_fun ".dat")); \
        snail::ByteArray::ptr ba2(new snail::ByteArray(base_len * 2)); \
        SNAIL_ASSERT(ba2->readFromFile("/tmp/" #type "_" #len "-" #read_fun ".dat")); \
        ba2->setPosition(0); \
        SNAIL_ASSERT(ba->toString() == ba2->toString()); \
        SNAIL_ASSERT(ba->getPosition() == 0); \
        SNAIL_ASSERT(ba2->getPosition() == 0); \
    }

    XX(int8_t, 100, writeFuint8, readFint8, 1);
    XX(uint8_t, 100, writeFuint8, readFuint8, 1);
    XX(int16_t, 100, writeFint16, readFint16, 1);
    XX(uint16_t, 100, writeFuint16, readFuint16, 1);
    XX(int32_t, 100, writeFint32, readFint32, 1);
    XX(uint32_t, 100, writeFuint32, readFuint32, 1);
    XX(int64_t, 100, writeFint64, readFint64, 1);
    XX(uint64_t, 100, writeFuint64, readFuint64, 1);

    XX(int32_t, 100, writeInt32, readInt32, 1);
    XX(uint32_t, 100, writeUint32, readUint32, 1);
    XX(int64_t, 100, writeInt64, readInt64, 1);
    XX(uint64_t, 100, writeUint64, readUint64, 1);
#undef XX

}

int main(int argc, char** argv[])
{
    test();
    return 0;
}