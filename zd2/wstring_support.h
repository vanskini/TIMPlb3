// wstring_support.h
#pragma once
#include <ostream>
#include <string>

inline std::ostream& operator<<(std::ostream& os, const std::wstring& wstr) {
    for (wchar_t c : wstr) {
        os << static_cast<char>(c); // простая побайтовая запись для диагностики
    }
    return os;
}