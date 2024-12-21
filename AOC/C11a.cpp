#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "55312", "./Resources/C11a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C11a_2.txt"}
};

using ParsedInput_t = std::vector<uint64_t>;

static ParsedInput_t ParseInput(std::string_view svInput)
{
	ParsedInput_t vecReturned{};

	std::string sValue{};

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		if (*itr == ' ')
		{
			if (sValue.size() > 0)
				vecReturned.emplace_back(std::atoll(sValue.c_str()));

			sValue.clear();
			continue;
		}

		sValue += *itr;
	}

	if (sValue.size() > 0)
		vecReturned.emplace_back(std::atoll(sValue.c_str()));

	return vecReturned;
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));
	ParsedInput_t vecSwap{};

	for (size_t i = 0; i < 25; ++i)
	{
		for (const auto& iValue : vecInput)
		{
			std::string sValue = std::to_string(iValue);
			if (iValue == 0)
				vecSwap.emplace_back(1);
			else if (sValue.size() % 2 == 0)
			{
				vecSwap.emplace_back(std::atoll(sValue.substr(0, sValue.size() / 2).c_str()));
				vecSwap.emplace_back(std::atoll(sValue.substr(sValue.size() / 2, sValue.size() / 2).c_str()));
			}
			else
				vecSwap.emplace_back(iValue * 2024);
		}

		vecInput = vecSwap;
		vecSwap.clear();
	}

	return vecInput.size();
}

void TestChallenge11a()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
