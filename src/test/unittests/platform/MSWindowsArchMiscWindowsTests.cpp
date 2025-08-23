#include "arch/win32/ArchMiscWindows.h"
#include <gtest/gtest.h>

using namespace inputleap;

TEST(MSWindowsArchMiscWindows, deleteKeyThrowsOnNullKey) {
    EXPECT_THROW(ArchMiscWindows::deleteKey(nullptr, _T("foo")), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, deleteKeyThrowsOnNullName) {
    HKEY dummy = reinterpret_cast<HKEY>(1);
    EXPECT_THROW(ArchMiscWindows::deleteKey(dummy, nullptr), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, deleteValueThrowsOnNullKey) {
    EXPECT_THROW(ArchMiscWindows::deleteValue(nullptr, _T("foo")), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, deleteValueThrowsOnNullName) {
    HKEY dummy = reinterpret_cast<HKEY>(1);
    EXPECT_THROW(ArchMiscWindows::deleteValue(dummy, nullptr), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, setValueThrowsOnNullKey) {
    EXPECT_THROW(ArchMiscWindows::setValue(nullptr, _T("foo"), std::string("bar")), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, setValueThrowsOnNullName) {
    HKEY dummy = reinterpret_cast<HKEY>(1);
    EXPECT_THROW(ArchMiscWindows::setValue(dummy, nullptr, std::string("bar")), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, setValueDwordThrowsOnNullKey) {
    EXPECT_THROW(ArchMiscWindows::setValue(nullptr, _T("foo"), static_cast<DWORD>(5)), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, setValueDwordThrowsOnNullName) {
    HKEY dummy = reinterpret_cast<HKEY>(1);
    EXPECT_THROW(ArchMiscWindows::setValue(dummy, nullptr, static_cast<DWORD>(5)), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, setValueBinaryThrowsOnNullKey) {
    EXPECT_THROW(ArchMiscWindows::setValueBinary(nullptr, _T("foo"), std::string("bar")), std::invalid_argument);
}

TEST(MSWindowsArchMiscWindows, setValueBinaryThrowsOnNullName) {
    HKEY dummy = reinterpret_cast<HKEY>(1);
    EXPECT_THROW(ArchMiscWindows::setValueBinary(dummy, nullptr, std::string("bar")), std::invalid_argument);
}

