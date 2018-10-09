CFLAGS += -I/usr/local/include/dbus-1.0/
CFLAGS += -I/usr/local/lib/dbus-1.0/include/
LDFLAGS += -ldbus-1


SRCS := main.c dbus.c
	
	
OBJS := $(SRCS:%.c=%.o)
	
	
demo : $(OBJS) 
	$(CC) -o demo $(OBJS) $(LDFLAGS)


.PHONY : clean
clean :
	-rm $(OBJS) demo