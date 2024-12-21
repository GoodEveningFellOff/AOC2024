#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "6", "./Resources/C6a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C6a_2.txt"}
};

enum class ETileType : uint8_t {
	Empty,
	Wall,
	Player,
	Outside
};

enum class EPlayerDirection : uint8_t {
	North,
	East,
	South,
	West
};

using ParsedInput_t = std::pair<EPlayerDirection, std::vector<std::vector<ETileType>>>;

static ParsedInput_t ParseInput(std::string_view svInput)
{
	EPlayerDirection eDirection{};

	std::vector<std::vector<ETileType>> vecTiles{};
	std::vector<ETileType> vecLine{};

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		if (*itr == '\n')
		{
			if (vecLine.size() > 0)
				vecTiles.emplace_back(vecLine);

			vecLine.clear();

			continue;
		}

		switch (*itr)
		{
		case('.'):
			vecLine.emplace_back(ETileType::Empty);
			break;
		case('#'):
			vecLine.emplace_back(ETileType::Wall);
			break;
		case('^'):
			eDirection = EPlayerDirection::North;
			vecLine.emplace_back(ETileType::Player);
			break;
		case('>'):
			eDirection = EPlayerDirection::East;
			vecLine.emplace_back(ETileType::Player);
			break;
		case('<'):
			eDirection = EPlayerDirection::West;
			vecLine.emplace_back(ETileType::Player);
			break;
		default:
			eDirection = EPlayerDirection::South;
			vecLine.emplace_back(ETileType::Player);
			break;
		}
	}

	if (vecLine.size() > 0)
		vecTiles.emplace_back(vecLine);

	return std::make_pair(eDirection, vecTiles);
}

static ETileType GetTileType(const std::vector<std::vector<ETileType>>& vecTiles, int64_t iLine, int64_t iColumn)
{
	if (iLine < 0 || iLine >= vecTiles.size())
		return ETileType::Outside;

	const auto& vecLine = vecTiles[iLine];
	if (iColumn < 0 || iColumn >= vecLine.size())
		return ETileType::Outside;

	return vecLine[iColumn];
}

static bool DoesSolutionLoop(const std::vector<std::vector<ETileType>>& vecTiles, int64_t iLine, int64_t iColumn, EPlayerDirection eDirection)
{
	std::set<std::pair<int64_t, int64_t>> setTurnsNorth{}, setTurnsEast{}, setTurnsSouth{}, setTurnsWest{};

	bool bDone = false;
	while (1)
	{
		switch (eDirection)
		{
		case(EPlayerDirection::North):
			switch (GetTileType(vecTiles, iLine - 1, iColumn)) {
			case(ETileType::Empty):
				--iLine;
				break;
			case(ETileType::Wall):
				eDirection = EPlayerDirection::East;
				if (!setTurnsNorth.contains(std::pair(iLine, iColumn)))
					setTurnsNorth.emplace(std::pair(iLine, iColumn));
				else
					return true;
				break;
			default:
				return false;
			}
			break;
		case(EPlayerDirection::East):
			switch (GetTileType(vecTiles, iLine, iColumn + 1)) {
			case(ETileType::Empty):
				++iColumn;
				break;
			case(ETileType::Wall):
				eDirection = EPlayerDirection::South;
				if (!setTurnsEast.contains(std::pair(iLine, iColumn)))
					setTurnsEast.emplace(std::pair(iLine, iColumn));
				else
					return true;
				break;
			default:
				return false;
			}
			break;
		case(EPlayerDirection::South):
			switch (GetTileType(vecTiles, iLine + 1, iColumn)) {
			case(ETileType::Empty):
				++iLine;
				break;
			case(ETileType::Wall):
				eDirection = EPlayerDirection::West;
				if (!setTurnsSouth.contains(std::pair(iLine, iColumn)))
					setTurnsSouth.emplace(std::pair(iLine, iColumn));
				else
					return true;
				break;
			default:
				return false;
			}
			break;
		case(EPlayerDirection::West):
			switch (GetTileType(vecTiles, iLine, iColumn - 1)) {
			case(ETileType::Empty):
				--iColumn;
				break;
			case(ETileType::Wall):
				eDirection = EPlayerDirection::North;
				if (!setTurnsWest.contains(std::pair(iLine, iColumn)))
					setTurnsWest.emplace(std::pair(iLine, iColumn));
				else
					return true;
				break;
			default:
				return false;
			}
			break;
		}
	}
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));
	auto& vecTiles = vecInput.second;


	int64_t iLine{}, iColumn{};
	EPlayerDirection eDirection = vecInput.first;
	for (int64_t i = 0; i < vecTiles.size(); ++i)
	{
		auto& vecLine = vecTiles[i];
		for (int64_t j = 0; j < vecLine.size(); ++j)
		{
			if (vecLine[j] == ETileType::Player)
			{
				iLine = i, iColumn = j;
				vecLine[j] = ETileType::Empty;
				break;
			}
		}

		if (iLine != 0 || iColumn != 0)
			break;
	}

	uint64_t iTotal{};
	for (int64_t i = 0; i < vecTiles.size(); ++i)
	{
		auto& vecLine = vecTiles[i];
		for (int64_t j = 0; j < vecLine.size(); ++j)
		{
			if (vecLine[j] == ETileType::Wall || (i == iLine && j == iColumn))
				continue;

			vecLine[j] = ETileType::Wall;
			if (DoesSolutionLoop(vecTiles, iLine, iColumn, eDirection))
				++iTotal;

			vecLine[j] = ETileType::Empty;
		}
	}

	return iTotal;
}

void TestChallenge6b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}