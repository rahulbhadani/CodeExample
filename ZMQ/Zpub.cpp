#include <iostream>
#include <string>
#include <cstring>
#include <zmq.hpp>
#include <unistd.h>
#include <chrono>
#include <cstdint>
#include <cmath>
uint64_t timeSinceEpochMillisec() 
{
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

using namespace std;

//  Convert C string to 0MQ string and send to socket
inline static int s_send(void *socket, const double msg, int flags = 0) 
{
    int rc;
    zmq_msg_t message;
    zmq_msg_init_size(&message, sizeof(double));
    memcpy(zmq_msg_data(&message), &msg, sizeof(double));
    rc = zmq_msg_send(&message, socket, flags);
    assert(-1 != rc);
    zmq_msg_close(&message);
    return (rc);
}

//  Convert string to 0MQ string and send to socket
inline static bool s_send (zmq::socket_t & socket, const double msg, int flags = 0) 
{

    //zmq::message_t message(string.size());
    //memcpy (message.data(), string.data(), string.size());
    zmq::message_t message(sizeof(double));
    memcpy(message.data(), &msg, sizeof(double));

    bool rc = socket.send (message, flags);
    return (rc);
}

inline static int s_send(void *socket, const char *string, int flags = 0) 
{
    int rc;
    zmq_msg_t message;
    zmq_msg_init_size(&message, strlen(string));
    memcpy(zmq_msg_data(&message), string, strlen(string));
    rc = zmq_msg_send(&message, socket, flags);
    assert(-1 != rc);
    zmq_msg_close(&message);
    return (rc);
}

//  Convert string to 0MQ string and send to socket
inline static bool s_send (zmq::socket_t & socket, const std::string & string, int flags = 0) 
{

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message, flags);
    return (rc);
}

//  Sends string as 0MQ string, as multipart non-terminal
inline static int s_sendmore(void *socket, char *string) 
{
    int rc;
    zmq_msg_t message;
    zmq_msg_init_size(&message, strlen(string));
    memcpy(zmq_msg_data(&message), string, strlen(string));
    //rc = zmq_send(socket, string, strlen(string), ZMQ_SNDMORE);
    rc = zmq_msg_send(&message, socket, ZMQ_SNDMORE);
    assert(-1 != rc);
    zmq_msg_close(&message);
    return (rc);
}

//  Sends string as 0MQ string, as multipart non-terminal
inline static bool s_sendmore (zmq::socket_t & socket, const std::string & string) 
{

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());

    bool rc = socket.send (message, ZMQ_SNDMORE);
    return (rc);
}

std::string getCurrentTimestamp()
{
    using std::chrono::system_clock;
    auto currentTime = std::chrono::system_clock::now();
    char buffer[80];

    auto transformed = currentTime.time_since_epoch().count() / 1000000;

    auto millis = transformed % 1000;

    std::time_t tt;
    tt = system_clock::to_time_t ( currentTime );
    auto timeinfo = localtime (&tt);
    strftime (buffer,80,"%F %H:%M:%S",timeinfo);
    sprintf(buffer, "%s:%03d",buffer,(int)millis);

    return std::string(buffer);
}




int main (void)
{
    //  Prepare our context and publisher
    void *context = zmq_ctx_new ();
    void *publisher = zmq_socket (context, ZMQ_PUB);
    zmq_bind (publisher, "tcp://*:4242");
    
    uint64_t old_t = timeSinceEpochMillisec();
    uint64_t new_t = timeSinceEpochMillisec();



    while (1) 
    {
        new_t = timeSinceEpochMillisec();
        double s = sin( 10.0*double(new_t - old_t ) );
        cout << "S "<< s << endl;
        //  Write two messages, each with an envelope and content
        char char_array[32];
        std::snprintf(char_array, sizeof(char_array), "%f", 121212.121);

        //s_sendmore (publisher, "A");
        //s_send (publisher, char_array);
        s_sendmore (publisher, "B");
        std::snprintf(char_array, sizeof(char_array), "%f", s);
        s_send (publisher, char_array);
        cout << "Current Time is "<< getCurrentTimestamp() <<endl;
        sleep (1);
    }
    //  We never get here, but clean up anyhow
    zmq_close (publisher);
    zmq_ctx_destroy (context);
    return 0;
}
