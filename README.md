# clds

**clds** is a lightweight and extensible dynamic web server written in **C**, with routing and application logic powered by **Lua**.  

---

## Features

- Lightweight HTTP server written in C
- Hot-reload of Lua routes
- Dynamic routing via Lua scripts
- Simple project structure
- CLI for project creation and execution
- Low memory footprint

---

## Project Structure

A generated project follows this layout:

```
project/
├── app/
│   ├── routes.lua
│   └── views/
│       ├── hello.html
│       └── index.html
└── clds-server
```

- `clds-server` is copied locally to each project
- `routes.lua` controls request handling
- HTML views are served dynamically

---

## Requirements

- GNU GCC
- Lua (5.3 or 5.4 recommended)
- pkg-config
- POSIX-compatible system (Linux, BSD, macOS)

### Debian / Ubuntu

```
sudo apt update
sudo apt install -y \
  build-essential \
  pkg-config \
  lua5.4 \
  liblua5.4-dev
```

### Arch Linux / Manjaro

```
sudo pacman -S --needed \
  base-devel \
  pkgconf \
  lua
```

### Fedora / RHEL / Rocky Linux / AlmaLinux

```
sudo dnf install -y \
  gcc \
  make \
  pkgconf-pkg-config \
  lua \
  lua-devel
```

---

## Installation

Clone the repository:

```
git clone https://github.com/nothingburguer/clds.git
cd clds
sudo make install
```

This installs:

```
/usr/local/bin/clds
/usr/local/bin/clds-server
```

To **uninstall** run `sudo make uninstall`

---

## Usage

### Create a new project

```
clds new myapp
```

This will:

- Create the project structure
- Generate default Lua routes and views
- Copy `clds-server` into the project directory

### Run the project

```
cd myapp
clds run
```

The server will start on:

```
http://localhost:8080
```

---

## Lua Routing Example

```lua
routes = {
  ["/"] = "index.html",
  ["/hello"] = "hello.html"
}

function handle(path)
  local view = routes[path]
  if not view then
    return "<h1>404</h1>"
  end

  local f = io.open("app/views/" .. view)
  local html = f:read("*a")
  f:close()
  return html
end
```

---
