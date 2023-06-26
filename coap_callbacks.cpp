#include "coap_callbacks.h"





 bool safe = true;
 int stepsLeft = 0;
 int target = 0;
 int reverse = 1;
 int currentStep = 0;
 int microStep = 1;
 int stepsPerSlot = 25 * microStep;
 int delayMicro = 1000;

 int maxSpeed = 2000;
 int prevSpeed = 0;
 int accel = 20000;
 int prevAccel = 0;




// int get_index_from_url(String url) {
//     char c_url[url.length() + 1];
//     memcpy(c_url, url.c_str(), url.length());
//     char *pch;
//     pch = strtok(c_url, "/");
//     pch = strtok(NULL, "/");
//     Serial.println(url);
//     return atoi(pch);
// }




void callback_core(CoapPacket &packet, String url, IPAddress ip, int port) {
  Serial.println("We are being discovered.");
  String resource_str = "</direction/cmd>,</step/cmd>,</multistep/cmd>,</microStep/cmd>,</delay/cmd>,</safe/cmd>,</total/cmd>,</led/cmd>,</speed/cmd>,</accel/cmd>";
  packet.payload = (uint8_t *)resource_str.c_str();
  packet.payloadlen = resource_str.length();
  packet.code = COAP_CONTENT;
  packet.type = COAP_ACK;

  memset(packet.options, 0, packet.optionnum);
  packet.optionnum = 0;
	uint8_t optionBuffer[2] {0};
  optionBuffer[0] = ((uint16_t)COAP_APPLICATION_LINK_FORMAT & 0xFF00) >> 8;
  optionBuffer[1] = ((uint16_t)COAP_APPLICATION_LINK_FORMAT & 0x00FF) ;
  packet.addOption(COAP_CONTENT_FORMAT, 2, optionBuffer);    
}

void clear_packet(CoapPacket &packet) {
    memset(packet.payload, 0, packet.payloadlen);
    if (
        packet.optionnum == 1 && 
        packet.options[0].number == COAP_OBSERVE && 
        packet.options[0].length > 0) {
        // we made this option so do not clear it. 
    }
    else {
        memset(packet.options, 0, packet.optionnum);
        packet.optionnum = 0;
    }      
    

}

void configure_response(CoapPacket &packet){
    packet.code = COAP_CONTENT;
    packet.type = COAP_ACK;
	uint8_t optionBuffer[2] {0};
    optionBuffer[0] = ((uint16_t)COAP_TEXT_PLAIN & 0xFF00) >> 8;
    optionBuffer[1] = ((uint16_t)COAP_TEXT_PLAIN & 0x00FF) ;
    packet.addOption(COAP_CONTENT_FORMAT, 2, optionBuffer); 
}

void callback_direction_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);
        if (message.equals("0")) {
            Serial.println("Forward");

            //digitalWrite(valve_cmd_map[valve_num], LOW);
            digitalWrite(pinOut_DIR, LOW);
            reverse = 1;
        } 
        else if(message.equals("1")) {
            Serial.println("backwards");
            //digitalWrite(valve_cmd_map[valve_num], HIGH);
            digitalWrite(pinOut_DIR, HIGH);
            reverse = -1;
        } 
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    uint8_t val = digitalRead(pinOut_DIR);
    //Serial.println((const char*)&val);
    packet.payload = &val;
    packet.payloadlen = 1;
    configure_response(packet); 
}

void callback_step_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);
        if (message.equals("0")) {
            Serial.println("step 0");

            //digitalWrite(valve_cmd_map[valve_num], LOW);
            digitalWrite(pinOut_STEP, LOW);
        } 
        else if(message.equals("1")) {
            Serial.println("step 1");
            //digitalWrite(valve_cmd_map[valve_num], HIGH);
            digitalWrite(pinOut_STEP, HIGH);
        } 
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    uint8_t val = digitalRead(pinOut_STEP);
    //Serial.println((const char*)&val);
    packet.payload = &val;
    packet.payloadlen = 1;
    configure_response(packet); 
}

void callback_multistep_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);
        
        Serial.println("Stepping!");

        
        stepsLeft = message.toInt();
        target = message.toInt();

        currentStep += message.toInt();


        // for(int x = 0; x < message.toInt(); x++) {
        //   digitalWrite(pinOut_STEP,HIGH); 
        //   delayMicroseconds(1000);    // by changing this time delay between the steps we can change the rotation speed
        //   digitalWrite(pinOut_STEP,LOW); 
        //   delayMicroseconds(1000); 

        //   Serial.println(x);
        // }
        Serial.println("step done"); 
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    uint8_t val = digitalRead(pinOut_STEP);
    //Serial.println((const char*)&val);
    packet.payload = &val;
    packet.payloadlen = 1;
    configure_response(packet); 

}

void callback_microstep_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);

        microStep = message.toInt();

        if (microStep == 1) {
          digitalWrite(pinOut_MS1, LOW);
          digitalWrite(pinOut_MS2, LOW);
          digitalWrite(pinOut_MS3, LOW);
        } else if (microStep == 2) {
          digitalWrite(pinOut_MS1, HIGH);
          digitalWrite(pinOut_MS2, LOW);
          digitalWrite(pinOut_MS3, LOW);
        } else if (microStep == 4) {
          digitalWrite(pinOut_MS1, LOW);
          digitalWrite(pinOut_MS2, HIGH);
          digitalWrite(pinOut_MS3, LOW);
        } else if (microStep == 8) {
          digitalWrite(pinOut_MS1, HIGH);
          digitalWrite(pinOut_MS2, HIGH);
          digitalWrite(pinOut_MS3, LOW);
        } else if (microStep == 16) {
          digitalWrite(pinOut_MS1, HIGH);
          digitalWrite(pinOut_MS2, HIGH);
          digitalWrite(pinOut_MS3, HIGH);
        } else {
          digitalWrite(pinOut_MS1, LOW);
          digitalWrite(pinOut_MS2, LOW);
          digitalWrite(pinOut_MS3, LOW);
        }

        // for(int x = 0; x < message.toInt(); x++) {
        //   digitalWrite(pinOut_STEP,HIGH); 
        //   delayMicroseconds(1000);    // by changing this time delay between the steps we can change the rotation speed
        //   digitalWrite(pinOut_STEP,LOW); 
        //   delayMicroseconds(1000); 

        //   Serial.println(x);
        // }
        Serial.print("Micro steps: "); 
        Serial.println(message.toInt());
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    Serial.print("Micro steps: "); 
    Serial.println(message.toInt());

    char payload[11];  // 10 digits plus NULL
    int val = microStep;
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet);  

}

void callback_delay_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);

        delayMicro = message.toInt();

      

        // for(int x = 0; x < message.toInt(); x++) {
        //   digitalWrite(pinOut_STEP,HIGH); 
        //   delayMicroseconds(1000);    // by changing this time delay between the steps we can change the rotation speed
        //   digitalWrite(pinOut_STEP,LOW); 
        //   delayMicroseconds(1000); 

        //   Serial.println(x);
        // }
        Serial.print("Delay: "); 
        Serial.println(message.toInt());
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    Serial.print("Delay: "); 
    Serial.println(message.toInt());

    char payload[11];  // 10 digits plus NULL
    int val = delayMicro;
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet);  

}

void callback_speed_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);

        maxSpeed = message.toInt();

      

        // for(int x = 0; x < message.toInt(); x++) {
        //   digitalWrite(pinOut_STEP,HIGH); 
        //   delayMicroseconds(1000);    // by changing this time delay between the steps we can change the rotation speed
        //   digitalWrite(pinOut_STEP,LOW); 
        //   delayMicroseconds(1000); 

        //   Serial.println(x);
        // }
        Serial.print("Max Speed: "); 
        Serial.println(message.toInt());
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    Serial.print("Max Speed: "); 
    Serial.println(message.toInt());

    char payload[11];  // 10 digits plus NULL
    int val = maxSpeed;
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet);  

}

void callback_accel_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);

        accel = message.toInt();

      

        // for(int x = 0; x < message.toInt(); x++) {
        //   digitalWrite(pinOut_STEP,HIGH); 
        //   delayMicroseconds(1000);    // by changing this time delay between the steps we can change the rotation speed
        //   digitalWrite(pinOut_STEP,LOW); 
        //   delayMicroseconds(1000); 

        //   Serial.println(x);
        // }
        Serial.print("Accel: "); 
        Serial.println(message.toInt());
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    Serial.print("Accel: "); 
    Serial.println(message.toInt());

    char payload[11];  // 10 digits plus NULL
    int val = accel;
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet);  

}


void callback_safe(CoapPacket &packet, String url, IPAddress ip, int port){
    // determine individual loop from url
    //int loop_num = get_index_from_url(url);
    

  // copy payload
    char p[packet.payloadlen + 1] = {};
    //Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);


    // clear out packet fields and craft response
    clear_packet(packet);

  

    uint8_t val = 1;

    if (safe) {
       val = 1; // motor not moving
       
    } else {
       val = 0; // motor moving
    }

    Serial.print("Value: ");
    Serial.println(val);

    Serial.print("safe: ");
    Serial.println(safe);
    
    //Serial.println((const char*)&val);
    packet.payload = &val;
    packet.payloadlen = 1;
    configure_response(packet); 
}

void callback_total(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);

        currentStep = message.toInt();

        // for(int x = 0; x < message.toInt(); x++) {
        //   digitalWrite(pinOut_STEP,HIGH); 
        //   delayMicroseconds(1000);    // by changing this time delay between the steps we can change the rotation speed
        //   digitalWrite(pinOut_STEP,LOW); 
        //   delayMicroseconds(1000); 

        //   Serial.println(x);
        // }
        Serial.print("total steps: "); 
        Serial.println(message.toInt());
    }    

    // clear out packet fields and craft response
    // clear out packet fields and craft response
    clear_packet(packet);


    int myVal = currentStep;
    packet.payloadlen = 2;
    packet.payload[0] = highByte(myVal);
    packet.payload[1] = lowByte(myVal);

    // uint8_t val = currentStep;
    // //Serial.println((const char*)&val);
    // packet.payload = &val;
    // packet.payloadlen = 1;
    configure_response(packet); 

}



void callback_led_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    //int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);
        if (message.equals("0")) {
            Serial.println("OFF!");

            //digitalWrite(valve_cmd_map[valve_num], LOW);
            digitalWrite(pinOut_LED, LOW);
        } 
        else if(message.equals("1")) {
            Serial.println("ON!");
            //digitalWrite(valve_cmd_map[valve_num], HIGH);
            digitalWrite(pinOut_LED, HIGH);
        } 
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    char payload[11];  // 10 digits plus NULL
    //uint8_t val = digitalRead(valve_cmd_map[valve_num]); 
    uint8_t val = digitalRead(pinOut_LED);
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet); 
}


/*
void callback_valve_cmd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    int valve_num = get_index_from_url(url);

    // copy payload
    char p[packet.payloadlen + 1] = {};
    Serial.println(packet.payloadlen);
    memcpy(p, packet.payload, packet.payloadlen);
    // p[packet.payloadlen] = NULL;
    String message(p);

    // process PUT
    if (packet.code == COAP_PUT) {
        Serial.println(message);
        if (message.equals("0")) {
            Serial.println("OFF!");
            digitalWrite(valve_cmd_map[valve_num], LOW);
        } 
        else if(message.equals("1")) {
            Serial.println("ON!");
            digitalWrite(valve_cmd_map[valve_num], HIGH);
        } 
    }    

    // clear out packet fields and craft response
    clear_packet(packet);

    char payload[11];  // 10 digits plus NULL
    uint8_t val = digitalRead(valve_cmd_map[valve_num]); 
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet); 
}

void callback_valve_voltage(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual valve from url
    int valve_num = get_index_from_url(url);
    
    // clear out packet fields and craft response
    clear_packet(packet);

    char payload[11];  // 10 digits plus NULL
    int val = analogRead(vfb_map[valve_num]);
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet);     
}

void callback_rtd(CoapPacket &packet, String url, IPAddress ip, int port) {
    // determine individual rtd from url
    int rtd_num = get_index_from_url(url);
    
    // clear out packet fields and craft response
    clear_packet(packet);

    char payload[11];  // 10 digits plus NULL
    int val = analogRead(rtd_map[rtd_num]);
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet);  
}

void callback_4_20(CoapPacket &packet, String url, IPAddress ip, int port){
    // determine individual loop from url
    int loop_num = get_index_from_url(url);
    
    // clear out packet fields and craft response
    clear_packet(packet);

    char payload[11];  // 10 digits plus NULL
    int val = analogRead(SPI_4_20[loop_num]);
    Serial.println(val);
    sprintf(payload, "%d", val);
    packet.payload = (uint8_t *)payload;
    packet.payloadlen = strlen(payload);
    configure_response(packet); 
}
*/
