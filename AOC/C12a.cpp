#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "1930", "./Resources/C12a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C12a_2.txt"}
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

struct Tile
{
	int8_t m_iEdges = -1;

	bool m_bCounted = false;

	void Solve(const ParsedInput_t& vecInput, int64_t iLine, int64_t iColumn) {
		m_iEdges = 0;

		char c = GetChar(vecInput, iLine, iColumn);
		if (c != GetChar(vecInput, iLine - 1, iColumn))
			++m_iEdges;

		if (c != GetChar(vecInput, iLine + 1, iColumn))
			++m_iEdges;

		if (c != GetChar(vecInput, iLine, iColumn - 1))
			++m_iEdges;

		if (c != GetChar(vecInput, iLine, iColumn + 1))
			++m_iEdges;
	};
};

static uint64_t GetScoreForGroup(const ParsedInput_t& vecInput, std::vector<std::vector<Tile>>& vecTiles, int64_t iLine, int64_t iColumn)
{
	char c = GetChar(vecInput, iLine, iColumn);
	if (c == '\0')
		return 0;

	std::stack<uint8_t> stackMovements{};
	uint64_t iTiles{}, iEdges{};

	while (1)
	{
		vecTiles[iLine][iColumn].m_bCounted = true;
		if (c == GetChar(vecInput, iLine - 1, iColumn) && !vecTiles[iLine - 1][iColumn].m_bCounted)
		{
			--iLine;
			stackMovements.push(0);
		}
		else if (c == GetChar(vecInput, iLine + 1, iColumn) && !vecTiles[iLine + 1][iColumn].m_bCounted)
		{
			++iLine;
			stackMovements.push(1);
		}
		else if (c == GetChar(vecInput, iLine, iColumn - 1) && !vecTiles[iLine][iColumn - 1].m_bCounted)
		{
			--iColumn;
			stackMovements.push(2);
		}
		else if (c == GetChar(vecInput, iLine, iColumn + 1) && !vecTiles[iLine][iColumn + 1].m_bCounted)
		{
			++iColumn;
			stackMovements.push(3);
		}
		else if (stackMovements.size() == 0)
		{
			iTiles += 1;
			iEdges += vecTiles[iLine][iColumn].m_iEdges;
			break;
		}
		else
		{
			iTiles += 1;
			iEdges += vecTiles[iLine][iColumn].m_iEdges;

			switch (stackMovements.top())
			{
			case(0):
				++iLine;
				break;
			case(1):
				--iLine;
				break;
			case(2):
				++iColumn;
				break;
			default:
				--iColumn;
				break;
			}

			stackMovements.pop();
		}
	}

	return iTiles * iEdges;
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));
	std::vector<std::vector<Tile>> vecTiles{};
	vecTiles.resize(vecInput.size());

	for (size_t i = 0; i < vecInput.size(); ++i)
	{
		vecTiles[i].resize(vecInput[i].size());

		for (size_t j = 0; j < vecInput[i].size(); ++j)
		{
			vecTiles[i][j].Solve(vecInput, i, j);
		}
	}

	uint64_t iTotal{};
	for (size_t i = 0; i < vecInput.size(); ++i)
	{
		for (size_t j = 0; j < vecInput[i].size(); ++j)
		{
			if (!vecTiles[i][j].m_bCounted)
				iTotal += GetScoreForGroup(vecInput, vecTiles, i, j);
		}
	}

	return iTotal;
}

void TestChallenge12a()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
