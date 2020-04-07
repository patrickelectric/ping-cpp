#pragma once

#include <memory>
#include <vector>

#include <boost/signals2.hpp>

#include "../ping-port.h"

class AbstractLink : public PingPort {
public:
    AbstractLink() = default;
    ~AbstractLink();

    static std::shared_ptr<AbstractLink> openUrl(const std::string& url);

    virtual void close() = 0;
    virtual bool isOpen() = 0;

    //virtual void write(const std::vector<uint8_t>& vector) = 0;

    boost::signals2::connection
    doOnReceived(std::function<void(std::vector<uint8_t>)> slot)
    {
        return _onReceived.connect(slot);
    }

protected:
    boost::signals2::signal<void(std::vector<uint8_t>)> _onReceived;

private:
    AbstractLink(const AbstractLink&) = delete;

    static const char* _urlStringRegex;
};
