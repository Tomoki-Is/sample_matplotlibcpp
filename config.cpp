#include "cas_util/config.hpp"
#include <fstream>
#include <cstddef>

Config::Config(const std::string & filename, const std::unordered_map<std::string, std::string> & defaults)
    : m_map(defaults)
{
    // テキストファイル読み込み
    std::ifstream ifs(filename);
    if (ifs.good())
    {
        std::string line;
        std::size_t lineCount = 0;
        while (std::getline(ifs, line, '\n'))
        {
            if (line.back() == '\r')
            {
                line.pop_back(); // 改行コードがCR+LFの場合はLFで区切るとCR('\r')が残るので別途削除
            }

            ++lineCount;

            // 空白行は無視
            if (line.empty())
            {
                continue;
            }

            // #から始まる行は無視 (TODO: 途中に#を付けた場合を考慮)
            if (line[0] == '#')
            {
                continue;
            }

            // キーを取り出す
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '=') && (line.length() > key.length()))
            {
                // ハッシュに値を追加
                // (iss.str()だと出力済みの文字列も含まれてしまうので代わりにイテレータを使用)
                m_map[key] = std::string(std::istreambuf_iterator<char>(iss), std::istreambuf_iterator<char>());
            }
            else
            {
                // '='がない行があったら念のため警告
                std::cout
                    << "Warning: Invalid setting in '" << filename << "' (line:" << lineCount << ")!\n"
                    << "         " << line << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Warning: Failed to load config '" << filename << "'!" << std::endl;
    }
}

// std::stringの場合はそのまま返す
// (ストリームを使うとスペースで切れてしまうのでstd::stringのみ特別扱いする必要がある)
template <>
std::string Config::getAs<std::string>(const std::string & key) const
{
    if (m_map.count(key))
    {
        return m_map.at(key);
    }
    else
    {
        // 存在しない場合はどうしようもないので空文字列を返す
        std::cout << "Error: Failed to get parameter '" << key << "' from config file!" << std::endl;
        return "";
    }
}

std::string Config::getString(const std::string & key) const
{
    return getAs<std::string>(key);
}

int Config::getInt(const std::string & key) const
{
    return getAs<int>(key);
}

double Config::getDouble(const std::string & key) const
{
    return getAs<double>(key);
}

void Config::overWrite(const std::string &key, const std::string &value)
{
   m_map[key] = value;
}