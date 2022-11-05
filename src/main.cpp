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

// some feed: https://feeds.simplecast.com/54nAGcIl

#define HOST_NAME "www.http.badssl.com"
#define HOST_PORT "80"
#define HOST_RESOURCE "/"


void handleFailure(int i){
    std::cout << "gone south: " << i << std::endl;
}

std::string getFileFeed(const std::string& filename)
{
    std::stringstream buffer;
    std::ifstream feedfile;
    feedfile.open(filename);
    if(feedfile.fail())
        throw feedreaderException::downloader("cannot open file: %s", filename);
    buffer << feedfile.rdbuf();
    return buffer.str();
}

int main(int argc, const char *argv[])
{
    int retval = 1;
    bool moreUrl = false;
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
            feedParser.parseFeed(getFileFeed(argParser.getValue(TEST_FILE)));
            return 0;
        }

//        for(const auto& [key, value] : parsedValues)
//        {
//            logger->write("Key: %s | value: %s \n", key.c_str(), value.c_str());
//        }

        urlParser::URLParser urlGetter(argParser.getValue(URLPOS_ARG), argParser.getValue(FEEDFILE_ARG), logger);
        struct urlParser::URLAddress address;
        feeddownloader::feedDownloader downloader(logger);
        downloader.setupCertificate(argParser.getValue(CERTDIR_ARG), argParser.getValue(CERTFILE_ARG));
        std::string xmlFeed;
        while(urlGetter.next(address))
        {
//            if(moreUrl)
//                logger->write("\n");
            xmlFeed = downloader.download(address);
            if(xmlFeed.empty())
            {
                logger->errWrite("Cannot load feed from %s\n", address.original.c_str());
                continue;
            }
            //logger->write(xmlFeed);
            feedParser.parseFeed(xmlFeed);

            moreUrl = true;
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







//    int sockfd;
//    struct addrinfo hints, *result, *p;
//    struct sockaddr_in *ip_acces4;
//    struct sockaddr_in6 *ip_access6;
//    int rv;
//    const char *hostname = "www.google.com";
//    char ip[INET6_ADDRSTRLEN];

//    memset(&hints, 0, sizeof(hints));
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    if(rv = getaddrinfo(hostname, "domain", &hints, &result))
//    {
//        std::cerr << "error loading" << std::endl;
//        return rv;
//    }

//    for(auto p = result; p != NULL; p = p->ai_next)
//    {
//        std::cout << "inet family: " << p->ai_family << std::endl;

//        if(p->ai_family == AF_INET)
//        {
//            ip_acces4 = (struct sockaddr_in *) p->ai_addr;
//            std::cout << "lookup ip4: " << inet_ntoa(ip_acces4->sin_addr) << std::endl;
//        }
//        else if(p->ai_family == AF_INET6)
//        {
//            ip_access6 = (struct sockaddr_in6 *) p->ai_addr;
//            std::cout << "lookup6: " << inet_ntop(AF_INET6, &ip_access6->sin6_addr, ip, INET6_ADDRSTRLEN) << std::endl;
//        }
//        std::cout << "lookup port: " << ip_acces4->sin_port << std::endl;


//    }


//    long res = 1;

//    SSL_CTX* ctx = NULL;
//    BIO *web = NULL, *out = NULL;
//    SSL *ssl = NULL;

//    //init_openssl_library();
//    SSL_library_init();

//    const SSL_METHOD* method = SSLv23_method();
//    if(!(NULL != method)) handleFailure(1);

//    ctx = SSL_CTX_new(method);
//    if(!(ctx != NULL)) handleFailure(2);

//    /* Cannot fail ??? */
//    //SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);

//    /* Cannot fail ??? */
//    SSL_CTX_set_verify_depth(ctx, 4);

//    /* Cannot fail ??? */
//    const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
//    SSL_CTX_set_options(ctx, flags);

//    res = SSL_CTX_load_verify_dir(ctx, "/etc/ssl/certs");
//    //res = SSL_CTX_load_verify_locations(ctx, "random-org-chain.pem", NULL);
//    if(!(1 == res)) handleFailure(3);

//    web = BIO_new_ssl_connect(ctx);
//    if(!(web != NULL)) handleFailure(4);

//    res = BIO_set_conn_hostname(web, HOST_NAME ":" HOST_PORT);
//    if(!(1 == res)) handleFailure(5);

//    BIO_get_ssl(web, &ssl);
//    if(!(ssl != NULL)) handleFailure(6);

//    const char* const PREFERRED_CIPHERS = "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4";
//    res = SSL_set_cipher_list(ssl, PREFERRED_CIPHERS);
//    if(!(1 == res)) handleFailure(7);

//    res = SSL_set_tlsext_host_name(ssl, HOST_NAME);
//    if(!(1 == res)) handleFailure(8);

//    out = BIO_new_fp(stdout, BIO_NOCLOSE);
//    if(!(NULL != out)) handleFailure(9);

//    res = BIO_do_connect(web);
//    if(!(1 == res)) handleFailure(10);

//    res = BIO_do_handshake(web);
//    if(!(1 == res)) handleFailure(11);

//    /* Step 1: verify a server certificate was presented during the negotiation */
//    X509* cert = SSL_get_peer_certificate(ssl);
//    if(cert) { X509_free(cert); } /* Free immediately */
//    if(NULL == cert) handleFailure(12);

//    /* Step 2: verify the result of chain verification */
//    /* Verification performed according to RFC 4158    */
//    res = SSL_get_verify_result(ssl);
//    if(!(X509_V_OK == res)) handleFailure(13);

//    /* Step 3: hostname verification */
//    /* An exercise left to the reader */

//    BIO_puts(web, "GET " HOST_RESOURCE " HTTP/1.1\r\n"
//                  "Host: " HOST_NAME "\r\n"
//                  "Connection: close\r\n\r\n");
//    BIO_puts(out, "\n");

//    int len = 0;
//    do
//    {
//      char buff[1536] = {};
//      len = BIO_read(web, buff, sizeof(buff));

//      if(len > 0)
//        BIO_write(out, buff, len);

//    } while (len > 0 || BIO_should_retry(web));

//    if(out)
//      BIO_free(out);

//    if(web != NULL)
//      BIO_free_all(web);

//    if(NULL != ctx)
//      SSL_CTX_free(ctx);




//    long res = 1;

//    SSL_CTX* ctx = NULL;
//    BIO *web = NULL, *out = NULL;
//    SSL *ssl = NULL;

//    web = BIO_new_connect(HOST_NAME ":" HOST_PORT);
//    if(!(web != NULL)) handleFailure(4);

//    out = BIO_new_fp(stdout, BIO_NOCLOSE);
//    if(!(NULL != out)) handleFailure(9);

//    res = BIO_do_connect(web);
//    if(!(1 == res)) handleFailure(10);

//    res = BIO_do_handshake(web);
//    if(!(1 == res)) handleFailure(11);


//    /* Step 3: hostname verification */
//    /* An exercise left to the reader */

//    BIO_puts(web, "GET " HOST_RESOURCE " HTTP/1.1\r\n"
//                  "Host: " HOST_NAME "\r\n"
//                  "Connection: close\r\n\r\n");
//    BIO_puts(out, "\n");

//    int len = 0;
//    do
//    {
//      char buff[1536] = {};
//      len = BIO_read(web, buff, sizeof(buff));

//      if(len > 0)
//        BIO_write(out, buff, len);

//    } while (len > 0 || BIO_should_retry(web));

//    std::cout << "something " << std::endl;


//    if(out)
//      BIO_free(out);

//    if(web != NULL)
//      BIO_free_all(web);

//    if(NULL != ctx)
//      SSL_CTX_free(ctx);


    return retval;
}
