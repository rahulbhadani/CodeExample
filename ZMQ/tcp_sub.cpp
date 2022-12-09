#include <zmq.hpp> 


char *s_recv(void *socket, int flags = 0) 
{
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

int main (void)
{
	//  Prepare our context and subscriber
	void *context = zmq_ctx_new ();
	void *subscriber = zmq_socket (context, ZMQ_SUB);
	zmq_connect (subscriber, "tcp://localhost:4242");
	zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, "topic1", 1);

	while (1) 
	{
		//  Read envelope with address
		char *address = s_recv (subscriber);
		//  Read message contents
		char *contents = s_recv (subscriber);
		printf ("[%s] %s\n", address, contents);
		free (address);
		free (contents);
	}
	//  We never get here, but clean up anyhow
	zmq_close (subscriber);
	zmq_ctx_destroy (context);
	return 0;
}

