#pragma once
#include <type_traits>

/// @cond
#if defined(KAI_USE_BOOST_OPTIONAL)
#include <boost/optional.hpp>
#define KAI_OPTIONAL boost::optional
#else
#include <optional>
#define KAI_OPTIONAL std::optional
#endif
/// @endcond

/**
* @file lazy.hpp
* @brief 関数の引数に遅延評価を導入するためのクラス群
*/

namespace kai
{
	/**
	* @brief lazy_argの基底クラス。ユーザーコードから明示的に使用しないクラス
	*/
    template<typename T>
    class lazy_base
    {
        static_assert(!std::is_reference<T>::value, "kai::lazy_arg argument type must not be reference!");
        public:
        auto& operator*()const noexcept
        {
            return value();
        }
		protected:
        virtual T& value()const noexcept=0;
		lazy_base()=default;
        virtual ~lazy_base()=default;
        private:
    };

	/**
	* @brief 遅延評価する仮引数の型
	* @tparam T 遅延評価する値の型
	*/
    template<typename T>
    using lazy_param=lazy_base<T>&&;

    template<typename Return, typename Lambda>
    class lazy_arg final : public lazy_base<Return>
    {
        public:
		/**
		* @brief 初回呼び出し時のみ計算
		*/
        Return& value()const noexcept override
        {
            if(!impl)impl=std::move(arg());
            return *impl;
        }
        lazy_arg(Lambda &&arg)noexcept:arg{std::move(arg)}{}
        private:
        const Lambda arg;
        mutable KAI_OPTIONAL<Return> impl;
    };
    
    template<typename T>
    lazy_arg(T)->lazy_arg<std::invoke_result_t<T>, T>;
}

#undef KAI_OPTIONAL
/**
* @brief 実引数記載時の補助マクロ
* @details 
*/
#define lazy_arg(ARG) lazy_arg([&](){return ARG;})
