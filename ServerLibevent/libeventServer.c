/* Эхо сервер TCP реализованный при помощи библиотеки libevent */

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static void echo_event_cb(struct bufferevent* bev, short events, void* ctx);
static void accept_conn_cb(struct evconnlistener* listener, 
	evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
static void accept_error_cb(struct evconnlistener* listener, 
	evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
static void echo_read_cb(struct bufferevent* bex, void* ctx);	

int main(int argc, char const *argv[])
{
	struct event_base* base = event_base_new();

	struct sockaddr_in server;
	memset(&server, '\0', sizeof(server));

	server.sin_family = PF_INET;
	server.sin_port = htons(12345);
	server.sin_addr.s_addr = htons(INADDR_ANY);

	struct evconnlistener* listener = evconnlistener_new_bind(base, 
		accept_conn_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
		-1, (struct sockaddr*)&server, sizeof(server));

	evconnlistener_set_error_cb(listener, accept_error_cb);

	event_base_dispatch(base);
	return 0;
}

static void accept_conn_cb(struct evconnlistener* listener, 
	evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx)
{
	struct event_base * base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd,
			BEV_OPT_CLOSE_ON_FREE);

	bufferevent_setcb(bev, echo_read_cb, NULL, echo_event_cb, NULL);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

static void accept_error_cb(struct evconnlistener* listener, 
	evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx)
{
	struct event_base * base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	fprintf(stderr, "Error = %d = %s\n", err, evutil_socket_error_to_string(err));
	event_base_loopexit(base, NULL);		
}

static void echo_read_cb(struct bufferevent* bev, void* ctx)
{
	struct evbuffer* input = bufferevent_get_input(bev);
	struct evbuffer* output = bufferevent_get_output(bev);

	size_t lenght = evbuffer_get_length(input);
	char* data = malloc(lenght);

	printf("data: %s\n", data);

	evbuffer_add_buffer(output, input);
	free(data);
}

static void echo_event_cb(struct bufferevent* bev, short events, void* ctx)
{
	if(events & BEV_EVENT_ERROR)
	{
		perror("Error");
		bufferevent_free(bev);
	}
	if(events & BEV_EVENT_EOF)
		bufferevent_free(bev);
}
