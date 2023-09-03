# chttp

*well C++ not C, but C++ http sounds bad and i dont care so*

Minimal HTTP server/library (why would you use this) built from scratch using C++ (only uses stdlib/STL).

## example

```c++
#include "http_server.hpp"
#include "template.hpp"

// open connection to localhost:3000 
HttpServer server = HttpServer{"3000"};

auto index = Template{"../templates/index.html"};

// see what i did there at the end? :)
server.AddRoute("/", RequestMethod::GET, "text/html", index.html())

// you can use any content type
server.AddRoute("/deez", RequestMethod::GET, "text/text", "nuts")
server.AddRoute("/deezhtml", RequestMethod::GET, "text/html", "<h1>deez nuts</h1>")

server.Start();
```

## features

- Blazingly fast 🚀🚀🚀
- No external libraries
- Routing with diff methods
- HTML Templating
