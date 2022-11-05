#include "xmlparser.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <cstring>

//TODO otestovate feedfile

void xmlParser::processor::parseFeed(const std::string &feed)
{
    xmlParser::xmlBuilder builder(feed);
    auto root = builder.getRoot();
    if(builder.getType() == xmlParser::feedType::atom)
        parseAtom(builder.getNode());
    else if(builder.getType() == xmlParser::feedType::rss)
        parseRss(builder.getNode());
    else
        mLogger->errWrite("Error parsing feed: unknown type of xml\n");

    xmlFreeDoc(root);
    xmlCleanupGlobals();
    xmlCleanupParser();

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
    const char *storage = "";
    std::map<std::string, std::string> tempMap;
    std::string defaultAuthor = "";

    auto title = findTag(node, "title");
    if(title.getNode() != NULL)
        mLogger->write("***%s***\n", title.getChild().getContent());
    title = findTag(node, "author");
    if(title.getNode() != NULL)
    {
        title = findTag(title.getChild(), "name");
        if(title.getNode() != NULL)
        defaultAuthor = title.getChild().getContent();
    }

    do
    {
        if(!strcmp(node.getName(), "entry"))
        {
            title = findTag(node.getChild(), "title");
            if(title.getNode() != NULL)
            {
                storage = title.getChild().getContent();
                mLogger->write("%s\n", storage);
            }
            if(mDateShow)
            {
                title = findTag(node.getChild(), "updated");
                if(title.getNode() != NULL)
                    mLogger->write("Aktualizace: %s\n", title.getChild().getContent());
            }
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
                else if(!defaultAuthor.empty())
                    mLogger->write("Autor: %s\n", defaultAuthor.c_str());
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
    if(!(mUrlShow || mDateShow || mAuthorShow))
        mLogger->write("\n");
}

void xmlParser::processor::parseRss(xmlNode node)
{
    const char *storage = "";
    std::map<std::string, std::string> tempMap;
    bool headTitle = false;
    node = node.getChild();
    do
    {

        if(!headTitle && !strcmp(node.getName(), "title"))
        {
            mLogger->write("***%s***\n", node.getChild().getContent());
            headTitle = true;
        }
        else if(!strcmp(node.getName(), "item"))
        {
            auto title = findTag(node.getChild(), "title");
            if(title.getNode() != NULL)
            {
                storage = title.getChild().getContent();
                mLogger->write("%s\n", storage);
            }
            if(mDateShow)
            {
                title = findTag(node.getChild(), "lastBuildDate");
                if(title.getNode() == NULL)
                    title = findTag(node.getChild(), "pubDate");
                if(title.getNode() != NULL)
                    mLogger->write("Aktualizace: %s\n", title.getChild().getContent());
            }
            if(mAuthorShow)
            {
                title = findTag(node.getChild(), "author");
                if(title.getNode() != NULL)
                    mLogger->write("Autor: %s\n", title.getChild().getContent());
            }
            if(mUrlShow)
            {
                title = findTag(node.getChild(), "link");
                if(title.getNode() != NULL)
                    mLogger->write("URL: %s\n", title.getChild().getContent());
            }
            if(mUrlShow || mDateShow || mAuthorShow)
                mLogger->write("\n");
        }
    }while(node.next());
    if(!(mUrlShow || mDateShow || mAuthorShow))
        mLogger->write("\n");
}
