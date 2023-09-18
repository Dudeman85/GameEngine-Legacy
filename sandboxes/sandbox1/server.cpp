#include <iostream>
#include <enet/enet.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "Error occurred while initializing\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetAddress address;
	ENetHost* server;
	ENetEvent event;


	address.host = ENET_HOST_ANY;
	address.port = 2315;

	server = enet_host_create(&address, 20, 1, 0, 0);

	if (server == NULL)
	{
		fprintf(stderr, "error while creating ENet server host.\n");
		return EXIT_FAILURE;
	}

	//LOOP
	while (true)
	{

		while (enet_host_service(server, &event, 1000) > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				printf("new client connected. \n",
					event.peer->address.host);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf("(Server) Message from client : %s\n", event.packet->data);
				// Lets broadcast this message to all
				enet_host_broadcast(server, 0, event.packet);
				break;


			case ENET_EVENT_TYPE_DISCONNECT:
				printf("client disconnected. \n",
					event.peer->address.host);
				break;
			}
		}
	}
	//END LOOP

	enet_host_destroy(server);

	return EXIT_SUCCESS;

}