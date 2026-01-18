#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define PORT 8080

int main() {
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    listen(server_fd, 10);

    printf("[*] >>> Server running on http://localhost:8080\n");

    while (1) {
        int client = accept(server_fd, NULL, NULL);
        if (client < 0) continue;

        char buf[2048];
        int n = read(client, buf, sizeof(buf)-1);
        if (n <= 0) {
            close(client);
            continue;
        }
        buf[n] = 0;

        char method[8], path[1024];
        sscanf(buf, "%s %s", method, path);

        if (luaL_dofile(L, "app/routes.lua") != LUA_OK) {
            printf("Lua error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            close(client);
            continue;
        }

        lua_getglobal(L, "handle");
        lua_pushstring(L, path);

        if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
            printf("Lua runtime error: %s\n", lua_tostring(L, -1));
            lua_pop(L, 1);
            close(client);
            continue;
        }

        const char *body = lua_tostring(L, -1);
        if (!body) body = "<h1>500</h1>";

        char response[8192];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(body), body
        );

        write(client, response, strlen(response));

        lua_pop(L, 1);
        close(client);
    }

    lua_close(L);
    close(server_fd);
    return 0;
}

