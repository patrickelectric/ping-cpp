#include "abstractlink.h"
#include "seriallink.h"
#include "udplink.h"

// TODO: Remove this
#include <iostream>

#include <boost/format.hpp>
#include <boost/xpressive/xpressive.hpp>

const char* AbstractLink::_urlStringRegex = R"((?P<type>udp|serial):(?P<host>.*):(?P<config>\d+))";

AbstractLink::~AbstractLink()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

std::shared_ptr<AbstractLink> AbstractLink::openUrl(const std::string& url)
{
    if (url.empty()) {
        return {};
    }

    const auto regex = boost::xpressive::sregex::compile(_urlStringRegex);
    boost::xpressive::smatch match;

    struct {
        std::string type;
        std::string host;
        std::string config;
    } urlStruct;

    if (!regex_search(url, match, regex)) {
        return {};
    }

    urlStruct.type = match["type"].str();
    urlStruct.host = match["host"].str();
    urlStruct.config = match["config"].str();

    if (urlStruct.type == "serial") {
        return std::make_shared<SerialLink>(urlStruct.host,
            std::stoi(urlStruct.config));
    } else if (urlStruct.type == "udp") {
        return std::make_shared<UdpLink>(urlStruct.host, urlStruct.config);
    }

    return {};
}
