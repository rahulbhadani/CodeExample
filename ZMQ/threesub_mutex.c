#include <zmq.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>

std::queue<zmq_msg_t> topic1_messages;
std::queue<zmq_msg_t> topic2_messages;
std::queue<zmq_msg_t> topic3_messages;
pthread_mutex_t topic1_mutex;
pthread_mutex_t topic2_mutex;
pthread_mutex_t topic3_mutex;

void *receive_topic1_messages(void *socket) 
{
    while (true) 
    {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, socket, 0);
        pthread_mutex_lock(&topic1_mutex);
        topic1_messages.push(message);
        pthread_mutex_unlock(&topic1_mutex);
    }
}

void *receive_topic2_messages(void *socket) 
{
    while (true) 
    {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, socket, 0);
        pthread_mutex_lock(&topic2_mutex);
        topic2_messages.push(message);
        pthread_mutex_unlock(&topic2_mutex);
    }
}

void *receive_topic3_messages(void *socket) 
{
    while (true) 
    {
        zmq_msg_t message;
        zmq_msg_init(&message);
        zmq_msg_recv(&message, socket, 0);
        pthread_mutex_lock(&topic3_mutex);
        topic3_messages.push(message);
        pthread_mutex_unlock(&topic3_mutex);
    }
}

int main() 
{
    // Create a ZMQ context and socket
    void *context = zmq_ctx_new();
    void *socket = zmq_socket(context, ZMQ_SUB);

    // Connect the socket to the tcp port 4242
    zmq_connect(socket, "tcp://localhost:4242");

    // Use the setsockopt method to subscribe to the three topics
    zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "100", 3);
    zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "101", 3);
    zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "102", 3);

    // Start the threads to receive messages from the three topics
    pthread_t topic1_thread;
    pthread_t topic2_thread;
    pthread_t topic3_thread;
    pthread_create(&topic1_thread, NULL, receive_topic1_messages, socket);
    pthread_create(&topic2_thread, NULL, receive_topic2_messages, socket);
    pthread_create(&topic3_thread, NULL, receive_topic3_messages, socket);

    while (true) 
    {
        // Process the messages from each topic in the correct order
        {
            pthread_mutex_lock(&topic1_mutex);
            while (!topic1_messages.empty()) {
                zmq_msg_t message = topic1_messages.front();
                topic1_messages.pop();

                // Discard the topic name and print the actual message
                zmq_msg_t actual_message;
                zmq_msg_init_size(&actual_message, zmq_msg_size(&message) - 3);
                memcpy(zmq_msg_data(&actual_message), (char*)zmq_msg_data(&message) + 3, zmq_msg_size(&message) - 3);
                char *message_str = (char*)zmq_msg_data(&actual_message);
                printf("Received message from topic1: %s\n", message_str);
                zmq_msg_close(&actual_message);
            }
            pthread_mutex_unlock(&topic1_mutex);
        }

        {
            pthread_mutex_lock(&topic2_mutex);
            while (!topic2_messages.empty()) {
                zmq_msg_t message = topic2_messages.front();
                topic2_messages.pop();

                // Discard the topic name and print the actual message
                zmq_msg_t actual_message;
                zmq_msg_init_size(&actual_message, zmq_msg_size(&message) - 3);
                memcpy(zmq_msg_data(&actual_message), (char*)zmq_msg_data(&message) + 3, zmq_msg_size(&message) - 3);
                char *message_str = (char*)zmq_msg_data(&actual_message);
                printf("Received message from topic2: %s\n", message_str);
                zmq_msg_close(&actual_message);
            }
            pthread_mutex_unlock(&topic2_mutex);
        }

        {
            pthread_mutex_lock(&topic3_mutex);
            while (!topic3_messages.empty()) {
                zmq_msg_t message = topic3_messages.front();
                topic3_messages.pop();

                // Discard the topic name and print the actual message
                zmq_msg_t actual_message;
                zmq_msg_init_size(&actual_message, zmq_msg_size(&message) - 3);
                memcpy(zmq_msg_data(&actual_message), (char*)zmq_msg_data(&message) + 3, zmq_msg_size(&message) - 3);
                char *message_str = (char*)zmq_msg_data(&actual_message);
                printf("Received message from topic3: %s\n", message_str);
            zmq_msg_close(&actual_message);
            }
            pthread_mutex_unlock(&topic2_mutex);
        } 
    }
    return 0;
}




