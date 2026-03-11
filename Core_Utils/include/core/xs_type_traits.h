#pragma once
#include <iostream>
#include <concepts>
namespace XS_Utils {
	namespace TypeTraits {
		// isPrintable, wirtten in C++20
		template<typename T>
		concept Printable = requires (std::ostream & os, T t) {
			{ os << t } -> std::convertible_to<std::ostream&>;
		};
	}
}