main bauen...

  httpServer
  //connect -> httpServer, newRequest; httpResponse, newResponse
  connect -> httpRequest, newRequest; httpResponse, newResponse

HTTPServer bauen...

  QTcpServer -> server

  constructor ->
    connect -> server, newConnection; this , newConnection

  start -> listen

  newConnection -> nextPendingConnection
  while (socket)
    HTTPParser
    connect -> HTTPParser, finished; parseFinished

  parseFinished -> emit newRequest(HTTPRequest(parser))
  ---

  signals:
    void newRequest(HTTPRequest request)

  private slots:
    newConnection
    parseFinished

HTTRrequest bauen...
HTTPResponse bauen...
HTTPParser bauen...
