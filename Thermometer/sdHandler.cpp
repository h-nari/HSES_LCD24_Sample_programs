#include <Arduino.h>
#include <ESP8266WebServer.h>
#include "sdHandler.h"

SdHandler::SdHandler(const char *path, const char *uri)
  : _uri(uri) , _path(path)
{
   _isFile = true;
   _baseUriLength = _uri.length();
}

bool SdHandler::canHandle(HTTPMethod method, String uri)
{
  bool r = true;
  if(method != HTTP_GET) r = false;
  if(!uri.startsWith(_uri)) r = false;
  return r;
}

bool SdHandler::handle(ESP8266WebServer& server, HTTPMethod requestMethod,
		      String requestUri)
{
  if(!canHandle(requestMethod, requestUri)) return false;

  String path(_path);
  String spath = requestUri.substring(_baseUriLength);

  if(path.endsWith("/")){
    while(spath.startsWith("/"))
      spath = spath.substring(1);
  }
  path += spath;

  String contentType = "text/plain";

  File f = SD.open(path);
  if(!f) return false;
  if(!f.isDirectory())
    server.streamFile(f, contentType);
  else { /* directory */
    String s;
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200,"text/html","");
    s += "<html><head><title>";
    s += path;
    s += "</title></head>";
    s += "<body>\r\n";
    s += "<h1> File List of '";
    s += path;
    s += "'</h1>\r\n";
    s += "<table>\r\n";
    server.sendContent(s);

    if(path != "/"){
      int i = path.lastIndexOf("/");
	s = "<tr><td><a href='";
	if(i == 0)
	  s += _uri;
	else
	  s += _uri + path.substring(0,i);
	s += "'>parent dir</a></td><td></td></tr>";
	server.sendContent(s);
    }

    f.rewindDirectory();
    for(File entry = f.openNextFile(); entry; entry = f.openNextFile()){
      s = "<tr><td>";
      s += "<a href='";
      s += _uri + path + "/" + entry.name();
      s += "'>";
      s += entry.name();
      s += "</a></td><td align='right'>";
      if(entry.isDirectory())
	s += "directory";
      else {
	s += entry.size();
	s += " bytes";
      }
      s += "</td></tr>\r\n";
      server.sendContent(s);
      entry.close();
    }
    server.sendContent("</table></body></html>\r\n");
  }
  f.close();
  return true;
}
