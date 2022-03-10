void getWeatherData() //client function to send/receive GET request data.
{ initweatherstr();
  if (WiFi.status() != WL_CONNECTED)  //starts client connection, checks for connection
	 {  Serial.println("No wifi -to get wheather");
      return;
   }
   //ready to get data from weather.org 
   //json
   DynamicJsonDocument doc(1024);
   HTTPClient http;  
   http.begin("https://api.openweathermap.org/data/2.5/weather?q=Gent,BE&units=metric&PT&APPID=d301b027de9432c915fda2d1b10753e3");
   int httpCode = http.GET();  // send the request
   Serial.println("response openweather");
   Serial.println(httpCode);
  
   if (httpCode > 0) // check the returning code
	  { 
      String payload = http.getString();   //Get the request response payload
     
      // Parse JSON object
      DeserializationError error = deserializeJson(doc, payload);
      if (error) 
       {
        //debug
        Serial.print("Deserialization failed with code: ");
        Serial.println(error.c_str());
        initweatherstr();
        http.end();//close connection
        return;
       }
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
    // Clean the document is memory pool is over 80% of its capacity
    if (doc.memoryUsage() > doc.capacity() * 4 / 5)
      {
        doc.garbageCollect();
      }  
}

//weather
void initweatherstr()
{ wthr = "";//weahterstring
  tmp  = "";//tempstring
  hp   = "";//hum and pr
  wnd  =  "";//wind
  //reset 
  stringStart = 0;
  stringEnd = 0;
  scrollCursor = screenWidth; //start from left to right
  counterscrollmessage = 1;
}



 
