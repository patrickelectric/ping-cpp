#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "abstractlink.h"

#include <iostream>

class SerialLink : public AbstractLink {
public:
    SerialLink(const std::string& port, uint32_t baudrate);
    ~SerialLink();

    virtual void close() override final { _serialPort.close(); };
    virtual bool isOpen() override final { return _serialPort.is_open(); };
    //void write(const std::vector<uint8_t>& vector) override final;

    virtual int read(uint8_t* buffer, int nBytes) override final;
    virtual int write(const uint8_t* data, int nBytes) override final;

private:
    SerialLink(const SerialLink&) = delete;
    const SerialLink& operator=(const SerialLink&) = delete;

    void bindRead();
    void doRead(boost::system::error_code error, size_t bytesReceived);

    struct {
        std::atomic<bool> run;
        boost::asio::io_context eventLoop;
        std::future<void> future;
    } _context;

    boost::asio::serial_port _serialPort;
    std::array<uint8_t, 4096> _rxBuffer;

    struct {
        std::vector<uint8_t> data;
        std::mutex mutex;
    } _linkBuffer;
};
