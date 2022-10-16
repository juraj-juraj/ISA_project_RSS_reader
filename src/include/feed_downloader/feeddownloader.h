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

namespace _downloaderconsts
{
    constexpr size_t BUFF_SIZE = 1024;
}

// implementation inspired by: https://wiki.openssl.org/index.php/SSL/TLS_Client#Server_Certificate
namespace feeddownloader
{

constexpr const char* DEFAULT_CERT_DIR = "/etc/ssl/certs";

class feedDownloader
{
public:
    feedDownloader(std::string certDir, std::string certFile);

    ~feedDownloader()
    {
        SSL_CTX_free(mCtx);
    }

    std::string& download(struct urlParser::URLAddress& address);

private:

    void httpsDownload(struct urlParser::URLAddress& address);

    void httpDownload (struct urlParser::URLAddress& address);

    static std::string createGet(struct urlParser::URLAddress& address);

    void readFromBio(BIO *web);

    SSL* mSsl = NULL;
    SSL_CTX* mCtx = NULL;
    BIO *mWeb = NULL;
    std::string mBuffer;
    constexpr static const char* const mPREFERRED_CIPHERS = "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4";
};

}
