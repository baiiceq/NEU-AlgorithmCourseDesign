// session.cpp
#include "session.h"
#include "sha256.h"
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cctype>

//构造函数实现
CrackingSession::CrackingSession() : lastCrackDirection_(-1) {}

// 主运行方法，协调整个流程
void CrackingSession::run(const std::string& target_password) {
    setupTargetPassword(target_password);
    executeAndReport();
}

// 实现 getTargetHash 函数
std::string CrackingSession::getTargetHash() const {
    if (originalPassword_.empty()) {
        return "";
    }
    return sha256::hash_string(originalPassword_);
}

// 实现 getLastCrackDirection 函数
int CrackingSession::getLastCrackDirection() const {
    return lastCrackDirection_;
}

// 步骤 1: 接收一个密码参数并设置目标哈希值
void CrackingSession::setupTargetPassword(const std::string& password_to_set) {
    if (password_to_set.length() == 3 && std::all_of(password_to_set.begin(), password_to_set.end(), ::isdigit)) {
        originalPassword_ = password_to_set;
    }
    else {
        std::cerr << "错误: 提供的密码 \"" << password_to_set << "\" 无效。密码必须是三位数字。程序将退出。\n";
        exit(1);
    }
    std::cout << "原密码 '" << originalPassword_ << "' 的SHA-256哈希值为:\n" << getTargetHash() << std::endl;
    std::cout << "----------------------------------------------------------------\n" << std::endl;
}

// 步骤 2: 接收破译线索
void CrackingSession::gatherClues() {
    int x;
    std::cout << "请输入线索数量 (0-3，输入0代表无任何线索): ";
    std::cin >> x;
    if (x < 0 || x > 3) {
        std::cerr << "错误：线索数量必须在 0 到 3 之间。程序将退出。" << std::endl;
        exit(1);
    }
    if (x == 0) {
        std::cout << "无任何线索，将进行暴力破解。" << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    clues_.clear();
    std::cout << "请输入 " << x << " 条线索, 每条一行 (例如, [-1,-1] 或 [1,0]):" << std::endl;
    for (int i = 0; i < x; ++i) {
        std::string line;
        std::getline(std::cin, line);
        std::vector<int> parsed = parseClue(line);
        if (!parsed.empty()) {
            clues_.push_back(parsed);
        }
        else {
            i--;
        }
    }
}

// 步骤 3 & 4: 创建破译器、执行破译并显示结果
void CrackingSession::executeAndReport() {
    PasswordCracker cracker(getTargetHash());
    for (const auto& clue : clues_) {
        cracker.addClue(clue);
    }
    std::cout << "\n正在根据线索破译密码..." << std::endl;
    std::string result = cracker.crack(1);

    int coinsDeducted = cracker.getDeductedCoins();
    attempts1 = cracker.getAttemptedPasswords();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "所有符合线索的有效猜测 (" << attempts1.size() << " 个):" << std::endl;
    if (attempts1.empty()) {
        std::cout << "(无)" << std::endl;
    }
    else {
        for (size_t i = 0; i < attempts1.size(); ++i) {
            std::cout << attempts1[i] << ((i % 10 == 9 || i == attempts1.size() - 1) ? "\n" : ", ");
        }
    }
    result = cracker.crack(0);
    coinsDeducted = cracker.getDeductedCoins();
    attempts2 = cracker.getAttemptedPasswords();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "所有符合线索的有效猜测 (" << attempts2.size() << " 个):" << std::endl;
    if (attempts2.empty()) {
        std::cout << "(无)" << std::endl;
    }
    else {
        for (size_t i = 0; i < attempts2.size(); ++i) {
            std::cout << attempts2[i] << ((i % 10 == 9 || i == attempts2.size() - 1) ? "\n" : ", ");
        }
    }
}

// 从 main.cpp 移过来的辅助函数
std::vector<int> CrackingSession::parseClue(const std::string& line) {
    std::vector<int> clue;
    std::string numbers_part;
    size_t start = line.find('[');
    size_t end = line.find(']');
    if (start != std::string::npos && end != std::string::npos) {
        numbers_part = line.substr(start + 1, end - start - 1);
    }
    else {
        std::cerr << "错误：无效的线索格式，缺少[]: " << line << std::endl;
        return {};
    }
    std::stringstream ss(numbers_part);
    std::string item;
    while (std::getline(ss, item, ',')) {
        try {
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
            clue.push_back(std::stoi(item));
        }
        catch (const std::exception& e) {
            std::cerr << "错误：无效的线索格式，无法解析数字: " << line << std::endl;
            return {};
        }
    }
    return clue;
}

// 接收一个哈希值，并尝试通过暴力破解（000-999）找到对应的三位数密码
std::string CrackingSession::hashToPassword(const std::string& hash_to_crack) {
    // 遍历所有可能的三位数密码
    for (int i = 0; i <= 999; ++i) {
        // 将数字格式化为三位字符串 (例如: 5 -> "005")
        std::string candidate = std::to_string(i);
        while (candidate.length() < 3) {
            candidate = "0" + candidate;
        }

        // 计算候选密码的哈希值
        std::string candidate_hash = sha256::hash_string(candidate);

        // 如果哈希值匹配，则返回找到的密码
        if (candidate_hash == hash_to_crack) {
            return candidate;
        }
    }

    // 如果遍历完所有可能都没有找到匹配项，返回空字符串
    return "";
}