#include <memory>
#include <string>

#include <iostream>

#include "udplink.h"

UdpLink::UdpLink(const std::string& host, const std::string& port)
    : AbstractLink()
    , _ioContext()
    , _runContext(true)
    , _socket(_ioContext)
    , _rxBuffer(4096)
{
    _endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(host),
        std::stoi(port));

    _socket.async_connect(
        _endpoint, [this](boost::system::error_code const& error) {
            if (error) {
                std::cerr << "Error while connecting with socket:" << error.message() << std::endl;
                return;
            }

            bindRead();

            write({ 0, 255 });
        });

    _futureContent = std::async(std::launch::async, [this] {
        // Run event loop
        while (_runContext) {
            using namespace std::chrono_literals;
            try {
                _ioContext.run_for(100ms);
            } catch (const std::exception& exception) {
                std::cerr << "Event loop failed to run:" << exception.what() << std::endl;
            }
        }
    });
}

void UdpLink::bindRead()
{
    _socket.async_receive(
        boost::asio::buffer(_rxBuffer),
        boost::bind(&UdpLink::doRead, this, boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void UdpLink::doRead(boost::system::error_code error, size_t bytesReceived)
{
    if (error) {
        // We get a canceled operation when closing class while an async read is still open
        if(error.value() == boost::system::errc::operation_canceled) {
            return;
        }
        std::cerr << "Error while reading socket: " << error.message() << std::endl;
        if (_socket.is_open()) {
            _socket.close();
        }
    } else {
        auto lastIterator = _rxBuffer.cbegin();
        std::advance(lastIterator, bytesReceived);
        std::vector<uint8_t> output(_rxBuffer.cbegin(), lastIterator);

        // emit signal
        _onReceived(output);

        // Redo next async request
        bindRead();
    }
}

void UdpLink::write(const std::vector<uint8_t>& vector)
{
    _socket.async_send(
        boost::asio::buffer(vector),
        [this](boost::system::error_code error, size_t /*bytes_transferred*/) {
            if (error) {
                std::cerr << "Error while sending data to socket: "
                          << _endpoint.address().to_string() << ":" << _endpoint.port() << std::endl;
                std::cerr << "Error: " << error.category().name() << " " << error.message() << std::endl;
            }
        });
}
