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

void copy_binary(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");
    if (!in) {
        perror("[CLDS] == ERROR: fopen src (clds-server not found)");
        exit(1);
    }

    FILE *out = fopen(dst, "wb");
    if (!out) {
        perror("[CLDS] == ERROR: fopen dst (non-null, exist)");
        exit(1);
    }

    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        fwrite(buf, 1, n, out);
    }

    fclose(in);
    fclose(out);

    chmod(dst, 0755);
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

    char server_dst[512];
    snprintf(server_dst, sizeof(server_dst), "%s/clds-server", name);

    copy_binary("/usr/local/bin/clds-server", server_dst);

    printf("[CLDS] == Project '%s'/ created\n", name);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf(
            "clds - C + Lua Dynamic Server\n"
            "\nA lightweight web server written in C\n"
            "with dynamic routing support via Lua\n\n"
            "Usage:\n"
            "\tclds new <project-name>   Create a new project\n"
            "\tclds run                  Run the current project\n\n"
            "Examples:\n"
            "\tclds new myapp\n"
            "\tcd myapp && clds run\n"
            "\nCopyright (c) 2026 grassleaff\n"
            "This program is distributed under the MIT License\n"
        );
        return 0;
    }

    if (strcmp(argv[1], "new") == 0) {
        if (argc < 3) return 0;
        create_project(argv[2]);
    }
    else if (strcmp(argv[1], "run") == 0) {
        system("./clds-server");
    }

    return 0;
}

