#include <fan_control.h>
#include <fstream>
#include <string>
// #include <ctime>
#include <chrono>
#include <thread>
#include <functional>
#include <unistd.h>


unsigned long FanControl::GetTemperatureImp()
{
    std::string cpu_tempfile = "/sys/class/thermal/thermal_zone0/temp";
    std::ifstream ifs(cpu_tempfile);
    if (!ifs.is_open())
    {
        std::cerr << "cpu_tempfile open fail" << std::endl;
    }

    std::string cpu_temperate_str;
    ifs >> cpu_temperate_str;

    unsigned long cpu_temperate = std::stoul(cpu_temperate_str);


    return cpu_temperate;
}

void FanControl::IsNightImp(std::mutex& lock)
{
    while (1)
    {
        std::lock_guard<std::mutex> guard(lock);
        auto current_time = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(current_time);

        tm *ltm = localtime(&tt);
        if (ltm->tm_hour >= m_target_begin_hour && ltm->tm_min >= m_target_min)
        {
            // return true;
            m_is_night = true;
        }
        else if (ltm->tm_hour <= m_target_end_hour)
        {
            m_is_night = true;
            // return true;
        }
        m_is_night = false;
        // return false;
        std::this_thread::sleep_for(std::chrono::minutes(10));
    }
}

double FanControl::SimplePidImp(unsigned long current_temperature)
{
    long error = static_cast<long>(current_temperature - m_target_temperature);
    return error * m_Kp;
}

void FanControl::SetFanSpeedImp(double pid_ret)
{
    if (pid_ret < 0)
    {
        sleep(static_cast<unsigned int>(std::abs(pid_ret) * 0.1));
        pid_ret = 0;
    }
    else
    {
        usleep(static_cast<useconds_t>(1000 - pid_ret));
        if (pid_ret > 100)
        {
            pid_ret = 100;
        }
    }

    // 占空比
    int duty_cycle = m_is_night ? 0 : static_cast<int>(m_period_ms * pid_ret / 100.0);
    for (int i = 0; i < m_period_ms; i++)
    {
        if (i < duty_cycle)     // 输出低电平使风扇转动
        {
            digitalWrite(m_wPi_pin, static_cast<int>(m_fan_on));
        }
        else// 输出高电平使风扇停止转动
        {
            digitalWrite(m_wPi_pin, static_cast<int>(m_fan_off));
        }

        usleep(1000);
        // std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

}

void FanControl::LoopImp(std::mutex& lock)
{
    unsigned long temperature;
    double pid_ret;
    while (1)
    {
        std::lock_guard<std::mutex> guard(lock);
        // 获取温度
        if (!m_is_night)    // 白天
        {
            temperature = Gettemperature();
            std::cout << temperature << std::endl;
            pid_ret = SimplePid(temperature);

            if (temperature >= m_target_temperature)
            {
                // 控制风扇
                SetFanSpeed(pid_ret);
                // 100 000 - 40 000 = 60 000 * 0.01 = 600
                usleep(static_cast<useconds_t>(1000 - pid_ret));
            }
            else
            {
                // 38 000 - 40 000 = 2000 * 0.01 = 20
                sleep(static_cast<unsigned int>(std::abs(pid_ret) * 0.1));
            }
        }
    }
}

void FanControl::GPIOInitImp(int wPi_pin)
{
    wiringPiSetup();
    pinMode(wPi_pin, OUTPUT);
}

FanControl::FanControl( 
                        int wPi_pin,
                        unsigned long target_temperature, 
                        int target_begin_hour, int target_end_hour, int target_min, 
                        int inter_second, int period_ms, double Kp
                        ) : 
                        m_wPi_pin(wPi_pin),
                        m_target_temperature(target_temperature), 
                        m_target_begin_hour(target_begin_hour), m_target_end_hour(target_end_hour), m_target_min(target_min),
                        m_inter_second(inter_second), m_period_ms(period_ms), m_Kp(Kp), 
                        m_is_night(false),
                        m_fan_on(FanLevel::low), m_fan_off(FanLevel::hight)
{

    GPIOInit(m_wPi_pin);
    // 子线程定时获取当前时间
    std::thread judge_night_thread(std::bind(&FanControl::IsNightImp, this, std::ref(m_lock)));
    judge_night_thread.detach();
    
    // // 主线程循环
    // std::thread main_thread(std::bind(&FanControl::LoopImp, this, std::ref(m_lock)));
    // main_thread.detach();
}

FanControl::~FanControl()
{
}

unsigned long FanControl::Gettemperature()
{
    return GetTemperatureImp();
}

// void FanControl::IsNight(std::mutex& lock)
// {
//     IsNightImp(lock);
// }

void FanControl::Loop(std::mutex & lock)
{
    LoopImp(lock);
}

void FanControl::Loop()
{
    unsigned long temperature;
    double pid_ret;
    while (1)
    {
        // 获取温度
        std::lock_guard<std::mutex> guard(m_lock);
        temperature = Gettemperature();
        // std::cout << temperature << std::endl;
        pid_ret = SimplePid(temperature);
        SetFanSpeed(pid_ret);
    }
}

double FanControl::SimplePid(unsigned long current_temperature)
{
    return SimplePidImp(current_temperature);
}

void FanControl::SetFanSpeed(double pid_ret)
{
    SetFanSpeedImp(pid_ret);
}

void FanControl::GPIOInit(int wPi_pin)
{
    GPIOInitImp(wPi_pin);
}
