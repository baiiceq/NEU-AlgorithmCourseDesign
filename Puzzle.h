#pragma once
#include <string>
class Puzzle 
{
private:
    std::string clue;
    std::string solution;
public:
    Puzzle(const std::string& clueText);
    bool checkSolution(const std::string& attempt);
    std::string getClue() const;
	std::string getSolution() const;

};
