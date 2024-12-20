#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "9", "./Resources/C4a_1.txt"},
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


static char GetChar(const ParsedInput_t& vecInput, int64_t iLine, int64_t iColumn)
{
	if (iLine < 0 || iLine >= vecInput.size())
		return '\0';

	const auto& vecLine = vecInput[iLine];
	if (iColumn < 0 || iColumn >= vecLine.size())
		return '\0';

	return vecLine[iColumn];
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
			if (vecLine[iColumn] != 'A')
				continue;

			std::array chars = {
				GetChar(vecInput, iLine - 1, iColumn - 1),
				GetChar(vecInput, iLine - 1, iColumn + 1),
				GetChar(vecInput, iLine + 1, iColumn + 1),
				GetChar(vecInput, iLine + 1, iColumn - 1)
			};

			uint8_t iMs = 0;
			uint8_t iSs = 0;
			for (auto& c : chars) {
				if (c == 'M')
					++iMs;
				else if (c == 'S')
					++iSs;
			}

			if (iMs == 2 && iSs == 2 && chars[0] != chars[2])
				++iTotal;
		}
	}

	return iTotal;
}

void TestChallenge4b()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
