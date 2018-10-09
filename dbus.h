#ifndef DBUS_H_
#define DBUS_H_


#define DBUS_MEMBER_SIGNAL		"signal"
#define DBUS_MEMBER_METHOD		"method"
#define DBUS_SIGNAL_RULE		"type='signal',interface='%s'"


////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////
typedef enum _DBUS_DATA_TYPE
{
	DBUS_DATA_TYPE_STRING,
	DBUS_DATA_TYPE_INT32

}DBUS_DATA_TYPE;

////////////////////////////////////////////////////////////
// D-Bus应用数据结构
////////////////////////////////////////////////////////////
typedef struct _DBUS_APPLICATION
{
	char* bus_name;
	char* object_path;
	char* interface_name;
	char* member_name;

}DBUS_APPLICATION;

////////////////////////////////////////////////////////////
// D-Bus数据数据结构
////////////////////////////////////////////////////////////
typedef struct _DBUS_DATA
{
	DBUS_DATA_TYPE type;
	char* value;
 
}DBUS_DATA;


int dbus_send_signal(DBUS_APPLICATION sender, DBUS_APPLICATION receiver, DBUS_DATA data);
int dbus_send_method_call(DBUS_APPLICATION sender, DBUS_APPLICATION receiver, DBUS_DATA data);
int dbus_receive(DBUS_APPLICATION self);


////////////////////////////////////////////////////////////
// D-BUS API REFERENCE
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		DBusConnection* dbus_bus_get(DBusBusType type, DBusError* error)
 * [Parameters]
 *		(1) type:		bus type
 *		(2) error:		address where an error can be returned
 * [Description]
 *		(1) Connects to a bus daemon and registers the client with it.
 *		(2) If a connection to the bus already exists, then that connection is returned. 
 *			The caller of this function owns a reference to the bus.
 *		(3) The caller may NOT call dbus_connection_close() on this connection; see dbus_connection_open() and dbus_connection_close() for details on that.
 *		(4) If this function obtains a new connection object never before returned from dbus_bus_get(), 
 *			it will call dbus_connection_set_exit_on_disconnect(), so the application will exit if the connection closes. 
 *			You can undo this by calling dbus_connection_set_exit_on_disconnect() yourself after you get the connection.
 *		(5) dbus_bus_get() calls dbus_bus_register() for you.
 *		(6) If returning a newly-created connection, this function will block until authentication and bus registration are complete.
 * [Returns]
 *		A DBusConnection with new ref or NULL on error.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		int dbus_bus_request_name(DBusConnection* connection, const char* name, unsigned int flags, DBusError* error)
 * [Parameters]
 *		(1) connection:	the connection
 *		(2) name:		the name to request
 *		(3) flags:		flags
 *		(4) error:		location to store the error
 * [Description]
 *		Asks the bus to assign the given name to this connection by invoking the RequestName method on the bus.
 * [Returns]
 *		A result code, -1 if error is set.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		void dbus_bus_add_match(DBusConnection* connection, const char* rule, DBusError* error)
 * [Parameters]
 *		(1) connection:	connection to the message bus
 *		(2)	rule:		textual form of match rule
 *		(3)	error:		location to store any errors
 * [Description]
 *		(1) Adds a match rule to match messages going through the message bus.
 *		(2) The "rule" argument is the string form of a match rule.
*/
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/**
 * [Function]
 *		DBusMessage* dbus_message_new_signal(const char* path, const char* iface, const char* name)
 * [Parameters]
 *		(1) path:		the path to the object emitting the signal
 *		(2) iface:		the interface the signal is emitted from
 *		(3) name:		name of the signal
 * [Description]
 *		(1) Constructs a new message representing a signal emission.
 *		(2) Returns NULL if memory can't be allocated for the message.
 *		(3) A signal is identified by its originating object path, interface, and the name of the signal.
 *		(4) Path, interface, and signal name must all be valid (the D-Bus specification defines the syntax of these fields).
 * [Returns]
 *		A new DBusMessage, free with dbus_message_unref().
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		DBusMessage* dbus_message_new_method_call(const char* destination, const char* path, const char* iface, const char* method)
 * [Parameters]
 *		(1) destination:name that the message should be sent to or NULL
 *		(2) path:		object path the message should be sent to
 *		(3) iface:		interface to invoke method on, or NULL
 *		(4)	method:		method to invoke
 * [Description]
 *		(1) Constructs a new message to invoke a method on a remote object.
 *		(2) Returns NULL if memory can't be allocated for the message. 
 *			The destination may be NULL in which case no destination is set; this is appropriate when using D-Bus in a peer-to-peer context (no message bus). 
 *		(3) The interface may be NULL, which means that if multiple methods with the given name exist it is undefined which one will be invoked.
 *		(4) The path and method names may not be NULL.
 *		(5) Destination, path, interface, and method name can't contain any invalid characters (see the D-Bus specification).
 * [Returns]
		A new DBusMessage, free with dbus_message_unref().
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		DBusMessage* dbus_message_new_method_return(DBusMessage* method_call)
 * [Parameters]
 *		(1) method_call:the message being replied to
 * [Description]
 *		(1) Constructs a message that is a reply to a method call.
 *		(2) Returns NULL if memory can't be allocated for the message.
 * [Returns]
 *		A new DBusMessage, free with dbus_message_unref().
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		void dbus_message_unref(DBusMessage* message)
 * [Parameters]
 *		(1) message:	the message
 * [Description]
 *		Decrements the reference count of a DBusMessage, freeing the message if the count reaches 0.
*/
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/**
 * [Function]
 *		const char* dbus_message_get_path(DBusMessage* message)
 * [Parameters]
 *		(1) message:	the message
 * [Description]
 *		(1) Gets the object path this message is being sent to (for DBUS_MESSAGE_TYPE_METHOD_CALL) 
 *			or being emitted from (for DBUS_MESSAGE_TYPE_SIGNAL).
 *		(2) Returns NULL if none.
 *		(3) The returned string becomes invalid if the message is modified, 
 *			since it points into the wire-marshaled message data.
 * [Returns]
 *		The path (should not be freed) or NULL.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		dbus_bool_t dbus_message_is_signal(DBusMessage* message, const char* iface, const char* signal_name)
 * [Parameters]
 *		(1) message:	the message
 *		(2) iface:		the name to check (must not be NULL)
 *		(3) signal_name:the name to check (must not be NULL)
 * [Description]
 *		(1) Checks whether the message is a signal with the given interface and member fields.
 *		(2) If the message is not DBUS_MESSAGE_TYPE_SIGNAL, or has a different interface or member field, returns FALSE.
 * [Returns]
 *		TRUE if the message is the specified signal.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		dbus_bool_t dbus_message_is_method_call(DBusMessage* message, const char* iface, const char* method)
 * [Parameters]
 *		(1) message:	the message
 *		(2) iface:		the name to check (must not be NULL)
 *		(3)	method:		the name to check (must not be NULL)
 * [Description]
 *		(1) Checks whether the message is a method call with the given interface and member fields.
 *		(2) If the message is not DBUS_MESSAGE_TYPE_METHOD_CALL, or has a different interface or member field, returns FALSE. 
 *			If the interface field is missing, then it will be assumed equal to the provided interface. 
 *			The D-Bus protocol allows method callers to leave out the interface name.
 * [Returns]
 *		TRUE if the message is the specified method call.
*/
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/**
 * [Function]
 *		dbus_bool_t dbus_message_iter_init(DBusMessage* message, DBusMessageIter* iter) 
 * [Parameters]
 *		(1) message:	the message
 *		(2) iter:		pointer to an iterator to initialize
 * [Description]
 *		(1) Initializes a DBusMessageIter for reading the arguments of the message passed in.
 *		(2) When possible, dbus_message_get_args() is much more convenient. 
 *			Some types of argument can only be read with DBusMessageIter however.
 * [Returns]
 *		FALSE if the message has no arguments.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		void dbus_message_iter_init_append(DBusMessage* message, DBusMessageIter* iter)
 * [Parameters]
 *		(1) message:	the message
 *		(2) iter:		pointer to an iterator to initialize
 * [Description]
 *		(1) Initializes a DBusMessageIter for appending arguments to the end of a message.
 *		(2) If appending any of the arguments fails due to lack of memory,
 *			the message is hosed and you have to start over building the whole message.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		dbus_bool_t dbus_message_iter_append_basic(DBusMessageIter* iter, int type, const void* value)
 * [Parameters]
 *		(1) iter:		the append iterator
 *		(2) type:		the type of the value
 *		(3) value:		the address of the value
 * [Description]
 *		(1) Appends a basic-typed value to the message.
 *		(2) The basic types are the non-container types such as integer and string.
 *		(3) The "value" argument should be the address of a basic-typed value. 
 *			So for string, const char**. For integer, dbus_int32_t*.
 *		(4) For Unix file descriptors this function will internally duplicate the descriptor you passed in. 
 *			Hence you may close the descriptor immediately after this call.
 * [Returns]
 *		FALSE if not enough memory.
 */
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		int dbus_message_iter_get_arg_type(DBusMessageIter* iter)
 * [Parameters]
 *		(1) iter:		the message iter
 * [Description]
 *		(1) Returns the argument type of the argument that the message iterator points to.
 *		(2) If the iterator is at the end of the message, returns DBUS_TYPE_INVALID. 
 * [Returns]
 *		The argument type.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		dbus_bool_t dbus_message_iter_next(DBusMessageIter* iter)	
 * [Parameters
 *		(1) iter:		the message iter
 * [Description]
 *		(1) Moves the iterator to the next field, if any.
 *		(2) If there's no next field, returns FALSE. If the iterator moves forward, returns TRUE.
 * [Returns]
 *		TRUE if the iterator was moved to the next field.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		void dbus_message_iter_get_basic(DBusMessageIter* iter, void* value)
 * [Parameters]
 *		(1) iter:		the iterator
 *		(2) value:		location to store the value
 * [Description]
 *		(1) Reads a basic-typed value from the message iterator.
 *		(2) Basic types are the non-containers such as integer and string.
 *		(3) The value argument should be the address of a location to store the returned value. 
 *			So for int32 it should be a "dbus_int32_t*" and for string a "const char**". 
 *			The returned value is by reference and should not be freed.
 *		(4) This call duplicates Unix file descriptors when reading them. 
 *			It is your job to close them when you don't need them anymore.
 *		(5) Unix file descriptors that are read with this function will have the FD_CLOEXEC flag set. 
 *			If you need them without this flag set, make sure to unset it with fcntl().
 *		(6) Be sure you have somehow checked that dbus_message_iter_get_arg_type() matches the type you are expecting, 
 *			or you'll crash when you try to use an integer as a string or something.
 *		(7) To read any container type (array, struct, dict) you will need to recurse into the container with dbus_message_iter_recurse(). 
 *			If the container is an array of fixed-length values (except Unix file descriptors), you can get all the array elements at once with dbus_message_iter_get_fixed_array(). 
 *			Otherwise, you have to iterate over the container's contents one value at a time.
*/
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/**
 * [Function]
 *		dbus_bool_t dbus_connection_send(DBusConnection* connection, DBusMessage* message, dbus_uint32_t* serial)
 * [Parameters]
 *		(1) connection:	the connection.
 *		(2) message:	the message to write.
 *		(3) serial:		return location for message serial, or NULL if you don't care.
 * [Description]
 *		(1) Adds a message to the outgoing message queue.
 *		(2) Does not block to write the message to the network; that happens asynchronously. 
 *			To force the message to be written, call dbus_connection_flush() however it is not necessary to call dbus_connection_flush() by hand; 
 *			the message will be sent the next time the main loop is run. 
 *			dbus_connection_flush() should only be used, for example, if the application was expected to exit before running the main loop.
 *		(3) Because this only queues the message, the only reason it can fail is lack of memory. 
 *			Even if the connection is disconnected, no error will be returned. If the function fails due to lack of memory, it returns FALSE. 
 *			The function will never fail for other reasons; even if the connection is disconnected, you can queue an outgoing message, though obviously it won't be sent.
 *		(4) The message serial is used by the remote application to send a reply.
 *		(5) dbus_message_unref() can be called as soon as this method returns as the message queue will hold its own ref until the message is sent.
 * [Returns]
 *		TRUE on success.
 *
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		dbus_bool_t dbus_connection_send_with_reply(DBusConnection* connection, DBusMessage* message, DBusPendingCall** pending_return, int timeout_milliseconds)
 * [Parameters]
 *		(1) connection:	the connection
 *		(2) message:	the message to send
 *		(3) pending_return:	
 *						return location for a DBusPendingCall object, or NULL if connection is disconnected 
 *						or when you try to send Unix file descriptors on a connection that does not support them.
 *		(4) timeout_milliseconds:
 *						timeout in milliseconds, -1 (or DBUS_TIMEOUT_USE_DEFAULT) for default or DBUS_TIMEOUT_INFINITE for no timeout
 * [Description]
 *		(1) Queues a message to send, as with dbus_connection_send(), 
 *			but also returns a DBusPendingCall used to receive a reply to the message.
 *		(2) If no reply is received in the given timeout_milliseconds, 
 *			this function expires the pending reply and generates a synthetic error reply 
 *			(generated in-process, not by the remote application) indicating that a timeout occurred.
 *		(3) A DBusPendingCall will see a reply message before any filters or registered object path handlers.
 *		(4) A DBusPendingCall will always see exactly one reply message, unless it's cancelled with dbus_pending_call_cancel().
 *		(5) If NULL is passed for the pending_return, the DBusPendingCall will still be generated internally,
 *			and used to track the message reply timeout. 
 *			This means a timeout error will occur if no reply arrives, unlike with dbus_connection_send().
 *		(6) If -1 is passed for the timeout, a sane default timeout is used. -1 is typically the best value for the timeout for this reason, 
 *			unless you want a very short or very long timeout. If DBUS_TIMEOUT_INFINITE is passed for the timeout, no timeout will be set and the call will block forever.
 * [Returns]
 *		FALSE if no memory, TRUE otherwise.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		void dbus_connection_flush(DBusConnection* connection)
 * [Parameters]
 *		(1) connection:	the connection.
 * [Description]
 *		Blocks until the outgoing message queue is empty.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		dbus_bool_t dbus_connection_read_write(DBusConnection* connection, int timeout_milliseconds)
 * [Parameters]
 *		(1) connection:	the connection
 *		(2) timeout_milliseconds:
 *						max time to block or -1 for infinite
 * [Description]
 *		(1)	This function is intended for use with applications that don't want to write a main loop and deal with DBusWatch and DBusTimeout.
 *		(2) As long as the connection is open, this function will block until it can read or write, 
 *			then read or write, then return TRUE.If the connection is closed, the function returns FALSE.
 *		(3) The return value indicates whether reading or writing is still possible, 
 *			i.e. whether the connection is connected.
 *		(4) Note that even after disconnection, messages may remain in the incoming queue that need to be processed. 
 *			dbus_connection_read_write_dispatch() dispatches incoming messages for you; 
 *			with dbus_connection_read_write() you have to arrange to drain the incoming queue yourself.
 * [Returns]
 *		TRUE if still connected.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		DBusMessage* dbus_connection_pop_message(DBusConnection* connection)
 * [Parameters]
 *		(1) connection:	the connection
 * [Description]
 *		(1) Returns the first-received message from the incoming message queue, removing it from the queue.
 *		(2) The caller owns a reference to the returned message. If the queue is empty, returns NULL.
 * [Returns]
 *		Next message in the incoming queue.
*/
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/**
 * [Function]
 *		void dbus_pending_call_block(DBusPendingCall* pending)
 * [Parameters]
 *		(1) pending:	the pending call
 * [Description]
 *		(1) Block until the pending call is completed.
 *		(2) The blocking is as with dbus_connection_send_with_reply_and_block(); 
 *			it does not enter the main loop or process other messages, it simply waits for the reply in question.
 *		(3) If the pending call is already completed, this function returns immediately.
 *		(4) When you start blocking, the timeout is reset, but it should really only use time remaining since the pending call was created. 
 *			This requires storing timestamps instead of intervals in the timeout.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		DBusMessage* dbus_pending_call_steal_reply(DBusPendingCall* pending)
 * [Parameters]
 *		pending:	the pending call
 * [Description]
 *		(1) Gets the reply, or returns NULL if none has been received yet.
 *		(2) Ownership of the reply message passes to the caller. 
 *		(3) This function can only be called once per pending call, since the reply message is tranferred to the caller.
 * [Returns]
 *		The reply message or NULL.
*/
////////////////////////////////////////////////////////////
/**
 * [Function]
 *		void dbus_pending_call_unref(DBusPendingCall* pending)
 * [Parameters]
 *		(1) pending:	the pending call object
 * [Description]
 *		Decrements the reference count on a pending call, freeing it if the count reaches 0.
*/
////////////////////////////////////////////////////////////


#endif // !DBUS_H_
