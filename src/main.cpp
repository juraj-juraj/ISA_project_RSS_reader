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
//    auto logger = std::make_shared<Utils::logger>(std::cout, std::cerr);
//    ParameterParser::ParameterParser argParser;
//    argParser.addValueParameter(FEEDFILE_ARG).isFile();
//    argParser.addParameter(AUTHOR_ARG);
//    argParser.addParameter(DATESHOW_ARG);
//    argParser.addParameter(URLSHOW_ARG);
//    argParser.addPositionalParameter(URLPOS_ARG, 0);
//    argParser.addValueParameter(CERTFILE_ARG).isFile();
//    argParser.addValueParameter(CARTADDR_ARG).isDirectory();
//    argParser.addParameter(HELP_ARG);

//    try
//    {
//        argParser.parse(argc, argv);
//        auto parsedValues = argParser.getValues();

//        for(const auto& [key, value] : parsedValues)
//        {
//            logger->write("Key: %s | value: %s \n", key, value);
//        }

//        urlParser::URLParser urlGetter(argParser.getValue(URLPOS_ARG), argParser.getValue(FEEDFILE_ARG), logger);
//        struct urlParser::URLAddress address;
//        while(urlGetter.next(address))
//        {

//        }

//    }
//    catch(const feedreaderException::URLParsing &err)
//    {
//        logger ->errWrite("Error parsing url addresses: %s", err.what());
//    }

//    catch(const feedreaderException::argumentParsing& err)
//    {
//        logger->errWrite("Error parsing arguments: %s\n", err.what());
//    }
//    catch(...)
//    {
//        logger->errWrite("Unknown error raised \n");
//    }

    int sockfd;
    struct addrinfo hints, *result, *p;
    struct sockaddr_in *ip_acces4;
    struct sockaddr_in6 *ip_access6;
    int rv;
    const char *hostname = "www.google.com";
    char ip[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if(rv = getaddrinfo(hostname, "domain", &hints, &result))
    {
        std::cerr << "error loading" << std::endl;
        return rv;
    }

    for(auto p = result; p != NULL; p = p->ai_next)
    {
        std::cout << "inet family: " << p->ai_family << std::endl;

        if(p->ai_family == AF_INET)
        {
            ip_acces4 = (struct sockaddr_in *) p->ai_addr;
            std::cout << "lookup ip4: " << inet_ntoa(ip_acces4->sin_addr) << std::endl;
        }
        else if(p->ai_family == AF_INET6)
        {
            ip_access6 = (struct sockaddr_in6 *) p->ai_addr;
            std::cout << "lookup6: " << inet_ntop(AF_INET6, &ip_access6->sin6_addr, ip, INET6_ADDRSTRLEN) << std::endl;
        }
        std::cout << "lookup port: " << ip_acces4->sin_port << std::endl;


    }

    return 0;
}
