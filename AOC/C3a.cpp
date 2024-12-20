#include "include.h"

struct Input_t {
	std::string_view m_svName;
	std::string_view m_svAnswer;
	std::string_view m_svPath;
};

static constexpr std::array g_aInput = {
	Input_t{ "Test", "161", "./Resources/C3a_1.txt"},
	Input_t{ "Real", "???", "./Resources/C3a_2.txt"}
};

using ParsedInput_t = std::vector<std::pair<uint64_t, uint64_t>>;

enum class EParsePosition {
	Invalid,
	M,
	U,
	L,
	OpeningParenthesis,
	Num1Digit1,
	Num1Digit2,
	Num1Digit3,
	Comma,
	Num2Digit1,
	Num2Digit2,
	Num2Digit3
};

static ParsedInput_t ParseInput(std::string_view svInput)
{
	ParsedInput_t vecReturned{};

	EParsePosition ePosition = EParsePosition::Invalid;
	std::string sValue1{};
	std::string sValue2{};

	for (std::string_view::const_iterator itr = svInput.begin(); itr != svInput.end(); ++itr)
	{
		switch (*itr) {
		case('m'):
			ePosition = (ePosition == EParsePosition::Invalid) ? EParsePosition::M : EParsePosition::Invalid;
			break;
		case('u'):
			ePosition = (ePosition == EParsePosition::M) ? EParsePosition::U : EParsePosition::Invalid;
			break;
		case('l'):
			ePosition = (ePosition == EParsePosition::U) ? EParsePosition::L : EParsePosition::Invalid;
			break;
		case(','):
			ePosition = (ePosition == EParsePosition::Num1Digit1 || ePosition == EParsePosition::Num1Digit2 || ePosition == EParsePosition::Num1Digit3) ? EParsePosition::Comma : EParsePosition::Invalid;
			break;
		case('('):
			if (ePosition == EParsePosition::L) {
				sValue1.clear();
				sValue2.clear();
				ePosition = EParsePosition::OpeningParenthesis;
			}
			else
				ePosition = EParsePosition::Invalid;
			break;
		case(')'):
			if (ePosition == EParsePosition::Num2Digit1 || ePosition == EParsePosition::Num2Digit2 || ePosition == EParsePosition::Num2Digit3)
				vecReturned.emplace_back(std::pair<uint64_t, uint64_t>(std::atoll(sValue1.c_str()), std::atoll(sValue2.c_str())));
			ePosition = EParsePosition::Invalid;
			break;
		case('0'):
		case('1'):
		case('2'):
		case('3'):
		case('4'):
		case('5'):
		case('6'):
		case('7'):
		case('8'):
		case('9'):
			if (ePosition == EParsePosition::OpeningParenthesis || ePosition == EParsePosition::Num1Digit1 || ePosition == EParsePosition::Num1Digit2)
			{
				sValue1 += *itr;
				ePosition = (ePosition == EParsePosition::OpeningParenthesis) ? EParsePosition::Num1Digit1 : (ePosition == EParsePosition::Num1Digit1) ? EParsePosition::Num1Digit2 : EParsePosition::Num1Digit3;
			}
			else if (ePosition == EParsePosition::Comma || ePosition == EParsePosition::Num2Digit1 || ePosition == EParsePosition::Num2Digit2)
			{
				sValue2 += *itr;
				ePosition = (ePosition == EParsePosition::Comma) ? EParsePosition::Num2Digit1 : (ePosition == EParsePosition::Num2Digit1) ? EParsePosition::Num2Digit2 : EParsePosition::Num2Digit3;
			}
			else
				ePosition = EParsePosition::Invalid;
			break;

		default:
			ePosition = EParsePosition::Invalid;
			break;
		}
	}

	return vecReturned;
}

static uint64_t Solve(std::string_view svPath)
{
	auto vecInput = ParseInput(LoadFileContents(svPath));

	uint64_t iTotal{};
	for (const auto& pairData : vecInput)
		iTotal += pairData.first * pairData.second;
	

	return iTotal;
}

void TestChallenge3a()
{
	for (const auto& stInput : g_aInput)
	{
		std::cout << std::format("{}: {} ({})\n", stInput.m_svName, Solve(stInput.m_svPath), stInput.m_svAnswer);
	}
}
