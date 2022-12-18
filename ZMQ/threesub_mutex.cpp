#include <zmq.hpp>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>

using namespace std;

std::queue<zmq::message_t> topic1_messages;
std::queue<zmq::message_t> topic2_messages;
std::queue<zmq::message_t> topic3_messages;
std::mutex topic1_mutex;
std::mutex topic2_mutex;
std::mutex topic3_mutex;

void receive_topic1_messages(zmq::socket_t& socket) 
{
    while (true) 
    {
//        cout << "Got message 1"<<endl;
        zmq::message_t message;

        socket.recv(&message, ZMQ_DONTWAIT);
        std::lock_guard<std::mutex> lock(topic1_mutex);
        topic1_messages.push(std::move(message));
    }
}

void receive_topic2_messages(zmq::socket_t& socket) 
{
    while (true) 
    {
  //      cout << "Got message 2"<<endl;
        zmq::message_t message;
        socket.recv(&message, ZMQ_DONTWAIT);
        std::lock_guard<std::mutex> lock(topic2_mutex);
        topic2_messages.push(std::move(message));
    }
}

void receive_topic3_messages(zmq::socket_t& socket) 
{
    while (true) 
    {
    //    cout << "Got message 3"<<endl;
        zmq::message_t message;
        socket.recv(&message, ZMQ_DONTWAIT);
        std::lock_guard<std::mutex> lock(topic3_mutex);
        topic3_messages.push(std::move(message));
    }
}

int main() {
    // Create a ZMQ context and socket
    zmq::context_t context(1);
    zmq::socket_t socket1(context, ZMQ_SUB);
    zmq::socket_t socket2(context, ZMQ_SUB);
    zmq::socket_t socket3(context, ZMQ_SUB);

    // Connect the socket to the tcp port 4242
    socket1.connect("tcp://localhost:4242");
    socket2.connect("tcp://localhost:4242");
    socket3.connect("tcp://localhost:4242");

    // Use the setsockopt method to subscribe to the three topics
    socket1.setsockopt(ZMQ_SUBSCRIBE, "100", 3);
    socket2.setsockopt(ZMQ_SUBSCRIBE, "101", 3);
    socket3.setsockopt(ZMQ_SUBSCRIBE, "102", 3);

    // Start the threads to receive messages from the three topics
    std::thread topic1_thread(receive_topic1_messages, std::ref(socket1));
    std::thread topic2_thread(receive_topic2_messages, std::ref(socket2));
    std::thread topic3_thread(receive_topic3_messages, std::ref(socket3));

        while (true) 
        {
        // Process the messages from each topic in the correct order
            {
                std::lock_guard<std::mutex> lock(topic1_mutex);
                while (!topic1_messages.empty()) 
                {
                    zmq::message_t message = std::move(topic1_messages.front());
                    topic1_messages.pop();

                    // Discard the topic name and print the actual message
                    //message.remove(0, 6);
                    if(message.size())
                    {
                        std::string message_str(static_cast<char*>(message.data()), message.size());
                        std::cout << "Received message from topic1: " << message_str << std::endl;
                    }
                }
            }

            {
                std::lock_guard<std::mutex> lock(topic2_mutex);
                while (!topic2_messages.empty()) 
                {
                    zmq::message_t message =  std::move(topic2_messages.front());
                    topic2_messages.pop();

                    // Discard the topic name and print the actual message
                    //message.remove(0, 6);
                   // cout <<" Message Len : "<<message.size() << endl;
                    
                    if(message.size())
                    {
                        std::string message_str(static_cast<char*>(message.data()), message.size());
                        std::cout << "Received message from topic2: " << message_str << std::endl;
                    }
                }
            }

            {
                std::lock_guard<std::mutex> lock(topic3_mutex);
                while (!topic3_messages.empty()) 
                {
                    zmq::message_t message =  std::move(topic3_messages.front());
                    topic3_messages.pop();

                    // Discard the topic name and print the actual message
                    //message.remove(0, 6);
                    if(message.size())
                    {
                        std::string message_str(static_cast<char*>(message.data()), message.size());
                        std::cout << "Received message from topic3: " << message_str << std::endl;
                    }
                }
            }
        }

        return 0;
}

