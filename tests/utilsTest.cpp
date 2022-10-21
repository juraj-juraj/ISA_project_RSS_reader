#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <iostream>
#include <cctype>

#include "utils/utils.h"

using retPair = std::pair<std::string, std::string>;

bool isNotSpace(char c)
{
    return not std::isspace(c);
}

TEST(splitTest, testOKSPlit)
{
    const std::string testInput = "location: value";
    const retPair refValue = {"location", "value"};
    auto actualValue = Utils::splitDelim(testInput, ':');
    ASSERT_EQ(actualValue.first, refValue.first);
    ASSERT_EQ(actualValue.second, refValue.second);
}

TEST(splitTest, testRightEmptySplit)
{
    const std::string testInput = "location:";
    const retPair refValue = {"location", ""};
    auto actualValue = Utils::splitDelim(testInput, ':');
    ASSERT_EQ(actualValue.first, refValue.first);
    ASSERT_EQ(actualValue.second, refValue.second);
}

TEST(splitTest, testLeftEmptySplit)
{
    const std::string testInput = ": value";
    const retPair refValue = {"", "value"};
    auto actualValue = Utils::splitDelim(testInput, ':');
    ASSERT_EQ(actualValue.first, refValue.first);
    ASSERT_EQ(actualValue.second, refValue.second);
}

TEST(splitTest, testNoDelimiter)
{
    const std::string testInput = " value";
    const retPair refValue = {"value", ""};
    auto actualValue = Utils::splitDelim(testInput, ':');
    ASSERT_EQ(actualValue.first, refValue.first);
    ASSERT_EQ(actualValue.second, refValue.second);
}

TEST(splitTest, testEmptyString)
{
    const std::string testInput = "";
    const retPair refValue = {"", ""};
    auto actualValue = Utils::splitDelim(testInput, ':');
    ASSERT_EQ(actualValue.first, refValue.first);
    ASSERT_EQ(actualValue.second, refValue.second);
}

TEST(trimTest, leftTrimOk)
{
    std::string refVal = "word";
    std::string actVal = "  word";
    Utils::lTrim(actVal, isspace);
    ASSERT_EQ(refVal, actVal);
}

TEST(trimTest, leftTrimNothing)
{
    std::string refVal = "word";
    std::string actVal = "word";
    Utils::lTrim(actVal, isspace);
    ASSERT_EQ(refVal, actVal);
}

TEST(trimTest, rightTrimNothing)
{
    std::string refVal = "word";
    std::string actVal = "word";
    Utils::rTrim(actVal, isspace);
    ASSERT_EQ(refVal, actVal);
}

TEST(trimTest, rightTrimOk)
{
    std::string refVal = "word";
    std::string actVal = "word   ";
    Utils::rTrim(actVal, isspace);
    ASSERT_EQ(refVal, actVal);
}
TEST(trimTest, BothTrimOk)
{
    std::string refVal = "word";
    std::string actVal = "  word  ";
    Utils::lrTrim(actVal, isspace);
    ASSERT_EQ(refVal, actVal);
}


