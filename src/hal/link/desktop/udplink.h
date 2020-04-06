#pragma once

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "abstractlink.h"

#include <iostream>

class UdpLink : public AbstractLink {
public:
    UdpLink(const std::string& host, const std::string& port);
    ~UdpLink()
    {
        close();

        // Ask context to finish and wait for it
        _runContext = false;
        _futureContent.get();
    };

    virtual void close() override final
    {
        _socket.close();
    };

    virtual bool isOpen() override final
    {
        return _socket.is_open();
    };

    virtual void write(const std::vector<uint8_t>& vector) final override;

    virtual int read(char* /*buffer*/, int /*nBytes*/) override final { return 0; };
    virtual int write(const char* /*data*/, int /*nBytes*/) override final { return 0; };

private:
    UdpLink(const UdpLink&) = delete;
    const UdpLink& operator=(const UdpLink&) = delete;

    void bindRead();
    void doRead(boost::system::error_code error, size_t bytesReceived);

    boost::asio::io_context _ioContext;
    std::future<void> _futureContent;
    std::atomic<bool> _runContext;
    boost::asio::ip::udp::socket _socket;
    boost::asio::ip::udp::endpoint _endpoint;
    std::vector<uint8_t> _rxBuffer;
};
