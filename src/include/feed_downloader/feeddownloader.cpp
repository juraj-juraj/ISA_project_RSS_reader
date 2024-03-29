#include <iostream>
#include <fstream>

#include "feeddownloader.h"
#include "utils/exceptions.h"

void feeddownloader::feedDownloader::setupCertificate(std::string certDir, std::string certFile)
{
    long res = 1;
    SSL_library_init();
    const SSL_METHOD* method = SSLv23_method();
    if(method == NULL)
        throw feedreaderException::downloader("Cannot setup ssl method");

    mCtx = SSL_CTX_new(method);
    if(mCtx == NULL)
        throw feedreaderException::downloader("Cannot create ssl context");

    SSL_CTX_set_verify_depth(mCtx, 4);

    const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
    SSL_CTX_set_options(mCtx, flags);

    if(!certDir.empty() || !certFile.empty())
        res = SSL_CTX_load_verify_locations(
          mCtx, (certFile.empty()) ? NULL : certFile.c_str(), (certDir.empty()) ? NULL : certDir.c_str());
    else
        res = SSL_CTX_set_default_verify_paths(mCtx);

    if(res != 1)
        throw feedreaderException::downloader("Cannot load certificate(s)");
}

std::string& feeddownloader::feedDownloader::download(urlParser::URLAddress &address)
{
    mBuffer.clear();
    if(address.protocol == urlConstants::http)
        this->httpDownload(address);
    else
        this->httpsDownload(address);

    if(mBuffer.empty())
        return mBuffer;
    auto startHeaderI = Utils::findIt(mBuffer, "\n");
    auto returnCode = getReturnCode(std::string(mBuffer.begin(), startHeaderI));
    auto endHeaderI = Utils::findIt(mBuffer, "\r\n\r\n");
    auto params = parseResponseHeader(std::string(startHeaderI+1, endHeaderI));

    if(returnCode >= 400)
        return mBuffer = "";
    if(returnCode >= 300)
    {
        urlParser::parseURL(params.at("location"), address);
        return feeddownloader::feedDownloader::download(address);
    }

    mBuffer = std::string(endHeaderI + 4, mBuffer.end());
    if(params.count("transfer-encoding") && params.at("transfer-encoding") == "chunked")
    {
        mBuffer = removeChunks(mBuffer);
    }
    Utils::toLinuxEndline(mBuffer);
    return mBuffer;
}

std::map<std::string, std::string> feeddownloader::feedDownloader::parseResponseHeader(const std::string& header)
{
    std::map<std::string, std::string> parsedVal;
    auto previt = header.begin();
    for(auto it = std::find(header.begin(), header.end(), '\n'); previt != header.end();it = std::find(it+1, header.end(), '\n'))
    {
        auto temp_string = std::string(previt, it);
        Utils::rTrim(temp_string, [] (char c) {return c == '\r';});
        auto paramPair = Utils::splitDelim(temp_string, ':');
        Utils::normalizeInPlace(paramPair.first);
        parsedVal.emplace(paramPair.first, paramPair.second);
        previt = it;
    }
    return parsedVal;
}

std::string feeddownloader::feedDownloader::removeChunks(const std::string &body)
{
    std::stringstream ss;
    size_t nextIndex;
    auto it = body.begin();
    for(size_t chunkSize = std::stoul(body, &nextIndex, 16); chunkSize != 0; chunkSize = std::stoul(std::string(it, body.end()), &nextIndex, 16))
    {
        it += nextIndex + 1;
        ss << std::string(it + 1, it + chunkSize + 1);
        it += chunkSize + 3;
    }
    return ss.str();
}

size_t feeddownloader::feedDownloader::getReturnCode(std::string head)
{
    auto startCodeIt = Utils::findIt(head, " ");
    return std::stoul(std::string(startCodeIt, head.end()));
}

void feeddownloader::feedDownloader::httpsDownload(urlParser::URLAddress &address)
{
    long res;
    mBuffer.clear();
    mWeb = BIO_new_ssl_connect(mCtx);
    if(mWeb == NULL)
    {
        mLogger->errWrite("Error donwloading feed: Cannot instantiate ssl connection to server %s\n", address.address.c_str());
        return;
    }

    res = BIO_set_conn_hostname(mWeb, std::string(address.address + ((address.port.empty()) ? ":443" : address.port)).c_str());
    if(res != 1)
    {
        mLogger->errWrite("Error downloading feed: Cannot set connection to server %s\n", address.address.c_str());
        return;
    }

    BIO_get_ssl(mWeb, &mSsl);
    if(mSsl == NULL)
    {
        mLogger->errWrite("Error downloading feed: Cannot get ssl connection to server %s\n", address.address.c_str());
        return;
    }

    SSL_set_mode(mSsl, SSL_MODE_AUTO_RETRY);

    res = SSL_set_cipher_list(mSsl, mPREFERRED_CIPHERS);
    if(res != 1)
    {
        mLogger->errWrite("Error downloading feed: Cannot set prefered cipher list to server: %s\n", address.address.c_str());
        return;
    }

    res = SSL_set_tlsext_host_name(mSsl, address.address.c_str());
    if(res != 1)
    {
        mLogger->errWrite("Error downloading feed: Cannot set tlsext to server: %s\n", address.address.c_str());
        return;
    }

    res = BIO_do_connect(mWeb);
    if(res != 1)
    {
        mLogger->errWrite("Error downloading feed: Cannot connect to server: %s\n", address.address.c_str());
        return;
    }

    X509* cert = SSL_get_peer_certificate(mSsl);
    if(cert)
    {
        X509_free(cert);
    }
    if(cert == NULL)
    {
        mLogger->errWrite("Error downloading feed: cerificate from server %s is not valid\n", address.address.c_str());
        return;
    }

    if(SSL_get_verify_result(mSsl) != X509_V_OK)
    {
        mLogger->errWrite("Error downloading feed: cerificate from server %s is not valid\n", address.address.c_str());
        return;
    }

    std::string getRequest = createGet(address);

    BIO_puts(mWeb, getRequest.c_str());

    readFromBio(mWeb);

    if(mWeb != NULL)
        BIO_free_all(mWeb);
    mWeb = NULL;

}

void feeddownloader::feedDownloader::httpDownload(urlParser::URLAddress &address)
{
    long res = 1;

    mWeb = BIO_new_connect(std::string(address.address + ((address.port.empty()) ? ":80" : address.port)).c_str());
    if(mWeb == NULL)
    {
        mLogger->errWrite("Error downloading feed: Cannot set connection to server %s\n", address.address.c_str());
        return;
    }

    res = BIO_do_connect(mWeb);
    if(res != 1)
    {
        mLogger->errWrite("Error downloading feed: Cannot set connection to server %s\n", address.address.c_str());
        return;
    }

    std::string getRequest = createGet(address);
    BIO_puts(mWeb, getRequest.c_str());

    readFromBio(mWeb);

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

std::string feeddownloader::getFileFeed(const std::string &filename)
{
    std::stringstream buffer;
    std::ifstream feedfile;
    feedfile.open(filename);
    if(feedfile.fail())
        throw feedreaderException::downloader("cannot open file: %s", filename);
    buffer << feedfile.rdbuf();
    return buffer.str();
}
