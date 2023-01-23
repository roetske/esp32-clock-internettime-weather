# esp32-clock-internettime-weather
esp32 project with lcd 20x4 =>clock internettime automatic update summer/wintertime,weather from openweather.org<br />

hardware
--------
esp32 <br />
lcd 20x4 <br />
i2c piggyback for lcd<br />
levelconverter 3.3v to 5v <br />
(recommended use it for your i2c com with lcd)<br />

remark: power lcd with 5v 3.3v is not sufficient for lcd.
----------------------------------
description function
---------------------
1)settings wifi automatic with autoconnect<br />

when started up esp32 creates accesspoint esp32ap to choose wifi ssid and set paswword<br />
you open wifi accesspoint with your cellphone and choose your ssid you want to connect and set password<br />
no more compiling needed to change ssid and pasword when it changes. Once set your ssid and password are<br />
stored in esp32.<br />

2)time winter/summer hour is automatically adjusted<br />

3)clock is updated every half hour when there is internet pool.org<br />

4)weather is scrolled on line 3<br />

5)time is set on 2 lines to make it more readable<br />
lines on lcd<br />

line 0  date + indication wifi present<br />

line1 and line2 by using 2 lines bignumbers time<br />

line3 scrolling text weatherinfo from openweather.org<br />
      wheather description<br />
      temperature min max<br />
      humidity and pressure<br />
      wind winddirection speed and max speed<br />
      
remark:<br />
-------<br />
You can eliminate get time from pool.org.<br />
Time is also given by openweather in epoch time.<br />
To get openweather data you need to register its free. <br />
      You can also test response openweather by sending request http<br />
      You get a json formatted response from openweather.<br />
      See code i have added string example in main code.<br />
First time or with reset you need wifi. Once set if wifi is lost clock <br />
still working. (as long that you do no reset with power or reset button)<br />
Also it tries to reconnect to wifi automatically.<br />

issues:<br />
-------
First runs got esp stuck after x hours. restart ok.<br />
Did not find real cause error. What was sure esp stuck.<br />
Now stable after code improvements<br />
See issues tab github.<br />




