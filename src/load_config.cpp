#include <load_config.h>

// std::tuple<unsigned long, int, int, int, int, int, double> LoadConfig::ParseImp()
// {

    
//     return std::tuple<unsigned long, int, int, int, int, int, double>();
// }

const std::unordered_map<std::string, std::string>& LoadConfig::ParseImp()
{
    std::ifstream ifs(m_path);
    if (!ifs.is_open())
    {
        std::cerr << m_path << " open failed" << std::endl;
    }
    std::string line;
    std::size_t pos = 0;
    while (std::getline(ifs, line))
    {
        pos = line.find(' ');
        while (pos != line.npos)
        {
            line.replace(pos, 1, "");
            pos = line.find(' ');
        }
        // std::cout << line << std::endl;
        if ((pos = line.find('#')) != line.npos)
        {
            line = line.substr(0, pos);
        }

        if (*line.rbegin() != ':')
        {
            if ((pos = line.find(':')) != line.npos)
            {
                m_parm_umap.insert(std::make_pair(line.substr(0, pos), line.substr(pos + 1)));
            }
            
        }
    }

    // std::cout << std::endl<< std::endl;
    // for (const auto &[key, val] : m_parm_umap)
    // {
    //     std::cout << key << " : " << val << std::endl;
    // }

    return m_parm_umap;
}

LoadConfig::LoadConfig(std::string path) : m_path(path)
{
    
}

LoadConfig::~LoadConfig()
{
}

const std::unordered_map<std::string, std::string>& LoadConfig::Parse()
{
    return ParseImp();
}

// std::tuple<unsigned long, int, int, int, int, int, double> LoadConfig::Parse()
// {
//     return ParseImp();
// }
