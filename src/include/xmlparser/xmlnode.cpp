#include "xmlnode.h"

#include "cstring"
#include "utils/exceptions.h"

const _xmlNode *xmlParser::xmlNode::next()
{
    if((mNode = mNode->next) == NULL)
        return NULL;
    if(mNode->type == xmlElementType::XML_TEXT_NODE)
        return (mNode = mNode->next);
    return mNode;
}

xmlParser::xmlNode xmlParser::xmlNode::getChild()
{
    return {mNode->children};
}

std::map<std::string, std::string> xmlParser::xmlNode::getProperties()
{
    auto props = mNode->properties;
    std::map<std::string, std::string> parsedVals;
    while(props)
    {
        parsedVals.insert({(char *)props->name, (props->children == NULL) ? "" : (char *) props->children->content});
        props = props->next;
    }
    return parsedVals;
}

const char *xmlParser::xmlNode::getName()
{
    return (const char *) mNode->name;
}

const char *xmlParser::xmlNode::getContent()
{
    return (const char *) mNode->content;
}

const _xmlNode *xmlParser::xmlNode::getNode()
{
    return mNode;
}




xmlParser::xmlBuilder::xmlBuilder(const std::string &source)
{
    mType = xmlParser::feedType::undefined;
    mRoot = xmlReadMemory(source.c_str(), source.length(),NULL,"UTF-8", XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
    if(mRoot == NULL || mRoot->children == NULL)
        return;

    mNode = mRoot->children;
    while(mNode->type == XML_PI_NODE)
        mNode = mNode->next;

    if(!strcmp((char *)mNode->name, "feed"))
        mType = xmlParser::feedType::atom;
    else if(!strcmp((char *)mNode->name, "rss"))
        mType = xmlParser::feedType::rss;
    else
        return;
    mNode = mNode->children;
    mNode = mNode->next;
}

xmlParser::xmlNode xmlParser::xmlBuilder::getNode()
{
    return {mNode};
}

xmlDocPtr xmlParser::xmlBuilder::getRoot()
{
    return mRoot;
}

xmlParser::feedType xmlParser::xmlBuilder::getType()
{
    return mType;
}




