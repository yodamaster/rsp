#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include <luajit.h>
#include <lauxlib.h>

#include "epollinterface.h"
#include "logging.h"
#include "server_socket.h"


char* get_config_opt(lua_State* L, char* name) {
    lua_getglobal(L, name);
    if (!lua_isstring(L, -1)) {
        fprintf(stderr, "%s must be a string", name);
        exit(1);
    }
    return (char*) lua_tostring(L, -1);
}


int main(int argc, char* argv[])
{
    if (argc != 2) {
        fprintf(stderr, 
                "Usage: %s <config_file>\n", 
                argv[0]);
        exit(1);
    }

    lua_State *L = lua_open();
    if (luaL_dofile(L, argv[1]) != 0) {
        fprintf(stderr, "Error parsing config file: %s\n", lua_tostring(L, -1));
        exit(1);
    }
    char* server_port_str = get_config_opt(L, "listenPort");
    char* backend_addr = get_config_opt(L, "backendAddress");
    char* backend_port_str = get_config_opt(L, "backendPort");

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Couldn't create epoll FD");
        exit(1);
    }

    create_server_socket_handler(epoll_fd, 
                                 server_port_str,
                                 backend_addr,
                                 backend_port_str);

    rsp_log("Started.  Listening on port %s.\n", server_port_str);
    do_reactor_loop(epoll_fd);

    return 0;
}

