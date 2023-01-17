/* ---------------------- ZMQ Publisher -------------------------------/
 * Author: Rahul Bhadani
 *
 * ZMQ Publisher on a <pub_topic_name> <pub_port> <sub_topic_name> <sub_port> <sample_time>
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

#include<control_toolbox/pid.h>

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

std::string s_recvmsg (zmq::socket_t & socket, int flags = 0)
{

    zmq::message_t message;
    socket.recv(&message, flags);
    return std::string(static_cast<char*>(message.data()), message.size());
}

class PIDController
{
    public:
        PIDController(double p, double i, double d): P(p), I(i), D(d), integral(0), derivative(0), last_error(0), last_time(high_resolution_clock::now())
    {

    }

        double compute(double reference, double measurement)
        {
            auto now = high_resolution_clock::now();
            auto dt = duration_cast<duration<double>>(now - last_time).count();
            last_time = now;

            double error = reference - measurement;
            integral += error*dt;

            derivative = (error - last_error)/dt;
            last_error = error;

            return ( P*error + I*integral + D*derivative);
        }
    private:
        double P, I, D;
        double integral;
        double derivative;
        double last_error;
        time_point<high_resolution_clock> last_time;
};

int main (int argc, char **argv)
{

    cout << "Usage: "<< argv[0] << " <pub_topic_name> <pub_port> <sub_host> <sub_topic_name> <sub_port> <sample_time>"<<endl<<endl;

    if(argc < 7)
    {
        cerr << "Usage: "<< argv[0] << " <pub_topic_name> <pub_port> <sub_host> <sub_topic_name> <sub_port> <sample_time>"<<endl<<endl;
        return 1;
    }

    double reference = 0;

    // Get the current time
    auto start = high_resolution_clock::now();

    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);    
    zmq::socket_t subscriber(context, ZMQ_SUB);

    std::string pub_topic_name(argv[1]);

    int pub_port;
    std::stringstream arg1_stream(argv[2]);
    arg1_stream >> pub_port;

    std::string sub_host(argv[3]);

    std::string sub_topic_name(argv[4]);

    int sub_port;
    std::stringstream arg2_stream(argv[5]);
    arg2_stream >> sub_port;

    double sample_time;
    std::stringstream arg3_stream(argv[6]);
    arg3_stream >> sample_time;
    cout << "Sample Time "<< sample_time<<endl;

    string pub_address = "tcp://*:" + string(argv[2]);
    publisher.bind(pub_address);

    string sub_address = "tcp://"+ string(argv[3]) + ":" + string(argv[5]);
    cout <<"Subscriber address: "<< sub_address<<endl;
    subscriber.connect(sub_address);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, sub_topic_name.c_str(), sub_topic_name.size());
    
    int timeout = 0;
	subscriber.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));

    std::string sub_msg;
    double sub_val, prev_sub_val;
    sub_val = 0;
    prev_sub_val = 0;

    PIDController pid(100.0, 1.0, 1.0);
        
    double control_signal = 0.0;

    while (1) 
    {
        // Get the data Angle Data from Inverted Pendulum Plant
        sub_msg = s_recvmsg(subscriber);
        if(sub_msg == sub_topic_name)
        {
            sub_msg = s_recvmsg(subscriber);
            if(sub_msg.size() == 0)
            {
                sub_val = prev_sub_val;
            }
            else
            {
                try
                {
                    sub_val = stod(sub_msg);
                    prev_sub_val = sub_val;
                }
                catch (const std::invalid_argument& e)
                {
                    sub_val = prev_sub_val;
                }
            }

        }
        else
        {
            if(sub_msg.size() == 0)
            {
                sub_val = prev_sub_val;
            }
            else
            {
                try
                {
                    sub_val = stod(sub_msg);
                    prev_sub_val = sub_val;
                }
                catch (const std::invalid_argument& e)
                {
                    sub_val = prev_sub_val;
                }
            }
        }

        control_signal = pid.compute(reference, sub_val);


        auto elapsed = high_resolution_clock::now() - start;
        double t = duration_cast<duration<double>>(elapsed).count();
        double s = sin(t);

        s = s + control_signal;

        std::cout << "sin(" << t << ") = " << s << std::endl;
        //  Write two messages, each with an envelope and content
        char buffer[32];

        s_sendmore (publisher, pub_topic_name);
        std::snprintf(buffer, sizeof(buffer), "%f", s);
        s_send(publisher,  const_cast<char*>(buffer));
        preciseSleep(sample_time);
    }
    //  We never get here, but clean up anyhow
    zmq_close (publisher);
    //zmq_ctx_destroy (context);
    return 0;
}
