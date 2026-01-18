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
  if not f then
    return "<h1>Error to open HTML</h1>"
  end

  local html = f:read("*a")
  f:close()
  return html
end
