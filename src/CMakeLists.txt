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


add_executable(feedreader ${SRC})
target_link_libraries(feedreader PUBLIC parameterParser)
