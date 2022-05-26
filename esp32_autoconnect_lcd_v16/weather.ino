void getWeatherData() //client function to send/receive GET request data.
{ initweatherstr();
  messageactive =false; //set true when ok
  if (WiFi.status() != WL_CONNECTED)  //starts client connection, checks for connection
	 {  Serial.println("No wifi -to get wheather");
      return;
   }
   //ready to get data from weather.org 
   
   HTTPClient http;  
   http.begin("https://api.openweathermap.org/data/2.5/weather?q=Gent,BE&units=metric&PT&APPID="your unique code subscription openweahter");
   int httpCode = http.GET();  // send the request
   Serial.println("response openweather");
   Serial.print("HttpCode:");
   Serial.println(httpCode);
  //json
   DynamicJsonDocument doc(1024);
   if (httpCode == 200) // check the returning code
	  { 
      String payload = http.getString();   //Get the request response payload
      
      // Parse JSON object
      DeserializationError error = deserializeJson(doc, payload);
      if (error) 
       {
        //debug
        Serial.print("Deserialization failed with code: ");
        Serial.println(error.c_str());
        clearline(3);
        lcd.print(error.c_str());
        http.end();//close connection
        doc.clear();
        return;
       }
    }
    else
    {//bad response
      Serial.print("Bad response http ");
      http.end();
      doc.clear();
      clearline(3);
      return;
    }  
    
    http.end();   //Close connection
    serializeJson(doc, Serial);
    //verder
    String  location       = doc["name"]; //ghent
    String  weather        = doc["weather"][0]["main"];             // [0] searched for hours is json array
    String description     = doc["weather"][0]["description"];    
    
    float  temperature  = doc["main"]["temp"]; // temp in degreesC
    float  min_temp     = doc["main"]["temp_min"];
    float  max_temp     = doc["main"]["temp_max"];
    int i_temp          = floattoint(temperature);
    int i_temp_min      = floattoint(min_temp);
    int i_temp_max      = floattoint(max_temp);
    
    float  humidity     = doc["main"]["humidity"]; //hum in %
    float  pressure     = doc["main"]["pressure"]; //mbar
    int i_humidity      = floattoint(humidity);
    int i_pressure      = floattoint(pressure);

    float windspeed     = doc["wind"]["speed"];
    float winddirection = doc["wind"]["deg"];
    float windmax       = doc["wind"]["gust"];
    int i_windkmh       =  mpstokmh( windspeed);
    int i_windmaxkmh    =  mpstokmh(windmax);
    String str_winddir  =  degreeToCompass(winddirection);

    //complete messages
    wthr = "Weather " + location + " " + weather +  "  " + description + " ";
    tmp ="Temperature "  + String(i_temp) +" C " + String(i_temp_min) +" minC " + String(i_temp_max) +" maxC ";
    hp = "Hum and Pressure " + String(i_humidity ) + "% " + String(i_pressure ) + " mBar ";
    wnd = "Direction " + str_winddir + " Speed "+ String(i_windkmh) + " Km/h " + String(i_windmaxkmh) +" maxKm/h ";//space at end string for scroll!!!
    //debug
    Serial.println("-");
    Serial.println(wthr);
    Serial.println(tmp);
    Serial.println (hp);
    Serial.println (wnd);
    //all ok scroll 
    messageactive =true;
    //nw approach clear doc for memory leaks
    doc.clear();
    // Clean the document is memory pool is over 80% of its capacity
    // if (doc.memoryUsage() > doc.capacity() * 4 / 5)
    //   {
    //     doc.garbageCollect();
    //   }  
}

//weather
void initweatherstr()
{ wthr = "";//weatherstring
  tmp  = "";//tempstring
  hp   = "";//hum and pr
  wnd  =  "";//wind
  //reset 
  stringStart = 0;
  stringEnd = 0;
  scrollCursor = screenWidth; //start from left to right
  counterscrollmessage = 1;
}



 
