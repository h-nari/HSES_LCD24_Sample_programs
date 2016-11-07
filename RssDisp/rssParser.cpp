#include <Arduino.h>
#include "rssParser.h"
#include "rssBuf.h"
#include "Humblesoft_ILI9341.h"

extern Humblesoft_ILI9341 tft;
extern RssBuf rssBuf;

RssParser::RssParser()
{
}


void RssParser::process_tag_close(const char *tag, const char *text)
{
  // Serial.printf("%s:%s\n", m_sPath, text);
  if(strcmp(m_sPath,"/rdf:RDF/item/title")==0 ||
     strcmp(m_sPath,"/rss/channel/item/title") == 0){
    rssBuf.addItem(text);
  }
  else if(strcmp(m_sPath,"/rdf:RDF/channel/title")==0 ||
	  strcmp(m_sPath,"/rss/channel/title")==0){
    rssBuf.setTitle(text);
  }
}
