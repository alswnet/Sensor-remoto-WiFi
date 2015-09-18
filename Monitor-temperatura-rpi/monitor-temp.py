#!/usr/bin/env python
# coding=utf8
import sys, signal, time
import serial.tools.list_ports as list_ports
import minimalmodbus
import paho.mqtt.publish as publish
import urllib2

#Parametros del servidor remoto
servidor = '50.116.76.54'
apikey = 'd5c746f11b3d2b34ba192ab8f22efe87'
nodo = 'Diszasa_Rpi'

def buscar_puerto():
  #Lista todos los puertos seriales disponibles
  lista = list_ports.comports()

  #Recorre la lista de puertos, buscando el primero cuyo nombre comience
  #con /dev/ttyUSB (no importa el numero de puerto)
  for i in lista:
    if i[0][:11] == '/dev/ttyUSB':
      return i[0]

def manejador_sigint(sig_num, stack_frame):
  print "Señal SIGINT recibida, terminando"
  sys.exit(0)

def manejador_sigterm(sig_num, stack_frame):
  print "Señal SIGTERM recibida, terminando"
  sys.exit(0)

def publicar_temperatura(temp):
    #Primero se publica la temperatura a mqtt en el equipo local
    #(Como es el servidor local, se asume que funciona y no se manejan errores)
    publish.single('emonhub/rx/1/values', str(temp), hostname='127.0.0.1')

    #Luego intenta publicar en el servidor remoto
    print "Enviando dato a", servidor, "...",
    url = 'http://' + servidor + '/sees/input/post.json?node=' + nodo + \
          '&apikey=' + apikey + '&json={temperatura:' + temp + '}'
    try:
      urllib2.urlopen(url)
    except urllib2.URLError:
      #Si no pudo publicar simplemente lo menciona y lo deja pasar
      print "Error al conectarse al servidor"
    else:
      print "OK"

#Primero registra los manejadores de señales
signal.signal(signal.SIGINT, manejador_sigint)
signal.signal(signal.SIGTERM, manejador_sigterm)

#Busca el primer puerto serie USB y abre una conexion a el mediante la
#libreria de modbus
puerto = buscar_puerto()
print "conectandose al puerto", puerto
ins = minimalmodbus.Instrument(puerto, 1)

#Entra en un lazo infinito que se repite mientras se pueda leer
#el sensor exitosamente. Si ocurren fallas (excepciones) se reintenta
#un maximo e 3 veces antes de terminar
reintentos = 0
while reintentos < 3:
  try:
    #Intenta leer la temperatura
    temp = str(ins.read_register(1, 2, 4))
  except:
    #Si ocurrio una excepcion, toma el mensaje de error y lo imprime
    e = sys.exc_info()[1]
    print e
    #Luego otorga un segundo de espera e incrementa el conteo de reintentos
    time.sleep(1)
    reintentos = reintentos + 1
  else:
    #Si la leyo exitosamente, imprime la temperatura
    print "Temperatura:", temp + "C"
    #Y la publica en los servidores web
    publicar_temperatura(temp)
    #Se otorga una espera de 5 segundos (emoncms no puede aceptar mas rapido)
    #y como hubo exito, se limpia la cuenta de reintentos
    time.sleep(5)
    reintentos = 0
