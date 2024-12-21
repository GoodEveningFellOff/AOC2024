#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "11387", "./Resources/C7a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C7a_2.txt"}
};

using ParsedInput_t = std::vector<std::pair<uint64_t, std::list<uint64_t>>>;

static ParsedInput_t ParseInput(std::string_view svInput)
{
	ParsedInput_t vecReturned{};

	uint64_t iGoalValue{};
	std::list<uint64_t> listCurrent{};
	std::string sValue{};

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		if (*itr == '\n')
		{
			if (sValue.size() > 0)
				listCurrent.emplace_back(std::atoll(sValue.c_str()));

			if (listCurrent.size() > 0)
				vecReturned.emplace_back(std::make_pair(iGoalValue, listCurrent));

			iGoalValue = 0;
			listCurrent.clear();
			sValue.clear();

			continue;
		}

		if (*itr == ':')
		{
			if (sValue.size() > 0)
				iGoalValue = std::atoll(sValue.c_str());

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
		vecReturned.emplace_back(std::make_pair(iGoalValue, listCurrent));

	return vecReturned;
}

static bool DoesResultMatchNumbers(uint64_t iResult, const std::list<uint64_t>& listNumbers)
{
	uint64_t iOperatorBits = 0;
	std::vector<uint64_t> vecNumbers(listNumbers.begin(), listNumbers.end());

	while (iOperatorBits < (1 << (vecNumbers.size() * 2)))
	{
		uint64_t iTotal = vecNumbers[0];
		for (uint8_t i = 1; i < vecNumbers.size(); ++i)
		{
			uint8_t iOperator = iOperatorBits >> (i * 2);
			if (iOperator & 1)
				iTotal *= vecNumbers[i];
			else if (iOperator & 2)
			{
				//std::cout << iTotal << ',' << vecNumbers[i] << '\n';
				iTotal *= static_cast<uint64_t>(powl(10, 1 + static_cast<uint64_t>(log10l(vecNumbers[i]))));
				iTotal += vecNumbers[i];
				//std::cout << iTotal << '\n';
			}
			else
				iTotal += vecNumbers[i];
		}

		if (iTotal == iResult)
			return true;

		++iOperatorBits;
	}

	return false;
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	uint64_t iTotal{};
	for (const auto& pairData : vecInput)
	{
		if (!DoesResultMatchNumbers(pairData.first, pairData.second))
			continue;

		iTotal += pairData.first;
	}

	return iTotal;
}

void TestChallenge7b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
