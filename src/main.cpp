#include "constants.h"
#include <iostream>
#include <fstream>
#include <memory.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "parameter_parser/parameterparser.h"
#include "utils/exceptions.h"
#include "utils/logger.h"
#include "include/urlParser/urlparser.h"

int main(int argc, const char *argv[])
{
    auto logger = std::make_shared<Utils::logger>(std::cout, std::cerr);
    ParameterParser::ParameterParser parser;
    parser.addValueParameter(FEEDFILE_ARG).isFile();
    parser.addParameter(AUTHOR_ARG);
    parser.addParameter(DATESHOW_ARG);
    parser.addParameter(URLSHOW_ARG);
    parser.addPositionalParameter(URLPOS_ARG, 0);
    parser.addValueParameter(CERTFILE_ARG).isFile();
    parser.addValueParameter(CARTADDR_ARG).isDirectory();
    parser.addParameter(HELP_ARG);

    try
    {
        parser.parse(argc, argv);
        auto parsedValues = parser.getValues();

        for(const auto& [key, value] : parsedValues)
        {
            logger->write("something out");
            std::cout << "key: " << key << std::endl;
        }


    }
    catch(const feedreaderException::URLParsing &err)
    {
        logger ->errWrite("Error parsing url addresses: %s", err.what());
    }

    catch(const feedreaderException::argumentParsing& err)
    {
        logger->errWrite("Error parsing arguments: %s\n", err.what());
    }
    catch(...)
    {
        logger->errWrite("Unknown error raised \n");
        std::cout << "Unknown error raised" << std::endl;
    }


    return 0;
}
