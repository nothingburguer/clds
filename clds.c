#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void create_dirs(const char *path) {
    char tmp[512];
    snprintf(tmp, sizeof(tmp), "%s", path);

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
    mkdir(tmp, 0755);
}

void write_file(const char *path, const char *content) {
    FILE *f = fopen(path, "w");
    if (!f) {
        perror("fopen");
        exit(1);
    }
    fputs(content, f);
    fclose(f);
}

const char *default_routes =
"routes = {\n"
"  [\"/\"] = \"index.html\",\n"
"  [\"/hello\"] = \"hello.html\"\n"
"}\n"
"\n"
"function handle(path)\n"
"  local view = routes[path]\n"
"  if view then\n"
"    local f = io.open(\"app/views/\" .. view)\n"
"    local html = f:read(\"*a\")\n"
"    f:close()\n"
"    return html\n"
"  end\n"
"  return \"<h1>404</h1>\"\n"
"end\n";

const char *index_html =
"<h1>Welcome</h1>\n"
"<p>This is your clds project</p>\n";

const char *hello_html =
"<h1>Howdy</h1>\n";

void create_project(const char *name) {
    char path[512];

    snprintf(path, sizeof(path), "%s/app/views", name);
    create_dirs(path);

    snprintf(path, sizeof(path), "%s/app/routes.lua", name);
    write_file(path, default_routes);

    snprintf(path, sizeof(path), "%s/app/views/index.html", name);
    write_file(path, index_html);

    snprintf(path, sizeof(path), "%s/app/views/hello.html", name);
    write_file(path, hello_html);

    printf("Project '%s' created\n", name);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: zlds <new|run>\n");
        return 0;
    }

    if (strcmp(argv[1], "new") == 0) {
        if (argc < 3) return 0;
        create_project(argv[2]);
    }
    else if (strcmp(argv[1], "run") == 0) {
        system("./server");
    }

    return 0;
}

