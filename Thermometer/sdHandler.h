#ifndef _sd_hanlder_h_
#define _sd_handler_h_

#include <detail/RequestHandler.h>
#include <SD.h>

class SdHandler : public RequestHandler {
 protected:
  String _uri;
  String _path;
  boolean _isFile;
  size_t _baseUriLength;
 public:
  SdHandler(const char *path,const char *uri);
  bool canHandle(HTTPMethod method, String uri) override;
  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod,
	      String requestUri) override;
  
};


#endif /* _sd_handler_h_ */

