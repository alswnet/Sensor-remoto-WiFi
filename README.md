# Sensor-remoto-WiFi
Firmware para monitores remotos WiFi basados en ESP8266.

Este programa permite enviar datos de los siguientes sensores:
- MQ-2 (Diferentes gases: LPG, i-butano, metano, alcohol, hidrogeno, humo)
- DS18B20 (Temperatura)
- DHT22 (Temperatura y humedad)

Los datos son enviados como solicitudes GET (HTTP), a internet. Los detalles de la nube son desconocidos (se trabajo bajo la base de conocimiento estrictamente necesario), pero puede adaptarse facilmente a sistemas como emoncms u otros.

Dependencias
------------
Para poder compilar este sketch, es necesario instalar en Arduino IDE la libreria One Wire de Jim Studt
http://www.pjrc.com/teensy/td_libs_OneWire.html

Liberado bajo licencia GPL v3, ver archivo LICENSE para mas detalles.
