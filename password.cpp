// password.cpp
#include "password.h"
#include "sha256.h"
#include <cmath>
#include <algorithm>
#include <random> 

// 构造函数实现
PasswordCracker::PasswordCracker(const std::string& targetHash)
    : targetHash_(targetHash), isFound_(false), deductedCoins_(0), crackDirection_(-1) {} // 初始化 crackDirection_

// 添加线索实现
void PasswordCracker::addClue(const std::vector<int>& clue) {
    clues_.push_back(clue);
}

// 破译功能主入口：根据随机数选择正序或逆序破解
std::string PasswordCracker::crack() {
    isFound_ = false;
    foundPassword_ = "";
    deductedCoins_ = 0;
    attemptedPasswords_.clear();

    // 生成随机数来决定破解方向
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);
    crackDirection_ = distrib(gen); // 0: 正序, 1: 逆序

    if (crackDirection_ == 1) {
        // 随机数为 1, 从 999 向 0 尝试
        for (int i = 999; i >= 0; --i) {
            if (isFound_) break;

            std::string currentAttempt = std::to_string(i);
            while (currentAttempt.length() < 3) {
                currentAttempt = "0" + currentAttempt;
            }

            if (checkClues(currentAttempt)) {
                attemptedPasswords_.push_back(currentAttempt);
                std::string currentHash = sha256::hash_string(currentAttempt);
                if (currentHash == targetHash_) {
                    foundPassword_ = currentAttempt;
                    isFound_ = true;
                }
                else {
                    deductedCoins_++;
                }
            }
        }
    }
    else {
        // 随机数为 0, 按照原有逻辑从 0 向 999 尝试
        for (int i = 0; i <= 999; ++i) {
            if (isFound_) break;

            std::string currentAttempt = std::to_string(i);
            while (currentAttempt.length() < 3) {
                currentAttempt = "0" + currentAttempt;
            }

            if (checkClues(currentAttempt)) {
                attemptedPasswords_.push_back(currentAttempt);
                std::string currentHash = sha256::hash_string(currentAttempt);
                if (currentHash == targetHash_) {
                    foundPassword_ = currentAttempt;
                    isFound_ = true;
                }
                else {
                    deductedCoins_++;
                }
            }
        }
    }

    return foundPassword_;
}


// 获取扣除的金币数
int PasswordCracker::getDeductedCoins() const {
    return deductedCoins_;
}

// 实现获取尝试列表的函数
const std::vector<std::string>& PasswordCracker::getAttemptedPasswords() const {
    return attemptedPasswords_;
}

// --- NEW ---
// 实现获取破解方向的函数
int PasswordCracker::getCrackDirection() const {
    return crackDirection_;
}


// 检查线索的函数
bool PasswordCracker::checkClues(const std::string& password) const {
    if (clues_.empty()) {
        return true;
    }

    int d1 = password[0] - '0';
    int d2 = password[1] - '0';
    int d3 = password[2] - '0';

    for (const auto& clue : clues_) {
        if (clue.size() == 2 && clue[0] == -1 && clue[1] == -1) {
            if (!isPrime(d1) || !isPrime(d2) || !isPrime(d3)) return false;
            if (d1 == d2 || d1 == d3 || d2 == d3) return false;
        }
        else if (clue.size() == 2 && clue[1] == 0) {
            int pos = clue[0];
            if ((pos == 1 && d1 % 2 != 0) || (pos == 2 && d2 % 2 != 0) || (pos == 3 && d3 % 2 != 0))
                return false;
        }
        else if (clue.size() == 2 && clue[1] == 1) {
            int pos = clue[0];
            if ((pos == 1 && d1 % 2 == 0) || (pos == 2 && d2 % 2 == 0) || (pos == 3 && d3 % 2 == 0))
                return false;
        }
        else if (clue.size() == 3 && clue[1] == -1 && clue[2] == -1) {
            if (d1 != clue[0]) return false;
        }
        else if (clue.size() == 3 && clue[0] == -1 && clue[2] == -1) {
            if (d2 != clue[1]) return false;
        }
        else if (clue.size() == 3 && clue[0] == -1 && clue[1] == -1) {
            if (d3 != clue[2]) return false;
        }
    }
    return true;
}

// 判断素数
bool PasswordCracker::isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}