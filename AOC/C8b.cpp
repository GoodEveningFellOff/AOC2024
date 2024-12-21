#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "34", "./Resources/C8a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C8a_2.txt"}
};

using ParsedInput_t = std::vector<std::vector<char>>;

static ParsedInput_t ParseInput(std::string_view svInput)
{
	ParsedInput_t vecReturned{};
	std::vector<char> vecLine{};

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		if (*itr == '\n')
		{
			if (vecLine.size() > 0)
				vecReturned.emplace_back(vecLine);

			vecLine.clear();

			continue;
		}

		vecLine.emplace_back(*itr);
	}

	if (vecLine.size() > 0)
		vecReturned.emplace_back(vecLine);

	return vecReturned;
}

static char GetChar(const ParsedInput_t& vecInput, int64_t iLine, int64_t iColumn)
{
	if (iLine < 0 || iLine >= vecInput.size())
		return '\0';

	const auto& vecLine = vecInput[iLine];
	if (iColumn < 0 || iColumn >= vecLine.size())
		return '\0';

	return vecLine[iColumn];
}

static void SetAntinodeForTwoNodes(ParsedInput_t& vecAntinodes, int64_t iLine1, int64_t iColumn1, int64_t iLine2, int64_t iColumn2)
{
	int64_t iLineOffset = iLine2 - iLine1, iColumnOffset = iColumn2 - iColumn1;

	char c{};
	int64_t iLine = iLine1, iColumn = iColumn1;
	while (1)
	{
		c = GetChar(vecAntinodes, iLine, iColumn);
		if (c == '\0')
			break;

		if (c == '.')
			vecAntinodes[iLine][iColumn] = '#';

		iLine -= iLineOffset;
		iColumn -= iColumnOffset;
	}

	iLine = iLine2, iColumn = iColumn2;
	while (1)
	{
		c = GetChar(vecAntinodes, iLine, iColumn);
		if (c == '\0')
			break;

		if (c == '.')
			vecAntinodes[iLine][iColumn] = '#';

		iLine += iLineOffset;
		iColumn += iColumnOffset;
	}
}

static void FillAntinodes(const ParsedInput_t& vecInput, ParsedInput_t& vecAntinodes, char c)
{
	std::vector<std::vector<char>> vecCleanedInput{};
	vecCleanedInput.resize(vecInput.size());

	uint64_t iCount = 0;
	for (size_t i = 0; i < vecInput.size(); ++i)
	{
		vecCleanedInput[i].resize(vecInput[i].size());

		for (size_t j = 0; j < vecInput[i].size(); ++j)
		{
			if (vecInput[i][j] != c)
			{
				vecCleanedInput[i][j] = '.';
				continue;
			}

			++iCount;
			vecCleanedInput[i][j] = vecInput[i][j];
		}
	}

	if (iCount < 2)
		return;

	for (uint64_t iLine1 = 0; iLine1 < vecCleanedInput.size(); ++iLine1)
	{
		auto& vecLine = vecCleanedInput[iLine1];
		for (uint64_t iColumn1 = 0; iColumn1 < vecLine.size(); ++iColumn1)
		{
			if (vecLine[iColumn1] != c)
				continue;

			uint64_t iColumn2 = iColumn1 + 1;
			while (iColumn2 < vecLine.size())
			{
				if (vecLine[iColumn2] == c)
				{
					SetAntinodeForTwoNodes(vecAntinodes, iLine1, iColumn1, iLine1, iColumn2);
				}

				++iColumn2;
			}

			for (uint64_t iLine2 = iLine1 + 1; iLine2 < vecCleanedInput.size(); ++iLine2)
			{
				auto& vecLine2 = vecCleanedInput[iLine2];
				for (iColumn2 = 0; iColumn2 < vecLine2.size(); ++iColumn2)
				{
					if (vecLine2[iColumn2] != c)
						continue;

					SetAntinodeForTwoNodes(vecAntinodes, iLine1, iColumn1, iLine2, iColumn2);
				}
			}

		}
	}
}
static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	ParsedInput_t vecAntinodes{};
	vecAntinodes.resize(vecInput.size());
	for (auto& vecLine : vecAntinodes)
	{
		vecLine.resize(vecInput[0].size());
		std::for_each(vecLine.begin(), vecLine.end(), [](char& c) { c = '.'; });
	}

	for (const auto& c : std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKNLMOPQRSTUVWXYZ0123456789"))
		FillAntinodes(vecInput, vecAntinodes, c);

	uint64_t iTotal{};
	for (auto& vecLine : vecAntinodes)
	{
		std::for_each(vecLine.begin(), vecLine.end(), [&iTotal](char c) {
			if (c == '#')
				++iTotal;
			});
	}
	return iTotal;
}

void TestChallenge8b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
