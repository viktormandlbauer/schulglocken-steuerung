#include "DEBUG.h"
#ifdef DEBUG_TIME
#define DEBUG
#endif

#include "Timequeue.h"
using namespace Timequeue;

alert alerts[100];

void Timequeue::insert_alert(alert new_alert);
void Timequeue::clear_alerts(int keep_last);
alert Timequeue::get_next_alert(DateTime datetime);
