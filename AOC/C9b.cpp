#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "2858", "./Resources/C9a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C9a_2.txt"}
};

using ParsedInput_t = std::vector<uint64_t>;

static constexpr auto ID_INVALID = UINT64_MAX;

static ParsedInput_t ParseInput(std::string_view svInput)
{
	ParsedInput_t vecReturned{};

	uint64_t iCurrentId = 0;
	bool bIsFile = true;

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		if (*itr == '\n')
			continue;

		switch (*itr)
		{
		case('0'):
			for (uint8_t i = 0; i < 0; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('1'):
			for (uint8_t i = 0; i < 1; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('2'):
			for (uint8_t i = 0; i < 2; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('3'):
			for (uint8_t i = 0; i < 3; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('4'):
			for (uint8_t i = 0; i < 4; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('5'):
			for (uint8_t i = 0; i < 5; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('6'):
			for (uint8_t i = 0; i < 6; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('7'):
			for (uint8_t i = 0; i < 7; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('8'):
			for (uint8_t i = 0; i < 8; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		case('9'):
			for (uint8_t i = 0; i < 9; ++i)
				vecReturned.emplace_back(bIsFile ? iCurrentId : ID_INVALID);
			break;
		}

		bIsFile = !bIsFile;
		if (!bIsFile)
			++iCurrentId;
	}

	return vecReturned;
}

static size_t GetRegionSize(ParsedInput_t& vecInput, ParsedInput_t::iterator itr, bool bReversed = false)
{
	size_t iSize = 0;
	uint64_t iId = *itr;
	auto itr2 = itr;

	if (bReversed)
	{
		while (itr2 != vecInput.begin())
		{
			if (*itr2 != iId)
				break;

			--itr2;
			++iSize;
		}

		if (itr2 == vecInput.begin() && *itr2 == iId)
			++iSize;
	}
	else
	{
		while (itr2 != vecInput.end())
		{
			if (*itr2 != iId)
				break;
			++itr2;
			++iSize;
		}
	}

	return iSize;
}

static ParsedInput_t::iterator GetNextFreeRegion(ParsedInput_t& vecInput, ParsedInput_t::iterator itr)
{
	auto itrReturned = itr;
	if (itr == vecInput.end())
		itrReturned = vecInput.begin();
	else
		while (*itrReturned == ID_INVALID && itrReturned != vecInput.end())
			++itrReturned;

	while (*itrReturned != ID_INVALID && itrReturned != vecInput.end())
		++itrReturned;
	
	return itrReturned;
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	auto itr2 = vecInput.end();
	--itr2;

	while (itr2 != vecInput.begin())
	{
		while (*itr2 == ID_INVALID)
			--itr2;

		if (itr2 == vecInput.begin())
			break;

		auto itr1 = GetNextFreeRegion(vecInput, vecInput.end());

		size_t iFileSize = GetRegionSize(vecInput, itr2, true);
		size_t iRegionSize = GetRegionSize(vecInput, itr1);
		while (itr1 < itr2 && iRegionSize < iFileSize)
		{
			itr1 = GetNextFreeRegion(vecInput, itr1);
			iRegionSize = GetRegionSize(vecInput, itr1);
		}

		if (itr1 >= itr2 || iRegionSize < iFileSize)
		{
			uint64_t iId = *itr2;
			while (*itr2 == iId && itr2 != vecInput.begin())
				--itr2;
			continue;
			
		}
		
		for (size_t i = 0; i < iFileSize; ++i)
		{
			*itr1 = *itr2;
			*itr2 = ID_INVALID;
			++itr1;
			--itr2;
		}
	}


	uint64_t iTotal = 0;
	for (uint64_t i = 0; i < vecInput.size(); ++i)
	{
		if (vecInput[i] == ID_INVALID)
			continue;

		iTotal += i * vecInput[i];
	}

	return iTotal;
}

void TestChallenge9b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
