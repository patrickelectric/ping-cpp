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

    void write(const std::vector<uint8_t>& vector);

    virtual int read(uint8_t* buffer, int nBytes) override final;
    virtual int write(const uint8_t* data, int nBytes) override final;

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

    struct {
        std::vector<uint8_t> data;
        std::mutex mutex;
    } _linkBuffer;
};
