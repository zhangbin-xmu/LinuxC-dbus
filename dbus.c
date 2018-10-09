#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>
#include "dbus.h"


////////////////////////////////////////////////////////////
// 功能：发送消息到指定进程
// 输入：发送方数据结构，接收方数据结构，消息数据结构
// 输出：
// 返回：0-成功 -1-失败
////////////////////////////////////////////////////////////
int dbus_send_signal(DBUS_APPLICATION sender, DBUS_APPLICATION receiver, DBUS_DATA data)
{
	// 1.初始化错误信息结构体
	DBusError error;
	dbus_error_init(&error);     
    
	// 2.连接到总线
	DBusConnection* connection = dbus_bus_get(DBUS_BUS_SESSION, &error);    
	if (!connection) {        
		if (dbus_error_is_set(&error)) {            
			printf("Connect Bus Error: %s\n", error.message);
			dbus_error_free(&error);
		}
		return -1;    
	}     
	
	// 3.为连接注册名称
	int ret = dbus_bus_request_name(connection, sender.bus_name, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);    
	if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {        
		if (dbus_error_is_set(&error)) {            
			printf("Connection Name Error: %s\n", error.message);
			dbus_error_free(&error);
		}			
		return -1;
	}      

	// 4.提取消息的类型以及数据
	int type;
	void* value;
	char* value_str;
	int value_int;

	switch (data.type) {
	case DBUS_DATA_TYPE_STRING:
		type = DBUS_TYPE_STRING;        
		value_str = data.value;
		value = &value_str;
		break;
	case DBUS_DATA_TYPE_INT32:
		type = DBUS_TYPE_INT32;        
		value_int = atoi(data.value);
		value = &value_int;
		break;
	default:
		printf("Error: Unknown Argument Type\n");   
		return -1;
	}    
	 
	// 5.创建D-Bus消息
	DBusMessage* message = dbus_message_new_signal(receiver.object_path, receiver.interface_name, receiver.member_name);
	if (!message) {
		printf("Error: Signal Message NULL\n");
		return -1;        
	}         

	// 6.完善D-Bus消息
	DBusMessageIter iter;	
	dbus_message_iter_init_append(message, &iter);
	if (!dbus_message_iter_append_basic(&iter, type, value)) {
		printf("Message Append Error: Out of Memory\n");  
		dbus_message_unref(message);
		return -1;        
	}         
	
	// 7.发送D-Bus消息
	dbus_uint32_t serial;
	if (!dbus_connection_send(connection, message, &serial)) {
		printf("Signal Send Error: Out of Memory\n");            
		dbus_message_unref(message);
		return -1;
	}
	dbus_connection_flush(connection);    
	dbus_message_unref(message);

	printf("Signal Sent\n");
	return 0;
}

////////////////////////////////////////////////////////////
// 功能：触发指定进程的函数调用
// 输入：发送方数据结构，接收方数据结构，消息数据结构
// 输出：
// 返回：0-成功 -1-失败
////////////////////////////////////////////////////////////
int dbus_send_method_call(DBUS_APPLICATION sender, DBUS_APPLICATION receiver, DBUS_DATA data)
{	
	// 1.初始化错误信息结构体
	DBusError error;
	dbus_error_init(&error);     
    
	// 2.连接到总线
	DBusConnection* connection = dbus_bus_get(DBUS_BUS_SESSION, &error);    
	if (!connection) {        
		if (dbus_error_is_set(&error)) {            
			printf("Connection Bus Error: %s\n", error.message);
			dbus_error_free(&error);
		}
		return -1;    
	}     
	
	// 3.为连接注册名称
	int ret = dbus_bus_request_name(connection, sender.bus_name, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);    
	if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {        
		if (dbus_error_is_set(&error)) {            
			printf("Set Connection Name Error: %s\n", error.message);
			dbus_error_free(&error);
		}			
		return -1;    
	}      

	// 4.提取消息的类型以及数据
	int type;
	void* value;
	char* value_str;
	int value_int;

	switch (data.type) {
	case DBUS_DATA_TYPE_STRING:
		type = DBUS_TYPE_STRING;
		value_str = data.value;
		value = &value_str;
		break;
	case DBUS_DATA_TYPE_INT32:
		type = DBUS_TYPE_INT32;
		value_int = atoi(data.value);
		value = &value_int;
		break;
	default:
		printf("Error: Unknown Argument Type\n");
		return -1;
	}   
	
	// 5.创建D-Bus消息
	DBusMessage* message = dbus_message_new_method_call(receiver.bus_name, receiver.object_path, receiver.interface_name, receiver.member_name);
	if (!message) {
		printf("Error: Method Call Message NULL\n");
		return -1;
	}         
	
	// 6.完善D-Bus消息
	DBusMessageIter iter;
	dbus_message_iter_init_append(message, &iter);
	if (!dbus_message_iter_append_basic(&iter, type, value)) {
		printf("Message Append Error: Out of Memory\n");
		dbus_message_unref(message);
		return -1;
	}         
	
	// 7.发送D-Bus消息并等待反馈
	DBusPendingCall* pending;
	if (!dbus_connection_send_with_reply(connection, message, &pending, DBUS_TIMEOUT_USE_DEFAULT)) {
		printf("Method Call Send Error: Out of Memory\n");
		dbus_message_unref(message);
		return -1;
	}         	
	if (!pending) {
		printf("Error: Pending Call NULL\n");
		dbus_message_unref(message);
		return -1;
	}         
	dbus_connection_flush(connection);
	dbus_message_unref(message);
	
	// 8.阻塞等待，提取反馈
	dbus_pending_call_block(pending);              
	message = dbus_pending_call_steal_reply(pending);
	dbus_pending_call_unref(pending);
	if (!message) {
		printf("Error: Reply Null\n");
		return -1;
	}         
    
	// 9.提取并解析反馈消息的数据
	if (!dbus_message_iter_init(message, &iter)) {
		printf("Error: Message Has No Argument\n");
		dbus_message_unref(message);
		return -1;
	}         
	
	pid_t pid = getpid();

	do {    
		ret = dbus_message_iter_get_arg_type(&iter);
		switch (ret) {
		case DBUS_TYPE_STRING:	
			dbus_message_iter_get_basic(&iter, &value_str);
			printf("[%d] Got Method Return STRING: %s\n", pid, value_str);
			break;
		case DBUS_TYPE_INT32:		
			dbus_message_iter_get_basic(&iter, &value_int);
			printf("[%d] Got Method Return INT32: %d\n", pid, value_int);
			break;
		default:
			printf("Error: Unkown Argument Type\n");
			break;
		}
	} while (dbus_message_iter_next(&iter));
	
	dbus_message_unref(message);
	return 0;
}

////////////////////////////////////////////////////////////
// 功能：远程函数调用反馈
// 输入：D-Bus连接，D-Bus消息
// 输出：
// 返回：0-成功 -1-失败
////////////////////////////////////////////////////////////
int dbus_reply_method_call(DBusConnection* connection, DBusMessage* message)
{
	// 1.创建用于反馈的D-Bus消息
	DBusMessage* reply = dbus_message_new_method_return(message);
	if (!reply) {
		printf("Error: Out of Memory\n");
		return -1;
	}

	// 2.提取请求消息的数据
	DBusMessageIter message_iter;
	if (!dbus_message_iter_init(message, &message_iter)) {
		printf("Error: Message Has No Argument\n");
		return -1;
	}

	// 3.解析、处理请求消息数据，并反馈处理结果	
	DBusMessageIter reply_iter;
	dbus_message_iter_init_append(reply, &reply_iter);

	pid_t pid = getpid();
	void* value;
	char* value_str;
	int value_int;

	do {
		int ret = dbus_message_iter_get_arg_type(&message_iter);
		switch (ret) {
		case DBUS_TYPE_STRING:
			dbus_message_iter_get_basic(&message_iter, &value_str);
			printf("[%d] Got Method Call Argument STRING: %s\n", pid, value_str);

			// 数据处理
			// ......

			value = &value_str;
			if (!dbus_message_iter_append_basic(&reply_iter, DBUS_TYPE_STRING, value)) {
				printf("Error: Out of Memory\n");
				dbus_message_unref(reply);
				return -1;
			}
			break;
		case DBUS_TYPE_INT32:		
			dbus_message_iter_get_basic(&message_iter, &value_int);
			printf("[%d] Got Method Call Argument INT32: %d\n", pid, value_int);

			// 数据处理
			// ......

			value = &value_int;
			if (!dbus_message_iter_append_basic(&reply_iter, DBUS_TYPE_INT32, value)) {
				printf("Error: Out of Memory\n");
				dbus_message_unref(reply);
				return -1;
			}
			break;
		default:
			printf("Error: Unknown Argument Type\n");
			break;
		}

	} while (dbus_message_iter_next(&message_iter));

	// 5.发送反馈消息
	dbus_uint32_t serial;
	if (!dbus_connection_send(connection, reply, &serial)) {
		printf("Error: Out of Memory\n");
		dbus_message_unref(reply);
		return -1;
	}
	dbus_connection_flush(connection);
	dbus_message_unref(reply);

	return 0;
}

////////////////////////////////////////////////////////////
// 功能：循环接收消息
// 输入：接收方（自身）数据结构
// 输出：
// 返回：0-成功 -1-失败
////////////////////////////////////////////////////////////
int dbus_receive(DBUS_APPLICATION self)
{
	// 1.初始化错误信息结构体
	DBusError error;
	dbus_error_init(&error);

	// 2.连接到总线
	DBusConnection* connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
	if (!connection) {
		if (dbus_error_is_set(&error)) {
			printf("Connect Bus Error: %s\n", error.message);
			dbus_error_free(&error);
		}
		return -1;
	}

	// 3.为连接注册名称
	int ret = dbus_bus_request_name(connection, self.bus_name, DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
	if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
		if (dbus_error_is_set(&error)) {
			printf("Connection Name Error: %s\n", error.message);
			dbus_error_free(&error);
		}
		return -1;
	}

	// 4.添加消息格式筛选
	char rule[128];
	snprintf(rule, sizeof(rule), DBUS_SIGNAL_RULE, self.interface_name);
	dbus_bus_add_match(connection, rule, &error);
	if (dbus_error_is_set(&error)) {
		printf("Match Error: %s\n", error.message);
		dbus_error_free(&error);
		return -1;
	}
	dbus_connection_flush(connection);

	// 5.进入消息接收循环
	DBusMessage* message;
	DBusMessageIter iter;
	const char* path;
	char* value_str;
	int value_int;

	while (1) {
		// 5.1.读取消息队列首条数据
		dbus_connection_read_write(connection, 0);
		message = dbus_connection_pop_message(connection);
		if (!message) {
			sleep(1);
			continue;
		}

		// 5.2.比对消息目标地址
		path = dbus_message_get_path(message);
		if (strcmp(path, self.object_path)) {
			dbus_message_unref(message);
			continue;
		}

		// 5.3.消息数据处理
		pid_t pid = getpid();
		do {
			if (dbus_message_is_signal(message, self.interface_name, DBUS_MEMBER_SIGNAL)) {

				if (!dbus_message_iter_init(message, &iter)) {
					printf("Error: Message Has No Argument\n");
					break;
				}

				ret = dbus_message_iter_get_arg_type(&iter);
				switch (ret) {
				case DBUS_TYPE_STRING:
					dbus_message_iter_get_basic(&iter, &value_str);
					printf("[%d] Got Signal With STRING: %s\n", pid, value_str);
					break;
				case DBUS_TYPE_INT32:
					dbus_message_iter_get_basic(&iter, &value_int);
					printf("[%d] Got Signal With INT32: %d\n", pid, value_int);
					break;
				default:
					printf("Error: Unkown Argument Type\n");
					break;
				}
			}
			else if (dbus_message_is_method_call(message, self.interface_name, DBUS_MEMBER_METHOD)) {
				dbus_reply_method_call(connection, message);
			}
			else {
				printf("Error: Unkown Message Type\n");
			}
		} while (0);
		
		dbus_message_unref(message);
	}

	return 0;
}
