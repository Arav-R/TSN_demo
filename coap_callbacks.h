#include "Arduino.h"
#include <QNEthernet.h>

#include "coap-simple.h"
#include "pinout.h"

#ifndef __CoAp__
#define __CoAp__
extern bool safe;
extern int stepsLeft;
extern int currentStep;
extern int microStep;
extern int stepsPerSlot;
extern int delayMicro;
#endif

//CoAP callbacks
void callback_core(CoapPacket &packet, String url, IPAddress ip, int port);

void callback_direction_cmd(CoapPacket &packet, String url, IPAddress ip, int port);
void callback_step_cmd(CoapPacket &packet, String url, IPAddress ip, int port);
void callback_multistep_cmd(CoapPacket &packet, String url, IPAddress ip, int port);
void callback_safe(CoapPacket &packet, String url, IPAddress ip, int port);
void callback_total(CoapPacket &packet, String url, IPAddress ip, int port);

void callback_microstep_cmd(CoapPacket &packet, String url, IPAddress ip, int port);
void callback_delay_cmd(CoapPacket &packet, String url, IPAddress ip, int port);


void callback_led_cmd(CoapPacket &packet, String url, IPAddress ip, int port);


// void callback_valve_cmd(CoapPacket &packet, String url, IPAddress ip, int port);
// void callback_valve_voltage(CoapPacket &packet, String url, IPAddress ip, int port);
// void callback_rtd(CoapPacket &packet, String url, IPAddress ip, int port);
// void callback_4_20(CoapPacket &packet, String url, IPAddress ip, int port);

void is_observe_packet(CoapPacket &packet);