#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include "seriallink.h"

using boost::asio::serial_port_base;

SerialLink::SerialLink(const std::string& port, uint32_t baudrate)
    : AbstractLink()
    , _ioContext()
    , _runContext(true)
    , _serialPort(_ioContext)
    , _rxBuffer()
    , _linkBuffer()
{
    try {
        _serialPort.open(port);
        _serialPort.set_option(serial_port_base::baud_rate(baudrate));
        _serialPort.set_option(serial_port_base::character_size(8));
        _serialPort.set_option(
            serial_port_base::parity(serial_port_base::parity::none));
        _serialPort.set_option(
            serial_port_base::stop_bits(serial_port_base::stop_bits::one));
        _serialPort.set_option(
            boost::asio::serial_port_base::character_size(8)
        );

        // We should remove flow control to make sure that will work in any case
        _serialPort.set_option(boost::asio::serial_port::flow_control(
            boost::asio::serial_port::flow_control::none));
    } catch (const std::exception& exception) {
        std::cerr << "Error while configuring serial port: " << exception.what() << std::endl;
        close();
        return;
    }

    _ioContext.post(boost::bind(&SerialLink::bindRead, this));

    _futureContent = std::async(std::launch::async, [this] {
        // Run event loop
        while (_runContext) {
            using namespace std::chrono_literals;
            try {
                _ioContext.run_for(100ms);
            } catch (const std::exception& exception) {
                std::cerr << "Event loop failed to run: " << exception.what() << std::endl;
            }
        }
        _ioContext.stop();
    });

    _serialPort.send_break();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    _serialPort.write_some(boost::asio::buffer("UUUUUUUUUU"));
}

SerialLink::~SerialLink()
{
    close();
    _runContext = false;
    _futureContent.wait();
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void SerialLink::bindRead()
{
    _serialPort.async_read_some(
        boost::asio::buffer(_rxBuffer),
        boost::bind(&SerialLink::doRead, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void SerialLink::doRead(boost::system::error_code error, size_t bytesReceived)
{
    if (error) {
        if (error.value() == boost::system::errc::operation_canceled) {
            return;
        }
        std::cerr << "Error while reading from serial port: " << error.message() << std::endl;
        if (_serialPort.is_open()) {
            _serialPort.close();
        }
    } else {
        // Redo next async request
        if (bytesReceived) {
            auto lastIterator = _rxBuffer.cbegin();
            std::advance(lastIterator, bytesReceived);
            std::vector<uint8_t> output(_rxBuffer.cbegin(), lastIterator);
            _onReceived(output);
            mtx.lock();
            int oldSize = _linkBuffer.size();
            auto init = _linkBuffer.begin();
            std::advance(init, oldSize);
            auto lastIterator2 = _rxBuffer.begin();
            std::advance(lastIterator2, bytesReceived);
            _linkBuffer.insert(init, std::begin(_rxBuffer), lastIterator2);
            mtx.unlock();
            bindRead();
        }
    }
}

int SerialLink::read(uint8_t* buffer, int nBytes)
{
    mtx.lock();
    //std::cout << __PRETTY_FUNCTION__ << " " << nBytes;
    const int amount = std::min(nBytes, static_cast<int>(_linkBuffer.size()));
    std::copy_n(std::begin(_linkBuffer), amount, buffer);
    auto lastIterator = _linkBuffer.begin();
    std::advance(lastIterator, amount);
    _linkBuffer.erase(_linkBuffer.begin(), lastIterator);
    mtx.unlock();
    return amount;
}

int SerialLink::write(const uint8_t* data, int nBytes)
{
    _serialPort.async_write_some(boost::asio::buffer(data, nBytes), [](const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
        if (error) {
            std::cout << "Error while writing in serial port: " << error.message() << std::endl;
        }
    });

    return nBytes;
}
