#pragma once
#include <iostream>
#include <mutex>
#include <wiringPi.h>
enum class FanLevel
{
    low=0,
    hight=1
};

class FanControl
{
private:
    unsigned long m_target_temperature;     //  目标时间

    int m_target_begin_hour;                // 风扇停转开始时间
    int m_target_end_hour;                  // 风扇停转终止时间
    int m_target_min;
    int m_inter_second;                     // 间隔时间

    int m_period_ms;                        // 周期
    double m_Kp;                            // pid参数
    bool m_is_night;           

    int m_wPi_pin;                          // wPi引脚编号

    FanLevel m_fan_on;
    FanLevel m_fan_off;

    std::mutex m_lock;             

private:
    unsigned long GetTemperatureImp();
    void IsNightImp(std::mutex& lock);
    double SimplePidImp(unsigned long current_temperature);
    void SetFanSpeedImp(double pid_ret);
    void LoopImp(std::mutex& lock);
    void GPIOInitImp(int wPi_pin);
public:
    FanControl( int wPi_pin,
                unsigned long target_temperature, 
                int target_begin_hour, int target_end_hour, int target_min, 
                int inter_second, int period_ms, double Kp);
    ~FanControl();

    unsigned long Gettemperature();

    // void IsNight(std::mutex& lock);
    void Loop(std::mutex& lock);
    void Loop();

    double SimplePid(unsigned long current_temperature);

    void SetFanSpeed(double pid_ret);

    void GPIOInit(int wPi_pin);
};
