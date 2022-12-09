#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <zmq.hpp>
#include <unistd.h>

// A simple class to represent a ZeroMQ publisher
class Publisher {
public:
  // Constructor that takes the topic name and port as arguments
  Publisher(const std::string& topic, int port)
    : topic_(topic), port_(port) {}

  // Publish a message of type double on the given topic and port
#if 0
  void publish(double message) {
    // Create a ZeroMQ context
    zmq::context_t context(1);

    // Create a ZeroMQ socket of type publisher
    zmq::socket_t socket(context, ZMQ_PUB);

    // Bind the socket to the given port
    std::string bind_address = "tcp://*:" + std::to_string(port_);
    socket.bind(bind_address);

    // Create a ZeroMQ message
    zmq::message_t zmq_message(sizeof(double));

    // Copy the message data into the ZeroMQ message
    std::memcpy(zmq_message.data(), &message, sizeof(double));

    // Send the ZeroMQ message on the socket
    socket.send(zmq_message);

    // Print the value of the message
    std::cout << "Published message: " << message << std::endl;
  }
#endif

  void publish(double message) {
  // Create a ZeroMQ context
  zmq::context_t context(1);

  // Create a ZeroMQ socket of type publisher
  zmq::socket_t socket(context, ZMQ_PUB);

  // Bind the socket to the given port
  std::string bind_address = "tcp://*:" + std::to_string(port_);
  socket.bind(bind_address);

  // Create a ZeroMQ message for the topic name
  zmq::message_t topic_message(topic_.size());
  std::memcpy(topic_message.data(), topic_.data(), topic_.size());

  // Create a ZeroMQ message for the message data
  zmq::message_t data_message(sizeof(double));
  std::memcpy(data_message.data(), &message, sizeof(double));

  // Send the topic name message on the socket
  socket.send(topic_message, ZMQ_SNDMORE);

  // Send the message data on the socket
  socket.send(data_message);

  // Print the value of the message
  std::cout << "Published message: " << message << std::endl;
}

private:
  // The topic name for the publisher
  std::string topic_;

  // The port for the publisher
  int port_;
};

int main() {
  // Create a publisher that will publish messages on the "example" topic
  // on port 5555
  Publisher publisher("topic1", 4242);

  // Generate and publish sine wave messages in an infinite while loop
  double x = 0.0;
  while (true) {
    double message = std::sin(x);
    publisher.publish(message);
    x += 0.1;
    sleep(1);
  }

  return 0;
}

