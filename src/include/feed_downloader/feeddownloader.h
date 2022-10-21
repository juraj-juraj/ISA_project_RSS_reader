#pragma once

#include <string>
#include <openssl/opensslv.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/buffer.h>
#include <openssl/x509v3.h>
#include <openssl/opensslconf.h>

#include "urlParser/urlparser.h"
#include "utils/logger.h"

namespace _downloaderconsts
{
    constexpr size_t BUFF_SIZE = 1024;
}

// implementation inspired by: https://wiki.openssl.org/index.php/SSL/TLS_Client#Server_Certificate
namespace feeddownloader
{

class feedDownloader
{
public:
    feedDownloader(std::shared_ptr<Utils::logger> logger) : mLogger(logger) {};

    void setupCertificate(std::string certDir, std::string certFile);

    ~feedDownloader()
    {
        SSL_CTX_free(mCtx);
        if(mWeb != NULL)
            BIO_free_all(mWeb);
    }

    std::string& download(struct urlParser::URLAddress& address);

    static std::map<std::string, std::string>  parseResponseHeader(const std::string& header);

    static std::string removeChunks(const std::string& body);

private:

    size_t getReturnCode(std::string head);

    void httpsDownload(struct urlParser::URLAddress& address);

    void httpDownload (struct urlParser::URLAddress& address);

    static std::string createGet(struct urlParser::URLAddress& address);

    void readFromBio(BIO *web);

    SSL* mSsl = NULL;
    SSL_CTX* mCtx = NULL;
    BIO *mWeb = NULL;
    std::string mBuffer;
    std::shared_ptr<Utils::logger> mLogger;
    constexpr static const char* const mPREFERRED_CIPHERS = "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4";
};

}
