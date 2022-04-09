// Function that gets current epoch time
unsigned long getTime()
{
  time_t currenttime;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
   {
    //Serial.println("Failed to obtain time");
    return(0);
   }
  time(&currenttime);
  
  return currenttime;
}
//connect with timeserver

void setNTPtime()
{
   time_t epoch = 0UL;
   if((epoch =  getFromNTP(timeServer)) != 0 || (epoch =  getFromNTP(timeServer2)) != 0){ // get from time server
      // Unix uses an epoch located at 1/1/1970-00:00h (UTC) and NTP uses 1/1/1900-00:00h. 
      //This leads to  an offset equivalent to 70 years in seconds (there are 17 leap years between the two dates so the offset is
      //(70*365 + 17)*86400 = 2208988800
      epoch -= 2208988800UL ;
      //offset timezone
      epoch += localTimeOffset;
      setTime(epoch += dst(epoch));//correction summertime + 1 hour or 0
      setTime(epoch);
      update = now() +  UPDATESECS; // set next update time if successful
      //set rtc
      rtc.setTime(epoch);  
   }
  
}


unsigned long getFromNTP(const char* server)
{
   udp.begin(localPort);
   sendNTPpacket(server);   // send an NTP packet to a time server
   delay(1000);         // wait to see if a reply is available
   int cb = udp.parsePacket();
   if (!cb) {
      //Serial.println("no packet yet");
      return 0UL;
   }
   //Serial.print("packet received, length=");
   //Serial.println(cb);
   // We've received a packet, read the data from it
   udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

   //the timestamp starts at byte 40 of the received packet and is four bytes,
   // or two words, long. First, extract the two words:

   unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
   unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
   // combine the four bytes (two words) into a long integer
   // this is NTP time (seconds since Jan 1 1900):
   udp.stop();
   return (unsigned long) highWord << 16 | lowWord;
}

// send an NTP request to the time server at the given address
void sendNTPpacket(const char* server)
{
   //Serial.print("sending NTP packet to ");
   //Serial.println(server);
   // set all bytes in the buffer to 0
   memset(packetBuffer, 0, NTP_PACKET_SIZE);
   // Initialize values needed to form NTP request
   // (see URL above for details on the packets)
   packetBuffer[0] = 0b11100011;   // LI, Version, Mode
   packetBuffer[1] = 0;     // Stratum, or type of clock
   packetBuffer[2] = 6;     // Polling Interval
   packetBuffer[3] = 0xEC;  // Peer Clock Precision
   // 8 bytes of zero for Root Delay & Root Dispersion
   packetBuffer[12]  = 49;
   packetBuffer[13]  = 0x4E;
   packetBuffer[14]  = 49;
   packetBuffer[15]  = 52;

   // all NTP fields have been given values, now
   // you can send a packet requesting a timestamp:
   udp.beginPacket(server, 123); //NTP requests are to port 123
   udp.write(packetBuffer, NTP_PACKET_SIZE);
   udp.endPacket();
}

int dst (time_t t) // calculate if summertime in Europe
{
   tmElements_t te;
   te.Year = year(t)-1970;
   te.Month =3;
   te.Day =1;
   te.Hour = 0;
   te.Minute = 0;
   te.Second = 0;
   time_t dstStart,dstEnd, current;
   dstStart = makeTime(te);
   dstStart = lastSunday(dstStart);
   dstStart += 2*SECS_PER_HOUR;  //2AM
   te.Month=10;
   dstEnd = makeTime(te);
   dstEnd = lastSunday(dstEnd);
   dstEnd += SECS_PER_HOUR;  //1AM
   if (t>=dstStart && t<dstEnd) return (3600);  //Add back in one hours worth of seconds - DST in effect
   else return (0);  //NonDST
}

time_t lastSunday(time_t t)
{
   t = nextSunday(t);  //Once, first Sunday
   if(day(t) < 4) return t += 4 * SECS_PER_WEEK;
   else return t += 3 * SECS_PER_WEEK;
}   
