/* ---------------------- ZMQ Publisher -------------------------------/
 * Author: Rahul Bhadani
 *
 * ZMQ Publisher on a topic, and port number with a specified sample time
 * 
 *
 *
 *
 *
 * ------------------------------------------------------------------*/


#include <iostream>
#include <string>
#include <cstring>
#include <zmq.hpp>
#include <unistd.h>
#include <chrono>
#include <cstdint>
#include <cmath>
#include <thread>
#include <math.h>
#include <type_traits>
#include <sstream>
#include <thread>
using namespace std::chrono;

using namespace std;

void preciseSleep(double seconds)
{
    using namespace std;
    using namespace std::chrono;

    static double estimate = 5e-3;
    static double mean = 5e-3;
    static double m2 = 0;
    static int64_t count = 1;

    while (seconds > estimate) {
        auto start = high_resolution_clock::now();
        this_thread::sleep_for(milliseconds(1));
        auto end = high_resolution_clock::now();

        double observed = (end - start).count() / 1e9;
        seconds -= observed;

        ++count;
        double delta = observed - mean;
        mean += delta / count;
        m2   += delta * (observed - mean);
        double stddev = sqrt(m2 / (count - 1));
        estimate = mean + stddev;
    }

    // spin lock
    auto start = high_resolution_clock::now();
    auto spinNs = int64_t(seconds * 1e9);
    auto delay = nanoseconds(spinNs);
    while (high_resolution_clock::now() - start < delay);
}

uint64_t timeSinceEpochMillisec() 
{
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


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

    zmq::message_t message(sizeof(double));
    memcpy(message.data(), &msg, sizeof(double));

    bool rc = socket.send (message, flags);
    return (rc);
}

/*inline static int s_send(void *socket, const char *string, int flags = 0) 
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
*/
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




int main (int argc, char **argv)
{
    
    cout << "Usage: ./a.out <topic_name> <port> <sample_time>"<<endl<<endl;
	
    // Get the current time
    auto start = high_resolution_clock::now();
	
    //  Prepare our context and publisher
    //void *context = zmq_ctx_new ();
    //void *publisher = zmq_socket (context, ZMQ_PUB);
	
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);    

    std::string topic_name(argv[1]);

    int port;
    std::stringstream arg1_stream(argv[2]);
    arg1_stream >> port;

    double sample_time;
    std::stringstream arg2_stream(argv[3]);
    arg2_stream >> sample_time;


    cout << "Sample Time "<< sample_time<<endl;

    string address = "tcp://*:" + string(argv[2]);

    publisher.bind(address);

    while (1) 
    {
        auto elapsed = high_resolution_clock::now() - start;
	double t = duration_cast<duration<double>>(elapsed).count();

	double s = sin(t);
	
        std::cout << "sin(" << t << ") = " << std::sin(t) << std::endl;
        //  Write two messages, each with an envelope and content
        char buffer[32];

        s_sendmore (publisher, topic_name);
        std::snprintf(buffer, sizeof(buffer), "%f", s);
	s_send(publisher,  const_cast<char*>(buffer));
        preciseSleep(sample_time);
    }
    //  We never get here, but clean up anyhow
    zmq_close (publisher);
    //zmq_ctx_destroy (context);
    return 0;
}
