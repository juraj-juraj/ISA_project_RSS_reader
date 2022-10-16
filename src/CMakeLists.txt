cmake_minimum_required (VERSION 3.0)

set(SRC
    main.cpp
    include/constants.h
)

add_library(feedreaderUtils
    include/utils/utils.h
    include/utils/utils.cpp
    include/utils/exceptions.h
    include/utils/exceptions.cpp
    include/utils/logger.h
    include/utils/logger.cpp
)

add_library(parameterParser
    include/parameter_parser/parameterparser.h
    include/parameter_parser/parameterparser.cpp
    include/parameter_parser/parametercontainer.h
    include/parameter_parser/parametercontainer.cpp
    include/parameter_parser/parametercheckers.h
    include/parameter_parser/parametercheckers.cpp
)
target_link_libraries(parameterParser PUBLIC feedreaderUtils)

add_library(URLParser
    include/urlParser/urlparser.h
    include/urlParser/urlparser.cpp
)

add_library(feedDownloader
    include/feed_downloader/feeddownloader.h
    include/feed_downloader/feeddownloader.cpp
)

target_link_libraries(URLParser PUBLIC feedreaderUtils)
target_link_libraries(feedDownloader PUBLIC feedreaderUtils ssl crypto)

add_executable(feedreader ${SRC})
target_link_libraries(feedreader PUBLIC parameterParser)
target_link_libraries(feedreader PUBLIC URLParser)
target_link_libraries(feedreader PUBLIC feedDownloader)
