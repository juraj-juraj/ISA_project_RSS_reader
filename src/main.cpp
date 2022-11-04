/**
 * Project ISA
 * RSS and Atom feedreader
 *
 * This application downloads and outputs data from given url or feedfile with urls
 * @author Juraj Novosád xnovos13
 * @mail xnovos13@stud.fir.vutbr.cz
 **/

#include <iostream>
#include <fstream>
#include <memory.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "constants.h"
#include "parameter_parser/parameterparser.h"
#include "utils/exceptions.h"
#include "utils/logger.h"
#include "urlParser/urlparser.h"
#include "feed_downloader/feeddownloader.h"
#include "xmlparser/xmlparser.h"


int main(int argc, const char *argv[])
{
    int retval = 1;
    auto logger = std::make_shared<Utils::logger>(std::cout, std::cerr);
    ParameterParser::ParameterParser argParser;
    argParser.addValueParameter(FEEDFILE_ARG).isFile();
    argParser.addParameter(AUTHOR_ARG);
    argParser.addParameter(DATESHOW_ARG);
    argParser.addParameter(URLSHOW_ARG);
    argParser.addPositionalParameter(URLPOS_ARG, 0);
    argParser.addValueParameter(CERTFILE_ARG).isFile();
    argParser.addValueParameter(CERTDIR_ARG).isDirectory();
    argParser.addValueParameter(TEST_FILE).isFile();
    argParser.addParameter(HELP_ARG);

    try
    {
        argParser.parse(argc, argv);
        auto parsedValues = argParser.getValues();
        if(argParser.argParsed(HELP_ARG))
        {
            logger->write(HELP_MSG);
            return 0;
        }

        xmlParser::processor feedParser(argParser.argParsed(AUTHOR_ARG), argParser.argParsed(DATESHOW_ARG), argParser.argParsed(URLSHOW_ARG), logger);
        if(argParser.argParsed(TEST_FILE))
        {
            feedParser.parseFeed(feeddownloader::getFileFeed(argParser.getValue(TEST_FILE)));
            return 0;
        }

        urlParser::URLParser urlGetter(argParser.getValue(URLPOS_ARG), argParser.getValue(FEEDFILE_ARG), logger);
        struct urlParser::URLAddress address;
        feeddownloader::feedDownloader downloader(logger);
        downloader.setupCertificate(argParser.getValue(CERTDIR_ARG), argParser.getValue(CERTFILE_ARG));
        std::string xmlFeed;
        while(urlGetter.next(address))
        {
            xmlFeed = downloader.download(address);
            if(xmlFeed.empty())
            {
                logger->errWrite("Cannot load feed from %s\n", address.original.c_str());
                continue;
            }
            feedParser.parseFeed(xmlFeed);

        }
        retval = 0;
    }
    catch(const feedreaderException::URLParsing &err)
    {
        logger ->errWrite("Error parsing url addresses: %s\n", err.what());
    }

    catch(const feedreaderException::argumentParsing& err)
    {
        logger->errWrite("Error parsing arguments: %s\n\n", err.what());
        logger->errWrite(HELP_MSG);
    }
    catch(const feedreaderException::downloader& err)
    {
        logger->errWrite("Error fetching feed: %s\n", err.what());
    }
    catch(const feedreaderException::xmlParser& err)
    {
        logger->errWrite("Error parsing fetched feed: %s\n", err.what());
    }

    catch(...)
    {
        logger->errWrite("Unknown error raised \n");
    }

    return retval;
}
