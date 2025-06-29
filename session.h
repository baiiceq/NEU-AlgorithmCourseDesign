// session.h
#ifndef SESSION_H
#define SESSION_H

#include "password.h"
#include <string>
#include <vector>

class CrackingSession {
public:
    // 构造函数
    CrackingSession(); // --- MODIFIED --- 显式声明构造函数以初始化新成员

    // 运行整个破译会话的主方法，接收一个目标密码
    void run(const std::string& target_password);

    // 获取由用户密码生成的SHA-256哈希值
    std::string getTargetHash() const;

    // 获取上一次破解会话使用的方向
    int getLastCrackDirection() const;

    // 接收一个哈希值，并尝试将其破解为三位数密码
    static std::string hashToPassword(const std::string& hash_to_crack);

private:
    // --- 私有成员变量 ---
    std::string originalPassword_;
    std::vector<std::vector<int>> clues_;
    // 用于存储从 cracker 获取的破解方向
    int lastCrackDirection_;

    // --- 私有辅助方法 ---
    void setupTargetPassword(const std::string& password_to_set);
    void gatherClues();
    void executeAndReport();
    static std::vector<int> parseClue(const std::string& line);
};

#endif // SESSION_H