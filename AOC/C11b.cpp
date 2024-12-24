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

static void SolveN(uint64_t n, std::vector<uint64_t>& vecReturned, std::vector<uint64_t>& vecSwap)
{
	vecReturned.clear();
	vecSwap.clear();
	vecReturned.emplace_back(n);

	for (size_t i = 0; i < 25; ++i)
	{
		for (auto& iValue : vecReturned)
		{
			uint64_t iLog = static_cast<uint64_t>(log10l(iValue));
			if (iValue == 0)
				iValue = 1;
			else if (iLog % 2 == 1)
			{
				uint64_t iMul = powl(10, iLog / 2 + 1);
				uint64_t iUpper = iValue / iMul;
				vecSwap.emplace_back(iValue - (iUpper * iMul));
				iValue = iUpper;
			}
			else
				iValue *= 2024;
		}

		vecReturned.insert(vecReturned.end(), vecSwap.begin(), vecSwap.end());
		vecSwap.clear();
	}
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));
	std::map<uint64_t, std::vector<uint64_t>> mapCache{};

	std::vector<uint64_t> vecLayer1{}, vecLayer2{}, vecSolveBuffer{}, vecSwapBuffer{};

	uint64_t iTotal{};
	for (const auto& iValue1 : vecInput)
	{
		if (!mapCache.contains(iValue1))
		{
			SolveN(iValue1, vecSolveBuffer, vecSwapBuffer);
			mapCache.emplace(iValue1, vecSolveBuffer);
			std::cout << std::format("[1] Inserted {} into map of size {}\n", iValue1, mapCache.size());
		}
		
		vecLayer1 = mapCache.find(iValue1)->second;
		for (const auto& iValue2 : vecLayer1)
		{
			if (!mapCache.contains(iValue2))
			{
				SolveN(iValue2, vecSolveBuffer, vecSwapBuffer);
				mapCache.emplace(iValue2, vecSolveBuffer);
				std::cout << std::format("[2] Inserted {} into map of size {}\n", iValue2, mapCache.size());
			}

			vecLayer2 = mapCache.find(iValue2)->second;
			for (const auto& iValue3 : vecLayer2)
			{
				if (!mapCache.contains(iValue3)) {
					SolveN(iValue3, vecSolveBuffer, vecSwapBuffer);
					mapCache.emplace(iValue3, vecSolveBuffer);
					std::cout << std::format("[3] Inserted {} into map of size {}\n", iValue3, mapCache.size());
				}

				iTotal += mapCache.find(iValue3)->second.size();
			}
		}
	}

	return iTotal;
}

void TestChallenge11b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
