#pragma once

#include <map>
#include <gtest/gtest.h>

namespace feedreaderTests
{

template<typename key_t,typename val_t>
void mapEqual(std::map<key_t, val_t>& expected, std::map<key_t, val_t>& actual)
{
    ASSERT_EQ(expected.size(), actual.size());
    for(const auto& [key, value] : expected)
    {
        ASSERT_TRUE(actual.count(key));
        ASSERT_EQ(actual[key], value);
    }
}
}
