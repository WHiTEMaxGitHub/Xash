#pragma once
#include "core/xs_type_traits.h"
#include <vector>
#include <concepts>
namespace XS_Utils {
	template<typename T>
	void print_vector(const std::vector<T>& container) {
		if constexpr (TypeTraits::Printable<T>) {
			int index = 1;
			for (const auto& content : container) {
				std::cout << index++ << ' ' << content << std::endl;
			}
			return;
		}
		std::cout << "This type is not printable." << std::endl;
	}
}