/* Iniciamos el módulo http */
var http = require("http");
/* Creamos el objeto del servidor */
http.createServer(function(request, response) {
    /* Preparamos las cabeceras de respuesta */
    response.writeHead(200, {"Content-Type": "text/html"});
    response.write("¿Que pasa tronco?");
    response.end();
}).listen(4444);