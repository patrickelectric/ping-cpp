#pragma once

#include <array>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "abstractlink.h"

#include <iostream>

class SerialLink : public AbstractLink {
public:
    SerialLink(const std::string& port, uint32_t baudrate);
    ~SerialLink()
    {
        close();
        _runContext = false;
        _futureContent.wait();
        _rxBuffer.clear();
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    };

    virtual void close() override final { _serialPort.close(); };
    virtual bool isOpen() override final { return _serialPort.is_open(); };
    void write(const std::vector<uint8_t>& vector) override final;

    virtual int read(char* /*buffer*/, int /*nBytes*/) override final { return 0; };
    virtual int write(const char* /*data*/, int /*nBytes*/) override final { return 0; };

private:
    SerialLink(const SerialLink&) = delete;
    const SerialLink& operator=(const SerialLink&) = delete;

    void bindRead();
    void doRead(boost::system::error_code error, size_t bytesReceived);

    boost::asio::io_context _ioContext;
    std::future<void> _futureContent;
    std::atomic<bool> _runContext;
    boost::asio::serial_port _serialPort;
    std::vector<uint8_t> _rxBuffer;
};
