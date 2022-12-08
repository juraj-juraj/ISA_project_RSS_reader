#pragma once
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <map>
#include <string>

namespace xmlParser
{
class xmlNode
{
public:
    xmlNode(struct _xmlNode *node) : mNode(node) {};

    const struct _xmlNode* next();
    xmlNode getChild();
    std::map<std::string, std::string> getProperties();

    const char* getName();
    const char* getContent();

    const struct _xmlNode * getNode();

private:
    struct _xmlNode * mNode;
};

enum feedType
{
    undefined,
    rss,
    atom
};

class xmlBuilder
{
public:
    xmlBuilder(const std::string& source);

    xmlParser::xmlNode getNode();

    xmlDocPtr getRoot();

    enum feedType getType();

private:
    xmlDocPtr mRoot;
    struct _xmlNode *mNode;
    enum feedType mType;
};

}
