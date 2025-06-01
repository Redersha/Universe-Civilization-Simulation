#include <bits/stdc++.h>
#include <windows.h> // 用于 ShellExecute 和设置控制台编码
using namespace std;

// 定义文明的结构体
struct civilization {
    string name;               // 文明的名称
    double total_resources;    // 文明的资源总量
    double resource_consumption_rate; // 资源消耗率（每年消耗的资源比例，0-1）
    double expansion_ability;  // 文明的扩张能力（0-1，0表示无法扩张，1表示极强扩张能力）
    int developed_years;       // 文明的发展年数
    int num_of_lifes;          // 文明的人口数量
    int developed_level;       // 文明的发展等级（1-10，1最低）
    double aggressiveness;     // 好战欲（0-1，0表示完全和平，1表示极端好战）
    double internal_conflict;  // 内斗情况（0-1，0表示无内斗，1表示内斗严重）
    double political_stability;// 政治稳定性（0-1，0表示完全不稳定，1表示完全稳定）
    double social_cohesion;    // 社会凝聚力（0-1，0表示社会分裂，1表示社会团结）
    double survival_index;     // 生存指数
    double future_potential;   // 发展前途
};

// 生存指数计算公式
double calculate_survival_index(int developed_years, int num_of_lifes, int developed_level, 
                                double aggressiveness, double internal_conflict, 
                                double total_resources, double resource_consumption_rate) {
    double remaining_resources = total_resources * (1 - resource_consumption_rate * developed_years);
    if (remaining_resources <= 0) remaining_resources = 0.1; // 避免资源耗尽导致除以零
    return (developed_years * developed_level * remaining_resources) / 
           (sqrt(num_of_lifes + 1) * (1 + aggressiveness + internal_conflict));
}

// 发展前途计算公式
double calculate_future_potential(int developed_years, int num_of_lifes, int developed_level, 
                                  double political_stability, double social_cohesion, 
                                  double expansion_ability) {
    return developed_level * log(developed_years + 1) * sqrt(num_of_lifes + 1) * 
           (political_stability + social_cohesion) * expansion_ability;
}

// 模拟文明在指定时间内的存活情况
bool simulate_civilization_over_time(civilization& civ, int years) {
    for (int year = 1; year <= years; ++year) {
        // 资源消耗
        civ.total_resources -= civ.resource_consumption_rate * civ.num_of_lifes;

        // 检查资源是否耗尽
        if (civ.total_resources <= 0) {
            civ.total_resources = 0;
            cout << "文明 " << civ.name << " 在第 " << year << " 年灭亡，原因：资源耗尽。\n";
            return false;
        }

        // 内斗和社会稳定性影响
        civ.num_of_lifes -= static_cast<int>(civ.num_of_lifes * civ.internal_conflict * 0.05); // 内斗导致人口减少
        if (civ.num_of_lifes <= 0) {
            civ.num_of_lifes = 0;
            cout << "文明 " << civ.name << " 在第 " << year << " 年灭亡，原因：内斗导致人口灭绝。\n";
            return false;
        }

        // 社会凝聚力下降
        civ.social_cohesion -= 0.01; // 每年社会凝聚力略微下降
        if (civ.social_cohesion <= 0) {
            civ.social_cohesion = 0;
            cout << "文明 " << civ.name << " 在第 " << year << " 年灭亡，原因：社会崩溃。\n";
            return false;
        }

        // 人口增长（如果资源充足）
        if (civ.total_resources > civ.resource_consumption_rate * civ.num_of_lifes) {
            civ.num_of_lifes += static_cast<int>(civ.num_of_lifes * 0.02); // 人口增长2%
        }
    }

    // 如果文明存活到指定时间
    return true;
}

// 从文件读取文明数据
vector<civilization> read_civilizations_from_file(const string& filename) {
    vector<civilization> civilizations;
    ifstream file(filename);
    if (file.is_open()) {
        string line;

        // 跳过前四行
        for (int i = 0; i < 4; i++) {
            getline(file, line);
        }

        // 读取文明数据
        while (getline(file, line)) {
            stringstream ss(line);
            civilization civ;
            string temp;

            try {
                getline(ss, civ.name, ',');
                getline(ss, temp, ','); civ.developed_years = stoi(temp);
                getline(ss, temp, ','); civ.num_of_lifes = stoi(temp);
                getline(ss, temp, ','); civ.developed_level = stoi(temp);
                getline(ss, temp, ','); civ.aggressiveness = stod(temp);
                getline(ss, temp, ','); civ.internal_conflict = stod(temp);
                getline(ss, temp, ','); civ.political_stability = stod(temp);
                getline(ss, temp, ','); civ.social_cohesion = stod(temp);
                getline(ss, temp, ','); civ.total_resources = stod(temp);
                getline(ss, temp, ','); civ.resource_consumption_rate = stod(temp);
                getline(ss, temp, ','); civ.expansion_ability = stod(temp);

                civilizations.push_back(civ);
            } catch (...) {
                cerr << "解析行失败，跳过该行: " << line << endl;
            }
        }
        file.close();
    } else {
        cerr << "无法打开文件 " << filename << "！\n";
    }
    return civilizations;
}

// 创建模板文件
void create_template_file(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法创建文件 " << filename << "！请检查路径或权限。\n";
        return;
    }

    file << "# 文明参数模板文件\n";
    file << "# 每一列依次为：文明名称, 发展年数, 人口数量, 发展等级, 好战欲, 内斗情况, 政治稳定性, 社会凝聚力, 资源总量, 资源消耗率, 扩张能力\n";
    file << "# 示例：Earth,1000,8000000,7,0.3,0.2,0.8,0.9,1000000,0.001,0.7\n";
    file.close();

    cout << "文件 " << filename << " 已创建，请填写文明参数后保存并关闭文件。\n";
}

int main() {
    // 设置控制台编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);

    cout<<"宇宙文明模拟器-12参数版 v0.1 6th-edition-Release\nby GitHub Copilot(GPT-4o).debugged&modified by Mechnology.\n";

    string filename = "civilizations.txt";

    // 检查文件是否存在
    ifstream file_check(filename);
    if (!file_check.good()) {
        // 如果文件不存在，则创建模板文件
        create_template_file(filename);
    } else {
        cout << "文件 " << filename << " 已存在。\n";
    }
    file_check.close();

    // 打开文件供用户编辑
    cout << "正在打开文件 " << filename << "，请填写文明参数后保存并关闭文件。\n";
    ShellExecute(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWNORMAL);

    // 提示用户填写文件后按任意键继续
    cout << "填写完成后输入 'yes' 确认继续...\n";
    string confirmation;
    cin >> confirmation;
    if (confirmation != "yes") {
        cout << "程序退出，请填写完成后重新运行。\n";
        return 0;
    }

    // 读取文明数据
    vector<civilization> civilizations = read_civilizations_from_file(filename);

    // 检查是否成功读取文明数据
    if (civilizations.empty()) {
        cerr << "错误：未能成功读取任何文明数据，请检查文件内容格式。\n";
        system("pause");
        return 0;
    }

    // 选择模式
    cout << "请选择模式：\n1. 计算生存指数和发展前途\n2. 模拟文明在指定时间内的存活情况\n";
    int mode;
    cin >> mode;

    if (mode == 1) {
        // 原模式：计算生存指数和发展前途
        for (auto& civ : civilizations) {
            civ.survival_index = calculate_survival_index(
                civ.developed_years, civ.num_of_lifes, civ.developed_level,
                civ.aggressiveness, civ.internal_conflict,
                civ.total_resources, civ.resource_consumption_rate
            );
            civ.future_potential = calculate_future_potential(
                civ.developed_years, civ.num_of_lifes, civ.developed_level,
                civ.political_stability, civ.social_cohesion,
                civ.expansion_ability
            );
        }

        // 输出结果
        for (const auto& civ : civilizations) {
            cout << "\n文明名称: " << civ.name << endl;
            cout << "生存指数: " << civ.survival_index << endl;
            cout << "发展前途: " << civ.future_potential << endl;
        }
    } else if (mode == 2) {
        // 新模式：模拟文明在指定时间内的存活情况
        int simulation_years;
        cout << "请输入要模拟的时间（单位：年）: ";
        cin >> simulation_years;

        cout << "\n模拟结果：\n";
        for (auto& civ : civilizations) {
            if (simulate_civilization_over_time(civ, simulation_years)) {
                // 输出存活下来的文明数据
                cout << "文明 " << civ.name << " 存活。\n";
                cout << "剩余资源: " << civ.total_resources << "\n";
                cout << "当前人口数量: " << civ.num_of_lifes << "\n";
                cout << "社会凝聚力: " << civ.social_cohesion << "\n";
                cout << "政治稳定性: " << civ.political_stability << "\n";
                cout << "扩张能力: " << civ.expansion_ability << "\n\n";
            }
        }
    } else {
        cout << "无效的模式选择，请重新运行程序。\n";
    }

    // 暂停程序，防止闪退
    cout << "程序执行完毕，按任意键退出...\n";
    system("pause");

    return 0;
}