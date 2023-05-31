#include "../snail/uri.h"
#include <iostream>



int main(int argc, char **argv)
{
    snail::Uri::ptr uri = snail::Uri::Create("http://www.baidu.com/index.html?a=1&b=2#test");
    std::cout << uri->toString() << std::endl;
    auto addr = uri->createAddress();
    std::cout << addr->toString() << std::endl;
    return 0;
}