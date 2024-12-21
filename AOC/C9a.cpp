#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "1928", "./Resources/C9a_1.txt"},
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

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	auto itr1 = vecInput.begin();
	auto itr2 = vecInput.end();
	--itr2;

	while (itr1 < itr2)
	{
		if (*itr1 != ID_INVALID)
		{
			++itr1;
			continue;
		}

		while (*itr2 == ID_INVALID)
		{
			--itr2;
			continue;
		}

		*itr1 = *itr2;
		*itr2 = ID_INVALID;
		++itr1;
		--itr2;
	}


	uint64_t iTotal = 0;
	for (uint64_t i = 0; i < vecInput.size(); ++i)
	{
		if (vecInput[i] == ID_INVALID)
			break;

		iTotal += i * vecInput[i];
	}

	return iTotal;
}

void TestChallenge9a()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
