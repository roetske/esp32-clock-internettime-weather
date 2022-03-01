# esp32-clock-internettime-weather
esp32 project with lcd 20x4 =>clock internettime automatic update summer/wintertime,weather from openweather.org

hardware
--------
esp32 -lcd 20x4 with i2c piggyback
optional levelconverter 3.3v to 5v

remark: power lcd with 5v 3.3v is not sufficient for lcd.
----------------------------------
description function
---------------------
1)settings wifi automatic with autoconnect

when started up esp32 creates accesspoint eps32ap to choose wifi ssid and set paswword
you open wifi accesspoint with your cellphone and choose your ssid you want to connect and set password
no more compiling needed to change ssid and pasword when it changes. Ones set your ssid and password are
stored in esp32.

2)time winter/summer hour is automatically adjusted

3)clock is updated every half hour when there is internet pool.org

4)weather is scrolled on line 3

5)time is set on 2 lines to make it more readable
lines on lcd

line 0  date + indication wifi present

line1 and line2 by using 2 lines bignumbers time

line3 scrolling text weatherinfo from openweather.org
      wheather description
      temperature min max
      humidity and pressure
      wind winddirection speed and max speed
      
remark:
-------
You can eliminate get time from pool.org.
Time is also given by openweather in epoch time.
To get openweather data you need to register its free. 
      You can also test response openweather by sending request http
      You get a json formatted response from openweather.
      See code i have added string example in main code.
First time or with reset you need wifi. Once set if wifi is lost clock 
still working. (as long that you do no reset with power or reset button)
Also it tries to reconnect to wifi automatically.

