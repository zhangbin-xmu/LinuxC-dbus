#include <stdio.h>
#include <string.h>
#include "dbus.h"


#define DBUS_SENDER_BUS_NAME        "com.dbus.sender_app"
#define DBUS_RECEIVER_BUS_NAME      "com.dbus.receiver_app"
#define DBUS_RECEIVER_PATH          "/com/dbus/object"
#define DBUS_RECEIVER_INTERFACE     "com.dbus.interface"


static void usage() 
{ 
	printf("Usage: ./demo [OPTIONS] [PARAMETERS]\n");
	printf("\treceive\n");
	printf("\t\t-- listen, wait a signal or a method call\n");
	printf("\t\t-- ./demo receive\n");
	printf("\n");
	printf("\tsend [mode] [type] [value]\n");
	printf("\t\t-- send a signal or call a method\n");
	printf("\t\t-- mode:  SIGNAL | METHOD\n");
	printf("\t\t-- type:  STRING | INT32\n");
	printf("\t-- value: string or number\n");
	printf("\n");
	printf("\t\t-- ./demo send SIGNAL STRING hello\n");
	printf("\t\t-- ./demo send METHOD INT32 99\n");
	printf("\n");
}

void main(int argc, char *argv[])
{
	if (argc < 2) {
		usage();
		return;
	}

	if (!strcmp(argv[1], "receive")) {

		DBUS_APPLICATION self;
		self.bus_name = DBUS_RECEIVER_BUS_NAME;
		self.object_path = DBUS_RECEIVER_PATH;
		self.interface_name = DBUS_RECEIVER_INTERFACE;
		dbus_receive(self);
	}
	else if (!strcmp(argv[1], "send")) {

		if (argc < 5) {
			usage();
			return;
		}

		DBUS_APPLICATION sender;
		sender.bus_name = DBUS_SENDER_BUS_NAME;

		DBUS_APPLICATION receiver;
		receiver.bus_name = DBUS_RECEIVER_BUS_NAME;
		receiver.object_path = DBUS_RECEIVER_PATH;
		receiver.interface_name = DBUS_RECEIVER_INTERFACE;
		
		DBUS_DATA data;
		if (!strcasecmp(argv[3], "STRING")) {
			data.type = DBUS_DATA_TYPE_STRING;
		}
		else if (!strcasecmp(argv[3], "INT32")) {
			data.type = DBUS_DATA_TYPE_INT32;
		}
		else {
			usage();
			return;
		}
		data.value = argv[4];

		if (!strcasecmp(argv[2], "SIGNAL")) {
			receiver.member_name = DBUS_MEMBER_SIGNAL;
			dbus_send_signal(sender, receiver, data);
		}
		else if (!strcasecmp(argv[2], "METHOD")) {
			receiver.member_name = DBUS_MEMBER_METHOD;
			dbus_send_method_call(sender, receiver, data);
		}
		else {
			usage();
		}
	}
	else {
		usage();
	}

	return;
}
