#pragma once
#include <cstddef>

namespace kai
{
	template<typename BaseIterator>
	class zip_with_index_iterator : public BaseIterator
	{
		public:
		zip_with_index_iterator(BaseIterator base) : BaseIterator(base)
		{}
		auto& operator++()
		{
			++index;
			this->BaseIterator::operator++();
			return *this;
		}
		auto operator*()const noexcept
		{
			struct
			{
				std::size_t index;
				typename BaseIterator::value_type& value;
			}ret{index, *static_cast<BaseIterator const&>(*this)};
			return ret;
		}
		private:
		std::size_t index=0;
	};

	template<typename Range>
	class zip_with_index_impl
	{
		public:
		zip_with_index_impl(Range &range):range(range)
		{}
		auto begin()const
		{
			return zip_with_index_iterator{std::begin(range)};
		}
		auto end()const
		{
			return zip_with_index_iterator{std::end(range)};
		}
		private:
		Range &range;
	};

	inline constexpr class
	{}zip_with_index;

	template<typename T>
	inline auto operator|(T& container, [[maybe_unused]]decltype(zip_with_index))
	{
		return zip_with_index_impl(container);
	}
}