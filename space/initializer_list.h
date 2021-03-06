#pragma once

namespace std {
	template<class T> class initializer_list {	// list of pointers to elements
	public:
		typedef T value_type;
		typedef const T& reference;
		typedef const T& const_reference;
		typedef unsigned size_type;
		typedef const T* iterator;
		typedef const T* const_iterator;
		constexpr initializer_list() noexcept
			: first(0), last(0) {	// empty list
		}
		constexpr initializer_list(const T *first_arg,
			const T *last_arg) noexcept
			: first(first_arg), last(last_arg) {	// construct with pointers
		}
		constexpr const T *begin() const noexcept {	// get beginning of list
			return first;
		}
		constexpr const T *end() const noexcept {	// get end of list
			return last;
		}
		constexpr unsigned size() const noexcept {	// get length of list
			return last - first;
		}
	private:
		const T *first;
		const T *last;
	};
}