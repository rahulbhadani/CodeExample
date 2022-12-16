#include <zmq.hpp>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>

std::queue<zmq::message_t> topic1_messages;
std::queue<zmq::message_t> topic2_messages;
std::queue<zmq::message_t> topic3_messages;
std::mutex topic1_mutex;
std::mutex topic2_mutex;
std::mutex topic3_mutex;

void receive_topic1_messages(zmq::socket_t& socket) {
    while (true) {
        zmq::message_t message;
        socket.recv(&message);
        std::lock_guard<std::mutex> lock(topic1_mutex);
        topic1_messages.push(std::move(message));
    }
}

void receive_topic2_messages(zmq::socket_t& socket) {
    while (true) {
        zmq::message_t message;
        socket.recv(&message);
        std::lock_guard<std::mutex> lock(topic2_mutex);
        topic2_messages.push(std::move(message));
    }
}

void receive_topic3_messages(zmq::socket_t& socket) {
    while (true) {
        zmq::message_t message;
        socket.recv(&message);
        std::lock_guard<std::mutex> lock(topic3_mutex);
        topic3_messages.push(std::move(message));
    }
}

int main() {
    // Create a ZMQ context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_SUB);

    // Connect the socket to the tcp port 4242
    socket.connect("tcp://localhost:4242");

    // Use the setsockopt method to subscribe to the three topics
    socket.setsockopt(ZMQ_SUBSCRIBE, "topic1", 6);
    socket.setsockopt(ZMQ_SUBSCRIBE, "topic2", 6);
    socket.setsockopt(ZMQ_SUBSCRIBE, "topic3", 6);

    // Start the threads to receive messages from the three topics
    std::thread topic1_thread(receive_topic1_messages, std::ref(socket));
    std::thread topic2_thread(receive_topic2_messages, std::ref(socket));
    std::thread topic3_thread(receive_topic3_messages, std::ref(socket));

        while (true) {
        // Process the messages from each topic in the correct order
        {
            std::lock_guard<std::mutex> lock(topic1_mutex);
            while (!topic1_messages.empty()) {
                zmq::message_t message = std::move(topic1_messages.front());
                topic1_messages.pop();

                // Discard the topic name and print the actual message
                //message.remove(0, 6);
                std::string message_str(static_cast<char*>(message.data()), message.size());
                std::cout << "Received message from topic1: " << message_str << std::endl;
            }
        }

        {
            std::lock_guard<std::mutex> lock(topic2_mutex);
            while (!topic2_messages.empty()) {
                zmq::message_t message =  std::move(topic2_messages.front());
                topic2_messages.pop();

                // Discard the topic name and print the actual message
                //message.remove(0, 6);
                std::string message_str(static_cast<char*>(message.data()), message.size());
                std::cout << "Received message from topic2: " << message_str << std::endl;
            }
        }

        {
            std::lock_guard<std::mutex> lock(topic3_mutex);
            while (!topic3_messages.empty()) {
                zmq::message_t message =  std::move(topic3_messages.front());
                topic3_messages.pop();

                // Discard the topic name and print the actual message
                //message.remove(0, 6);
                std::string message_str(static_cast<char*>(message.data()), message.size());
                std::cout << "Received message from topic3: " << message_str << std::endl;
            }
        }
    }

    return 0;
}

