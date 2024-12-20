#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "18", "./Resources/C4a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C4a_2.txt"}
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

enum class EDirection {
	NorthWest,
	North,
	NorthEast,
	East,
	SouthEast,
	South,
	SouthWest,
	West
};


static char GetChar(const ParsedInput_t& vecInput, int64_t iLine, int64_t iColumn)
{
	if (iLine < 0 || iLine >= vecInput.size())
		return '\0';

	const auto& vecLine = vecInput[iLine];
	if (iColumn < 0 || iColumn >= vecLine.size())
		return '\0';

	return vecLine[iColumn];
}

static bool IsValidEntryDirection(const ParsedInput_t& vecInput, int64_t iLine, int64_t iColumn, EDirection eDirection)
{
	switch (eDirection)
	{
	case(EDirection::NorthWest):
		return GetChar(vecInput, iLine - 1, iColumn - 1) == 'M' && GetChar(vecInput, iLine - 2, iColumn - 2) == 'A' && GetChar(vecInput, iLine - 3, iColumn - 3) == 'S';
	case(EDirection::North):
		return GetChar(vecInput, iLine - 1, iColumn) == 'M' && GetChar(vecInput, iLine - 2, iColumn) == 'A' && GetChar(vecInput, iLine - 3, iColumn) == 'S';
	case(EDirection::NorthEast):
		return GetChar(vecInput, iLine - 1, iColumn + 1) == 'M' && GetChar(vecInput, iLine - 2, iColumn + 2) == 'A' && GetChar(vecInput, iLine - 3, iColumn + 3) == 'S';
	case(EDirection::East):
		return GetChar(vecInput, iLine, iColumn + 1) == 'M' && GetChar(vecInput, iLine, iColumn + 2) == 'A' && GetChar(vecInput, iLine, iColumn + 3) == 'S';
	case(EDirection::SouthEast):
		return GetChar(vecInput, iLine + 1, iColumn + 1) == 'M' && GetChar(vecInput, iLine + 2, iColumn + 2) == 'A' && GetChar(vecInput, iLine + 3, iColumn + 3) == 'S';
	case(EDirection::South):
		return GetChar(vecInput, iLine + 1, iColumn) == 'M' && GetChar(vecInput, iLine + 2, iColumn) == 'A' && GetChar(vecInput, iLine + 3, iColumn) == 'S';
	case(EDirection::SouthWest):
		return GetChar(vecInput, iLine + 1, iColumn - 1) == 'M' && GetChar(vecInput, iLine + 2, iColumn - 2) == 'A' && GetChar(vecInput, iLine + 3, iColumn - 3) == 'S';
	case(EDirection::West):
		return GetChar(vecInput, iLine, iColumn - 1) == 'M' && GetChar(vecInput, iLine, iColumn - 2) == 'A' && GetChar(vecInput, iLine, iColumn - 3) == 'S';
	}
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	uint64_t iTotal{};
	for (int64_t iLine = 0; iLine < vecInput.size(); ++iLine)
	{
		const auto& vecLine = vecInput[iLine];
		for (int64_t iColumn = 0; iColumn < vecLine.size(); ++iColumn)
		{
			if (vecLine[iColumn] != 'X')
				continue;

			for (EDirection eDirection = EDirection::NorthWest; eDirection <= EDirection::West; eDirection = static_cast<EDirection>(1 + static_cast<int64_t>(eDirection)))
			{
				if (IsValidEntryDirection(vecInput, iLine, iColumn, eDirection))
					++iTotal;
			}
		}
	}

	return iTotal;
}

void TestChallenge4a()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
