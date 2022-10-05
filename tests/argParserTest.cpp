#include <gtest/gtest.h>
#include <map>
#include <string>

#include "parameter_parser/parameterparser.h"
#include "utils/exceptions.h"
#include "testFunctions.h"

class parameterParserMock: public ::testing::Test {
protected:
    virtual void SetUp() {
            parser.addValueParameter("-f");
            parser.addValueParameter("-c").isFile();
            parser.addValueParameter("-C");
            parser.addParameter("-a");
            parser.addParameter("-T");
            parser.addParameter("-u");
            parser.addPositionalParameter("URL", 0);
    }

    virtual void TearDown()
    {
        parser.clear();
    }

    ParameterParser::ParameterParser parser;

};


TEST_F(parameterParserMock, NoArgumentsGiven)
{
    parser.parse(1, NULL);

    ASSERT_EQ(parser.getValues().size(), 0);
}


TEST_F(parameterParserMock, CorrectArgumentsGiven)
{
    const int argc = 3;
    const char *values[argc] = {"program", "-a", "-T"};
    parser.parse(argc,  values);

    std::map<std::string, std::string> expectedParserValues = {{"-a", ""}, {"-T", ""}};
    auto actualParserValues = parser.getValues();

    feedreaderTests::mapEqual(expectedParserValues, actualParserValues);
}

TEST_F(parameterParserMock, CombinedArgumentsGiven)
{
    const int argc = 2;
    const char *values[argc] = {"program", "-aT"};
    parser.parse(argc,  values);

    std::map<std::string, std::string> expectedParserValues = {{"-a", ""}, {"-T", ""}};
    auto actualParserValues = parser.getValues();

    feedreaderTests::mapEqual(expectedParserValues, actualParserValues);
}

TEST_F(parameterParserMock, ValueArgumentsGiven)
{
    const int argc = 4;
    const char *values[argc] = {"program", "-aT", "-f", "file"};
    parser.parse(argc,  values);

    std::map<std::string, std::string> expectedParserValues = {{"-a", ""}, {"-T", ""}, {"-f", "file"}};
    auto actualParserValues = parser.getValues();

    feedreaderTests::mapEqual(expectedParserValues, actualParserValues);
}

TEST_F(parameterParserMock, ValueMissingArgumentGiven)
{
    const int argc = 3;
    const char *values[argc] = {"program", "-aT", "-f"};
    EXPECT_THROW(parser.parse(argc,  values), feedreaderException::argumentParsing);
}

TEST_F(parameterParserMock, PositionalArgumentGiven)
{
    const int argc = 5;
    const char *values[argc] = {"program", "-aT", "-f", "file", "theUrl"};
    parser.parse(argc,  values);

    std::map<std::string, std::string> expectedParserValues = {{"-a", ""}, {"-T", ""}, {"-f", "file"}, {"URL","theUrl"}};
    auto actualParserValues = parser.getValues();

    feedreaderTests::mapEqual(expectedParserValues, actualParserValues);
}

TEST_F(parameterParserMock, TooMuchPositionalArgumentGiven)
{
    const int argc = 6;
    const char *values[argc] = {"program", "-aT", "-f", "file", "theUrl", "secondUrl"};

    EXPECT_THROW(parser.parse(argc,  values), feedreaderException::argumentParsing);
}

