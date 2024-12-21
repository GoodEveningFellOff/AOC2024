#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "81", "./Resources/C10a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C10a_2.txt"}
};

using ParsedInput_t = std::vector<std::vector<int8_t>>;

static ParsedInput_t ParseInput(std::string_view svInput)
{
	ParsedInput_t vecReturned{};
	std::vector<int8_t> vecLine{};

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		if (*itr == '\n')
		{
			if (vecLine.size() > 0)
				vecReturned.emplace_back(vecLine);

			vecLine.clear();

			continue;
		}

		switch (*itr)
		{
		case('0'):
			vecLine.emplace_back(0);
			break;
		case('1'):
			vecLine.emplace_back(1);
			break;
		case('2'):
			vecLine.emplace_back(2);
			break;
		case('3'):
			vecLine.emplace_back(3);
			break;
		case('4'):
			vecLine.emplace_back(4);
			break;
		case('5'):
			vecLine.emplace_back(5);
			break;
		case('6'):
			vecLine.emplace_back(6);
			break;
		case('7'):
			vecLine.emplace_back(7);
			break;
		case('8'):
			vecLine.emplace_back(8);
			break;
		case('9'):
			vecLine.emplace_back(9);
			break;
		default:
			vecLine.emplace_back(-1);
			break;
		}
	}

	if (vecLine.size() > 0)
		vecReturned.emplace_back(vecLine);

	return vecReturned;
}

static int8_t GetHeight(const ParsedInput_t& vecInput, int64_t iLine, int64_t iColumn)
{
	if (iLine < 0 || iLine >= vecInput.size())
		return -1;

	const auto& vecLine = vecInput[iLine];
	if (iColumn < 0 || iColumn >= vecLine.size())
		return -1;

	return vecLine[iColumn];
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	std::vector<std::vector<uint8_t>> vecTiles{};
	vecTiles.resize(vecInput.size());
	for (size_t i = 0; i < vecInput.size(); ++i)
		for (size_t j = 0; j < vecInput[i].size(); ++j)
			vecTiles[i].emplace_back(0);

	for (int64_t iLine = 0; iLine < vecInput.size(); ++iLine)
	{
		const auto& vecLine = vecInput[iLine];
		for (int64_t iColumn = 0; iColumn < vecLine.size(); ++iColumn)
		{
			if (vecLine[iColumn] != 8)
				continue;

			auto& iCurrent = vecTiles[iLine][iColumn];
			if (GetHeight(vecInput, iLine - 1, iColumn) == 9)
				iCurrent += 1;

			if (GetHeight(vecInput, iLine, iColumn - 1) == 9)
				iCurrent += 1;

			if (GetHeight(vecInput, iLine + 1, iColumn) == 9)
				iCurrent += 1;

			if (GetHeight(vecInput, iLine, iColumn + 1) == 9)
				iCurrent += 1;

		}
	}

	for (int8_t i = 7; i >= 0; --i)
	{
		for (int64_t iLine = 0; iLine < vecInput.size(); ++iLine)
		{
			const auto& vecLine = vecInput[iLine];
			for (int64_t iColumn = 0; iColumn < vecLine.size(); ++iColumn)
			{
				if (vecLine[iColumn] != i)
					continue;

				auto& iCurrent = vecTiles[iLine][iColumn];
				if (GetHeight(vecInput, iLine - 1, iColumn) == i + 1)
					iCurrent += vecTiles[iLine - 1][iColumn];

				if (GetHeight(vecInput, iLine, iColumn - 1) == i + 1)
					iCurrent += vecTiles[iLine][iColumn - 1];

				if (GetHeight(vecInput, iLine + 1, iColumn) == i + 1)
					iCurrent += vecTiles[iLine + 1][iColumn];

				if (GetHeight(vecInput, iLine, iColumn + 1) == i + 1)
					iCurrent += vecTiles[iLine][iColumn + 1];

			}
		}
	}

	uint64_t iTotal{};
	for (int64_t iLine = 0; iLine < vecInput.size(); ++iLine)
	{
		const auto& vecLine = vecInput[iLine];
		for (int64_t iColumn = 0; iColumn < vecLine.size(); ++iColumn)
		{
			if (vecLine[iColumn] == 0)
				iTotal += vecTiles[iLine][iColumn];
		}
	}

	return iTotal;
}

void TestChallenge10b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
