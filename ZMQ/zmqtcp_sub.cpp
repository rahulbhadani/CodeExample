#include <iostream>
#include <string>
#include <zmq.hpp>

class ZMQReceiver {
public:
    ZMQReceiver(const std::string& topic, int port)
        : topic_(topic), port_(port)
    {
        context_ = new zmq::context_t(1);
        socket_ = new zmq::socket_t(*context_, ZMQ_SUB);
        socket_->bind("tcp://*:" + std::to_string(port_));
        socket_->setsockopt(ZMQ_SUBSCRIBE, topic_.c_str(), topic_.size());
    }

    ~ZMQReceiver()
    {
        delete socket_;
        delete context_;
    }

    double receive_message()
    {
        zmq::message_t topic, message;
        socket_->recv(&topic);
        socket_->recv(&message);
        return std::stod(std::string(static_cast<char*>(message.data()), message.size()));
    }

private:
    std::string topic_;
    int port_;
    zmq::context_t* context_;
    zmq::socket_t* socket_;
};

int main()
{
    ZMQReceiver receiver("topic1", 4242);
    while (true) {
        double message = receiver.receive_message();
        std::cout << message << std::endl;
    }

    return 0;
}
