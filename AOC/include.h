#pragma once

#include <vector>
#include <span>
#include <array>
#include <string>
#include <string_view>
#include <format>
#include <iostream>
#include <cstdint>
#include <functional>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <set>
#include <filesystem>
#include <fstream>
#include <variant>

inline std::string LoadFileContents(const std::string_view& svPath)
{
	std::ifstream file;
	file.open(svPath);

	if (!file.is_open() || !file.good())
		throw std::runtime_error("Unable to open file!");

	std::string sReturned{};
	char c{};
	while (file.get(c))
		sReturned += c;
	
	file.close();
	return sReturned;
}
