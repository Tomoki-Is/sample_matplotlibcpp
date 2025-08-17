#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class Config
{
private:
    std::unordered_map<std::string, std::string> m_map;

public:
    explicit Config(const std::string & filename, const std::unordered_map<std::string, std::string> & defaults = {});

    // 型指定で設定値を取得
    template <typename T>
    T getAs(const std::string & key) const;

    // 文字列型で設定値を取得(getAs<std::string>()の別名)
    std::string getString(const std::string & key) const;

    // 整数型で設定値を取得(getAs<int>()の別名)
    int getInt(const std::string & key) const;

    // 整数型で設定値を取得(getAs<double>()の別名)
    double getDouble(const std::string & key) const;

    void overWrite(const std::string &key, const std::string &value);
};

// T型に変換して返す
// (Tがstd::stringのときは例外的にそのまま返すためconfig.cpp側でテンプレート特殊化)
template <typename T>
T Config::getAs(const std::string & key) const
{
    if (m_map.count(key))
    {
        T value;
        std::istringstream iss(m_map.at(key));
        iss >> value;
        return value;
    }
    else
    {
        // 存在しない場合はどうしようもないのでT型のデフォルト値を返す
        std::cout << "Error: Failed to get parameter '" << key << "' from config file!" << std::endl;
        return T();
    }
}

template <>
std::string Config::getAs<std::string>(const std::string & key) const;
