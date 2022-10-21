#pragma once

#include <libxml/parser.h>
#include <string>
#include "xmlparser/xmlnode.h"
#include "utils/logger.h"

namespace xmlParser
{

class processor
{
public:
  processor(bool authorShow, bool dateShow, bool urlShow, std::shared_ptr<Utils::logger> logger) :
      mAuthorShow(authorShow),
      mDateShow(dateShow),
      mUrlShow(urlShow),
      mLogger(logger){};

  void parseFeed(std::string& feed);

  ~processor()
  {
    //xmlFreeDoc(mRootXml);
    xmlCleanupGlobals();
    xmlCleanupParser();
  };

private:
    void parseAtom(xmlParser::xmlNode node);
    void parseRss(xmlParser::xmlNode node);

    bool mAuthorShow;
    bool mDateShow;
    bool mUrlShow;
    std::shared_ptr<Utils::logger> mLogger;
    xmlDocPtr mRootXml;
};

}
