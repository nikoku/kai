#include <iostream>
#include "elapsed_time.hpp"

void elapsed_time_test()
{
    auto a=kai::processing_timer<>|[]()
    {
        
    };
    auto b=kai::processing_timer<std::chrono::nanoseconds>.repeat(300)|[]()
    {
    //‚È‚ñ‚â‚©‚ñ‚â
    };
    auto [max_i, max_v]=b.max();
    std::cout<<"max:"<<max_v<<"[ns] (index:"<<max_i<<")"<<std::endl;
    std::cout<<"min:"<<b.min().value<<"[ns]"<<std::endl;
    std::cout<<"ave:"<<b.average()<<"[ns]"<<std::endl;
    std::cout<<"sum:"<<b.sum()<<"[ns]"<<std::endl;
}
