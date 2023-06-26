
#include <AccelStepper.h>

#include <QNEthernet.h>

#include "coap-simple.h"
#include "coap_callbacks.h"


// Accel Stepper library stuff
AccelStepper stepper1(1, 34, 33);

// Configure namespace for network stack
namespace qn = qindesign::network;
using namespace std;

//Initialize network stack
IPAddress IP_Addr(10, 1, 1, 59);
IPAddress netmask(255, 255, 255, 0);
IPAddress gateway(10, 1, 1, 1);
unsigned int localPort = 5683;

qn::EthernetUDP Udp;
Coap coap(Udp);

void setup_network() {
  qn::Ethernet.setHostname("teensy_motor2");
  qn::Ethernet.begin();


  byte macbuffer[6];
  qn::Ethernet.MACAddress(macbuffer);
  qn::Ethernet.begin(IP_Addr, netmask, gateway);

  

  
  Udp.begin(localPort);
}

void setup_pins() {
  pinMode(pinOut_LED, OUTPUT);
  pinMode(pinOut_DIR, OUTPUT);
  pinMode(pinOut_STEP, OUTPUT);

  pinMode(pinOut_MS1, OUTPUT);
  pinMode(pinOut_MS2, OUTPUT);
  pinMode(pinOut_MS3, OUTPUT);
}

// Register CoAP callbacks
void setup_coap() {
  char buffer[100];
  coap.server(callback_core, ".well-known/core");

  sprintf(buffer, "direction/cmd");
  Serial.println(buffer);
  coap.server(callback_direction_cmd, buffer);

  sprintf(buffer, "step/cmd");
  Serial.println(buffer);
  coap.server(callback_step_cmd, buffer);

  sprintf(buffer, "multistep/cmd");
  Serial.println(buffer);
  coap.server(callback_multistep_cmd, buffer);

  sprintf(buffer, "microstep/cmd");
  Serial.println(buffer);
  coap.server(callback_microstep_cmd, buffer);

  sprintf(buffer, "delay/cmd");
  Serial.println(buffer);
  coap.server(callback_delay_cmd, buffer);

  sprintf(buffer, "safe/cmd");
  Serial.println(buffer);
  coap.server(callback_safe, buffer);

  sprintf(buffer, "total/cmd");
  Serial.println(buffer);
  coap.server(callback_total, buffer);

  sprintf(buffer, "led/cmd");
  Serial.println(buffer);
  coap.server(callback_led_cmd, buffer);


  // for ( int i = 0; i < *(&valve_cmd_map + 1) - valve_cmd_map; i++){
  //   sprintf(buffer, "valve/%d/cmd", i);
  //   Serial.println(buffer);
  //   coap.server(callback_valve_cmd, buffer);
  // }

  // for ( int i = 0; i < *(&vfb_map + 1) - vfb_map; i++){
  //   sprintf(buffer, "valve/%d/adc_count", i);
  //   Serial.println(buffer);
  //   coap.server(callback_valve_voltage, buffer);
  // }  

  // for ( int i = 0; i < *(&rtd_map + 1) - rtd_map; i++){
  //   sprintf(buffer, "rtd/%d/adc_count", i);
  //   Serial.println(buffer);
  //   coap.server(callback_rtd, buffer);
  // } 

  // for ( int i = 0; i < *(&SPI_4_20 + 1) - SPI_4_20; i++){
  //   sprintf(buffer, "loop/%d/adc_count", i);
  //   Serial.println(buffer);
  //   coap.server(callback_rtd, buffer);
  // } 
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(5000);

  stepper1.setMaxSpeed(2000); // Set maximum speed value for the stepper
  stepper1.setAcceleration(20000); // Set acceleration value for the stepper
  stepper1.setCurrentPosition(0); // Set the current position to 0 steps

  delay(1000);

  setup_network();
  delay(10);
  Serial.println(qn::Ethernet.localIP());

  delay(10);  

  setup_pins();
  setup_coap();
  delay(10);
  coap.start();


}

void loop() {

  //digitalWrite(pinOut_MS1,HIGH);

  
  // put your main code here, to run repeatedly:
  int ret = coap.loop();
  // Serial.println(coap.print_shit());
  //delay(10);


  //Serial.println(safe);
  
  
  if (currentStep % stepsPerSlot == 0){
             safe = true;
     } 
  stepper1.move(target * reverse);
  while(stepper1.currentPosition() != target * reverse) {
          stepper1.run();

          safe = false;
         
          coap.loop();
      }
  stepper1.setCurrentPosition(0); // reset
  target = 0;
}
