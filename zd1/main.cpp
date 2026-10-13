#include "wstring_support.h"
#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher1.h"
#include <locale>

struct KeyB_fixture {
    modAlphaCipher* p;
    KeyB_fixture() {
        p = new modAlphaCipher(L"Б");
    }
    ~KeyB_fixture() {
        delete p;
    }
};

SUITE(KeyTest) {
    TEST(ValidKey) {
        CHECK_EQUAL(L"БВГБВ", modAlphaCipher(L"БВГ").encrypt(L"ААААА"));
    }
    TEST(LongKey) {
        CHECK_EQUAL(L"БВГДЕ", modAlphaCipher(L"БВГДЕЁЖЗИЙ").encrypt(L"ААААА"));
    }
    TEST(LowCaseKey) {
        CHECK_EQUAL(L"БВГБВ", modAlphaCipher(L"бвг").encrypt(L"ААААА"));
    }
    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cp(L"Б1"), cipher_error);
    }
    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cp(L"Б,В"), cipher_error);
    }
    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cp(L"Б В"), cipher_error);
    }
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cp(L""), cipher_error);
    }
    // Тест на вырожденный ключ (если добавите проверку в код)
    TEST(WeakKey) {
         CHECK_THROW(modAlphaCipher cp(L"ААА"), cipher_error);
     }
}

SUITE(EncryptTest) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_EQUAL(L"ОЁЛЙУПТ", p->encrypt(L"НЕКИТОС"));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_EQUAL(L"ОЁЛЙУПТ", p->encrypt(L"некитос"));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithWhitspaceAndPunct) {
        CHECK_EQUAL(L"ОЁЛЙУПТ", p->encrypt(L"Некит, ос!"));
    }
    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) {
        // "Некит 2019 ос" -> после валидации "НЕКИТОС" -> "ОЁЛЙУПТ"
        CHECK_EQUAL(L"ОЁЛЙУПТ", p->encrypt(L"Некит 2019 ос"));
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt(L"1234+8765=9999"), cipher_error);
    }
    TEST(MaxShiftKey) {
        CHECK_EQUAL(L"МДЙЗСНР", modAlphaCipher(L"Я").encrypt(L"НЕКИТОС"));
    }
}

SUITE(DecryptText) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_EQUAL(L"НЕКИТОС", p->decrypt(L"ОЁЛЙУПТ"));
    }
    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt(L"оёлйупт"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt(L"ОЁЛ ЙУПТ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, DigitsString) {
        CHECK_THROW(p->decrypt(L"ОЁЛ1УПТ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, PunctString) {
        CHECK_THROW(p->decrypt(L"ОЁЛ,УПТ"), cipher_error);
    }
    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }
    TEST(MaxShiftKey) {
        CHECK_EQUAL(L"НЕКИТОС", modAlphaCipher(L"Я").decrypt(L"МДЙЗСНР"));
    }
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
    return UnitTest::RunAllTests();
}