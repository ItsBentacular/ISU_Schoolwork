import http from "http";

const server = http.createServer((req, res) => {
    console.log(req.url);
    res.writeHead(200, { "Content-Type" : "text/plain"});
    res.end("Hello Class SE/ComS 3190");

});
console.log("Server is listening on port 3924");
server.listen(3924, "127.0.0.1");