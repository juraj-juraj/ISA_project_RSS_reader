#include <gtest/gtest.h>
#include <string>
#include <iostream>

#include "urlParser/urlparser.h"
#include "utils/exceptions.h"

#define FEEDFILE_NO_URLS 4
class urlParserMock : public ::testing::Test
{
    virtual void SetUp() {
        URLTemp.clear();
        fileTemp.clear();
    }
protected:
    virtual void checkURL(struct urlParser::URLAddress &actualURL)
    {
        ASSERT_EQ(actualURL.protocol, "http");
        ASSERT_EQ(actualURL.address, "www.fit.vutbr.cz");
        ASSERT_EQ(actualURL.port, "");
        ASSERT_EQ(actualURL.path, "/news/news-rss.php");
        ASSERT_EQ(actualURL.options, "");
    }

    std::string URLTemp;
    std::string fileTemp;
    std::shared_ptr<Utils::logger> log = std::make_shared<Utils::logger>(std::cout, std::cerr);
    std::string testURL = R"(http://www.fit.vutbr.cz/news/news-rss.php)";
    std::string testFeedfile = {TEST_DIRECTORY "/test_feedfile.txt"};
    std::string testFeedfileContent[FEEDFILE_NO_URLS] = {R"(http://www.fit.vutbr.cz/news/news-rss.php)",
      R"(https://www.theregister.co.uk/headlines.atom)",
      R"(https://www.theregister.co.uk/data_centre/headlines.atom)",
      R"(https://xkcd.com/atom.xml)"};
};

TEST(urlParser, testOKURL)
{
    struct urlParser::URLAddress actualURL;
    urlParser::parseURL( R"(http://www.fit.vutbr.cz/news/news-rss.php)", actualURL);
    ASSERT_EQ(actualURL.protocol, "http");
    ASSERT_EQ(actualURL.address, "www.fit.vutbr.cz");
    ASSERT_EQ(actualURL.port, "");
    ASSERT_EQ(actualURL.path, "/news/news-rss.php");
    ASSERT_EQ(actualURL.options, "");
}

TEST(urlParser, testURLPort)
{
    struct urlParser::URLAddress actualURL;
    urlParser::parseURL( R"(http://www.fit.vutbr.cz:80/news/news-rss.php)", actualURL);
    ASSERT_EQ(actualURL.protocol, "http");
    ASSERT_EQ(actualURL.address, "www.fit.vutbr.cz");
    ASSERT_EQ(actualURL.port, "80");
    ASSERT_EQ(actualURL.path, "/news/news-rss.php");
    ASSERT_EQ(actualURL.options, "");

}

TEST(urlParser, testURLWithOptions)
{
    struct urlParser::URLAddress actualURL;
    urlParser::parseURL( R"(http://www.fit.vutbr.cz:80/news/news-rss.php?param=4;param2=fds/d)", actualURL);
    ASSERT_EQ(actualURL.protocol, "http");
    ASSERT_EQ(actualURL.address, "www.fit.vutbr.cz");
    ASSERT_EQ(actualURL.port, "80");
    ASSERT_EQ(actualURL.path, "/news/news-rss.php");
    ASSERT_EQ(actualURL.options, R"(?param=4;param2=fds/d)");
}

TEST(urlParser, testURLWithoutAnything)
{
    struct urlParser::URLAddress actualURL;
    urlParser::parseURL( R"(http://www.fit.vutbr.cz)", actualURL);
    ASSERT_EQ(actualURL.protocol, "http");
    ASSERT_EQ(actualURL.address, "www.fit.vutbr.cz");
    ASSERT_EQ(actualURL.port, "");
    ASSERT_EQ(actualURL.path, "");
    ASSERT_EQ(actualURL.options, "");
}

TEST(urlParser, testURLWithoutWWW)
{
    struct urlParser::URLAddress actualURL;
    urlParser::parseURL( R"(http://en.fit.vutbr.cz/d)", actualURL);
    ASSERT_EQ(actualURL.protocol, "http");
    ASSERT_EQ(actualURL.address, "en.fit.vutbr.cz");
    ASSERT_EQ(actualURL.port, "");
    ASSERT_EQ(actualURL.path, "/d");
    ASSERT_EQ(actualURL.options, "");
}


TEST(urlParser, testURLWithoutPathWithOptionsSlash)
{
    struct urlParser::URLAddress actualURL;
    urlParser::parseURL( R"(http://www.fit.vutbr.cz/?param=4;param2=fds/d)", actualURL);
    ASSERT_EQ(actualURL.protocol, "http");
    ASSERT_EQ(actualURL.address, "www.fit.vutbr.cz");
    ASSERT_EQ(actualURL.port, "");
    ASSERT_EQ(actualURL.path, "/");
    ASSERT_EQ(actualURL.options, R"(?param=4;param2=fds/d)");
}

TEST(urlParser, testURLWithoutPathWithOptions)
{
    struct urlParser::URLAddress actualURL;
    urlParser::parseURL( R"(http://www.fit.vutbr.cz?param=4;param2=fds/d)", actualURL);
    ASSERT_EQ(actualURL.protocol, "http");
    ASSERT_EQ(actualURL.address, "www.fit.vutbr.cz");
    ASSERT_EQ(actualURL.port, "");
    ASSERT_EQ(actualURL.path, "");
    ASSERT_EQ(actualURL.options, R"(?param=4;param2=fds/d)");
}

TEST(urlParser, teskURLWithoutPathWithOptionsPort)
{
    struct urlParser::URLAddress actualURL;
    urlParser::parseURL( R"(http://www.fit.vutbr.cz:80?param=4;param2=fds/d)", actualURL);
    ASSERT_EQ(actualURL.protocol, "http");
    ASSERT_EQ(actualURL.address, "www.fit.vutbr.cz");
    ASSERT_EQ(actualURL.port, "80");
    ASSERT_EQ(actualURL.path, "");
    ASSERT_EQ(actualURL.options, R"(?param=4;param2=fds/d)");
}

TEST_F(urlParserMock, checkSingleUrl)
{
    urlParser::URLParser parser(testURL, fileTemp, log);
    struct urlParser::URLAddress actualURL;
    ASSERT_TRUE(parser.next(actualURL));
    checkURL(actualURL);
    ASSERT_FALSE(parser.next(actualURL));
}

TEST_F(urlParserMock, checkURLandFeedfile)
{
    EXPECT_THROW(urlParser::URLParser parser(URLTemp, fileTemp, log), feedreaderException::URLParsing);
}

TEST_F(urlParserMock, checkNonExistringFile)
{
    fileTemp = "NonExisting";
    EXPECT_THROW(urlParser::URLParser parser(URLTemp, fileTemp, log), feedreaderException::URLParsing);
}

TEST_F(urlParserMock, checkReadingFeedfile)
{
    struct urlParser::URLAddress actualURL;
    urlParser::URLParser parser(URLTemp, testFeedfile, log);
    for(size_t i = 0; i < FEEDFILE_NO_URLS; ++i)
    {
        ASSERT_TRUE(parser.next(actualURL));
        ASSERT_EQ(actualURL.original, testFeedfileContent[i]);
    }
}


