#include <iostream>
#include <string>
#include <cstring>
#include <zmq.hpp>
#include <unistd.h>

using namespace std;

//  Receive 0MQ string from socket and convert into C string
//  Caller must free returned string.
inline static char *
s_recv(void *socket, int flags = 0) {
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
inline static std::string
s_recv (zmq::socket_t & socket, int flags = 0) {
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


int main (void)
{
    //  Prepare our context and subscriber
    void *context = zmq_ctx_new ();
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    zmq_connect (subscriber, "tcp://localhost:4242");
    zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, "B", 1);

    while (1) {
        //  Read envelope with address
        char *address = s_recv (subscriber);
        //  Read message contents
        char *contents = s_recv (subscriber);
	cout << contents<<endl;
	double message_value = stod(contents);
	cout << message_value;
	cout<<endl<<endl;
	//double message_value = std::stod(*contents);
        //printf ("[%s] %f\n", address, message_value);
        free (address);
        free (contents);
    }
    //  We never get here, but clean up anyhow
    zmq_close (subscriber);
    zmq_ctx_destroy (context);
    return 0;
}
