#include "xmlparser.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <cstring>
#include "utils/exceptions.h"

void xmlParser::processor::parseFeed(std::string &feed)
{
    xmlParser::xmlBuilder builder(feed);
    mRootXml = builder.getRoot();
    if(builder.getType() == xmlParser::feedType::atom)
        parseAtom(builder.getNode());
    else if(builder.getType() == xmlParser::feedType::rss)
        parseRss(builder.getNode());
    else
        throw feedreaderException::xmlParser("Unknown type of xml");
}

xmlParser::xmlNode findTag(xmlParser::xmlNode node, const std::string& value)
{
    do{
        if(node.getName() == value)
            return node;
    }while(node.next());
    return {NULL};
}

void xmlParser::processor::parseAtom(xmlNode node)
{
    const char *storage;
    std::map<std::string, std::string> tempMap;
    bool headTitle = false;
    do
    {
        if(!headTitle && !strcmp(node.getName(), "title"))
        {
            mLogger->write("***%s***\n", node.getChild().getContent());
            headTitle = true;
        }
        if(!strcmp(node.getName(), "entry"))
        {
            auto title = findTag(node.getChild(), "title");
            if(title.getNode() != NULL)
                storage = title.getChild().getContent();
            mLogger->write("%s\n", storage);
            if(mAuthorShow)
            {
                title = findTag(node.getChild(), "author");
                if(title.getNode() != NULL)
                {
                    title = findTag(title.getChild(), "name");
                    if(title.getNode() != NULL)
                    {
                        mLogger->write("Autor: %s\n", title.getChild().getContent());
                    }
                }
            }
            if(mDateShow)
            {
                title = findTag(node.getChild(), "updated");
                if(title.getNode() != NULL)
                    mLogger->write("Aktualizace: %s\n", title.getChild().getContent());
            }
            if(mUrlShow)
            {
                title = findTag(node.getChild(), "link");
                if(title.getNode() != NULL)
                {
                    tempMap = title.getProperties();
                    if(tempMap.count("href"))
                        mLogger->write("URL: %s\n", tempMap.at("href").c_str());
                }
            }
            if(mUrlShow || mDateShow || mAuthorShow)
                mLogger->write("\n");
        }
    }while(node.next());
}

void xmlParser::processor::parseRss(xmlNode node)
{

    do
    {

    }while(node.next());


}
