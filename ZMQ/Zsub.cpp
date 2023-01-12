/* ---------------------- ZMQ Publisher -------------------------------/
 *
 * Author: Rahul Bhadani
 *
 * ZMQ subscriber on a topic, and port number and host
 * 
 * -------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <cstring>
#include <zmq.hpp>
#include <unistd.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

//  Receive 0MQ string from socket and convert into C string
//  Caller must free returned string.
inline static char *
s_recv(void *socket, int flags = 0)
{
    //cout << "1"<<endl;
    zmq_msg_t message;
    zmq_msg_init(&message);

    int rc = zmq_msg_recv(&message, socket, flags);

    if (rc < 0)
        return nullptr;           //  Context terminated, exit

    size_t size = zmq_msg_size(&message);
    char *string = (char*)malloc(size + 1);
    memcpy(string, zmq_msg_data(&message), size);
    zmq_msg_close(&message);
    string[size] = 0;
    return (string);
}

//  Receive 0MQ string from socket and convert into string
inline static std::string s_recv (zmq::socket_t & socket, int flags = 0) 
{
    //cout << "2"<<endl;

    zmq::message_t message;
    socket.recv(&message, flags);

    return std::string(static_cast<char*>(message.data()), message.size());
}

inline static bool s_recv(zmq::socket_t & socket, std::string & ostring, int flags = 0)
{
    zmq::message_t message;
    bool rc = socket.recv(&message, flags);

    if (rc) {
        ostring = std::string(static_cast<char*>(message.data()), message.size());
    }

    return (rc);
}


int main (int argc, char **argv)
{
    // Get the current time
    auto start = high_resolution_clock::now();

    
    std::string topic_name(argv[1]);

    int port;
    std::stringstream arg1_stream(argv[2]);
    arg1_stream >> port;

    std::string host(argv[3]);

    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);

    std::string address = "tcp://"+  host + ":" + string(argv[2]);

    subscriber.connect(address);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, topic_name.c_str(), topic_name.size());

    int timeout = 0.05;

    subscriber.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));

    while (1) 
    {

        //  Read envelope with address
        //string address = s_recv (subscriber);
       // cout <<" Address"<<address<<endl;
        //  Read message contents
        string contents = s_recv (subscriber);
	if(contents == topic_name)
		continue;
     //   cout <<" Content"<<contents<<endl;
//        cout << contents<<endl;
	if(contents.size() == 0)
		continue;
        
	auto elapsed = high_resolution_clock::now() - start;
        double t = duration_cast<duration<double>>(elapsed).count();
        //cout <<"Content= "<<contents<<endl;
	double message_value = stod(contents);
        cout <<"Time: "<<t<<", Data:"<< message_value;
        cout<<endl<<endl;
        //double message_value = std::stod(*contents);
        //printf ("[%s] %f\n", address, message_value);
    //    free (address);
    //    free (contents);
    }
    //  We never get here, but clean up anyhow
  //  zmq_close (subscriber);
  //  zmq_ctx_destroy (context);
    return 0;
}
