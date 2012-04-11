Description:
---
- Learning project: Try to build a HTTPServer with C++/Qt


Content:
---
- Qt/Event-Driven HTTPServer without Threading
- JSON communication


lighttpd config
---
# cat /etc/lighttpd/conf-enabled/10-proxy.conf 
server.modules   += ( "mod_proxy" )

## Redirect all queries requested by e.g. http://www.example.invalid/test 
## define port via server.start

proxy.server  = ( "test" => ( 
                       ( "host" => "127.0.0.1",
                         "port" => 8888 ))
                )


Compile:
---
qmake server.pro && make


Contact:
---
- via github.com: andreaseri/HTTPServer 
