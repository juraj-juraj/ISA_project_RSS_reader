#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <iostream>

#include "feed_downloader/feeddownloader.h"

TEST(parseHeader, parseOk)
{
    std::string headerVal =
(R"(Date: Mon, 17 Oct 2022 20:20:06 GMT
Server: Apache
Connection: close
Transfer-Encoding: chunked
Content-Type: text/xml; charset=utf-8)");

    auto actualVal = feeddownloader::feedDownloader::parseResponseHeader(headerVal);
    auto refVal = "Mon, 17 Oct 2022 20:20:06 GMT";
    ASSERT_EQ(actualVal["date"], refVal);
}
