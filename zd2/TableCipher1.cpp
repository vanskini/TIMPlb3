#include "TableCipher.h"
#include <cmath>
#include <vector>
#include <cwctype>

TableCipher::TableCipher(int key) : columns(key)
{
    getValidKey(key);
}

std::wstring TableCipher::getValidKey(int key) const
{
    if (key < 1)
        throw cipher_error("Key must be > 0");
    if (key > 1000)
        throw cipher_error("Key is too large");
    return std::wstring();
}

std::wstring TableCipher::getValidOpenText(const std::wstring& s) const
{
    std::wstring tmp;
    for (auto c : s) {
        if (iswalpha(c)) {
            if (iswlower(c))
                tmp.push_back(towupper(c));
            else
                tmp.push_back(c);
        }
    }
    if (tmp.empty())
        throw cipher_error("Empty open text");
    return tmp;
}

std::wstring TableCipher::getValidCipherText(const std::wstring& s) const
{
    if (s.empty())
        throw cipher_error("Empty cipher text");

    for (auto c : s) {
        if (!iswupper(c))
            throw cipher_error("Invalid cipher text");
    }
    return s;
}

std::wstring TableCipher::encrypt(const std::wstring& open_text) const
{
    const std::wstring valid = getValidOpenText(open_text);
    const int len  = static_cast<int>(valid.size());
    const int rows = static_cast<int>(std::ceil(static_cast<double>(len) / columns));

    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(columns, L'\0'));

    for (int i = 0; i < len; ++i)
        table[i / columns][i % columns] = valid[i];

    std::wstring result;
    result.reserve(len);
    for (int c = columns - 1; c >= 0; --c)
        for (int r = 0; r < rows; ++r)
            if (table[r][c] != L'\0')
                result.push_back(table[r][c]);

    return result;
}

std::wstring TableCipher::decrypt(const std::wstring& cipher_text) const
{
    const std::wstring valid = getValidCipherText(cipher_text);
    const int len  = static_cast<int>(valid.size());
    const int rows = static_cast<int>(std::ceil(static_cast<double>(len) / columns));

    std::vector<std::vector<bool>> filled(rows, std::vector<bool>(columns, false));
    for (int i = 0; i < len; ++i)
        filled[i / columns][i % columns] = true;

    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(columns, L'\0'));
    int idx = 0;
    for (int c = columns - 1; c >= 0; --c)
        for (int r = 0; r < rows; ++r)
            if (filled[r][c])
                table[r][c] = valid[idx++];

    std::wstring result;
    result.reserve(len);
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (table[r][c] != L'\0')
                result.push_back(table[r][c]);

    return result;
}