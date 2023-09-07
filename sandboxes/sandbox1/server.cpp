//#include <iostream>
//#include <enet/enet.h>
//
//int main()
//{
//	if (enet_initialize() != 0)
//	{
//		fprintf(stderr, "Error occurred while initializing\n");
//		return EXIT_FAILURE;
//	}
//	atexit(enet_deinitialize);
//
//	ENetAddress address;
//	ENetHost* server;
//	ENetEvent event;
//
//	address.host = ENET_HOST_ANY;
//	address.port = 1234;
//
//	server = enet_host_create(&address, 20, 1, 0, 0);
//
//	if (server == NULL)
//	{
//		fprintf(stderr, "error while creating ENet server host.\n");
//		return EXIT_FAILURE;
//	}
//
//	//LOOP
//	while (true)
//	{
//
//		while (enet_host_service(server, &event, 1000) > 0)
//		{
//			switch (event.type)
//			{
//			case ENET_EVENT_TYPE_CONNECT:
//				printf("new client connected from %x:%u. \n",
//					event.peer -> address.host,
//					event.peer -> address.port);
//				break;
//
//			case ENET_EVENT_TYPE_RECEIVE:
//				printf("A packet of length %u containing %s was received from %s on channel %u.\n",
//					event.packet->dataLength,
//					event.packet->data,
//					event.peer->address.host,
//					event.peer->address.port,
//					event.channelID);
//				break;
//
//			case ENET_EVENT_TYPE_DISCONNECT:
//				printf("%x:%u disconnected. \n",
//					event.peer->address.host,
//					event.peer->address.port);
//				break;
//			}
//		}
//	}
//	//END LOOP
//
//	enet_host_destroy(server);
//
//	return EXIT_SUCCESS;
//
//}