#pragma once

// This file is part of 'Atoms' library - https://github.com/yaqwsx/atoms
// Author: Jan 'yaqwsx' Mrázek

// Atmel Studio and the others doesn't ship STL library, this is a small
// and humble replacement

namespace std {
	template<bool B, class T = void>
	struct enable_if {};

	template<class T>
	struct enable_if<true, T> { typedef T type; };
}