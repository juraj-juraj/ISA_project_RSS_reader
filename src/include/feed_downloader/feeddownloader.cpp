#include <iostream>

#include "feeddownloader.h"
#include "utils/exceptions.h"

feeddownloader::feedDownloader::feedDownloader(std::string certDir, std::string certFile)
{
    long res = 1;
    SSL_library_init();
    const SSL_METHOD* method = SSLv23_method();
    if(!(NULL != method))
        throw feedreaderException::downloader("Cannot setup ssl method");

    mCtx = SSL_CTX_new(method);
    if(!(mCtx != NULL))
        throw feedreaderException::downloader("Cannot create ssl context");

    SSL_CTX_set_verify_depth(mCtx, 4);

    /* Cannot fail ??? */
    const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
    SSL_CTX_set_options(mCtx, flags);

    if(!certDir.empty() || !certFile.empty())
        res = SSL_CTX_load_verify_locations(mCtx, certFile.c_str(), certDir.c_str());
    else
        res = SSL_CTX_set_default_verify_paths(mCtx);

    if(res != 1)
        throw feedreaderException::downloader("Cannot load certificate(s)");
}

std::string& feeddownloader::feedDownloader::download(urlParser::URLAddress &address)
{
    mBuffer.clear();
    if(address.protocol == "http")
        this->httpDownload(address);
    else
        this->httpsDownload(address);
    return mBuffer;
}

void feeddownloader::feedDownloader::httpsDownload(urlParser::URLAddress &address)
{
    long res;
    BIO *web = NULL;

    web = BIO_new_ssl_connect(mCtx);
    if(web == NULL)
        throw feedreaderException::downloader("Cannot instantiate ssl connection");

    res = BIO_set_conn_hostname(web, std::string(address.address + ":" + ((address.port.empty()) ? "443" : address.port)).c_str());
    if(res != 1)
        throw feedreaderException::downloader("Cannot set connection to server %s", address.address.c_str());

    BIO_get_ssl(web, &mSsl);
    if(mSsl == NULL)
        throw feedreaderException::downloader("Cannot get ssl connection to server %s", address.address.c_str());


    res = SSL_set_cipher_list(mSsl, mPREFERRED_CIPHERS);
    if(res != 1)
        throw feedreaderException::downloader("Cannot set prefered cipher list to server %s", address.address.c_str());

    res = SSL_set_tlsext_host_name(mSsl, address.address.c_str());
    if(res != 1)
        throw feedreaderException::downloader("Cannot set tlsext to server %s", address.address.c_str());

    res = BIO_do_connect(web);
    if(res != 1)
        throw feedreaderException::downloader("Cannot connect to server %s", address.address.c_str());

    res = BIO_do_handshake(web);
    if(res != 1)
        throw feedreaderException::downloader("Cannot estabilish handshake to server %s", address.address.c_str());

    /* Step 1: verify a server certificate was presented during the negotiation */
    X509* cert = SSL_get_peer_certificate(mSsl);
    if(cert)
    {
        X509_free(cert);
    }
    if(cert == NULL)
        throw feedreaderException::downloader("Certificate from server %s is not valid", address.address.c_str());

    /* Step 2: verify the result of chain verification */
    /* Verification performed according to RFC 4158    */
    if(SSL_get_verify_result(mSsl) != X509_V_OK)
        throw feedreaderException::downloader("Certificate from server %s is not valid", address.address.c_str());

    std::string getRequest = createGet(address);

    BIO_puts(web, getRequest.c_str());
    //TODO maybe delete this puts
    //BIO_puts(out, "\n");

    readFromBio(web);

//    if(out)
//        BIO_free(out);
    if(web != NULL)
        BIO_free_all(web);

}

void feeddownloader::feedDownloader::httpDownload(urlParser::URLAddress &address)
{
    long res = 1;

    mWeb = BIO_new_connect(std::string(address.address + ":" + ((address.port.empty()) ? "80" : address.port)).c_str());
    if(mWeb == NULL)
        throw feedreaderException::downloader("Cannot set connection to server %s", address.address.c_str());

    res = BIO_do_connect(mWeb);
    if(res != 1)
        throw feedreaderException::downloader("Cannot set connection to server %s", address.address.c_str());

    res = BIO_do_handshake(mWeb);
    if(res != 1)
        throw feedreaderException::downloader("Cannot do handshake to server %s", address.address.c_str());

    std::string getRequest = createGet(address);
    BIO_puts(mWeb, getRequest.c_str());

    //readFromBio(mWeb, out);

    int len = 0;
    char buff[_downloaderconsts::BUFF_SIZE] = {0};
    do
    {
        len = BIO_read(mWeb, buff, _downloaderconsts::BUFF_SIZE);
        mBuffer.append(buff, len);

    } while(len > 0 || BIO_should_retry(mWeb));

    if(mWeb != NULL)
        BIO_free_all(mWeb);
    mWeb = NULL;
}

std::string feeddownloader::feedDownloader::createGet(urlParser::URLAddress &address)
{
    return Utils::string_format("GET %s%s HTTP/1.1\r\n"
                                "Host: %s\r\n"
                                "Accept: text/html \r\n"
                                "Connection: close \r\n\r\n", address.path.c_str(), address.options.c_str(), address.address.c_str());
}

void feeddownloader::feedDownloader::readFromBio(BIO *web)
{
    int len = 0;
    char buff[_downloaderconsts::BUFF_SIZE] = {0};
    do
    {
        len = BIO_read(web, buff, _downloaderconsts::BUFF_SIZE);
        mBuffer.append(buff, len);

    } while(len > 0 || BIO_should_retry(web));
}
