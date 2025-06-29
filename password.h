// password.h
#ifndef PASSWORD_H
#define PASSWORD_H

#include <string>
#include <vector>
#include <iostream>
#include <random> 

class PasswordCracker {
public:
    // 构造函数，接收目标哈希值
    PasswordCracker(const std::string& targetHash);

    // 添加一条密码线索
    void addClue(const std::vector<int>& clue);

    // 开始破译过程，返回找到的密码或一个空字符串
    std::string crack();

    // 获取因密码错误而扣除的金币总数
    int getDeductedCoins() const;

    // 所有尝试的密码列表
    const std::vector<std::string>& getAttemptedPasswords() const;

    // 获取本次破解使用的方向 (0: 正序, 1: 逆序)
    int getCrackDirection() const;

private:
    std::string targetHash_;
    std::vector<std::vector<int>> clues_;
    std::string foundPassword_;
    bool isFound_;
    int deductedCoins_;

    // 存储破解方向的随机数 (0 for 0-999, 1 for 999-0)
    int crackDirection_;

    // 存储所有符合线索的尝试
    std::vector<std::string> attemptedPasswords_;

    // 检查一个给定的密码是否满足所有线索
    bool checkClues(const std::string& password) const;

    // 辅助函数：判断一个数字是否为素数
    static bool isPrime(int n);
};

#endif // PASSWORD_H