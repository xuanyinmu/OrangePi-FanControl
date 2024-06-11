#pragma once
#include <iostream>
#include <fstream>
#include <string>
// #include <tuple>
#include <unordered_map>

/*
    unsigned long m_target_temperature;     //  目标温度

    int m_target_begin_hour;                // 风扇停转开始时间
    int m_target_end_hour;                  // 风扇停转终止时间
    int m_target_min;
    int m_inter_second;                     // 间隔时间

    int m_period_ms;                        // 周期
    double m_Kp;                            // pid参数
*/

class LoadConfig
{
private:
    std::string m_path;
    std::unordered_map<std::string, std::string> m_parm_umap;

private:
    // std::tuple<unsigned long, int, int, int, int, int, double> ParseImp();
    const std::unordered_map<std::string, std::string>& ParseImp();
public:
    LoadConfig(std::string path);
    ~LoadConfig();

    const std::unordered_map<std::string, std::string>& Parse();
    // std::tuple<unsigned long, int, int, int, int, int, double> Parse();
};

