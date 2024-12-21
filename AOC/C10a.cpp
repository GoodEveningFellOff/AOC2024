#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "36", "./Resources/C10a_1.txt"},
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

struct Tile_t {
	bool m_bChecked   : 1 = false;
	bool m_bCanMoveNorth : 1;
	bool m_bCanMoveEast  : 1;
	bool m_bCanMoveSouth : 1;
	bool m_bCanMoveWest  : 1;

	void UpdateTileInformation(const ParsedInput_t& vecInput, int64_t iLine, int64_t iColumn)
	{
		auto iCurrentHeight = GetHeight(vecInput, iLine, iColumn);
		auto iNextHeight = GetHeight(vecInput, iLine - 1, iColumn);
		m_bCanMoveNorth = iNextHeight != -1 && iCurrentHeight < iNextHeight && iNextHeight - iCurrentHeight == 1;

		iNextHeight = GetHeight(vecInput, iLine, iColumn + 1);
		m_bCanMoveEast = iNextHeight != -1 && iCurrentHeight < iNextHeight && iNextHeight - iCurrentHeight == 1;

		iNextHeight = GetHeight(vecInput, iLine + 1, iColumn);
		m_bCanMoveSouth = iNextHeight != -1 && iCurrentHeight < iNextHeight && iNextHeight - iCurrentHeight == 1;

		iNextHeight = GetHeight(vecInput, iLine, iColumn - 1);
		m_bCanMoveWest = iNextHeight != -1 && iCurrentHeight < iNextHeight && iNextHeight - iCurrentHeight == 1;
	}
};

enum class EDirections : uint8_t
{
	North,
	East,
	South,
	West
};

static uint64_t GetTrailheadScore(const ParsedInput_t& vecInput, std::vector<std::vector<Tile_t>> vecTiles, int64_t iLine, int64_t iColumn)
{
	int64_t iCurrentLine = iLine, iCurrentColumn = iColumn;
	auto vecTilesCopy = vecTiles;
	vecTilesCopy[iLine][iColumn].m_bChecked = true;
	
	std::stack<EDirections> stackMovements{};
	uint64_t iScore{};
	while (1)
	{
		auto tileCurrent = vecTilesCopy[iLine][iColumn];
		vecTilesCopy[iLine][iColumn].m_bChecked = true;
		if (tileCurrent.m_bCanMoveNorth && !vecTilesCopy[iLine - 1][iColumn].m_bChecked)
		{
			stackMovements.push(EDirections::North);
			--iLine;
		}
		else if (tileCurrent.m_bCanMoveEast && !vecTilesCopy[iLine][iColumn + 1].m_bChecked)
		{
			stackMovements.push(EDirections::East);
			++iColumn;
		}
		else if (tileCurrent.m_bCanMoveSouth && !vecTilesCopy[iLine + 1][iColumn].m_bChecked)
		{
			stackMovements.push(EDirections::South);
			++iLine;
		}
		else if (tileCurrent.m_bCanMoveWest && !vecTilesCopy[iLine][iColumn - 1].m_bChecked)
		{
			stackMovements.push(EDirections::West);
			--iColumn;
		}
		else if (stackMovements.size() == 0)
		{
			break;
		}
		else
		{
			if (GetHeight(vecInput, iLine, iColumn) == 9)
				++iScore;

			switch (stackMovements.top())
			{
			case(EDirections::North):
				++iLine;
				break;
			case(EDirections::East):
				--iColumn;
				break;
			case(EDirections::South):
				--iLine;
				break;
			case(EDirections::West):
				++iColumn;
				break;
			}

			stackMovements.pop();
		}
	}

	return iScore;
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	std::vector<std::vector<Tile_t>> vecTiles{};
	vecTiles.resize(vecInput.size());
	for (size_t i = 0; i < vecInput.size(); ++i)
	{
		vecTiles[i].resize(vecInput[i].size());
		for (size_t j = 0; j < vecInput[i].size(); ++j)
			vecTiles[i][j].UpdateTileInformation(vecInput, i, j);
	}

	uint64_t iTotal{};
	for (int64_t iLine = 0; iLine < vecInput.size(); ++iLine)
	{
		const auto& vecLine = vecInput[iLine];
		for (int64_t iColumn = 0; iColumn < vecLine.size(); ++iColumn)
		{
			if (vecLine[iColumn] == 0)
				iTotal += GetTrailheadScore(vecInput, vecTiles, iLine, iColumn);
		}
	}

	return iTotal;
}

void TestChallenge10a()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
