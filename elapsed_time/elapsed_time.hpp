#include <cstddef>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <utility>
#include <vector>

namespace kai
{
    template<typename duration>
    class elapsed_time final
    {
        public:
            template<typename Lambda>
            elapsed_time(std::size_t times, Lambda&& lambda)
            {
                using namespace std::chrono;
                results.reserve(times);
                for(std::size_t i=0;i<times;++i)
                {
                    auto start=high_resolution_clock::now();
                    lambda();
                    auto end=high_resolution_clock::now();
                    results.push_back(duration_cast<duration>(end-start));
                }
            }
            template<typename ToDuration=duration>
            auto max()const
            {
                auto result=std::max_element(results.begin(), results.end());
                return tuple
                {
                    std::distance(results.begin(), result),
                    std::chrono::duration_cast<ToDuration>(*result).count()
                };
            }
            auto max()const{return max<duration>();}
            template<typename ToDuration=duration>
            auto min()const
            {
                auto result=std::min_element(results.begin(), results.end());
                return tuple
                {
                    std::distance(results.begin(), result),
                    std::chrono::duration_cast<ToDuration>(*result).count()
                };
            }
            auto min()const{return min<duration>();}
            template<typename ToDuration=duration>
            auto sum()const
            {
                std::vector<typename ToDuration::rep> count(results.size());
                std::transform(results.begin(), results.end(), count.begin(), [](const duration& n)
                {
                    return std::chrono::duration_cast<ToDuration>(n).count();
                });
                return std::accumulate(count.begin(), count.end(), 0);
            }
            auto sum()const{return sum<duration>();}
            template<typename ToDuration=duration>
            auto average()const
            {
                return sum<ToDuration>()/results.size();
            }
            auto average()const{return average<duration>();}
        private:
            std::vector<duration> results;
            template<typename Rep>
            struct tuple
            {
                typename std::vector<duration>::difference_type index;
                Rep value;
                constexpr tuple(typename std::vector<duration>::difference_type index, Rep value)
                :index{index},value{value}
                {}
            };
    };
    
    template<typename Duration>
    class processing_timer_t final
    {
        public:
        constexpr processing_timer_t(){}
        constexpr processing_timer_t(std::size_t times):times{times}{}
        template<typename Lambda>
        [[nodiscard]]constexpr auto operator|(Lambda&& lambda)const
        {
            return elapsed_time<Duration>(times, std::forward<Lambda>(lambda));
        }
        constexpr auto repeat(std::size_t times)const
        {
            return processing_timer_t(times);
        }
        private:
        const std::size_t times=1;
    };
    template<typename Duration=std::chrono::nanoseconds>
    constexpr inline processing_timer_t<Duration> processing_timer;
}
