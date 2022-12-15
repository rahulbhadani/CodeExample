#include <iostream>
#include <string>
#include <cstring>
#include <zmq.hpp>

// A simple class to represent a ZeroMQ subscriber
class Subscriber {
public:
  // Constructor that takes the topic name and port as arguments
  Subscriber(const std::string& topic, int port)
    : topic_(topic), port_(port) {}

  // Receive a message of type double on the given topic and port
  double receive() {
    // Create a ZeroMQ context
    zmq::context_t context(1);

    // Create a ZeroMQ socket of type subscriber
    zmq::socket_t socket(context, ZMQ_SUB);

    // Connect the socket to the given port
    std::string connect_address = "tcp://localhost:" + std::to_string(port_);
    socket.connect(connect_address);

    // Subscribe to the given topic
    socket.setsockopt(ZMQ_SUBSCRIBE, topic_.data(), 1);

    // Receive the message data
    zmq::message_t zmq_message;
    socket.recv(&zmq_message);

    // Copy the message data from the ZeroMQ message
    double message;
    std::memcpy(&message, zmq_message.data(), sizeof(double));

    // Return the message data
    return message;
  }

private:
  // The topic name for the subscriber
  std::string topic_;

  // The port for the subscriber
  int port_;
};

int main() {
  // Create a subscriber that will receive messages on the "example" topic
  // on port 5555
  Subscriber subscriber("topic1", 4242);

  // Receive and print messages in an infinite while loop
  while (true) {
    double message = subscriber.receive();
    std::cout << "Received message: " << message << std::endl;
  }

  return 0;
}

