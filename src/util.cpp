#include "HTTP_PAGE.h"
#include "myutil.h"

#define DEBUG

using namespace myutil;
String myutil::create_http_response_time(String time)
{
#ifdef DEBUG
    Serial.println("[Info] Using myutil::create_http_response_time(String time)");

#endif

    String temp = "<html><head></head><body>" + time + "</body></html>";
    return temp;
}
