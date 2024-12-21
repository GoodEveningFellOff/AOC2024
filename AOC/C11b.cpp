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

	for (size_t i = 0; i < 75; ++i)
	{
		for (const auto& iValue : vecInput)
		{
			uint64_t iLog = static_cast<uint64_t>(log10l(iValue));
			if (iValue == 0)
				vecSwap.emplace_back(1);
			else if (iLog % 2 == 1)
			{
				uint64_t iMul = pow(10, iLog / 2 + 1);
				uint64_t iUpper = iValue / iMul;
				vecSwap.emplace_back(iUpper);
				vecSwap.emplace_back(iValue - (iUpper * iMul));
			}
			else
				vecSwap.emplace_back(iValue * 2024);
		}

		std::cout << i << '\n';

		vecInput = std::move(vecSwap);
		vecSwap.clear();
	}

	return vecInput.size();
}

void TestChallenge11b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
