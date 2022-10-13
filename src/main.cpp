#include "constants.h"
#include <iostream>
#include <fstream>

#include "parameter_parser/parameterparser.h"
#include "utils/exceptions.h"
#include "utils/logger.h"

int main(int argc, const char *argv[])
{
    ParameterParser::ParameterParser parser;
    parser.addValueParameter(FEEDFILE_ARG);
    parser.addParameter(AUTHOR_ARG);
    parser.addParameter(DATESHOW_ARG);

    Utils::logger logger(std::cout, std::cerr);

    try
    {
        parser.parse(argc, argv);
        auto parsedValues = parser.getValues();

        for(const auto& [key, value] : parsedValues)
        {
            logger.write("something out");
            std::cout << "key: " << key << std::endl;
        }
    }
    catch(const feedreaderException::argumentParsing& err)
    {
        std::cout << "Error parsing arguments: " << err.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "Unknown error raised" << std::endl;
    }

    std::cout << "hello world" << std::endl;
    return 0;
}
