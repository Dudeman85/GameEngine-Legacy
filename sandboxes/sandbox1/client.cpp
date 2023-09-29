#include <iostream>
#include <enet/enet.h>
#include <stdio.h>
#include <string.h>


void SendPacket(ENetPeer* peer, const char* data)
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);
}


int main(int argc, char** argv)
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
	

	//in command promt, use "ipconfig" to find out ip address
	enet_address_set_host(&address, "172.31.18.64");
	address.port = 2310;
	peer = enet_host_connect(client, &address, 1, 0);
	if (peer == NULL)
	{
		fprintf(stderr, "no available peers for initiating ENet connection./n");
		return EXIT_FAILURE;
	}

	if (enet_host_service(client, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		puts("Connection to host succeeded.");

	}
	else
	{
		enet_peer_reset(peer);
		puts("Connection to host failed.");
		return EXIT_SUCCESS;
	}

	SendPacket(peer, "test_data");
	 //Create a reliable packet of size 7 containing "packet\0" 
	/*ENetPacket * packet = enet_packet_create("packet", strlen("packet") + 1, ENET_PACKET_FLAG_RELIABLE);
	
	enet_peer_send(peer, 0, packet);*/
	
	//LOOP
	while (enet_host_service(client, &event, 1000) > 0)
	{
		
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			printf("(Client) Message from server : %s\n", event.packet->data);
			
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			puts("disconnection succeeded");
			break;
		}
	}
	//END LOOP

	

	enet_peer_disconnect(peer, 0);
	return EXIT_SUCCESS;
}