#pragma once
#include <string>
#include <stdexcept>

class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg)
        : std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg)
        : std::invalid_argument(what_arg) {}
};

class TableCipher
{
private:
    int columns;

    std::wstring getValidKey(int key) const;
    std::wstring getValidOpenText(const std::wstring& s) const;
    std::wstring getValidCipherText(const std::wstring& s) const;

public:
    TableCipher() = delete;
    explicit TableCipher(int key);
    std::wstring encrypt(const std::wstring& open_text) const;
    std::wstring decrypt(const std::wstring& cipher_text) const;
};