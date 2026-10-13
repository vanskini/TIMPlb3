#include "wstring_support.h"
#include <UnitTest++/UnitTest++.h>
#include "TableCipher.h"
#include <locale>

// Фикстура для тестов с ключом 3
struct Key3_fixture {
    TableCipher* p;
    Key3_fixture() {
        p = new TableCipher(3);
    }
    ~Key3_fixture() {
        delete p;
    }
};

SUITE(KeyTest) {
    TEST(ValidKey) {
        CHECK_EQUAL(L"ИТРЕПВ", TableCipher(3).encrypt(L"ПРИВЕТ"));
    }
    TEST(KeyOne) {
        // Ключ 1: шифрование не меняет текст
        CHECK_EQUAL(L"ПРИВЕТ", TableCipher(1).encrypt(L"ПРИВЕТ"));
    }
    TEST(LargeKey) {
        // Ключ 100, текст короче — одна строка, реверс
        CHECK_EQUAL(L"ТЕВИРП", TableCipher(100).encrypt(L"ПРИВЕТ"));
    }
    TEST(ZeroKey) {
        CHECK_THROW(TableCipher cp(0), cipher_error);
    }
    TEST(NegativeKey) {
        CHECK_THROW(TableCipher cp(-5), cipher_error);
    }
    TEST(TooLargeKey) {
        CHECK_THROW(TableCipher cp(1001), cipher_error);
    }
}

SUITE(EncryptTest) {
    TEST_FIXTURE(Key3_fixture, UpCaseString) {
        CHECK_EQUAL(L"ИТРЕПВ", p->encrypt(L"ПРИВЕТ"));
    }
    TEST_FIXTURE(Key3_fixture, LowCaseString) {
        CHECK_EQUAL(L"ИТРЕПВ", p->encrypt(L"привет"));
    }
    TEST_FIXTURE(Key3_fixture, StringWithWhitspaceAndPunct) {
        // "Привет, мир!" -> "ПРИВЕТМИР" (9 символов)
        // Таблица 3x3:
        // П Р И
        // В Е Т
        // М И Р
        // Считывание: столбец 2 -> И, Т, Р; столбец 1 -> Р, Е, И; столбец 0 -> П, В, М
        CHECK_EQUAL(L"ИТРРЕИПВМ", p->encrypt(L"Привет, мир!"));
    }
    TEST_FIXTURE(Key3_fixture, IncompleteString) {
        // "НЕКИТ" (5 символов), таблица 2x3:
        // Н Е К
        // И Т _
        // Считывание: столбец 2 -> К; столбец 1 -> Е, Т; столбец 0 -> Н, И
        CHECK_EQUAL(L"КЕТНИ", p->encrypt(L"НЕКИТ"));
    }
    TEST_FIXTURE(Key3_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }
    TEST_FIXTURE(Key3_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt(L"1234+8765=9999"), cipher_error);
    }
    TEST(MaxShiftKey) {
        // Ключ 7, текст "НЕКИТОС" (7 символов) -> одна строка -> реверс
        CHECK_EQUAL(L"СОТИКЕН", TableCipher(7).encrypt(L"НЕКИТОС"));
    }
}

SUITE(DecryptText) {
    TEST_FIXTURE(Key3_fixture, UpCaseString) {
        CHECK_EQUAL(L"ПРИВЕТ", p->decrypt(L"ИТРЕПВ"));
    }
    TEST_FIXTURE(Key3_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt(L"иТРЕПВ"), cipher_error);
    }
    TEST_FIXTURE(Key3_fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt(L"ИТ РЕПВ"), cipher_error);
    }
    TEST_FIXTURE(Key3_fixture, DigitsString) {
        CHECK_THROW(p->decrypt(L"ИТ1ЕПВ"), cipher_error);
    }
    TEST_FIXTURE(Key3_fixture, PunctString) {
        CHECK_THROW(p->decrypt(L"ИТ,ЕПВ"), cipher_error);
    }
    TEST_FIXTURE(Key3_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }
    TEST(MaxShiftKey) {
        CHECK_EQUAL(L"НЕКИТОС", TableCipher(7).decrypt(L"СОТИКЕН"));
    }
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
    return UnitTest::RunAllTests();
}