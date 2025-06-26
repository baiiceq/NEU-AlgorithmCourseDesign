// session.h
#ifndef SESSION_H
#define SESSION_H

#include "password.h"
#include <string>
#include <vector>

class CrackingSession {
public:
    // 构造函数
    CrackingSession() = default;

    // 运行整个破译会话的主方法
    void run();


    // 获取由用户密码生成的SHA-256哈希值
    // 声明为 const 是因为此函数不会修改类的任何成员变量
    std::string getTargetHash() const;

private:
    // --- 私有成员变量 ---
    std::string originalPassword_;
    std::string targetHash_;
    std::vector<std::vector<int>> clues_;

    // --- 私有辅助方法 ---
    void setupTargetPassword();
    void gatherClues();
    void executeAndReport();
    static std::vector<int> parseClue(const std::string& line);
};

#endif // SESSION_H