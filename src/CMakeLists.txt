cmake_minimum_required (VERSION 3.0)

set(SRC
    main.cpp
    include/constants.h
    include/parameter_parser/parameterparser.h
    include/parameter_parser/parameterparser.cpp
    include/parameter_parser/parametercontainer.h
    include/parameter_parser/parametercontainer.cpp
    include/parameter_parser/parametercheckers.h
    include/parameter_parser/parametercheckers.cpp
    include/utils/utils.h
    include/utils/utils.cpp
    include/utils/exceptions.h
    include/utils/exceptions.cpp
    include/utils/logger.h
    include/utils/logger.cpp
)

add_executable(feedreader ${SRC})
