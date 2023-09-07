#include <iostream>
#include <enet/enet.h>

int main()
{
	
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "Error occurred while initializing\n");
		return EXIT_FAILURE;
	}
	atexit(enet_deinitialize);

	ENetHost* client;
	client = enet_host_create(NULL, 1, 1, 0, 0);

	if (client == NULL)
	{
		fprintf(stderr, "Error occurred while trying to create client host.\n");
		exit(EXIT_FAILURE);
	}

	ENetAddress address;
	ENetEvent event;
	ENetPeer* peer;

	enet_address_set_host(&address, "193.167.124.3");
	address.port = 1234;
	peer = enet_host_connect(client, &address, 1, 0);
	if (peer == NULL)
	{
		fprintf(stderr, "no available peers for initiating ENet connection./n");
		return EXIT_FAILURE;
	}

	if (enet_host_service(client, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		puts("Connection to 193.167.124.3:1234 succeeded.");
	}
	else
	{
		enet_peer_reset(peer);
		puts("Connection to 193.167.124.3:1234 failed.");
		return EXIT_SUCCESS;
	}

	//LOOP
	while (enet_host_service(client, &event, 1000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			printf("A packet of length %u containing %s was received from %s on channel %u.\n",
				event.packet->dataLength,
				event.packet->data,
				event.peer->address.host,
				event.peer->address.port,
				event.channelID);
			break;
		}
	}
	//END LOOP

	enet_peer_disconnect(peer, 0);

	while (enet_host_service(client, &event, 3000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			puts("disconnection succeeded");
			break;
		}
	}
	
	return EXIT_SUCCESS;
}