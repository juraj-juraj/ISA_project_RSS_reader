#pragma once

#include <vector>
#include <string>
#include <exception>

namespace ParserFunctor
{
class parseFunctorI
{
public:
    virtual ~parseFunctorI() = default;

    virtual void operator()(std::string &value, std::vector<std::string>::iterator& actualValue,const std::vector<std::string>::iterator& end) = 0;
};


class withValue : public parseFunctorI
{
public:
   void operator()(std::string &value, std::vector<std::string>::iterator& actualValue,const std::vector<std::string>::iterator& end) override;
};

class positionalValue : public parseFunctorI
{
public:
    void operator()(std::string &value, std::vector<std::string>::iterator& actualValue,const std::vector<std::string>::iterator& end) override;
};

class isFile : public parseFunctorI
{
    void operator()(std::string &value, std::vector<std::string>::iterator& actualValue,const std::vector<std::string>::iterator& end) override;
};


class isDirectory : public parseFunctorI
{
    void operator()(std::string &value, std::vector<std::string>::iterator& actualValue,const std::vector<std::string>::iterator& end) override;
};

}
