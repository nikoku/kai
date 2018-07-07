#include <iostream>
#include "lazy.hpp"

int ret_1()
{
    std::cout << "calc" << std::endl;
    return 1;
}

void test(kai::lazy_param<int> lazy)
{
    std::cout << "test start" << std::endl;
    std::cout << *lazy << std::endl;
}

void lazy()
{
    test(kai::lazy_arg(ret_1()+2));
    test(kai::lazy_arg{[&]{return ret_1()+2;}});
}
