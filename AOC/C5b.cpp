#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "123", "./Resources/C5a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C5a_2.txt"}
};

using ParsedInput_t = std::pair<std::vector<std::pair<uint64_t, uint64_t>>, std::vector<std::list<uint64_t>>>;

static ParsedInput_t ParseInput(std::string_view svInput)
{
	std::vector<std::pair<uint64_t, uint64_t>> vecRules{};
	std::vector<std::list<uint64_t>> vecUpdates{};

	bool bInUpdatesSection = false;
	bool bInFirstNumber = true;
	std::list<uint64_t> listCurrent{};
	std::string sValue1{};
	std::string sValue2{};

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		if (*itr == '\n')
		{
			if (bInFirstNumber)
				bInUpdatesSection = true;

			if (!bInUpdatesSection)
			{
				if (sValue1.size() > 0 && sValue2.size() > 0)
					vecRules.emplace_back(std::atoll(sValue1.c_str()), std::atoll(sValue2.c_str()));

				sValue2.clear();

				bInFirstNumber = true;
			}
			else {
				if (sValue1.size() > 0)
					listCurrent.emplace_back(std::atoll(sValue1.c_str()));

				if (listCurrent.size() > 0)
					vecUpdates.emplace_back(listCurrent);

				listCurrent.clear();
			}

			sValue1.clear();
			continue;
		}

		if (!bInUpdatesSection)
		{
			if (*itr == '|')
			{
				bInFirstNumber = false;
				continue;
			}

			if (bInFirstNumber)
				sValue1 += *itr;
			else
				sValue2 += *itr;

			continue;
		}

		if (*itr == ',')
		{
			if (sValue1.size() > 0)
				listCurrent.emplace_back(std::atoll(sValue1.c_str()));

			sValue1.clear();
			continue;
		}

		sValue1 += *itr;
	}

	if (sValue1.size() > 0)
		listCurrent.emplace_back(std::atoll(sValue1.c_str()));

	if (listCurrent.size() > 0)
		vecUpdates.emplace_back(listCurrent);

	return std::make_pair(vecRules, vecUpdates);
}

static bool VerifyUpdateFollowsRules(const std::map<uint64_t, std::set<uint64_t>>& mapRules, const std::list<uint64_t>& listUpdate)
{
	for (auto itr = listUpdate.begin(); itr != listUpdate.end(); ++itr)
	{
		if (!mapRules.contains(*itr))
			continue;

		auto setRule = mapRules.find(*itr)->second;
		for (auto itr2 = itr; itr2 != listUpdate.end(); ++itr2)
		{
			if (setRule.contains(*itr2))
				return false;
		}
	}

	return true;
}

static uint64_t GetMiddleValue(const std::list<uint64_t>& listData)
{
	auto itr = listData.begin();
	auto itr2 = listData.end();
	--itr2;
	while (itr != itr2)
	{
		++itr;
		if (itr == itr2)
			break;

		--itr2;
	}

	return *itr;
}


static uint64_t Solve(std::string_view svPath)
{
	auto pairInput = ParseInput(LoadFileContents(svPath));
	const auto& vecRules = pairInput.first;
	const auto& vecUpdates = pairInput.second;

	std::map<uint64_t, std::set<uint64_t>> mapRules{};
	for (const auto& pairRule : vecRules)
	{
		if (!mapRules.contains(pairRule.second))
			mapRules.emplace(pairRule.second, std::set<uint64_t>{});

		mapRules.find(pairRule.second)->second.emplace(pairRule.first);
	}

	uint64_t iTotal{};
	for (const std::list<uint64_t>& listData : vecUpdates)
	{
		if (listData.size() < 1)
		{
			iTotal += *listData.begin();
			continue;
		}

		if (VerifyUpdateFollowsRules(mapRules, listData))
		{
			//iTotal += GetMiddleValue(listData);
			continue;
		}

		std::list<uint64_t> listCopiedData = listData;
		std::list<uint64_t> listCorrect{};
		while (listCopiedData.size() > 0)
		{
			auto itr = listCopiedData.begin();
			if (listCopiedData.size() == 1)
			{
				listCorrect.emplace_back(*itr);
				listCopiedData.erase(itr);
				continue;
			}

			bool bFound = true;
			for (; itr != listCopiedData.end(); ++itr)
			{
				bFound = true;
				
				if (mapRules.contains(*itr))
				{
					auto setRule = mapRules.find(*itr)->second;
					for (auto itr2 = listCopiedData.begin(); itr2 != listCopiedData.end(); ++itr2)
					{
						if (itr == itr2 || *itr == *itr2)
							continue;

						if (setRule.contains(*itr2))
						{
							bFound = false;
							break;
						}
					}

					if (!bFound)
						continue;
				}

				listCorrect.emplace_back(*itr);
				listCopiedData.erase(itr);
				break;
			}

			if (!bFound)
				break;
		}
		
		if (listCorrect.size() == listData.size() && VerifyUpdateFollowsRules(mapRules, listCorrect))
		{
			iTotal += GetMiddleValue(listCorrect);
		}
	}

	return iTotal;
}

void TestChallenge5b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
