#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "1206", "./Resources/C12a_1.txt"},
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
	uint8_t m_iEdges : 3 = 0;
	bool m_bCounted  : 1 = false;
	
	void Solve(const ParsedInput_t& vecInput, int64_t iLine, int64_t iColumn) {
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

	std::set<std::pair<int64_t, int64_t>> setEdgeTiles{};
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
			if (vecTiles[iLine][iColumn].m_iEdges > 0)
				setEdgeTiles.emplace(std::make_pair(iLine, iColumn));
			break;
		}
		else
		{
			iTiles += 1;
			if (vecTiles[iLine][iColumn].m_iEdges > 0)
				setEdgeTiles.emplace(std::make_pair(iLine, iColumn));

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

	std::queue<std::pair<int64_t, int64_t>> queueDebugTiles{};
	std::pair<int8_t, int8_t> pairMoveDirection{ 1, 0 };
	std::pair<int64_t, int64_t> pairStartPosition{ -1, -1 };

	auto fnFixMovement = [c, &iEdges, &pairMoveDirection, &iLine, &iColumn, &vecTiles, vecInput](){
		while (1)
		{
			if (pairMoveDirection.first == 1 && GetChar(vecInput, iLine, iColumn - 1) == c)
			{
				std::cout << std::format("+({}, {})\n", iLine, iColumn);
				++iEdges;
				pairMoveDirection.first = 0;
				pairMoveDirection.second = -1;
			}
			else if (pairMoveDirection.second == -1 && GetChar(vecInput, iLine + 1, iColumn) == c)
			{
				std::cout << std::format("+({}, {})\n", iLine, iColumn);
				++iEdges;
				pairMoveDirection.first = -1;
				pairMoveDirection.second = 0;
			}
			else if (pairMoveDirection.first == -1 && GetChar(vecInput, iLine, iColumn + 1) == c)
			{
				std::cout << std::format("+({}, {})\n", iLine, iColumn);
				++iEdges;
				pairMoveDirection.first = 0;
				pairMoveDirection.second = 1;
			}
			else if (pairMoveDirection.second == 1 && GetChar(vecInput, iLine - 1, iColumn) == c)
			{
				std::cout << std::format("+({}, {})\n", iLine, iColumn);
				++iEdges;
				pairMoveDirection.first = 1;
				pairMoveDirection.second = 0;
			}
			else if (c != GetChar(vecInput, iLine - pairMoveDirection.first, iColumn + pairMoveDirection.second))
			{
				std::cout << std::format("+({}, {})\n", iLine, iColumn);
				++iEdges;
				if (pairMoveDirection.first == 1)
				{
					pairMoveDirection.first = 0;
					pairMoveDirection.second = 1;
				}
				else if (pairMoveDirection.second == 1)
				{
					pairMoveDirection.first = -1;
					pairMoveDirection.second = 0;
				}
				else if (pairMoveDirection.first == -1)
				{
					pairMoveDirection.first = 0;
					pairMoveDirection.second = -1;
				}
				else
				{
					pairMoveDirection.first = 1;
					pairMoveDirection.second = 0;
				}

				continue;
			}

			break;
		}
	};
	
	while (1)
	{
		queueDebugTiles.emplace(std::make_pair(iLine, iColumn));
		if (pairStartPosition.first == -1 && pairStartPosition.second == -1)
		{
			if (setEdgeTiles.size() == 0)
				break;

			pairMoveDirection.first = 1;
			pairMoveDirection.second = 0;
			pairStartPosition.first = iLine = setEdgeTiles.begin()->first;
			pairStartPosition.second = iColumn = setEdgeTiles.begin()->second;
			setEdgeTiles.erase(std::make_pair(iLine, iColumn));

			if (vecTiles[iLine][iColumn].m_iEdges == 4)
			{
				iEdges += 4;
				pairStartPosition.first = pairStartPosition.second = -1;
				continue;
			}

			fnFixMovement();
			iLine -= pairMoveDirection.first;
			iColumn += pairMoveDirection.second;
		}

		if (setEdgeTiles.contains(std::make_pair(iLine, iColumn)))
			setEdgeTiles.erase(std::make_pair(iLine, iColumn));

		if (iLine == pairStartPosition.first && iColumn == pairStartPosition.second)
		{
			if (pairMoveDirection.first == 1 || pairMoveDirection.second == -1)
				++iEdges;
			pairStartPosition.first = -1;
			pairStartPosition.second = -1;
			continue;
		}

		fnFixMovement();
		
		iLine -= pairMoveDirection.first;
		iColumn += pairMoveDirection.second;
	}

	std::cout << c << '\n';
	while (queueDebugTiles.size() > 0)
	{
		std::cout << std::format("({}, {})\n", queueDebugTiles.front().first, queueDebugTiles.front().second);
		queueDebugTiles.pop();
	}
	std::cout << std::format("[{}] {} * {} = {}\n", c, iTiles, iEdges, iTiles * iEdges);
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

void TestChallenge12b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
