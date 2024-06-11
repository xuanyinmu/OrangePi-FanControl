#include "load_config.h"
#include "fan_control.h"
#include <memory>

int main()
{
    auto lfp = std::make_unique<LoadConfig>("fan_config.yaml");
    const auto& parms_umap = lfp->Parse();

    auto fcp = std::make_unique<FanControl>(
                                            std::stod(parms_umap.at("wPi")),
                                            std::stoul(parms_umap.at("fan_on")),
                                            std::stoi(parms_umap.at("begin_hour")),
                                            std::stoi(parms_umap.at("end_hour")),
                                            std::stoi(parms_umap.at("min")),
                                            std::stoi(parms_umap.at("inter_second")),
                                            std::stoi(parms_umap.at("period_ms")),
                                            std::stod(parms_umap.at("Kp")));
    fcp->Loop();

    return 0;
}