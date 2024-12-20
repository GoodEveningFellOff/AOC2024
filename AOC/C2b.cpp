#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "4", "./Resources/C2a_1.txt"},
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

static bool DoesListPass(const std::list<uint64_t>& listData)
{
	auto itr = listData.begin();
	if (listData.size() < 1)
		return true;

	uint64_t iValue1 = *itr;
	uint64_t iValue2 = *(++itr);
	++itr;

	if (iValue1 == iValue2 || std::max(iValue1, iValue2) - std::min(iValue1, iValue2) > 3)
		return false;

	bool bIncreasing = iValue1 < iValue2;
	for (; itr != listData.end(); ++itr)
	{
		iValue1 = iValue2;
		iValue2 = *itr;

		if (iValue1 == iValue2 || std::max(iValue1, iValue2) - std::min(iValue1, iValue2) > 3
			|| (bIncreasing && iValue1 > iValue2) || (!bIncreasing && iValue1 < iValue2))
		{
			return false;
		}
	}

	return true;
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	uint64_t iTotal{};
	for (const std::list<uint64_t>& listData : vecInput)
	{
		if (DoesListPass(listData))
		{
			++iTotal;
			continue;
		}
	
		std::list<uint64_t> listAlternate{};
		for (size_t i = 0; i < listData.size(); ++i)
		{
			listAlternate.clear();
			auto itr = listData.begin();
			for (size_t j = 0; j < listData.size(); ++j)
			{
				if (i != j)
					listAlternate.emplace_back(*itr);
				
				++itr;
			}

			if (DoesListPass(listAlternate))
			{
				++iTotal;
				break;
			}
		}
	}

	return iTotal;
}

void TestChallenge2b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
