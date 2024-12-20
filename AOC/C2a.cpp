#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "2", "./Resources/C2a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C2a_2.txt"}
};

using ParsedInput_t = std::vector<std::list<uint64_t>>;

static ParsedInput_t ParseInput(std::string_view svInput)
{
	ParsedInput_t vecReturned{};

	std::list<uint64_t> listCurrent{};
	std::string sValue{};

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		if (*itr == '\n')
		{
			if (sValue.size() > 0)
				listCurrent.emplace_back(std::atoll(sValue.c_str()));

			if (listCurrent.size() > 0)
				vecReturned.emplace_back(listCurrent);
			
			listCurrent.clear();
			sValue.clear();

			continue;
		}


		if (*itr == ' ')
		{
			if (sValue.size() > 0)
			{
				listCurrent.emplace_back(std::atoll(sValue.c_str()));
				sValue.clear();
			}

			continue;
		}


		sValue += *itr;
	}

	if (sValue.size() > 0)
		listCurrent.emplace_back(std::atoll(sValue.c_str()));

	if (listCurrent.size() > 0)
		vecReturned.emplace_back(listCurrent);

	return vecReturned;
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	uint64_t iTotal{};
	for (const std::list<uint64_t>& listData : vecInput)
	{
		auto itr = listData.begin();

		if (listData.size() < 1)
			++iTotal;

		uint64_t iValue1 = *itr;
		uint64_t iValue2 = *(++itr);
		++itr;



		if (iValue1 == iValue2 || std::max(iValue1, iValue2) - std::min(iValue1, iValue2) > 3)
			continue;

		bool bIncreasing = iValue1 < iValue2;
		bool bPassed = true;
		for (; itr != listData.end(); ++itr)
		{
			iValue1 = iValue2;
			iValue2 = *itr;
		
			if (iValue1 == iValue2 || std::max(iValue1, iValue2) - std::min(iValue1, iValue2) > 3
				|| (bIncreasing && iValue1 > iValue2) || (!bIncreasing && iValue1 < iValue2))
			{
				bPassed = false;
				break;
			}
		}

		if (bPassed)
			++iTotal;
	}

	return iTotal;
}

void TestChallenge2a()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
