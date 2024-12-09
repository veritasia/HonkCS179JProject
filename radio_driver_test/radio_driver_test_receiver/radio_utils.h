#include <stdint.h>
#include <string.h>

#ifndef __RADIO_UTILS_H
#define __RADIO_UTILS_H

using namespace std;

String MakeSetIDPacket(const int tgtID) {
    if (tgtID < 0) {
        return "";
    }
    if (tgtID > 65535) {
        return "";
    }
    return "AT+ADDRESS=" + char(tgtID);
}

String MakeSendPacket(const int destinationID, uint8_t pitch, uint8_t roll, uint8_t fingerState) {
    String packet = "AT+SEND="; 
    packet += String(destinationID); 
    packet += ",3,";
    packet += String(char(pitch)); 
    packet += String(char(roll));
    packet += String(char(fingerState));
    return packet;
}

char* ParseReceivePacket(String packet) {
    char* payload[3];
    int startOfPayload = 0;
    char raw_packet[30];
    
    packet.toCharArray(raw_packet, 30);
    // strcpy(raw_packet, packet.c_str());
    char* data = strtok(raw_packet, ",");
    data = strtok(NULL, ",");
    data = strtok(NULL, ",");
    return data;
}

#endif