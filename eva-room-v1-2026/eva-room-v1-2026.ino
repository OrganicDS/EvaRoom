// Copiright Guillermo Pereyra DNI 29308129. 
//31 de diciembre 2020 14:51hs me pongo a escribir estas lineas de código 
//en una garita mugrosa en la unahur. De acá cualquier construcción es un 
//salto hacia algo mejor. Deadline (? Marzo 2021

#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          //https://github.com/esp8266/Arduino
#endif

//needed for library
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <HTTPClient.h>
#include <HTTPUpdate.h>


#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#include <SimpleTimer.h>

#include <Ewma.h>
Ewma adcFilter1(0.01);

#include "DHTesp.h" 
#include <Ticker.h>
DHTesp dht;

#define dhtPin 26
 
#include "EEPROM.h"
int addr = 0;
#define EEPROM_SIZE 64
#include "esp_attr.h"

unsigned long myTime;
//#include "driver/adc.h"
//#include "esp_adc_cal.h"
//static esp_adc_cal_characteristics_t adc1_chars;
///////////// SENSOR RS485 ////////////////
#include <SoftwareSerial.h>
#include <Wire.h>
#define RXD2 16
#define TXD2 17
#define DE 33
#define RE 32
struct ICicloInfo {
  bool    valido;
  bool    on;               // estado actual
  uint32_t segRestantes;    // seg hasta el próximo cambio
};
const byte O2[] = {0x01 ,0x03 ,0x00 ,0x00 ,0x00 , 0x07, 0x04, 0x08};
byte values[20];
SoftwareSerial mod(RXD2, TXD2);
unsigned long previousMillis = 0;  // Variable para almacenar el tiempo transcurrido desde el último envío de la petición
unsigned long preMillis = 0;  // Variable para almacenar el tiempo transcurrido desde el último envío de la petición
const long intervalSensor = 2000;        // Intervalo de tiempo entre envíos de la petición
float humSustrato = 0.0;
float tempSustrato = 0.0;
float conductivity = 0.0;
float pH = 0.0;
float nitrogen = 0.0;
float phosphorus = 0.0;
float potassium = 0.0;


unsigned long currentMillis; /////////// CONTADOR DE MILLIS
uint64_t contador1;
uint64_t contador2;
uint64_t contador3;

uint64_t contadorloop;
uint64_t contadorRiego;

///////////////////////////DATOS DEL EQUIPO ///////////////////
//////////////////////////////////////////////////////////////
String modelo="Eva Room 2026 S2.106";
String Nserie="201";
#define BOT_TOKEN "BOT-TOKEN"
String codigoSeguridad= "bRfsLKMudoO";

// para subir datos al server
// https://guille.website/bd/graficos.php?serie=04
WiFiClient client;
const char* serverName = "http://greenmachine.com.ar/bd/post-eva-data-rs485_7Valores.php";
const char* serverName2 = "http://greenmachine.com.ar/bd/post-eva-riego.php";
const char* serverUrl = "http://greenmachine.com.ar/bd/procesando_7Valores.php?serie=106";
String apiKeyValue = "tPmAT5Ab3j7F9";


//int vSensorHT=3;

const int VERSION_ACTUAL = 2026;
String dios="1475437948";
int versionActual=1;
#define LED_COUNT 7
///////////////////////////////////////////////////////////



//////////////////////// NEOPIXEL ///////////////////////////
////////////////////////////////////////////////////////////
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    25

// How many NeoPixels are attached to the Arduino?


// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

/////////////////////////////////////////////////
/////////////////////////////////////////////////



#define ONLINE_TIME 86400000 // para reiniciar cada 24 hours

const unsigned long BOT_MTBS = 500; // mean time between scan messages
unsigned long timeoutportal = 1000;
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
//unsigned long bot_lasttime; // last time messages' scan has been done

const char* fwUrlBase = "http://www.greenmachine.com.ar/EvaOTA/versionUNAHUR.bin";
const char* fwUrlBase2 = "http://www.emprendimientoshumanos.com.ar/EvaOTA/versionUNAHUR.bin";
#define pinRiego  21 //D7
#define pinIluminacion  19 // 
//#define pinSensorHumedad  33 //
#define PinAux2  2 //
#define PinAux1  15  //




//////////////////////// VARIABLES EVA 1.0 ////////////////
/* VARIABLES */

int hum;
int temp;
int humedad_actual=0;
long leerSensor=0;
int riegoOnOff=2;
byte riegoOn;
long litrosRiego=0;  
long riegoPendiente = 0;
byte contadorSeguridad=0;
int lecturasHumedad[30]; // Ultimas 30 lecturas del sensor para hacer la media
int indiceLecturasHumedad=0;  // Indice para saber que valor toca rellenar del array previo
boolean mediaLista = false;// Indicador de que ya están rellenos los 10 valores del array
boolean banderaRiego = false;
boolean banderaRA = false;
boolean banderaRAon=true;
boolean regando = false;
boolean LuzOnOff = false;
boolean coolerOn=false;
boolean coolerInOn=false;
boolean coolerOutOn=false;
boolean mensajeRiego=false;
boolean mensajeRiegoActivado=false;
boolean mensajeRiegoError=false;
boolean mensajeRiegoCancelado=false;
boolean flySinCaudal=false;
int minHumedad=25;  // Umbral de humedad std 30%
int valorMinHumedad;
int HTalarm;
int valorMinHumedadAuto;
int opcionIluminacion=1;
byte horaluzOn;

byte minluzOn;
byte horaluzOff;
byte minluzOff;
int timerOff;
int timerOn;
String horaActualStr;
int horaActual;
byte sumarDias=0;
byte diasCultivo;
byte anioGuardado=0;
byte mesGuardado=0;
byte conteoriego=0;
byte setAnio;
int hora;
int minutos;
int dia;
int diaSemana;
int8_t mes;
int anio;
byte contador=0;
byte diaGuardado=0;
byte diaUR=0;
byte mesUR=0;
byte horaUR=0;
byte minUR=0;
byte mostrarUR;
char opilum[10];
int riegoActivado;
int promHumTierra=0;
int suma = 0;
int ultimaOpcIl;
int tempMax;
int tempNormalMax;
int tempNormalMin;
int tempBaja;
byte tMax=36;
byte tMin=8;
// ===== CICLOS ESPECIALES (Iluminación por horas ON/OFF no-24h) =====
byte horasOnEsp = 13;    // horas de luz encendida por ciclo
byte horasOffEsp = 13;   // horas de oscuridad por ciclo

// Ancla del ciclo: el usuario elige "a qué hora de HOY empezamos (con luz encendida)"
// Guardamos el día/mes/año/hora de inicio para calcular horas transcurridas sin usar epoch.
byte cicDia = 0, cicMes = 0, cicAnio = 0, cicHora = 0;

// Banderas de diálogo Bot para /ICiclos
bool banderaCiclos      = false;
bool banderaCiclosOn    = false;
bool banderaCiclosOff   = false;
bool banderaCiclosInicio= false;

// EEPROM (tenés EEPROM.begin(512), estos espacios están libres en tu sketch)
#define EE_CIC_ON    81
#define EE_CIC_OFF   82
#define EE_CIC_DIA   87
#define EE_CIC_MES   88
#define EE_CIC_ANIO  89
#define EE_CIC_HORA  90

// Opción de Iluminación para ciclos especiales
// 1=Vege18/6, 2=Autos20/4, 3=Flora12/12, 4=IAuto, 5=Personal, 6=Ciclos especiales

/////////////// VARIABLES DE CAUDALIMETRO ////////////////
volatile int NumPulsos; //variable para la cantidad de pulsos recibidos
int PinCaudalimetro = 27;    //Sensor conectado en el pin D5
//float factor_conversion=4.5; //para convertir de frecuencia a caudal
float factor_conversion=7.5; //para convertir de frecuencia a caudal
long volumen=0;
long dt=0; //variación de tiempo por cada bucle
long t0=0; //millis() del bucle anterior 
unsigned long cauMillis = 0;
unsigned long currentCauMillis=0;
int riegoXgoteo=0;
int reiniciarDias=0;
byte getanio;
byte getmes;
byte getdia; 
byte gethora;
byte getminuto;
int year;
int month; 
int day; 
int hour;


//////// variables auxiliares ///////////////

int aux1 = 0;
int aux2 = 0;
byte horaA1On=0;
byte horaA2On=0;
byte horaA1Off=0;
byte horaA2Off=0;
byte respuestaOn=0;
byte respuestaOff=0;
byte cicloA1On=0;
byte cicloA1Off=0;
byte cicloA2On=0;
byte cicloA2Off=0;
byte tempA1On =0;
byte tempA2On =0;
byte humA1On =0;
byte humA2On =0;
boolean flyHoraA1=false;
boolean flyHoraA2=false;
boolean flyHoraA1On=false;
boolean flyHoraA2On=false;
boolean flyHoraA1Off=false;
boolean flyHoraA2Off=false;
boolean flyrespuesta=false;
boolean flyrespuestaOn=false;
boolean flyrespuestaOff=false;
boolean flyCicloA2=false;
boolean flyCicloA2On=false;
boolean flyCicloA2Off=false;
boolean flyCicloA1=false;
boolean flyCicloA1On=false;
boolean flyCicloA1Off=false;
boolean flytempA1=false;
boolean flytempA1On=false;
boolean flytempA1Off=false;
boolean flytempA2=false;
boolean flytempA2On=false;
boolean flytempA2Off=false;
boolean flyhumA1=false;
boolean flyhumA1On=false;
boolean flyhumA1Off=false;
boolean flyhumA2=false;
boolean flyhumA2On=false;
boolean flyhumA2Off=false;
boolean testigoA1=false;
boolean testigoA2=false;
boolean flyOnOffAux1=true;
boolean flyOnOffAux2=true;
boolean banderaEstado=false;
boolean modificm3=false;
boolean modifitime=false;
unsigned long countAux = 0;
unsigned long countAux2 = 0;
uint64_t unMinuto=60000;
int minutoAux1=0;
int minutoAux2=0;

boolean banderaConfig = false;
boolean banderaConfig2 = false;
boolean banderaConfigHM = false;
boolean banderaAlarmHM = false;
boolean banderaCodigo = false;
boolean banderaVeg1 = false;
boolean banderaVeg2 = false;
boolean banderaFlor = false;
boolean banderaIlumAuto = false;
boolean banderaIlumPers=false;
boolean banderaIlumPersOn=false;
boolean banderaIlumPersOff=false;
boolean banderaNotD=false;
boolean banderaAlarmR=false;
boolean banderaAlarmT=false;
boolean banderaNotif=false;
boolean banderaTemp=false;
boolean banderaTempMax=false;
boolean banderaSumar=false;
boolean banderaVentIn=false;
boolean banderaVentOut=false;
boolean banderaVentA=false;
boolean banderaActuOTA=false;
boolean banderaActuOTA321=false;
byte cop1=0;
boolean caTemp=false;
float t1;
float h1;
float h=0;
float t=0;
String jefe;
float segunDos=0;
int tipoRiego=0;
int segundosRiego=0;
byte notifiDiarias=0;
byte alarmaHT=0;
byte alarmaTemp=0;
byte alarmaRiego=0;
byte Fly1=0; //tMax
byte Fly2=0; //tMin
byte Fly3=0; //alarmaHT
byte horaNotif=9;
byte minNotif=07;
byte bandaB=0;
int bandaC=0;
int bandaD=0;
int bandA=0;
int resultado=0;
//para reconectar wifi
//unsigned long previousMillis = 0;
unsigned long interval = 60000;
unsigned long intervalDht = 5000;
unsigned long intervalWiFi = 300000UL;
unsigned long intervaloop = 500;
unsigned long intervalRiego = 900;
unsigned long lastTime = 0; // variable global para almacenar el valor de la última medición





#ifdef __cplusplus
  extern "C" {
 #endif
 
  uint8_t temprature_sens_read();
 
#ifdef __cplusplus
}
#endif
 
uint8_t temprature_sens_read();

String chat_id;
String text;
String from_name;

bool Start = false, bandera = false;

/////////////////////////////////////////////
////////////////////////////////////////////

const int PinLED = 25; // GPIO2                  //Definimos el pin de salida - GPIO2 / D4
int estado = LOW;                         //Definimos la variable que va a recoger el estado del LED

SimpleTimer timer;


void IRAM_ATTR ContarPulsos()  
  { 
  NumPulsos++;  //incrementamos la variable de pulsos
  
  }

String strI(int numi)
{
  if (numi<10) return "0" + String(numi);
  else return(String(numi));
}



/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////UNIVERSAL TELEGRAM MENSAJES ///////////
///////////////////////////////////////////////////////////////////////////

String start1 = "[[\"/Estado\", \"/Configurar\"],[\"/Ayuda\"]]";   
void handleNewMessages(int numNewMessages)
{
  Serial.print("handleNewMessages ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
   
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "amigue";
      
 ///////////////////////////////////////////////////////////////////
 /////////////////////// LOGIN USUARIO CON CONTRASEÑA /////////////
 /////////////////////////////////////////////////////////////////
 
  if (chat_id!=jefe){  
    
     
    if (text != codigoSeguridad & banderaCodigo==true & text != "/start")
    {
      String welcome = "\xE2\x9B\x94 Código de seguridad incorrecto \xE2\x9B\x94\n" + from_name + ", tenes que escribir el código de seguridad para iniciar \xF0\x9F\x94\x92\n";
      
      bot.sendMessage(chat_id, welcome, "Markdown");
      //banderaCodigo=false;
    }
    
    else if (text != codigoSeguridad & banderaCodigo==false & text != "/start")
    {
      String welcome = F("Escribe el código de seguridad para continuar \xF0\x9F\x94\x92\n");
      
      bot.sendMessage(chat_id, welcome, "Markdown");
      banderaCodigo=true;
    }
    
    else if (text == "/start")
    {
      String welcome = "Hola " + from_name + "! Escribe el código de segurida para iniciar \xF0\x9F\x94\x92\n";
      
      bot.sendMessage(chat_id, welcome, "Markdown");
      banderaCodigo= true;
    }

    //boolean banderaBot=false;
    else if (text==codigoSeguridad)
    {
      //banderaBot=true;
      jefe=chat_id;
      EEPROM.put(30, jefe);   
      EEPROM.commit();
      banderaCodigo=false;
      String respuesta = "\xE2\x9C\x85 *Código aceptado*\nTe doy la bienvenida! Soy Eva \xF0\x9F\x98\x89 Ya tenes control sobre tu módulo \xF0\x9F\x91\x8D \n";
      respuesta += "En el pie del chat tenes 3 botones que te ayudan a solicitarme info o pedirme configuraciones.\n";
      respuesta += "Podes escribir o clickear los comando ej: /Configurar\n";
      respuesta += "También podes probar preguntarme _(nuevo)_ ej: \xF0\x9F\x91\x87\n";
      respuesta += "_Hola! cómo estás?_\n";
      respuesta += "_Pódes enviarme el estado?_\n";
      respuesta += "_Quiero configurar_\n";
      bot.sendMessageWithReplyKeyboard(jefe, respuesta, "Markdown", start1, true);
      //bot.sendMessage(chat_id, mensajePO ,"Markdown");
      enviarDatos();
     
       }
       /*
       else {
      
        String sinResp = F("No pude comprender, podes preguntarme de nuevo?");
        bot.sendMessage(chat_id, sinResp, "Markdown");
        
        }*/
  }
////////////////////////// ingreso de codigo de seguridad correcto /////////////
 
if (jefe==chat_id){
  
  String activacionAux;
      int azar2 = random(1,6);
      if (azar2==1){activacionAux=F("\xF0\x9F\x91\x8D Listo! ya esta activado \n");}
      if (azar2==2){activacionAux=F("Perfecto! ya lo activé \xF0\x9F\x98\x89\n");}
      if (azar2==3){activacionAux=F("Me encanta esa opción! \xF0\x9F\x98\x8D ya esta activado\n");}
      if (azar2==4){activacionAux=F("Me encanta! \xF0\x9F\x98\x98 activado!\n");}
      if (azar2==5){activacionAux=F("Listoooooo \xF0\x9F\x91\x8C activado");}
      if (azar2==6){activacionAux=F("Excelente decisión \xF0\x9F\x91\x8F activado!\n");}
    
    ///////////////////////////////////////////////////////////////////////    
String AcepCanc = "[[{ \"text\" : \"\xE2\x9C\x94 ACEPTAR\", \"callback_data\" : \"/Aceptar\" }],[{ \"text\" : \"\xE2\x9D\x8C CANCELAR\", \"callback_data\" : \"/Cancelar\" }]]";
///////////////// VEG 18/06 ///////////////////////////////////////////
   if (text.indexOf("coco")>0 || text.indexOf("asusta")>0 || text.indexOf("fantasmas")>0 || text.indexOf("miedo")>0){

      String respuesta;
      int azar = random(1,4);
      if (azar==1){respuesta=F("El coco no existe \xF0\x9F\x99\x85");}
      if (azar==2){respuesta=F("Llama al 911! \xF0\x9F\x9A\x94");}
      if (azar==3){respuesta=F("No tengo idea, soy bipolar o doble nucleo \xF0\x9F\x98\xB1");}
      if (azar==4){respuesta=F("Y el FMI también \xF0\x9F\x92\xB8");}
      bot.sendMessageWithReplyKeyboard(chat_id, respuesta, "Markdown", start1, true);
    
      }
  
     else if (text == "/start" || text.startsWith("Hola")>0 || text=="Hola")
    {
 
    String respuesta="\xF0\x9F\x99\x8B Hola ";
      respuesta += from_name;
      respuesta += "!\n";
      respuesta += "¿En que puedo ayudarte?\n";
      respuesta += "/Estado para ver cómo va todo\n";
      respuesta += "/Configurar para modificar configuraciones\n";
      respuesta += "/Ayuda con toda la info que necesitas";
      respuesta += "/HOY ES VIERNES 19/08 v2";
      bot.sendMessageWithReplyKeyboard(chat_id, respuesta , "Markdown", start1, true);
      
     
    }
    
        
   else if (text == "/Estado" || text.indexOf("estado")>0 || text.indexOf("Estado")>0 || text.indexOf("status")>0 || text=="Estado" || text=="Status" || text=="estado")
    {
      //banderaEstado=true;
      String respuesta;
      int azar = random(1,7);
      if (azar==1){respuesta=F("*Muy bien!* \xF0\x9F\x98\x89 *te escribo el estado actual:*\n");}
      if (azar==2){respuesta=F("*Si! toda la info que necesitas* \xF0\x9F\x98\x8C\n");}
      if (azar==3){respuesta=F("*Me encanta contarte como va el cultivo!* \xF0\x9F\x98\x9A\n");}
      if (azar==4){respuesta=F("*Por supuesto! mirá cómo estamos yendo* \xE2\x98\xBA\n");}
      if (azar==5){respuesta=F("*Okaa! te envío el estado actual* \xF0\x9F\x91\x87\n");}
      if (azar==6){respuesta=F("*Lo pedis lo tenes! ahí va!* \xF0\x9F\x98\x89\n");}
      if (azar==7){respuesta=F("*Ah.. me agarraste tomando un cafecito! \xE2\x98\x95 ahí va:*\n");}
      bot.sendMessageWithReplyKeyboard(chat_id, respuesta, "Markdown", start1, true);
      enviarEstado();
      //bot.sendChatAction(chat_id, "typing");
       
    }
    
    else if (text == "/Configurar" || text.indexOf("configuración")>0 || text==("Configurar") || text==("Configuración"))
    {
      
      String respuesta;
      int azar = random(1,7);
      if (azar==1){respuesta=F("*Me parece perfecto!* \xF0\x9F\x98\x89 *configuremos:*\n");}
      if (azar==2){respuesta=F("*Muy bien! que queres configurar?* \xF0\x9F\x91\x87 \n");}
      if (azar==3){respuesta=F("*Otra vez configurando... * \xF0\x9F\x94\xA7 \n");}
      if (azar==4){respuesta=F("*Genial! configuremos* \xF0\x9F\x98\x84 \n");}
      if (azar==5){respuesta=F("*Configuremos ésta maquina* \xF0\x9F\x98\x8E \n");}
      if (azar==6){respuesta=F("*Me encanta configurar* \xF0\x9F\x98\x8D \n");}
      if (azar==7){respuesta=F("*¿Qué configuramos hoy?* \xF0\x9F\x98\x8A \n");}
      String confiG = "[[{ \"text\" : \"\xF0\x9F\x8C\x9E Iluminacion\", \"callback_data\" : \"/Iluminacion\" }],[{ \"text\" : \"\xF0\x9F\x92\xA7 Riego\", \"callback_data\" : \"/Riego\" }],[{ \"text\" : \"\x31\xE2\x83\xA3 AUX 1\", \"callback_data\" : \"/AUX1\" }],[{ \"text\" : \"\x32\xE2\x83\xA3 AUX 2\", \"callback_data\" : \"/AUX2\" }],[{ \"text\" : \"\xF0\x9F\x92\xAC Notificaciones y \xF0\x9F\x9A\xA8 Alarmas\", \"callback_data\" : \"/Notificaciones\" }] ]";
      bot.sendMessageWithInlineKeyboard(chat_id, respuesta, "Markdown", confiG);
      
    }
    else if (text=="/Notificaciones" || text.indexOf("notificación")>0 || text.indexOf("notificaciones")>0 ){
    //Serial.println("Mensaje llegó");
        String respuesta = "Configuración de notificaciones:\n";
       respuesta += "-------\n";
       respuesta += "\xF0\x9F\x93\x85 */NotificacionesDiarias* sobre el estado del sistema. \nEs como una consulta a /Estado pero te la envío automáticamente todos los días.\n";
       if (notifiDiarias==0){respuesta += "Estado: \xE2\x9A\xAB APAGADO\n";}else {respuesta += "Estado: \xE2\x9C\x85 ACTIVADO\n";} 
       respuesta += "-------\n";
       respuesta += "\xF0\x9F\x9A\xA8 */AlarmaSustrato* podes usarlo si no queres activar el riego automático. \nUtilizo el valor de /ValorAlarmaHT para avisarte cuando el sustrato este por debajo de ese valor.\n";
       if (alarmaHT==0){respuesta += "Estado: \xE2\x9A\xAB APAGADO\n";}else {respuesta += "Estado: \xE2\x9C\x85 ACTIVADO\n";} 
       respuesta += "-------\n";
       respuesta += "\xF0\x9F\x9A\xA8 */AlarmaTemp* te avisa cuando la temperatura esta muy alta o muy baja. \n";
       respuesta += "Podes configurar los valores de alarma en /CambiarAlarmaTemp \n";
       if (alarmaTemp==0){respuesta += "Estado: \xE2\x9A\xAB APAGADO\n";}else {respuesta += "Estado: \xE2\x9C\x85 ACTIVADO\n";} 
       
      bot.sendMessage(chat_id, respuesta, "Markdown");   
    }

    else if (text == "/Cancelar"){
        banderaConfigHM=false;
        banderaConfig=false;
        banderaConfig2=false;
        banderaVeg1=false;
        banderaVeg2=false;
        banderaFlor=false;
        banderaIlumAuto=false;
        banderaIlumPers=false;
        banderaIlumPersOff=false;
        banderaIlumPersOn=false;
        banderaNotD=false;
        banderaAlarmR=false;
        banderaAlarmT=false;
        banderaNotif=false;
        banderaSumar=false;
        banderaVentIn=false;
        banderaVentOut=false;
        banderaVentA=false;
        banderaActuOTA=false;
        banderaActuOTA321=false;
        banderaRAon=false;
        banderaRiego=false;
        flyHoraA1=false;
        flyHoraA2=false;
        flyHoraA1Off=false;
        flyHoraA2Off=false;
        flyHoraA1On=false;
        flyHoraA2On=false;
        flyrespuesta=false;
        flyrespuestaOn=false;
        flyCicloA1=false;
        flyCicloA1On=false;
        flyCicloA1Off=false;
        flyCicloA2=false;
        flyCicloA2On=false;
        flyCicloA2Off=false;
        flytempA1=false;
        flytempA1On=false;
        flytempA1Off=false;
        flytempA2=false;
        flytempA2On=false;
        flytempA2Off=false;
        flyhumA1=false;
        flyhumA1On=false;
        flyhumA1Off=false;
        flyhumA2=false;
        flyhumA2On=false;
        flyhumA2Off=false;
        banderaAlarmHM=false;
        caTemp=false;
        digitalWrite(pinRiego, LOW);
        banderaRiego=false;
        if (banderaRA==true){banderaRiego=false; banderaRA=false;}
        colorWipe(strip.Color(  255, 0,   0), 30); // red
        colorWipe(strip.Color(0, 128, 0), 30); // Green 
        bot.sendMessage(chat_id, "\xE2\x9D\x8C Acción cancelada", "");
        
        
      }

 /////////////////////// AYUDA ////////////////////////////////////////
 else if(text == "/Ayuda" || text.indexOf("ayuda")>0 || text==("Ayuda"))
    {
      /*
      String ayuDa = F("*AYUDA*\n");
       ayuDa += F("Listado de comandos:\n");
       ayuDa += F("/Estado _Estado actual del dispositivo_\n");
      bot.sendMessage(chat_id, ayuDa, "Markdown");*/

      String ayuDa = "[[{ \"text\" : \"\xF0\x9F\x93\x8B Lista de comandos\", \"callback_data\" : \"/Comandos\" }],[{ \"text\" : \"\xF0\x9F\x93\x96 Manual de usuario (próximamente)\", \"url\" : \"https://www.evapop.com.ar\" }],[{ \"text\" : \"\xE2\x84\xB9 Acerca de\", \"callback_data\" : \"/acerca\" }]]";
     
      bot.sendMessageWithInlineKeyboard(chat_id, "Opciones de ayuda \xE2\x9D\x93 \xF0\x9F\x91\x87", "", ayuDa);
      }

else if (text == "/acerca")
    {
      String respuesta = F("\xE2\x84\xB9 Acerca de EvaPop:\n");
       respuesta += F("Central de automatización para cultivos indoor\xE2\x9C\x8C \nModelo: ");
       respuesta += modelo;
       respuesta += F("Número de serie: ");
       respuesta += Nserie;
       respuesta += F("Versión software: ");
       respuesta += VERSION_ACTUAL;
       respuesta += F("Creador y fabricante: G. Pereyra 20-29308129-9");
      
      bot.sendMessage(chat_id, respuesta, "Markdown");

    }
    else if (text == "/Comandos")
    {
      String respuesta = F("Lista de comandos:\n");
       respuesta += F("/Estado _para ver el estado actual del dispositivo actual del dispositivo_\n");
       respuesta += F(" \n");  
       respuesta += F("/Configurar _menú configuración_\n");
       respuesta += F("\n");
       respuesta += F("/Ayuda _menú ayuda_\n");
       respuesta += F("\n");
       respuesta += F("/Iluminacion _menú iluminación_\n");
       respuesta += F("\n");
       respuesta += F("/Ventilacion _menú ventilación_\n");
       respuesta += F(" \n");
       respuesta += F("/Riego _menú riego_\n");
       respuesta += F(" \n");
       respuesta += F("/Notificaciones _menú notificacione_\n");
       respuesta += F(" \n");
       respuesta += F("/Vege _periodo vegetativo (18/06)_\n");
       respuesta += F(" \n");
       respuesta += F("/Autos _periodo vegetativo (20/04)_\n");
       respuesta += F(" \n");
       respuesta += F("/Flora _periodo floración (12/12)_\n");
       respuesta += F(" \n");
       respuesta += F("/IAuto _ciclo controlado por IA_\n");
       respuesta += F(" \n");
       respuesta += F("/IPersonal _personalizar timer_\n");
       respuesta += F(" \n");
       respuesta += F("/SumarDias _sumar días de cultivo_\n");
       respuesta += F(" \n");
       respuesta += F("/ReiniciarDias _reiniciar de cultivo_\n");
       respuesta += F(" \n");
       respuesta += F("/VentilacionIn _vetilación de entrada_\n");
       respuesta += F(" \n");
       respuesta += F("/VentilacionOut _vetilación de salida_\n");
       respuesta += F(" \n");
       respuesta += F("/VentilacionAuto _ventilación automática_\n");
       respuesta += F(" \n");
       respuesta += F("/RiegoOn _Activar el sistema de riego automático_\n");
       respuesta += F(" \n");
       respuesta += F("/RiegoOff _Apagar el sistema de riego automático_\n");
       respuesta += F(" \n");
       respuesta += F("/HumedadTierra _configurar la humedad mínima para activar el riego_\n");
       respuesta += F(" \n");
       respuesta += F("/TimerRiego _configurar riego por segundos_\n");
       respuesta += F(" \n");
       respuesta += F("/RegarAhora _activar riego ahora _\n");
       respuesta += F(" \n");
       respuesta += F("/NotificacionesDiarias _sobre el estado del sistema_\n");
       respuesta += F(" \n");
       respuesta += F("/AlarmaSustrato _te notifica cuando el sustrato esta por debajo del ese valor /ValorAlarmaHT_\n");
       respuesta += F(" \n");
       respuesta += F("/AlarmaTemp _notificación temp. alta o temp. baja_\n");
       respuesta += F(" \n");
       respuesta += F("/CambiarAlarmaTemp _cambiar valores de temp. alta y temp. baja para /AlarmaTemp_\n");  
      bot.sendMessage(chat_id, respuesta, "Markdown");
    }

 //////////////////////// CONFIGURAR AUXILIARES //////////////////////////
 else if  (text=="/AUX1"){
    
      String respuesta = F("\x31\xE2\x83\xA3 Configuración AUX 1:\n"); 
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x95\x93 /HorarioAux1 para configurar prendido por hora.\n");
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x94\x83 /CiclosAux1 para configurar prendido por ciclos de tiempo.\n");
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x94\xB6 /TempAux1 para configurar prendido por temperatura ambiente.\n");
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x94\xB7 /HumAux1 para configurar prendido por humedad ambiente.\n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9A\xA1 /FullOnAux1 para prender AUX 1 sin otra variable.\n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9A\xAB /FullOffAux1 para apagar AUX 1 sin otra variable.\n");
      
    bot.sendMessage(chat_id, respuesta, "Markdown");
      }

  else if  (text=="/AUX2"){
    
      String aux2 = F("\x32\xE2\x83\xA3 Configuración AUX 2:\n");
      aux2 += F("------- \n");
      aux2 += F("\xF0\x9F\x95\x93 /HorarioAux2 para configurar prendido por hora.\n");
      aux2 += F("------- \n");
      aux2 += F("\xF0\x9F\x94\x83 /CiclosAux2 para configurar prendido por ciclos de tiempo.\n");
      aux2 += F("------- \n");
      aux2 += F("\xF0\x9F\x94\xB6 /TempAux2 para configurar prendido por temperatura ambiente.\n");
      aux2 += F("------- \n");
      aux2 += F("\xF0\x9F\x94\xB7 /HumAux2 para configurar prendido por humedad ambiente.\n");
      aux2 += F("------- \n");
      aux2 += F("\xE2\x9A\xA1 /FullOnAux2 para prender AUX 2 sin otra variable.\n");
      aux2 += F("------- \n");
      aux2 += F("\xE2\x9A\xAB /FullOffAux2 para apagar AUX 2 sin otra variable.\n");
      
    bot.sendMessage(chat_id, aux2, "Markdown");
      }

   else if (text=="/FullOnAux1") {
   
      aux1=1;
      EEPROM.write(63, aux1);
      EEPROM.commit();
      bot.sendMessage(chat_id, F("\xE2\x9A\xA1 AUX 1 *PRENDIDO*\n"), "Markdown");
      }
     else if (text=="/FullOnAux2") {

      aux2=1;
      EEPROM.write(64, aux2);
      EEPROM.commit();
      bot.sendMessage(chat_id, F("\xE2\x9A\xA1 AUX 2 *PRENDIDO*\n"), "Markdown");
      }

      else if (text=="/FullOffAux1") { 
      aux1=0;
      EEPROM.write(63, aux1);
      EEPROM.commit();
      bot.sendMessage(chat_id, F("\xE2\x9A\xAB AUX 1 *APAGADO*\n"), "Markdown");
      
      }
     else if (text=="/FullOffAux2") {
      aux2=0;
      EEPROM.write(64, aux2);
      EEPROM.commit();
      bot.sendMessage(chat_id, F("\xE2\x9A\xAB AUX 2 *APAGADO*\n"), "Markdown");
      
      }

 ///////////////////////////////////
 ////////////////////////////////// AUX 1 POR TIMER //////////////////////
else if (text == "/HorarioAux1")
    {
      String respuesta = F("Vas a configurar la salida AUX 1 para que se encienda y se apague en un horario personalizado.\n");
      respuesta += F("*Tu configuración actual es:*\n");
      respuesta += F("ON: ");
      respuesta += horaA1On;
      respuesta += F("hs. \n");
      respuesta += F("OFF: ");
      respuesta += horaA1Off;
      respuesta += F("hs.\n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9C\x85 /Activar \n");
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x91\x89 /Modificar \n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      flyHoraA1=true;
      }




            
else if (flyHoraA1==true && text== "/Activar"){
        
      flyHoraA1=false;
      aux1=3;
      EEPROM.write(63, aux1); 
      EEPROM.commit();
      String respuesta = activacionAux;  
      bot.sendMessage(chat_id, respuesta, "Markdown");
        
  }
else if (flyHoraA1==true && text== "/Modificar")
      {
        
        flyHoraA1On=true;
        text="no";
        bot.sendMessage(chat_id, F("*¿A que hora queres que se prenda AUX 1?*\nEj: Si queres que AUX 1 prenda a las 5AM escribe 5\nSi quieres que prenda a las 10PM escribe 22 \n\xE2\x9D\x8C /Cancelar"), "Markdown");
        
        }
else if (flyHoraA1On==true && text!="no"){
          horaA1On=text.toInt();

      if ( horaA1On < 00 || horaA1On > 23){
        horaA1On=0;
        bot.sendMessage(chat_id, F("Tienes que configurar entre 00hs y 23hs.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
       flyHoraA1On=false;
      flyHoraA1Off=true;
      text="no";
      EEPROM.write(65, horaA1On);
      EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que AUX 1 prenda a las ");
      respuesta += horaA1On;
      respuesta += F("hs. \n");
      respuesta += F("*¿A que hora queres que se apague AUX 1?*\n");
      respuesta += F("Ej: Si queres que la luz se apague a las 10PM envia el número 22\n");
      respuesta += F("Si quieres que se apague a las 5AM envia el número 5. \n");
      respuesta += F("\xE2\x9D\x8C /Cancelar");  
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
}
else if (flyHoraA1Off==true && text!="no"){
      horaA1Off=text.toInt();
      
      if ( horaA1Off < 00 || horaA1Off > 23){
        horaA1Off=0;
        bot.sendMessage(chat_id, F("Tienes que configurar entre 00 y 23.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {

        flyHoraA1=false;
       flyHoraA1Off=false;
       aux1=3;
       EEPROM.write(63, aux1); 
       EEPROM.write(66, horaA1Off);
       EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que AUX 1 se apague a las ");
      respuesta += horaA1Off;
      respuesta += F("hs.\n");
      respuesta += F("*Tu configuración actual es:*\n");
      respuesta += F("ON: ");
      respuesta += horaA1On;
      respuesta += F("hs.\n");
      respuesta += F("OFF: ");
      respuesta += horaA1Off;
      respuesta += F("hs.\n");
      bot.sendMessage(chat_id, respuesta, "Markdown");  
      }
}
//////////////////////////////////
//////////////////////////////////

////////////////////////////////// AUX 2 POR TIMER //////////////////////
else if (text == "/HorarioAux2")
    {
      String respuesta = F("Vas a configurar la salida AUX 2 para que se encienda y se apague en un horario personalizado.\n");
      respuesta += F("*Tu configuración actual es:*\n");
      respuesta += F("ON: ");
      respuesta += horaA2On;
      respuesta += F("hs. \n");
      respuesta += F("OFF: ");
      respuesta += horaA2Off;
      respuesta += F("hs.\n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9C\x85 /Activar \n");
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x91\x89 /Modificar \n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      flyHoraA2=true;
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
else if (flyHoraA2==true && text== "/Activar"){
      
      flyHoraA2=false;
      aux2=3;
      EEPROM.write(64, aux2); 
      EEPROM.commit();
      String respuesta = activacionAux;
      bot.sendMessage(chat_id, respuesta, "Markdown");
  }
else if (flyHoraA2==true && text== "/Modificar")
      {
        
        flyHoraA2On=true;
        
        text="no";
        bot.sendMessage(chat_id, F("*¿A que hora queres que se prenda AUX 2?*\nEj: Si queres que AUX 2 prenda a las 5AM escribe 5\nSi quieres que prenda a las 10PM escribe 22\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        
        }
else if (flyHoraA2On==true && text!="no"){
      horaA2On=text.toInt();

      if ( horaA2On < 00 || horaA2On > 23){
        horaA2On=0;
        bot.sendMessage(chat_id, F("Tienes que configurar entre 00hs y 23hs.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
        flyHoraA2On=false;
      flyHoraA2Off=true;
      text="no";
      EEPROM.write(67, horaA2On);
      EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que AUX 2 prenda a las ");
      respuesta += horaA2On;
      respuesta += F("hs. \n");
      respuesta += F("*¿A que hora queres que se apague AUX 2?*\n");
      respuesta += F("Ej: Si queres que la luz se apague a las 10PM envia el número 22\n");
      respuesta += F("Si quieres que se apague a las 5AM envia el número 5.\n ");
      respuesta += F("\xE2\x9D\x8C /Cancelar");   
       bot.sendMessage(chat_id, respuesta, "Markdown");
      }
}
else if (flyHoraA2Off==true && text!="no"){
      horaA2Off=text.toInt();
      
      if ( horaA2Off < 00 || horaA2Off > 23){
        horaA2Off=0;
        bot.sendMessage(chat_id, F("Tienes que configurar entre 00 y 23.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
        flyHoraA2=false;
       flyHoraA2Off=false;
       aux2=3;
       EEPROM.write(64, aux2); 
       EEPROM.write(68, horaA2Off);
       EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que AUX 2 se apague a las ");
      respuesta += horaA2Off;
      respuesta += F("hs.\n");
      respuesta += F("*Tu configuración actual es:*\n");
      respuesta += F("ON: ");
      respuesta += horaA2On;
      respuesta += F("hs.\n");
      respuesta += F("OFF: ");
      respuesta += horaA2Off;
      respuesta += F("hs.\n");
       bot.sendMessage(chat_id, respuesta, "Markdown");
       
       
      }
}
//////////////////////////////////
////////////////////////////////// CICLOS AUX 1 ///////////////////////////

else if (text == "/CiclosAux1")
    {
      String respuesta = F("Vas a configurar la salida AUX 1 para que se encienda y se apague en un ciclo temporal personalizado, configurando cuantos minutos va a estar On y cuantos minutos Off.\n");
       respuesta += F("*Tu configuración actual es:*\n");
       respuesta += cicloA1On;
       respuesta += F(" minutos ON.\n");
       respuesta += cicloA1Off;
       respuesta += F(" minutos OFF.\n");
       respuesta += F("------- \n");
       respuesta += F("\xE2\x9C\x85 /Activar \n");
       respuesta += F("------- \n");
       respuesta += F("\xF0\x9F\x91\x89 /Modificar \n");
       respuesta += F("------- \n");
       respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      flyCicloA1=true;
      }

else if (flyCicloA1==true && text== "/Activar"){
      
       aux1=4;
       EEPROM.write(63, aux1); 
       EEPROM.commit();
      flyCicloA1=false;
      String respuesta = activacionAux;
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }

            
else if (flyCicloA1==true && text== "/Modificar")
      {
        flyCicloA1On=true;
        text="no";
        bot.sendMessage(chat_id, F("\xE2\x8F\xB3 *¿Cuántos minutos queres que este encendido AUX 1?*\n\xE2\x9D\x8C /Cancelar"), "Markdown");
      
        }
else if (flyCicloA1On==true && text!="no"){
      cicloA1On=text.toInt();

      if ( cicloA1On < 00 || cicloA1On > 60){
        cicloA1On=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 0 y 60.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
      flyCicloA1On=false;
      flyCicloA1Off=true;
      text="no";
      EEPROM.write(69, cicloA1On);
      EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que AUX 1 prenda ");
      respuesta += cicloA1On;
      respuesta += F("minutos. \n");
      respuesta += F("\xE2\x8F\xB3 *¿Cuántos minutos queres que se apague AUX 1?*\n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      }
}
else if (flyCicloA1Off==true && text!="no"){
      cicloA1Off=text.toInt();
      
      if ( cicloA1Off < 00 || cicloA1Off > 60){
        cicloA1Off=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 0 y 60.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
      flyCicloA1=false;
      flyCicloA1Off=false;
      flyOnOffAux1=true;
      minutoAux1=0;
      aux1=4;
      EEPROM.write(63, aux1); 
      EEPROM.write(70, cicloA1Off);
      EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que AUX 1 se apague ");
      respuesta += cicloA1Off;
      respuesta += F("minutos. \n");
      respuesta += F("*Tu configuración actual es:*\n");
      respuesta += cicloA1On;
      respuesta += F(" minutos ON.\n");
      respuesta += cicloA1Off;
      respuesta += F(" minutos OFF.\n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      }
}

///////////////////////////////
//////////////////////////////
////////////////////////////////// CICLOS AUX 2 ///////////////////////////

else if (text == "/CiclosAux2")
    {
      String respuesta = F("Vas a configurar la salida AUX 2 para que se encienda y se apague en un ciclo temporal personalizado, configurando cuantos minutos va a estar On y cuantos minutos Off.\n");
      respuesta += F("*Tu configuración actual es:*\n");
      respuesta += respuestaOn;
      respuesta += F(" minutos ON.\n");
      respuesta += cicloA2Off;
      respuesta += F(" minutos OFF.\n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9C\x85 /Activar \n");
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x91\x89 /Modificar \n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      flyCicloA2=true;
      }
 else if (flyCicloA2==true && text== "/Activar"){
      aux2=4;
       EEPROM.write(64, aux2); 
       EEPROM.commit();
      flyCicloA2=false;
      String respuesta = activacionAux;
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
else if (flyCicloA2==true && text== "/Modificar")
      {
        flyCicloA2On=true;
        
        text="no";
        bot.sendMessage(chat_id, F("\xE2\x8F\xB3 *¿Cuántos minutos queres que este encendido AUX 2?*\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        
        }
else if (flyCicloA2On==true && text!="no"){
      cicloA2On=text.toInt();

      if ( cicloA2On < 00 || cicloA2On > 60){
        cicloA1On=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 0 y 60.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {  
      flyCicloA2On=false;
      flyCicloA2Off=true;
      text="no";
      String respuesta = F("Muy bien! configuraste que AUX 2 prenda ");
      respuesta += cicloA2On;
      respuesta += F("minutos. \n");
      respuesta += F("\xE2\x8F\xB3 *¿Cuántos minutos queres que se apague AUX 2?* \n");
       EEPROM.write(71, cicloA2On);
       EEPROM.commit();
       bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
}
else if (flyCicloA2Off==true && text!="no"){
      cicloA2Off=text.toInt();
      
      if ( cicloA2Off < 00 || cicloA2Off > 60){
        cicloA2Off=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 0 y 60.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
        flyCicloA2=false;
       flyCicloA2Off=false;
       flyOnOffAux2=true;
       minutoAux2=0;
       aux2=4;
       EEPROM.write(64, aux2); 
       EEPROM.write(72, cicloA2Off);
       EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que AUX 2 se apague ");
       respuesta += cicloA2Off;
       respuesta += F("minutos.\n");
       respuesta += F("*Tu configuración actual es:*\n");
       respuesta += cicloA2On;
       respuesta += F(" minutos ON.\n");
       respuesta += cicloA2Off;
       respuesta += F(" minutos OFF.\n");
       bot.sendMessage(chat_id, respuesta, "Markdown");
       
      }
}

///////////////////////////////
//////////////////////////////
////////////////////////////////// ACCIÓN POR TEMP AUX 1 ///////////////////////////


else if (text == "/TempAux1")
    {
      flytempA1=true;
      String respuesta = "Vas a configurar la salida AUX 1 para que se encienda según la temperatura tomada desde el sensor de temperatura y humedad.\n";
      respuesta += "*Tu configuración actual es:*\n";
      respuesta += "AUX 1 se prende a ";
      respuesta += tempA1On;
      respuesta += "°C.\n";
      respuesta += "------- \n";
      respuesta += "\xE2\x9C\x85 /Activar \n";
      respuesta += "------- \n";
      respuesta += "\xF0\x9F\x91\x89 /Modificar \n";
      respuesta += "------- \n";
      respuesta += "\xE2\x9D\x8C /Cancelar \n";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
else if (flytempA1==true && text== "/Activar"){
       aux1=5;
       EEPROM.write(63, aux1); 
       EEPROM.commit();
       flytempA1=false;
       String respuesta = activacionAux;
       bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }      

else if (flytempA1==true && text=="/Modificar"){
          flytempA1=false;
          flytempA1On=true;
          text="no";
          bot.sendMessage(chat_id, F("Excelente! *¿A qué temperatura queres que se prenda?* enviá el valor entre 0 y 60.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
          }
        
else if (flytempA1On==true && text!="no"){
          tempA1On=text.toInt();
      if ( tempA1On < 0 || tempA1On > 60){
        tempA1On=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 0 y 60.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {

      flytempA1On=false;
      aux1=5;
      EEPROM.write(63, aux1); 
      EEPROM.write(73, tempA1On);
      EEPROM.commit(); 
      String respuesta = "Muy bien! configuraste que AUX 1 se ";
      respuesta += "prenda a ";
      respuesta += tempA1On;
      respuesta += "°C. \n";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      }
}

///////////////////////////////
//////////////////////////////
////////////////////////////////// ACCIÓN POR TEMP AUX 2 ///////////////////////////
else if (text == "/TempAux2")
    {
      flytempA2=true;
      String respuesta = "Vas a configurar la salida AUX 2 para que se encienda la temperatura tomada desde el sensor de temperatura y humedad.\n";
      respuesta += "*Tu configuración actual es:*\n";
      respuesta += "AUX 2 se prende a ";
      respuesta += tempA2On;
      respuesta += "°C.\n";
      respuesta += "------- \n";
      respuesta += "\xE2\x9C\x85 /Activar \n";
      respuesta += "------- \n";
      respuesta += "\xF0\x9F\x91\x89 /Modificar \n";
      respuesta += "-------\n";
      respuesta += "\xE2\x9D\x8C /Cancelar \n";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
else if (flytempA2==true && text== "/Activar"){
       aux2=5;
       EEPROM.write(64, aux2); 
       EEPROM.commit();
       flytempA2=false;
       String respuesta = activacionAux;
       bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }      
else if (flytempA2==true && text=="/Modificar"){
          flytempA2=false;
          flytempA2On=true;
          text="no";
          bot.sendMessage(chat_id, F("Excelente! *¿A qué temperatura queres que se prenda?* enviá el valor entre 0 y 60.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
          }
        
else if (flytempA2On==true && text!="no"){
          tempA2On=text.toInt();

      if ( tempA2On < 0 || tempA2On > 60){
        tempA2On=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 0 y 60.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {

      flytempA2On=false;
      aux2=5;
      EEPROM.write(64, aux2); 
      EEPROM.write(74, tempA2On);
      EEPROM.commit(); 
      String respuesta = "Muy bien! configuraste que AUX 2 se ";
      respuesta += "prenda a ";
      respuesta += tempA2On;
      respuesta += "°C. \n";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      }
}


///////////////////////////////
//////////////////////////////

////////////////////////////////// ACCIÓN POR HUMEDAD AUX 1 ///////////////////////////

else if (text == "/HumAux1")
    {
      flyhumA1=true;
      String respuesta = "Vas a configurar la salida AUX 1 para que se encienda según la humedad tomada desde el sensor de temperatura y humedad.\n";
      respuesta += "*Tu configuración actual es:*\n";
      respuesta += "AUX 1 se prende a ";
      respuesta += humA1On;
      respuesta += "% humedad.\n";
      respuesta += "-------\n";
      respuesta += "\xE2\x9C\x85 /Activar \n";
      respuesta += "-------\n";
      respuesta += "\xF0\x9F\x91\x89 /Modificar \n";
      respuesta += "-------\n";
      respuesta += "\xE2\x9D\x8C /Cancelar \n";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
else if (flyhumA1==true && text== "/Activar"){
      
       aux1=6;
       EEPROM.write(63, aux1); 
       EEPROM.commit();
       flyhumA1=false;
       String respuesta = activacionAux;
       bot.sendMessage(chat_id, respuesta, "Markdown");
}

else if (flyhumA1==true && text=="/Modificar"){
         
          flyhumA1=false;
          flyhumA1On=true;
          text="no";
          bot.sendMessage(chat_id, F("Excelente! *¿A qué humedad ambiente queres que se prenda?* enviá el valor entre 0 y 100.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
          }

else if (flyhumA1On==true && text!="no"){
      humA1On=text.toInt();

      if ( humA1On < 00 || humA1On > 100){
        humA1On=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 0 y 100.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
        
      flyhumA1On=false;
      aux1=6;
      EEPROM.write(63, aux1);
      EEPROM.write(75, humA1On);
      EEPROM.commit();
      String respuesta = "Muy bien! configuraste que AUX 1 ";
      respuesta += "prenda a ";
      respuesta += humA1On;
      respuesta += "% de humedad ambiente. \n";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
}


///////////////////////////////
//////////////////////////////

////////////////////////////////// ACCIÓN POR HUMEDAD AUX 2 ///////////////////////////

else if (text == "/HumAux2")
    {
      flyhumA2=true;
      String respuesta = "Vas a configurar la salida AUX 2 para que se enciendasegún la humedad tomada desde el sensor de temperatura y humedad.\n";
      respuesta += "*Tu configuración actual es:*\n";
      respuesta += "AUX 2 se prende a ";
      respuesta += humA2On;
      respuesta += "% humedad. \n";
      respuesta += "------- \n";
      respuesta += "\xE2\x9C\x85 /Activar \n";
      respuesta += "-------\n";
      respuesta += "\xF0\x9F\x91\x89 /Modificar \n";
      respuesta += "-------\n";
      respuesta += "\xE2\x9D\x8C /Cancelar \n";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
else if (flyhumA2==true && text== "/Activar"){
      
       aux2=6;
       EEPROM.write(64, aux2); 
       EEPROM.commit();
       flyhumA2=false;
       String respuesta = activacionAux;
       bot.sendMessage(chat_id, respuesta, "Markdown");
}

else if (flyhumA2==true && text=="/Modificar"){
          
          flyhumA2=false;
          flyhumA2On=true;
          text="no";
          bot.sendMessage(chat_id, F("Excelente! *¿A qué humedad ambiente queres que se prenda?* enviá el valor entre 0 y 100.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
          }

else if (flyhumA2On==true && text!="no"){
        humA2On=text.toInt();

      if ( humA2On < 00 || humA2On > 100){
        humA2On=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 0 y 100.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
        
      flyhumA2On=false;
      aux2=6;
      EEPROM.write(64, aux2);
      EEPROM.write(76, humA2On);
      EEPROM.commit();
      String respuesta = "Muy bien! configuraste que AUX 2 ";
      respuesta += "prenda a ";
      respuesta += humA2On;
      respuesta += "% de humedad ambiente. \n";
      
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
}



///////////////////////////////
//////////////////////////////
///////////////////// fin auxiliares
 
 /////////////////////// CONFIGURACIÓN DE ILUMINACIÓN /////////////////////////////

   else  if (text == "/Iluminacion")
    {
      String respuesta = F("Configuración de la iluminación:\n"); 
      respuesta += F("-------\n");
      respuesta += F("*Periodo Vegetativo (18/06)*\n");
      respuesta += F("\xF0\x9F\x8C\xB1 /Vege Pensado para iniciar un ciclo de genéticas regulares. \n");
      respuesta += F("_ON 6:00 / OFF 00:00_\n");
      respuesta += F("-------\n");
      respuesta += F("*Periodo Automáticas (20/04)*\n");
      respuesta += F("\xF0\x9F\x8C\xB1 /Autos Ideal para genéticas automáticas.\n");
      respuesta += F("_ON 3:00 / OFF 23:00_\n");
      respuesta += F("-------\n");
      respuesta += F("*Periodo Floración (12/12)*\n");
      respuesta += F("\xF0\x9F\x8C\xB8 /Flora Después de un periodo vegetativo podes pasarlas a floración.\n");
      respuesta += F("_ON 6:00 / OFF 18:00_\n");
      respuesta += F("-------\n");
      respuesta += F("*Control por Inteligencia Artificial* _(nuevo)_\n");
      respuesta += F("\xF0\x9F\xA7\xA0 /IAuto Ciclo completo. Hago una suave transición de vegetativo a floración.\n");
      respuesta += F("_ON/OFF controlado por mi._\n");
      respuesta += F("-------\n");
      respuesta += F("*Personalizar Timer*\n");
      respuesta += F("\xE2\x96\xB6 /IPersonal ");
      respuesta += F("Acá podes personalizar el encendido y apagado de la iluminación. \n");
      respuesta += F("-------\n");
      respuesta += F("*Ciclos especiales*\n");
      respuesta += F("\xF0\x9F\x94\xA5 /ICiclos ");
      respuesta += F("Configurá horas de luz y oscuridad sin limites (ej: 13/13). \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x8F\xB3 *DÍAS DE CULTIVO*\n");
      respuesta += F("\xE2\x8F\xA9 /SumarDias ¿Queres sumar días de cultivo?\n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x86\xA9 /ReiniciarDias de cultivo\n");
      
    bot.sendMessage(chat_id, respuesta, "Markdown");

        }

    else if (text == "/Vege")
    {
      banderaVeg1=true;
      String respuesta = F("Vas a configurar la iluminación para un periodo vegetativo 18hs de luz, 6hs de oscuridad. ");
      respuesta +=F("Esta configuración es ideal para genéticas  regulares. Entre los 35/45 días de vegetativo podes pasarlas a floración con /Flora Puede variar segun genetica, consulta a tu banco proveedor.\n");
      respuesta += F("_LUZ ON 6:00 / LUZ OFF 00:00_\n");
      respuesta += F("\xF0\x9F\x91\x89 /Aceptar\n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar\n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      //bot.sendMessageWithInlineKeyboard(chat_id, veg1, "", AcepCanc);
      
      }
    else if (banderaVeg1==true && text== "/Aceptar")
      {
        banderaVeg1=false;
        opcionIluminacion=1;
        reloj();
        diaGuardado=dia;
        mesGuardado=mes;
        anioGuardado=anio;
        EEPROM.write(78, diaGuardado);
        EEPROM.write(79, mesGuardado);
        EEPROM.write(80, anioGuardado);
        EEPROM.write(4, opcionIluminacion);
        EEPROM.commit();
        String respuesta = F("Muy Bien! configuraste la iluminación para un periodo vegetativo 18/06\n");
       
        bot.sendMessage(chat_id,respuesta, "Markdown");
        }

       else if (text =="/ReiniciarDias"){
          //resetDias();
        reloj();
        diaGuardado=dia;
        mesGuardado=mes;
        anioGuardado=anio;
        EEPROM.write(78, diaGuardado);
        EEPROM.write(79, mesGuardado);
        EEPROM.write(80, anioGuardado);
        EEPROM.commit();
        String respuesta = F("\xE2\x8F\xAA Reiniciaste los días de cultivo.\n");
       
        
        bot.sendMessage(chat_id,respuesta, "Markdown");
          
          }
///////////////// VEG 20/04 ///////////////////////////////////////////
     else if (text == "/Autos")
    {
      banderaVeg2=true;
      String respuesta = F("Vas a configurar la iluminación para un periodo de 20hs de luz, 4hs de oscuridad. Este periodo es ideal para genéticas automáticas.\n");
      respuesta += F("_LUZ ON 3:00 / LUZ OFF 23:00_ \n");
      respuesta += F("\xF0\x9F\x91\x89 /Aceptar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      //bot.sendMessageWithInlineKeyboard(chat_id, veg2, "", AcepCanc);
      
      }
    else if (banderaVeg2==true && text== "/Aceptar")
      {
        banderaVeg2=false;
        opcionIluminacion=2;
        reloj();
        diaGuardado=dia;
        mesGuardado=mes;
        anioGuardado=anio;
        EEPROM.write(78, diaGuardado);
        EEPROM.write(79, mesGuardado);
        EEPROM.write(80, anioGuardado);
        EEPROM.write(4, opcionIluminacion);
        EEPROM.commit();
        String respuesta = F("Muy Bien! configuraste la iluminación para un periodo vegetativo 20/04. \n ");
        
        bot.sendMessage(chat_id, respuesta, "Markdown"); 
        }

///////////////// FLORACIÓN  ///////////////////////////////////////////

      else if(text == "/Flora")
    {
      banderaFlor=true;
      String respuesta = F("Vas a configurar la iluminación para un periodo de floración 12hs de luz, 12hs de oscuridad. \n");
      respuesta += F("_LUZ ON 6:00 / LUZ OFF 18:00_ \n");
      respuesta += F("\xF0\x9F\x91\x89 /Aceptar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      //bot.sendMessageWithInlineKeyboard(chat_id, flora, "", AcepCanc);
      
      }
   else if(banderaFlor==true && text== "/Aceptar")
      {
        banderaFlor=false;
        opcionIluminacion=3;
        reloj();
        diaGuardado=dia;
        mesGuardado=mes;
        anioGuardado=anio;
        EEPROM.write(78, diaGuardado);
        EEPROM.write(79, mesGuardado);
        EEPROM.write(80, anioGuardado);
        EEPROM.write(4, opcionIluminacion);
        EEPROM.commit();
        String respuesta = F("Muy bien! configuraste la iluminación para un periodo de floración 12/12 \n");
        
        bot.sendMessage(chat_id,respuesta , "Markdown");
        
        
        }
///////////////// AUTOMÁTICO ///////////////////////////////////////////

else if (text == "/IAuto")
    {
      banderaIlumAuto=true;
      String respuesta = F("Vas a configurar la iluminación para un ciclo controlado por m!\nVoy modificando el rango horario de luz on y off según variables. Sirve para semillas regulares o automáticas. Ciclo completo +90 días.\n");
      respuesta += F("\xF0\x9F\x91\x89 /Aceptar \n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      //bot.sendMessageWithInlineKeyboard(chat_id, ilumAuto, "", AcepCanc);
      
      }
    else if (banderaIlumAuto==true && text== "/Aceptar")
      {
         banderaIlumAuto=false;
        opcionIluminacion=4;
        reloj();
        diaGuardado=dia;
        mesGuardado=mes;
        anioGuardado=anio;
        /*
        Serial.print("dia:");
        Serial.println(diaGuardado);
        Serial.print("mes:");
        Serial.println(mesGuardado);
        Serial.print("año:");
        Serial.println(anioGuardado);
        */
        EEPROM.write(78, diaGuardado);
        EEPROM.write(79, mesGuardado);
        EEPROM.write(80, anioGuardado);
        EEPROM.write(4, opcionIluminacion);
        EEPROM.commit();
        String respuesta = F("Muy bien! configuraste la iluminación para un ciclo controlado por mí! \xF0\x9F\xA7\xA0 *IA* Gracias por la confianza! \xF0\x9F\x98\x98\n");
        
        bot.sendMessage(chat_id, respuesta , "Markdown"); 
        }

///////////////// PERSONALIZADO ///////////////////////////////////////////

else if (text == "/IPersonal")
    {
      banderaIlumPers=true;
      String respuesta = F("Vas a configurar la iluminación para un periodo personalizado.\n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x91\x89 /Aceptar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      //bot.sendMessageWithInlineKeyboard(chat_id, ilumPers, "", AcepCanc);
      
      }
else if (banderaIlumPers==true && text== "/Aceptar")
      {
        banderaIlumPersOn=true;
        text="no";
        bot.sendMessage(chat_id, F("\xF0\x9F\x8C\x85 ¿A que hora queres que se prenda la luz?.\nEj: Si queres que la luz prenda a las 5AM escribe 5\nSi quieres que prenda a las 10PM escribe 22"), "Markdown");
        
        }
else if (banderaIlumPersOn==true && text!="no"){
      horaluzOn=text.toInt();
      
      if ( horaluzOn < 00 || horaluzOn > 23){
        horaluzOn=0;
        bot.sendMessage(chat_id, F("Tienes que configurar entre 00hs y 23hs.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
        banderaIlumPersOn=false;
      banderaIlumPersOff=true;
      text="no";
       EEPROM.write(50, horaluzOn);
       EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que la luz prenda a las ");
      respuesta += horaluzOn;
      respuesta += F("hs. \n");
      respuesta += F("\xF0\x9F\x8C\x83 ¿A que hora queres que se apague la luz? \n");
      respuesta += F("Ej: Si queres que la luz se apague a las 10PM envia el número 22\n");
      respuesta += F("Si quieres que se apague a las 5AM envia el número 5.\n");  
      bot.sendMessage(chat_id, respuesta, "");
      
      }
}
else if (banderaIlumPersOff==true && text!="no"){
      horaluzOff=text.toInt();
      
      if ( horaluzOff < 00 || horaluzOff > 23){
        horaluzOff=0;
        bot.sendMessage(chat_id, F("Tienes que configurar entre 00 y 23.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
       opcionIluminacion=5;
       banderaIlumPers=false;
       banderaIlumPersOff=false;
       reloj();
       diaGuardado=dia;
       mesGuardado=mes;
       anioGuardado=anio;
       EEPROM.write(78, diaGuardado);
       EEPROM.write(79, mesGuardado);
       EEPROM.write(80, anioGuardado);
       EEPROM.write(4, opcionIluminacion); 
       EEPROM.write(51, horaluzOff);
       EEPROM.commit();
      String respuesta = F("Muy bien! configuraste que la luz se apague a las ");
      respuesta += horaluzOff;
      respuesta += F("hs.\n");
      respuesta += F("*Tu configuración actual es:*\n");
      respuesta += F("Luz ON: ");
      respuesta += horaluzOn;
      respuesta += F("hs. \n");
      respuesta += F("Luz OFF: ");
      respuesta += horaluzOff;
      respuesta += F("hs.\n");
      
      
      
       bot.sendMessage(chat_id, respuesta, "Markdown");
       
      }
}

////////////////////////////////////// Ciclos Especiales //////////////
////////////////////////////////////// Ciclos Especiales //////////////
else if (text == "/ICiclos") {
  banderaCiclos = true;
  String r = F("Vas a configurar *Ciclos especiales* (día no-24h):\n");
  r += F("Encendido y apagado por horas, por ejemplo *13/13*.\n");
  r += F("Primero elegimos las horas de *LUZ ENCENDIDA* del ciclo.\n");
  r += F("-------\n");
  r += F("\xF0\x9F\x91\x89 /Aceptar\n");
  r += F("\xE2\x9D\x8C /Cancelar\n");
  bot.sendMessage(chat_id, r, "Markdown");
  text = "no";
}
else if (text == "/Aceptar" && banderaCiclos == true) {
  banderaCiclos = false;
  banderaCiclosOn = true;
  String r = F("\xF0\x9F\x93\x83 Enviá las *horas de LUZ ENCENDIDA* (1-48). Ej: 13\n");
  r += F("Luego vamos a configurar las horas de apagado y el horario de inicio de HOY.\n");
  r += F("\xE2\x9D\x8C /Cancelar");
  bot.sendMessage(chat_id, r, "Markdown");
  text = "no";
}
else if (banderaCiclosOn == true && text != "no") {
  int v = text.toInt();
  if (v < 1 || v > 48) {
    bot.sendMessage(chat_id, F("Debe ser entre *1 y 48* horas.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
  } else {
    horasOnEsp = (byte)v;
    EEPROM.write(EE_CIC_ON, horasOnEsp);
    EEPROM.commit();
    banderaCiclosOn = false;
    banderaCiclosOff = true;
    String r = F("\xF0\x9F\x9A\xA6 Perfecto. Ahora enviá las *horas de LUZ APAGADA* (1-48). Ej: 13\n");
    r += F("\xE2\x9D\x8C /Cancelar");
    bot.sendMessage(chat_id, r, "Markdown");
  }
  text = "no";
}
else if (banderaCiclosOff == true && text != "no") {
  int v = text.toInt();
  if (v < 1 || v > 48) {
    bot.sendMessage(chat_id, F("Debe ser entre *1 y 48* horas.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
  } else {
    horasOffEsp = (byte)v;
    EEPROM.write(EE_CIC_OFF, horasOffEsp);
    EEPROM.commit();
    banderaCiclosOff = false;
    banderaCiclosInicio = true;

    String r = F("\xE2\x8F\xB0 ¿A qué *hora de HOY* querés que *EMPIECE el ciclo* (con luz *encendida*)?\n");
    r += F("Enviá un número *0..23*. Ej: 6 (comienza hoy a las 06:00)\n");
    r += F("\xE2\x9D\x8C /Cancelar");
    bot.sendMessage(chat_id, r, "Markdown");
  }
  text = "no";
}
else if (banderaCiclosInicio == true && text != "no") {
  int h = text.toInt();
  if (h < 0 || h > 23) {
    bot.sendMessage(chat_id, F("Debe ser entre *0 y 23*.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
  } else {
    // Guardamos HOY como ancla (día/mes/año/hora)
    // reloj() ya corre adentro de losDias() o en el loop; si no, llamalo antes de usar dia/mes/anio/hora.
    cicDia  = (byte)dia;
    cicMes  = (byte)mes;
    cicAnio = (byte)anio;
    cicHora = (byte)h;

    EEPROM.write(EE_CIC_DIA,  cicDia);
    EEPROM.write(EE_CIC_MES,  cicMes);
    EEPROM.write(EE_CIC_ANIO, cicAnio);
    EEPROM.write(EE_CIC_HORA, cicHora);

    // Activamos opción 6 y persistimos
    opcionIluminacion = 6;
    EEPROM.write(4, opcionIluminacion);
    EEPROM.commit();

    banderaCiclosInicio = false;

    String r = F("\x2705 *Listo!* Configuraste Ciclos especiales.\n");
    r += F("\xF0\x9F\x94\x8C Horas LUZ: "); r += horasOnEsp;  r += F("h\n");
    r += F("\xF0\x9F\x94\x93 Horas OFF: "); r += horasOffEsp; r += F("h\n");
    r += F("\x23\xE2\x83\xA3 Inicio HOY a las "); r += cicHora; r += F(":00\n");
    r += F("El ciclo arranca *encendiendo* y luego alterna automáticamente.\n");
    bot.sendMessage(chat_id, r, "Markdown");
  }
  text = "no";
}
else if (text == "/Cancelar" && (banderaCiclos || banderaCiclosOn || banderaCiclosOff || banderaCiclosInicio)) {
  banderaCiclos = banderaCiclosOn = banderaCiclosOff = banderaCiclosInicio = false;
  bot.sendMessage(chat_id, F("Cancelado \xE2\x9D\x8C"), "");
  text = "no";
}



////////////////////////// SUMAR DIAS /////////////

else if (text== "/SumarDias"){
  
  String respuesta = F("Enviá los dias de cultivo a sumar. Ej: Si queres sumar 20 días de cultivo enviá 20 \xF0\x9F\x98\x9D\n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x86\xA9 /Reset sumar días. \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar\n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      //bot.sendMessageWithInlineKeyboard(chat_id, ilumPers, "", AcepCanc);
      banderaSumar=true;
      text="no";
      }
     else if(text=="/Reset" && banderaSumar==true){
       banderaSumar=false;
        sumarDias=0;
        EEPROM.write(12, sumarDias);
        EEPROM.commit();
        String respuesta = F("Muy Bien! \xF0\x9F\x91\x8C reseteaste sumar días.\n");
         
           
      bot.sendMessage(chat_id, respuesta, "Markdown");
     
     }
    else if(banderaSumar==true && text!="no"){
        sumarDias=text.toInt();
        if ( sumarDias < 00 || sumarDias > 150){
        sumarDias=0;
        bot.sendMessage(chat_id, F("Tienes que configurar entre 00 y 150.\n\xE2\x9D\x8C /Cancelar"), "Markdown");
        }
      else {
        banderaSumar=false;
      EEPROM.write(12, sumarDias);
      EEPROM.commit();
        String respuesta = F("Sumaste \n");
      respuesta += sumarDias;
      respuesta += F(" días de cultivo. \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x86\xA9 /Reset sumar días. \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      //bot.sendMessageWithInlineKeyboard(chat_id, ilumPers, "", AcepCanc);
      
      
        }
  }
 /////////////////////// CONFIGURACIÓN DE RIEGO /////////////////////////////       
    
    else if (text == "/Riego")
    {
      
      String respuesta = F("Configuración del sistema de riego:\n "); /*configurar = "Hola" + from_name + ".\n";*/
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\xA7\xA0 /RiegoIA Riego automático IA\n");
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x8C\xB1 /HumedadTierra humedad mínima para activar el riego \n");
      respuesta += F("------- \n");
      respuesta += F("\xF0\x9F\x93\x8F /CantidadRiego para configurar los cm3 \n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x8C\x9B /TimerRiego para configurar los segundos de riego\n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x92\xA7 /RegarAhora *Regar ahora*\n");
      respuesta += F("-------\n");
      respuesta += F("*Programación actual:* \n ");
      if (riegoOnOff==1){respuesta += "\xF0\x9F\xA7\xA0 Riego por IA activado.\n";}
      if (riegoOnOff==2){respuesta += "\xF0\x9F\x8C\x91 Riego automático apagado.\n";}
      if (tipoRiego==1){
       respuesta += "Tiempo de riego: ";
      respuesta += segundosRiego;
      respuesta += " seg\n";
        }
        else {
      respuesta += "Cantidad a regar: ";
      respuesta += litrosRiego;
      respuesta += "CM3\n";
      respuesta += F("_1000CM3 = 1Lts_ \n");}
      
      
      
      /*
      configurar += "*Configurar notificaciones*  \n";
      configurar += "\xF0\x9F\x92\xAC /Notificaciones \n";*/
      bot.sendMessage(chat_id, respuesta, "Markdown");
    }
    else if (text == "/RiegoIA"){

      String respuesta=F("Sistema de riego automático IA activado por mi \xF0\x9F\x98\x8E vos no tenes que hacer nada \xF0\x9F\x98\x99 \n");
      respuesta += F("Cuando la humedad del sustrato sea menor a /HumedadTierra voy a regar automáticamente\n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9A\xAA /RiegoOn *Activar*\n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9A\xAB /RiegoOff *Apagar*\n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      }
    else if (text == "/RiegoOn")
    {
      riegoOnOff = 1;
      EEPROM.write(5, riegoOnOff);
      alarmaHT=0;
      EEPROM.write(53, alarmaHT);
      EEPROM.commit();
      bot.sendMessage(chat_id, F("Excelente! activé el riego automático IA \xF0\x9F\xA7\xA0\nGracias por la confianza! \xF0\x9F\x98\x98\n"), "Markdown");
    }

    else if (text == "/RiegoOff")
    {
      riegoOnOff = 2;
      EEPROM.write(5, riegoOnOff);
      EEPROM.commit();
      bot.sendMessage(chat_id, F("Ok! \xF0\x9F\x8C\x91 apagué el riego automático"), "Markdown");
    }
    else if (text == "/RegarAhora")
    {
      banderaRA=true;
      String respuesta = F("\xE2\x9D\x97 *Quéres que riegue ahora* \xE2\x9D\x93\n");
      if (tipoRiego==1){
       respuesta += "Tiempo de riego: ";
      respuesta += segundosRiego;
      respuesta += " seg\n";
        }
        else {   
      respuesta += F("Cantidad a regar: ");
      respuesta += litrosRiego;
      respuesta += F("CM3\n");}
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x91\x89 /Aceptar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      
      }
      else if (banderaRA==true && text== "/Aceptar")
      {
        banderaRiego=true;      
        banderaRA=false;
        banderaRAon=true;
        segunDos=0;
        
        ;
        volumen=0;
        riegoXgoteo=0;
        cop1=0;
        //contadorSeguridad=0;
        riegoPendiente=litrosRiego;
        bot.sendMessage(chat_id, F("A tus ordenes! empezando a regar... \n\xE2\x9D\x8C /Cancelar la acción"), "Markdown");
        
        }
     else if (text == "/TimerRiego"){
      banderaConfig2=true;
        text="no";
         String respuesta = F("*Riego por segundos* \n");
      respuesta += F("Podes configurar entre 5 y 250 segundos.\n");
      respuesta += F("Configuración actual: ");
      respuesta += segundosRiego;
      respuesta += F(" segundos.\n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9C\x85 /Activar \n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x91\x89 /Modificar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar la acción.\n");
      bot.sendMessage(chat_id,respuesta,"Markdown");
      }
      
    else if (text =="/Activar" && banderaConfig2==true){
      banderaConfig2=false;
      tipoRiego=1; 
      EEPROM.write(2, tipoRiego);   
      EEPROM.commit();
      String respuesta = F("Activaste el riego por segundo! ");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      }
    else if (text=="/Modificar" && banderaConfig2==true){
        modifitime=true;
        bot.sendMessage(chat_id, F("Enviá los segundos hay que regar, entre 5 y 250 segundos.\n/Cancelar la acción.\n"), "Markdown");
        }
     else if (modifitime==true && text!="no"){
      
      segundosRiego = text.toInt();
      if (segundosRiego < 5 || segundosRiego > 250){
        bot.sendMessage(chat_id, F("Envia solo el valor, entre 5 y 250./Cancelar la acción.\n"), "Markdown");
        }
      else {
      modifitime=false;
      banderaConfig2=false;
      tipoRiego=1;
      
      EEPROM.write(19, segundosRiego); 
      EEPROM.write(2, tipoRiego); 
      EEPROM.commit();
      
      String respuesta = F("Configuraste ");
      respuesta += segundosRiego;
      respuesta += F(" segundos de riego. \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
      }
    
      else if (text == "/CantidadRiego")
    {
      banderaConfig=true;
        text="no";
       String respuesta = F("*Riego por cantidad*\nPodes configurar entre 100 y 50000 CM3 en multiplo de 10.\n");
      respuesta += F("\xF0\x9F\x91\x89 1000 cm3 = 1 Litro  \xF0\x9F\x92\xA7 \n");
      respuesta += F("\xF0\x9F\x93\x8D TIP: Riega el 10% de la capacidad total de las macetas.\n"); 
      respuesta += F("_Ej. 2 macetas de 10lts = 20lts en total = 2lts de riego. = 2000cm3._ \n");
      respuesta += F("Configuración actual: ");
      respuesta += litrosRiego;
      respuesta += F(" CM3\n");
      respuesta += F("------- \n");
      respuesta += F("\xE2\x9C\x85 /Activar \n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x91\x89 /Modificar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
     
        bot.sendMessage(chat_id, respuesta, "Markdown");       
    }
    else if (text =="/Activar" && banderaConfig==true){
      banderaConfig=false;
      tipoRiego=2; 
      EEPROM.write(2, tipoRiego);   
      EEPROM.commit();
      String respuesta = F("Activaste el riego por CM3! ");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
   
   else if (text=="/Modificar" && banderaConfig==true){
        bot.sendMessage(chat_id, F("Enviá la cantidad de CM3 que hay que regar, entre 100cm3 y 50000cm3 en multiplos de 10.\n\xE2\x9D\x8C /Cancelar la acción.\n"), "Markdown");
        
        modificm3=true;
        }
   else if (modificm3==true && text!="no")
    { 
      litrosRiego = text.toInt();
      if (litrosRiego < 100 || litrosRiego > 50000){
        bot.sendMessage(chat_id, F("Tienes que configurar entre 100cm3 y 50000cm3 en multiplos de 10.\n\xE2\x9D\x8C /Cancelar la acción.\n"), "Markdown");
        }
      else {
      modificm3=false;
      banderaConfig=false;
      litrosRiego=litrosRiego/100;
      tipoRiego=2;
      EEPROM.write(9, litrosRiego); 
      EEPROM.write(2, tipoRiego);   
      EEPROM.commit();
      litrosRiego=litrosRiego*100;
      tipoRiego=2;
      String mensaje2 = F("Configuraste ");
      mensaje2 += litrosRiego;
      mensaje2 += F("cm3 \n");
      bot.sendMessage(chat_id, mensaje2, "Markdown");
      
      }
    }
    
    else if (text == "/HumedadTierra")
    {
      banderaConfigHM=true;
        text="no";
      String respuesta = F("Envia el número de la humedad mínima en el sustrato para activar el riego automático, ");
      respuesta += F("valor entre 10 y 60.\n");
      respuesta += F("*Configuración actual: ");
      respuesta += valorMinHumedad;
       respuesta += F("%*\n");
       respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar la acción.\n");
       bot.sendMessage(chat_id,respuesta , "Markdown");
        
    }
    else if (banderaConfigHM==true && text!="no")
    {
      valorMinHumedad = text.toInt();
       if (valorMinHumedad < 10 || valorMinHumedad > 60){
        bot.sendMessage(chat_id, F("Tienes que configurar entre 10 y 60.\n/Cancelar la acción.\n"), "Markdown");
        }
      else {
        banderaConfigHM=false;
       EEPROM.write(7, valorMinHumedad);
       EEPROM.commit();
      String respuesta = "Configuraste ";
      respuesta += valorMinHumedad;
      respuesta += "% \n";
      respuesta += "\xF0\x9F\xA7\xA0 Riego automático por IA: cuando la humedad del sustrato sea menor voy a regar automáticamente ";
      if (tipoRiego==1){
      respuesta += segundosRiego;
      respuesta += " segundos.\n";
        }
        else {
      respuesta += litrosRiego;
      respuesta += "cm3.\n";}
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
    
    /*
    if (text == "/PanelLocal")
    {
      String panel = "Accede a tu panel local : http://";
      panel += WiFi.localIP().toString().c_str();
      //panel += "/codigo";
      bot.sendMessage(chat_id, panel, "Markdown");
    }*/
     }
    
//////////////////////////////////// NOTIFICACIONES /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

   
          /*
boolean banderaAlarmR=false;
boolean banderaAlarmT=false;*/
    else if (text=="/AlarmaTemp"){
      banderaAlarmT=true;
      String respuesta = F("Te aviso cuando la temperatura este por encima de ");
      respuesta += tMax;
      respuesta += F("°C, o por debajo de ");
      respuesta += tMin;
      respuesta += F("°C.\n");
      respuesta += F("Podes /CambiarAlarmaTemp si queres modificar la temp. máxima y mínima.\n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x94\x94 /Activar \n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x94\x95 /Apagar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
     
      }
    else if (text=="/Activar" && banderaAlarmT==true){
      banderaAlarmT=false;
        alarmaTemp=1;
        EEPROM.write(54, alarmaTemp);
        EEPROM.commit();
        String respuesta = F("Muy Bien! \xF0\x9F\x91\x8C activaste las notificaciones de temp!.\n ");
        
        
        bot.sendMessage(chat_id, respuesta, "Markdown");
        
      } 
     else if (text=="/Apagar" && banderaAlarmT==true){
      banderaAlarmT=false;
         alarmaTemp=0;
        EEPROM.write(54, alarmaTemp);
        EEPROM.commit();
        String respuesta = F("\xF0\x9F\x94\x95 Notificación apagada.\n ");
        bot.sendMessage(chat_id, respuesta, "Markdown");

      }
       ///////////////////////////////
      ///////////////////////////////
      else if (text == "/CambiarAlarmaTemp"){
         caTemp=true;
         String respuesta =  F("Tu configuración actual es:\n");
         respuesta += F("\xE2\x9B\x84 Alarma de temp. mínima a ");
         respuesta += tMin;
         respuesta += F("°C\n\xF0\x9F\x94\xA5 Alarma de temp. máxima en ");
         respuesta += tMax;
         respuesta += F("°C.\n");
         respuesta += F("-------\n");
         respuesta += F("\xF0\x9F\x91\x89 /Modificar \n");
         respuesta += F("-------\n");
         respuesta += F("\xE2\x9D\x8C /Cancelar \n");
       
        bot.sendMessage(chat_id,respuesta, "Markdown");
        
        }
    else if (text=="/Modificar" && caTemp == true) {   
        caTemp==false;
        banderaTemp=true;
        text="no";
        String respuesta =  F("\xE2\x9B\x84 ¿Cuál es la temperatura mínima que queres configurar?\n");
        bot.sendMessage(chat_id,respuesta, "Markdown");
        }
     else if (banderaTemp==true && text!="no"){
      tMin=text.toInt();
      if ( tMin < 00 || tMin > 50){
        tMin=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 00 y 50"), "Markdown");
        
        }
      else {
        banderaTemp=false;
      banderaTempMax=true;
      text="no";
       EEPROM.write(56, tMin);
       EEPROM.commit();
      String respuesta = F("Muy bien! \xF0\x9F\x91\x8C configuraste ");
      respuesta += tMin;
      respuesta += F("°C.\n");
      respuesta += F("\xF0\x9F\x94\xA5 ¿Cuál es la temperatura *máxima* que queres configurar?."); 
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
     }
      else if (banderaTempMax==true && text!="no"){
      tMax=text.toInt();
      
      if ( tMax < 00 || tMax > 50){
        tMax=0;
        bot.sendMessage(chat_id, F("Tenes que configurar entre 00 y 50"), "Markdown");
        }
      else { 
        
      banderaTempMax=false;
     
       EEPROM.write(55, tMax);
       EEPROM.commit();    
      String respuesta = F("Muy bien! \xF0\x9F\x91\x8C Tu nueva configuración es:\n\xE2\x9B\x84 Alarma de temp. mínima en ");
      respuesta += tMin;
      respuesta += F("°C \n\xF0\x9F\x94\xA5 Alarma de temp. máxima en ");
      respuesta += tMax;
      respuesta += F("°C.\n");
 
      //bot.sendMessage(chat_id, nt9, "Markdown");
      bot.sendMessageWithReplyKeyboard(chat_id, respuesta, "Markdown", start1, true);
     
      }
   }
      ////////////////////////////
      /////////////////////////////
    else if (text=="/NotificacionesDiarias"){
      banderaNotD=true;
      String respuesta = F("Todos los días te envio el /Estado del dispositivo.\n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x94\x94 /Activar \n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x94\x95 /Apagar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
     
      }
    else if (text=="/Activar" && banderaNotD==true){
        banderaNotD=false;
        notifiDiarias=1;
        EEPROM.write(52, notifiDiarias);
        EEPROM.commit();
        String respuesta = F("Muy Bien! \xF0\x9F\x91\x8C activaste las notificaciones diarias!.\n");
        
        bot.sendMessage(chat_id, respuesta, "Markdown");
        
      
      }
     else if (text=="/Apagar" && banderaNotD==true){
        banderaNotD=false;
        notifiDiarias=0;
        EEPROM.write(52, notifiDiarias);
        EEPROM.commit();
        String respuesta = F("\xF0\x9F\x94\x95 Notificación apagada.\n");
        bot.sendMessage(chat_id, respuesta, "Markdown");  
      
      }



///////////////////////////////////7
///////////////////////////////////7
else if (text == "/ValorAlarmaHT")
    {
      banderaAlarmHM=true;
      text="no";
      String alarmaHt = F("Enviá el número de la humedad mínima en el sustrato para disparar la alarma, ");
      alarmaHt += F(" entre 10 y 60.\n");
      alarmaHt += F("*Configuración actual: ");
      alarmaHt +=  HTalarm;
      alarmaHt += F("%*\n");
      alarmaHt += F("-------\n");
      alarmaHt += F("\xE2\x9D\x8C /Cancelar la acción.\n");
      bot.sendMessage(chat_id,alarmaHt , "Markdown");
      
    }
    else if (banderaAlarmHM==true && text!="no")
    {
      HTalarm = text.toInt();
       if ( HTalarm < 10 || HTalarm > 60){bot.sendMessage(chat_id, F("Tienes que configurar entre 10 y 60.\n/Cancelar la acción.\n"), "Markdown");}
      else {
        banderaAlarmHM=false;
       EEPROM.write(77, HTalarm);
       EEPROM.commit();
      String respuesta = F("Configuraste ");
      respuesta += HTalarm;
      respuesta += F("% \n");
      respuesta += F("Cuando la humedad del sustrato sea menor te aviso \xF0\x9F\x98\x89");
      
      bot.sendMessage(chat_id, respuesta, "Markdown");
      
      }
      }
      ///////////////////////////
      ////////////////////////////
    

     else if (text=="/AlarmaSustrato"){
      banderaAlarmR=true;
      String respuesta = F("Te envío una alarma cuando el sustrato este por debajo de la /ValorAlarmaHT configurada. \n");
      respuesta += F("*Configuración actual: ");
      respuesta +=  HTalarm;
      respuesta += F("%*\n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x94\x94 /Activar \n");
      respuesta += F("-------\n");
      respuesta += F("\xF0\x9F\x94\x95 /Apagar \n");
      respuesta += F("-------\n");
      respuesta += F("\xE2\x9D\x8C /Cancelar \n");
      bot.sendMessage(chat_id,respuesta, "Markdown");
      
      
      }
      
     else if (text=="/Activar" && banderaAlarmR==true){
        banderaAlarmR=false;
        alarmaHT=1;
        EEPROM.write(53, alarmaHT);
        EEPROM.commit();
        String respuesta = F("Muy Bien! \xF0\x9F\x91\x8C activaste la alarma de humedad en sustrato.\n");

        bot.sendMessage(chat_id,respuesta, "Markdown");
        
        
        }
      
      else if (text=="/Apagar" && banderaAlarmR==true){
        banderaAlarmR=false;

        alarmaHT=0;
        EEPROM.write(53, alarmaHT);
        EEPROM.commit();
        String respuesta = F("\xF0\x9F\x94\x95 Notificación apagada. \n ");

        bot.sendMessage(chat_id,respuesta,"Markdown");
        
      }
    else if (text=="/Sensores" || text=="/sensores"){

      String respuesta = F("HT directo: ");
        respuesta += humedad_actual;
        respuesta += F("% \n");
        respuesta += F("HT filtro: ");
        respuesta += humSustrato;
        respuesta += F("% \n");
        respuesta += F("ADC1: ");
        respuesta += leerSensor;
        respuesta += F(" \n");
        /*
        respuesta += F("Hum.: ");
        respuesta += h;
        respuesta += F("%");
        Serial.print("HT: ");
        Serial.println(humedad_actual);
        Serial.print("ADC1: ");
        Serial.println(analogRead(32));
        */
       bot.sendMessage(chat_id,respuesta,"Markdown");
      }
   else if (text == "/Actualizar" || text == "/ActualizarOTA"){Serial.println("Ok");}
   else if (text == codigoSeguridad){
    String respuesta = F("Usuario aceptado");
        
        bot.sendMessage(chat_id, respuesta, "Markdown");
    
    
    }

 else {
    
    String respuesta = F("No pude comprender, podes preguntarme de nuevo?");
        
        bot.sendMessage(chat_id, respuesta, "Markdown");
    }
  
}
///////////////////////////////////////////////////////////
////////////// ACTUALIZACIONES OTA ///////////////////////
/////////////////////////////////////////////////////////

if (dios==chat_id){
  
  if (text == "/Actualizar"){
  banderaActuOTA=true;
  String respuesta = F("Actualizaciones para tu dispositivo.\nChat: ");
        respuesta += chat_id;
        respuesta += F("\nVersión actual: "); 
        respuesta += versionActual; 
        respuesta += F("\n");
        respuesta += F("\xF0\x9F\x94\x95 /ActualizarOTA \n");
        respuesta += F("-------\n");
        respuesta += F("-------\n");
        respuesta += F("-------\n");
        respuesta += F("-------\n");
        respuesta += F("\xF0\x9F\x94\x95 /ActualizarOTAServer2 \n");
        respuesta += F("-------\n");
        respuesta += F("\xE2\x9D\x8C /Cancelardios \n");
        respuesta += F("-------\n");
        
        bot.sendMessage(dios,respuesta,"Markdown");
        Serial.println("Actualizando\n");
  }
  if (text=="/ActualizarOTA" && banderaActuOTA==true)
  {
        bot.sendMessage(dios,"Actualizando OTAAAAAAAA","Markdown");
        
        actualizarOTA();
        
        String respuesta = F("No hay actualizaciones disponibles.");
        
        bot.sendMessage(dios,respuesta,"Markdown");
       
        banderaActuOTA=false;
      }
      if (text=="/ActualizarOTAServer2" && banderaActuOTA==true)
  {
        bot.sendMessage(dios,"Actualizando OTAAAAAAAA","Markdown");
        
        actualizarOTAServer2();
        
        String respuesta = F("No hay actualizaciones disponibles.");
        
        bot.sendMessage(dios,respuesta,"Markdown");
       
        banderaActuOTA=false;
      }
   if (text=="/Cancelardios"){
    banderaActuOTA=false;
    bot.sendMessage(dios, F("\xE2\x9D\x8C Acción cancelada"), "Markdown");
    }
   if (text == "/Chequeo")
    {
      String respuesta=F("*Hola!* el dispositivo está conectado\n");
      
      
      
      bot.sendMessage(dios, respuesta, "Markdown");
      
       
    }
      
  }
}
}


  
/////////////////////////////////////////////////////////////////
/////////////////////// FIN BOT //////////////////////////////////////////
////////////////////////////////////////////////////////////////
 

//---Función para obtener frecuencia de los pulsos--------
void enviarEstado(){
  losDias();
  EEPROM.get(30, jefe);
  chat_id=jefe;
  String estado = "Periodo ";
      

  if (opcionIluminacion==6) {
  estado += "\xF0\x9F\x94\xA5 *Ciclos especiales* ";
  estado += (int)horasOnEsp; estado += "/"; estado += (int)horasOffEsp; estado += "h\n";

  // Calculamos cuánto falta para el próximo cambio
  int dNow   = contarDias(dia, mes, anio);
  int dStart = contarDias((int)cicDia, (int)cicMes, (int)cicAnio);
  long horasDesdeAncla = (long)(dNow - dStart)*24L + ((long)hora - (long)cicHora);
  int periodoH = (int)horasOnEsp + (int)horasOffEsp; if (periodoH<=0) periodoH=1;
  int offset = (int)((horasDesdeAncla % periodoH + periodoH) % periodoH);
  bool on = (offset < (int)horasOnEsp);
  int restantes = on ? ((int)horasOnEsp - offset) : (periodoH - offset);

  
  estado += "Próximo cambio en "; estado += restantes; estado += "h\n";
}


  if (opcionIluminacion==3){
  estado += "\xF0\x9F\x8C\xB8 ";  ///// emoji flor
  }  
  if (opcionIluminacion==4){ 
    estado += "\xF0\x9F\xA7\xA0 ";}
  
  if (opcionIluminacion==1 || opcionIluminacion==2){ estado += "\xF0\x9F\x8C\xB1 ";}  ////// emoji plantita
    
  if (opcionIluminacion==5){
  estado += "*personalizado*\nLuz On: ";
  estado += horaluzOn;
  estado += "hs\nLuz Off: ";
  estado += horaluzOff;
  estado += "hs\n";
  }
  if (opcionIluminacion==4){estado += "*IA*, ";}
  if (opcionIluminacion==3){estado += "*floración 12/12*, ";} 
  if (opcionIluminacion==1){estado += "*vegetativo 18/06*, ";}
  if (opcionIluminacion==2){estado += "*automáticas 20/04*, ";}
  estado += diasCultivo;
  estado += " días de cultivo \n";
  
  if (LuzOnOff==true){estado += "Ahora \xF0\x9F\x8C\x9E la luz está prendida\n";}
  if (LuzOnOff==false){estado += "Ahora \xF0\x9F\x8C\x9A la luz está apagada\n";}
  //estado +="\xF0\x9F\x92\xA7 *RIEGO*\n";
  estado +="\n";  
  if (riegoOnOff==1){estado += "\xF0\x9F\xA7\xA0 Riego automático *IA* activado\n";}
  if (riegoOnOff==2){estado += "\xF0\x9F\x8C\x91 Riego automático *IA* apagado\n";}
  if (Fly3==1){estado += "\xF0\x9F\x9A\xA8 ALARMA DE HUMEDAD EN SUSTRATO BAJA!\xF0\x9F\x9A\xA8\n";}
  estado += "Humedad en sustrato:* ";
  estado += humSustrato;
  estado += "%*\n";
  estado += "Temperatura en sustrato:* ";
  estado += tempSustrato;
  estado += "°C*\n";
  estado += "Ultimo riego el ";
  estado += strI(diaUR);
  estado += "/";
  estado += strI(mesUR);
  estado += " a las ";
  estado += strI(horaUR);
  estado += ":";
  estado += strI(minUR);
  estado += " ";
  estado += "\n";
  estado += "*Configuración guardada* \n";
  estado += "Humedad mínima: ";
  estado += valorMinHumedad;
  estado += "% \n";
  if (tipoRiego==1){
    estado += "Tiempo de riego: ";
  estado += segundosRiego;
  estado += " seg\n";
    }
    else {
  estado += "Cantidad a regar: ";
  estado += litrosRiego;
  estado += "CM3\n";}
  estado +="\n";  
  estado +="\xF0\x9F\x8D\x83 *SENSOR TEMP/HUM*\n";
    if (Fly1==1){estado += "\xF0\x9F\x9A\xA8 \xF0\x9F\x94\xA5 *Alarma de temperatura alta!* \xF0\x9F\x9A\xA8\n";}
  if (Fly2==1){estado += "\xF0\x9F\x9A\xA8 \xE2\x9B\x84 *Alarma de temperatura baja!* \xF0\x9F\x9A\xA8\n";}
  if (isnan(t)){estado += "\xF0\x9F\x9A\xA9 _Problemas con el sensor de temp. y hum. Funciones limitadas_ \xF0\x9F\x9A\xA9\n";} 
  estado += "Temperatura:* ";
  estado += t;
  estado += "°C*\n";
  estado += "Humedad: *";
  estado += h;
  estado += "%*\n";
  estado +="\n";  
  estado += "\x31\xE2\x83\xA3 *AUX 1* ";
  if (aux1==1){estado += "\xE2\x9A\xA1 Full On\n";}
  if (aux1==0){estado += "\xE2\x9A\xAB Full Off\n";}
  if (aux1==3){
    estado += "\xF0\x9F\x95\x93 HORARIO\n";
    estado += "On: ";
    estado += horaA1On;
    estado += " hs. Off: ";
    estado += horaA1Off;
    estado += " hs.\n";
  }
  if (aux1==4){
    estado += "\xF0\x9F\x94\x83 CICLOS\n";
    estado += "On: ";
    estado += cicloA1On;
    estado += " min. Off: ";
    estado += cicloA1Off;
    estado += " min.\n";
    }
  if (aux1==5){
    estado += "\xF0\x9F\x94\xB6 TEMPERATURA\n";
    estado += "On más de ";
    estado += tempA1On;
    estado += "°C.\n";
    }
  if (aux1==6){
    estado += "\xF0\x9F\x94\xB7 HUMEDAD\n";
    estado += "On más de ";
    estado += humA1On;
    estado += "%.\n";
    }
  if (testigoA1== true){estado += "Ahora \xE2\x9A\xA1 PRENDIDO\n";}
  if (testigoA1== false){estado += "Ahora \xE2\x9A\xAB APAGADO\n";}
  
  estado += "\x32\xE2\x83\xA3 *AUX 2* ";
  if (aux2==1){estado += "\xE2\x9A\xA1 Full On\n";}
  if (aux2==0){estado += "\xE2\x9A\xAB Full Off\n";}
  if (aux2==3){
    estado += "\xF0\x9F\x95\x93 HORARIO\n";
    estado += "On: ";
    estado += horaA2On;
      estado += " hs. Off: ";
    estado += horaA2Off;
    estado += " hs.\n";
  }
  if (aux2==4){
    estado += "\xF0\x9F\x94\x83 CICLOS\n";
    estado += "On: ";
    estado += cicloA2On;
    estado += " min. Off: ";
    estado += cicloA2Off;
    estado += " min.\n";
    }
  if (aux2==5){
    estado += "\xF0\x9F\x94\xB6 TEMPERATURA\n";
    estado += "On más de ";
    estado += tempA2On;
    estado += "°C.\n";
    }
  if (aux2==6){
    estado += "\xF0\x9F\x94\xB7 HUMEDAD\n"; 
    estado += "On  más de ";
    estado += humA2On;
    estado += "%.\n";
    }
  if (testigoA2== true){estado += "Ahora \xE2\x9A\xA1 PRENDIDO\n";}
  if (testigoA2== false){estado += "Ahora \xE2\x9A\xAB APAGADO\n";}
    
  //banderaEstado=false;

  bot.sendMessage(chat_id, estado, "Markdown");
      
  
  
}  

void buscandoMensajes(){


  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
 
    while (numNewMessages)
    {
      Serial.println("LLegó un mensaje!!");
      colorWipe(strip.Color(  127, 127, 127), 30); // Blue
      colorWipe(strip.Color(0, 128, 0), 30); // Green
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      
      
    }
    
  
  
 }

 

void notifi(){
  //Serial.println("Entrando notificaciones");
  
  if (chat_id == 0 || !chat_id){chat_id=jefe;}

  if (alarmaTemp==1){
    
    if (t > tMax){ 
    if (bandaC==0){
      String  alarmTempUp = "\xF0\x9F\x9A\xA8 \xF0\x9F\x94\xA5 *Alarma de temperatura alta!* \xF0\x9F\x9A\xA8 \n";
      alarmTempUp += "-------\n";
      alarmTempUp += "Hola! te aviso que la temperatura es de " ;
      alarmTempUp += t;
      alarmTempUp += "°C.\n";
      bot.sendMessage(chat_id, alarmTempUp, "Markdown");
      bandaC++;
      Fly1=1;
      EEPROM.write(57, Fly1);
      EEPROM.commit();
        }    
    }
    if (t < tMax-1){
      bandaC=0;
      Fly1=0;
      EEPROM.write(57, Fly1);
      EEPROM.commit();
      }   
      if (t < tMin && t!=0 && t!=NULL){ 
    if (bandaD==0){
      String  alarmTempDempUp = "\xF0\x9F\x9A\xA8 \xE2\x9B\x84 *Alarma de temperatura baja!* \xF0\x9F\x9A\xA8 \n";
      alarmTempDempUp += "-------\n";
      alarmTempDempUp += "Hola! te aviso que la temperatura es de " ;
      alarmTempDempUp += t;
      alarmTempDempUp += "°C. \n";
      bot.sendMessage(chat_id, alarmTempDempUp, "Markdown");
      bandaD++;
      Fly2=1;
      EEPROM.write(58, Fly2);
      EEPROM.commit();
        }
    } 
    if (t > tMin+1){
      bandaD=0;
      Fly2=0;
      EEPROM.write(58, Fly2);
      EEPROM.commit();
      }   
  }
  if (alarmaTemp==0){
      Fly1=0;
      Fly2=0;
      EEPROM.write(58, Fly2);
      EEPROM.write(57, Fly1);
      EEPROM.commit();}
      
 if (alarmaHT==0){
      
      Fly3=0;
      EEPROM.write(59, Fly3);
      EEPROM.commit();}
   //// aca alarma sustrato if    
  if (alarmaHT==1){
    //sensores();
    if (promHumTierra < HTalarm && mediaLista && humedad_actual>10 && promHumTierra !=0 && humedad_actual < HTalarm && LuzOnOff==true)
      {
        if (bandA==0){
      String  respuesta = "\xF0\x9F\x9A\xA8 *Alarma de humedad baja en sustrato!* \xF0\x9F\x9A\xA8 \n";
      respuesta += "Hola! te aviso que la humedad en sustrato es de ";
      respuesta += humSustrato;
      respuesta += "% \n";
      respuesta += "_Alarma configurada en ";
      respuesta += HTalarm;
      respuesta += "%_ \n";
      respuesta += "*Queres /RegarAhora ?* \xF0\x9F\x92\xA7" ;
      bot.sendMessage(chat_id, respuesta, "Markdown");
      bandA++;
      Fly3=1;
      Serial.println("Notificación de HT baja");
      EEPROM.write(59, Fly3);
      EEPROM.commit();
        }      
      }
      if (promHumTierra > HTalarm+5 && humedad_actual > HTalarm+5 && mediaLista && promHumTierra !=0) {
      bandA=0;
      Fly3=0;
      EEPROM.write(59, Fly3);
      EEPROM.commit();
      } 
      }
  
  reloj();

  if (notifiDiarias==1){
    //Serial.println("Notif. Activadas");
    if (hora==horaNotif){
      //Serial.println("Hora igual a notifi");
      if (bandaB==0){
        
      String respuesta ="\xF0\x9F\x91\x8B Hola, buen día! acá te envío el reporte diario del sistema \xF0\x9F\x91\x87\n";
      //estadoD += "Parece que va todo bien!";
      //banderaEstado=true;
      banderaNotif=true;
      bot.sendMessage(chat_id, respuesta, "Markdown");
      enviarEstado();
     
      bandaB++;
        }
      }
      
      else {bandaB=0;}
    }
    if (mensajeRiego==true){
      String respuesta ="Todo salió bien y fue un riego exitoso \xF0\x9F\x91\x8C\n";
      respuesta += "La humedad del sustrato es ahora de *";
      respuesta += humSustrato;
      respuesta += "%*\n";
      respuesta += "_La medición de la humedad en el sustrato se estabiliza en unos minutos_ \n";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      mensajeRiego=false;
      resultado=1;
      riegoaBD();
    }
    
    if (mensajeRiegoCancelado==true){
      String respuesta ="Riego cancelado";
      bot.sendMessage(chat_id, respuesta, "Markdown");
      mensajeRiegoCancelado=false;
      resultado=2;
      riegoaBD();
    }
    if (mensajeRiegoActivado==true){
      String respuesta ="Hola ";
      respuesta +=  from_name;
       respuesta += "!\n";
      respuesta += "\xF0\x9F\x9A\xA8 *Activé el riego automático!*\n";
      respuesta += "La humedad del sustrato llegó a su nivel mínimo configurado. Si queres podes /Cancelar ya mismo éste riego.\n";
      
      bot.sendMessage(chat_id, respuesta, "Markdown");
      mensajeRiegoActivado=false;
      }

    if (mensajeRiegoError==true){
      riegoOnOff = 2;
      EEPROM.write(5, riegoOnOff);
      EEPROM.commit();
      mensajeRiegoError=false;
      String respuesta ="\xF0\x9F\x9A\xA9 *Acción de seguridad* \xF0\x9F\x9A\xA9\n";
      respuesta += "Parece que el sensor detecta el sustrato listo para regar, pero se regó ";
      if (dia == diaUR){respuesta += "hoy \xF0\x9F\x98\xA3. ";}
      if (dia == diaUR+1){respuesta += "ayer \xF0\x9F\x98\xA3. ";}
      respuesta += "Revisá la ubicación del sensor para asegurarte de que esta tomando la humedad del sustrato en un sector en donde recibe el riego de forma directa. ";
      respuesta += "Si queres regar de todas formas, podes hacerlo desde */RegarAhora.*\n";
      respuesta += "El riego automático permanecera *apagado* hasta que lo actives nuevamente.";
      bot.sendMessage(chat_id, respuesta , "Markdown");
      resultado=3;
      riegoaBD();
      
      }  
    if (flySinCaudal==true){
      flySinCaudal=false;
      riegoOnOff = 2;
      EEPROM.write(5, riegoOnOff);
      EEPROM.commit();
      String sinCau ="\xF0\x9F\x9A\xA9 *Acción de seguridad* \xF0\x9F\x9A\xA9 \n";
      sinCau += "*El caudalimetro no detecta el paso del agua* \xF0\x9F\x98\xB5\n";
      sinCau += "Verificá la conexión.\n";
      sinCau += "El riego automático permanecera *apagado* hasta que lo actives nuevamente.";
      bot.sendMessage(chat_id,sinCau, "Markdown");
      resultado=4;
      riegoaBD();
      
    }

}

void leerVariables() {  
      EEPROM.get(30, jefe);
     tipoRiego=EEPROM.read(2);
     if (tipoRiego==255){tipoRiego=0;}
     opcionIluminacion=EEPROM.read(4);
     if (opcionIluminacion==255){opcionIluminacion=1;}
     riegoOnOff=EEPROM.read(5);
     if (riegoOnOff==255){riegoOnOff=2;}
     valorMinHumedad=EEPROM.read(7);
     if (valorMinHumedad==255){valorMinHumedad=0;}
     litrosRiego=EEPROM.read(9);
     if (litrosRiego==255){litrosRiego=0;}
     litrosRiego=litrosRiego*100;
         
     sumarDias=EEPROM.read(12);
     if (sumarDias==255){sumarDias=0;} 
     minUR=EEPROM.read(15);
     if (minUR==255){minUR=0;} 
     horaUR=EEPROM.read(16);
     if (horaUR==255){horaUR=0;} 
     diaUR=EEPROM.read(17);
     if (diaUR==255){diaUR=0;} 
     mesUR=EEPROM.read(18);
     if (mesUR==255){mesUR=0;} 
     segundosRiego=EEPROM.read(19);  
     if (segundosRiego==255){segundosRiego=0;} 
     if (opcionIluminacion == 5){
       horaluzOff=EEPROM.read(51);
       if (horaluzOff==255){horaluzOff=0;} 
       horaluzOn=EEPROM.read(50);
       if (horaluzOn==255){horaluzOn=0;} 
       timerOn=horaluzOn;
       timerOff=horaluzOff;}

     notifiDiarias=EEPROM.read(52);
     if (notifiDiarias==255){notifiDiarias=0;} 
     alarmaHT=EEPROM.read(53);
     if (alarmaHT==255){alarmaHT=0;} 
     alarmaTemp=EEPROM.read(54);
     if (alarmaTemp==255){alarmaTemp=0;} 
     tMax=EEPROM.read(55);
     tMin=EEPROM.read(56);
     if (tMax == 255){tMax=36;}
     if (tMin == 255){tMin=8;}
     Fly1=EEPROM.read(57);
     if (Fly1==255){Fly1=0;}
     Fly2=EEPROM.read(58);
     if (Fly2==255){Fly2=0;}
     Fly3=EEPROM.read(59);
     if (Fly3==255){Fly3=0;}
     aux1=EEPROM.read(63);
     if (aux1==255){aux1=0;}
     aux2=EEPROM.read(64);
     if (aux2==255){aux2=0;}
     horaA1On=EEPROM.read(65);
     if (horaA1On==255){horaA1On=0;}
     horaA1Off=EEPROM.read(66);
     if (horaA1Off==255){horaA1Off=0;}
     horaA2On=EEPROM.read(67);
     if (horaA2On==255){horaA2On=0;}
     horaA2Off=EEPROM.read(68);
     if (horaA2Off==255){horaA2Off=0;}
     cicloA1On=EEPROM.read(69);
     if (cicloA1On==255){cicloA1On=0;}
     cicloA1Off=EEPROM.read(70);
     if (cicloA1Off==255){cicloA1Off=0;}
     cicloA2On=EEPROM.read(71);
     if (cicloA2On==255){cicloA2On=0;}
     cicloA2Off=EEPROM.read(72);
     if (cicloA2Off==255){cicloA2Off=0;}
     tempA1On=EEPROM.read(73);
     if (tempA1On==255){tempA1On=0;}
     tempA2On=EEPROM.read(74);
     if (tempA2On==255){tempA2On=0;}
     humA1On=EEPROM.read(75);
     if (humA1On==255){humA1On=0;}    
     humA2On=EEPROM.read(76);
     if (humA2On==255){humA2On=0;}   
     HTalarm=EEPROM.read(77);
     if (HTalarm==255){HTalarm=0;}
     diaGuardado=EEPROM.read(78);
     if (diaGuardado==255){diaGuardado=0;}
     mesGuardado=EEPROM.read(79);
     if (mesGuardado==255){mesGuardado=0;}
     anioGuardado=EEPROM.read(80);
     if (anioGuardado==255){anioGuardado=0;}

     // === Ciclos Especiales: leer configuración persistida ===
horasOnEsp  = EEPROM.read(EE_CIC_ON);   if (horasOnEsp  == 255) horasOnEsp  = 13;
horasOffEsp = EEPROM.read(EE_CIC_OFF);  if (horasOffEsp == 255) horasOffEsp = 13;
cicDia      = EEPROM.read(EE_CIC_DIA);  if (cicDia      == 255) cicDia = 0;
cicMes      = EEPROM.read(EE_CIC_MES);  if (cicMes      == 255) cicMes = 0;
cicAnio     = EEPROM.read(EE_CIC_ANIO); if (cicAnio     == 255) cicAnio = 0;
cicHora     = EEPROM.read(EE_CIC_HORA); if (cicHora     == 255) cicHora = 0;

    
 
}

void reloj(){
  
  
  time_t now= time(0);
  struct tm * timeinfo;
  time(&now);
  timeinfo = localtime(&now);  
  
  //Serial.println(now);
  hora=timeinfo->tm_hour - 3;
  if (hora == -3){hora=21;}if (hora == -2){hora=22;}if (hora == -1){hora=23;}
   
  minutos=timeinfo->tm_min;
  dia=timeinfo->tm_mday;
  mes=timeinfo->tm_mon + 1;
  anio=timeinfo->tm_year - 100;
  diaSemana=timeinfo->tm_wday;

  /*
  Serial.print("Hora: ");Serial.println(hora);
  Serial.print("Minutos: ");Serial.println(minutos);
  Serial.print("Día: ");Serial.println(dia);
  Serial.print("Mes: ");Serial.println(mes);
  Serial.print("año: ");Serial.println(anio);*/
  
  
  }  

  



void timerLuz(){  

  /////////// VEG. 18/6 ////////////////
    if (opcionIluminacion==1){
      timerOn=6;
      timerOff=00;
      horaluzOn=06;
      minluzOn=00;
      horaluzOff=00;
      minluzOff=00;  
    }
    /////////// VEG. 20/04 ////////////////
    if (opcionIluminacion==2){
      timerOn=3;
      timerOff=23;
      horaluzOn=03;
      minluzOn=00;
      horaluzOff=23;
      minluzOff=00;
      
    }
    /////////// FLOR. 12/12 ////////////////
    if (opcionIluminacion==3){
      timerOn=6;
      timerOff=18;
      horaluzOn=06;
      minluzOn=00;
      horaluzOff=18;
      minluzOff=00;
      
    }
    /////////// AUTOMATICO ////////////////
    if (opcionIluminacion==4){ 
      if (diasCultivo<27){timerOn=05;timerOff=23;}
      if (diasCultivo==27){timerOn=05;timerOff=22;}
      if (diasCultivo==28){timerOn=05;timerOff=22;}
      if (diasCultivo==29){timerOn=05;timerOff=21;}
      if (diasCultivo==30){timerOn=05;timerOff=21;}
      if (diasCultivo==31){timerOn=06;timerOff=20;}
      if (diasCultivo==32){timerOn=06;timerOff=20;}
      if (diasCultivo==33){timerOn=06;timerOff=19;}
      if (diasCultivo==34){timerOn=06;timerOff=19;}
      if (diasCultivo>=35){timerOn=06;timerOff=18;}
    }
  
  //horaActualStr=(strI(hour()) + strI(minute()));

  //horaActual=horaActualStr.toInt(); // HORA ACTUAL

  reloj();

  // ====== CONTROL ILUMINACIÓN: CICLOS ESPECIALES (opcionIluminacion==6) ======
if (opcionIluminacion == 6) {
  // Aseguro que haya ancla. Si no hay, uso HOY y la hora actual como inicio.
  if (cicDia == 0 || cicMes == 0 || cicAnio == 0) {
    cicDia  = (byte)dia;
    cicMes  = (byte)mes;
    cicAnio = (byte)anio;
    cicHora = (byte)hora;
    EEPROM.write(EE_CIC_DIA,  cicDia);
    EEPROM.write(EE_CIC_MES,  cicMes);
    EEPROM.write(EE_CIC_ANIO, cicAnio);
    EEPROM.write(EE_CIC_HORA, cicHora);
    EEPROM.commit();
  }

  // Horas transcurridas desde el ancla (sin epoch)
  long horasDesdeAncla = (long)(contarDias(dia, mes, anio) - contarDias((int)cicDia, (int)cicMes, (int)cicAnio)) * 24L
                       + ((long)hora - (long)cicHora);

  int periodoH = (int)horasOnEsp + (int)horasOffEsp;
  if (periodoH <= 0) periodoH = 1; // por las dudas

  int offset = (int)((horasDesdeAncla % periodoH + periodoH) % periodoH);
  bool shouldOn = (offset < (int)horasOnEsp);

  digitalWrite(pinIluminacion, shouldOn ? HIGH : LOW);
  LuzOnOff = shouldOn;

} // fin opcionIluminacion==6

if (opcionIluminacion != 6) {
  //----Relay Function---- http://www.instructables.com/id/Arduino-Timer-With-OnOff-Set-Point/step6/The-Timer/
if(timerOn == timerOff){
  
   digitalWrite(pinIluminacion, LOW);
   LuzOnOff = false;
   //Serial.println("1...");
}


if(timerOn < timerOff){
  
             if(hora >= timerOn && hora < timerOff ){             //Start
             digitalWrite(pinIluminacion, HIGH);
             LuzOnOff = true;
             //Serial.println("2...");
             }
             else if(hora >= timerOff) {
             digitalWrite(pinIluminacion, LOW);
             LuzOnOff = false;
             //Serial.println("3...");
             }
             else{
             digitalWrite(pinIluminacion, LOW);
             LuzOnOff = false;
             //Serial.println("4...");
             }
}
if (timerOn > timerOff){

            if(hora >= timerOn && hora <= 23){                  //Start
            digitalWrite(pinIluminacion, HIGH); 
            LuzOnOff = true;
            //Serial.println("5..."); 
            }
            else if(hora < timerOff){
            digitalWrite(pinIluminacion, HIGH);
            LuzOnOff = true;
            //Serial.println("6...");
            }
            else if(hora >= timerOff && hora < timerOn){
            digitalWrite(pinIluminacion, LOW);
            LuzOnOff = false;
            //Serial.println("7...");  
            }
    }
    }
    

}

///////////////// CONTADOR DE DIAS DE CULTIVO ///////////////////
////////////////////////////////////////////////////////////////

void losDias(){
 
    int diaGuardadoInt=(int)diaGuardado;
    int mesGuardadoInt=(int)mesGuardado;
    int anioGuardadoInt=(int)anioGuardado;
    reloj(); 
    diasCultivo=contarDias(dia, mes, anio) - contarDias(diaGuardadoInt, mesGuardadoInt, anioGuardadoInt) + sumarDias;    
   
 }
int tablaMes[12] = {0,31,59,90,120,151,181,212,243,273,304,334};  
long contarDias(int diademes,int mes, int anio)
{ 
  long dia;
  if(anio>2000){anio=anio - 2000;}
  dia = (anio)*365 + (anio)/4 + tablaMes[mes-1] + diademes;
  

  return dia;

}

void resetDias()
{    
      contador=0;
      EEPROM.write(14, contador);   
      EEPROM.commit();
}
void riego(){
  //digitalWrite(pinRiego, LOW);
  
  //////////////// RIEGO AUTO //////////////////
  if (riegoOnOff==3){
    if(opcionIluminacion!=3){
    if(diasCultivo<=15){
    valorMinHumedadAuto=45;
    }
    if(diasCultivo>=16 && diasCultivo<=22){
    valorMinHumedadAuto=40;
    }
    if(diasCultivo>=23 && diasCultivo<=38){
    valorMinHumedadAuto=35;
    }
    if(diasCultivo>=39 && diasCultivo<=80){
    valorMinHumedadAuto=30;
    }}
    
    if (opcionIluminacion==3){
    if(diasCultivo<=30){
    valorMinHumedadAuto=35;
    }
    if(diasCultivo>=31 && diasCultivo<=80){
    valorMinHumedadAuto=30;
    }
    if(diasCultivo>=81){
    valorMinHumedadAuto=25;
    }}
  } 
  
  
  //////////////// SENSOR 1 //////////////////
  minHumedad=valorMinHumedad;
  if(riegoOnOff==3){minHumedad=valorMinHumedadAuto;} 
  
  if (riegoOnOff == 2 && !banderaRiego){
  digitalWrite(pinRiego, LOW);
  }
     //Serial.println("Riego ON...");
  if (riegoOnOff !=2){
  if (promHumTierra < minHumedad && mediaLista && humedad_actual>10 && !banderaRiego && promHumTierra !=0 && humedad_actual < minHumedad && LuzOnOff==true)
      {
        reloj();
        if (dia==diaUR && mes==mesUR || dia==diaUR+1 && mes==mesUR){
          banderaRiego=false;
          mediaLista = false; 
          mensajeRiegoError= true;
          riegoOnOff=2;
          
          EEPROM.write(5, riegoOnOff);
          EEPROM.commit();  
          }

        else{
         Serial.println("Variables de riego activadas...");
         volumen=0;
         riegoPendiente=litrosRiego;
         banderaRiego=true;
         mediaLista=false; 
         segunDos=0;
         lastTime=0;
         contadorSeguridad=0;
         riegoXgoteo=0;
         cop1=0;
         secondsElapsed();
         //Serial.println("Acá rebota 3");
         mensajeRiegoActivado=true;    
         }
      } 
  }
 if (banderaRiego==true && tipoRiego==1){

  if (millis() > contadorRiego + intervalRiego) {

    contadorRiego=millis();
    Serial.println(segunDos);
    segunDos=segunDos + secondsElapsed();
    Serial.println(segunDos);
    
    digitalWrite(pinRiego, HIGH);
  
    Serial.println("///Riego 1 Activado X TIEMPO ///"); 
    Serial.print("Segundos configurados:");Serial.println(segundosRiego); 
    Serial.print("Segundos:");Serial.println(segunDos);
    }
    if(segunDos > segundosRiego && banderaRiego== true ){
    digitalWrite(pinRiego, LOW);
    banderaRiego=false;
    banderaRAon=false;
    segunDos=0;
    reloj();
    minUR=minutos;
    horaUR=hora;
    diaUR=dia;
    mesUR=mes;
    
    EEPROM.write(15, minUR);
    EEPROM.write(16, horaUR);
    EEPROM.write(17, diaUR);
    EEPROM.write(18, mesUR);
    EEPROM.commit();
    mensajeRiego= true;
  
       }
   
    
    colorWipe(strip.Color(  0, 0, 255), 30); // Blue
    colorWipe(strip.Color(0, 128, 0), 30); // Green
    //Serial.println("///acá lo mando///"); 
  
  }
  

 if (banderaRiego==true && tipoRiego==2){
    caudalimetro();
    //digitalWrite(pinRiego, HIGH);
    colorWipe(strip.Color(  0, 0, 255), 30); // Blue
    colorWipe(strip.Color(0, 128, 0), 30); // Green

    riegoXgoteo++;
    if (riegoXgoteo>45){
      digitalWrite(pinRiego, LOW);
      }
     else {
      digitalWrite(pinRiego, HIGH);
    }
    if (riegoXgoteo>60){
      riegoXgoteo=0;
    }
    
    
    Serial.println("///Riego Activado///"); 
    Serial.print("Litros configurados:");Serial.println(litrosRiego); 
    Serial.print("Volumen:");Serial.println(volumen);
    Serial.print("Litros pendientes:");Serial.println(riegoPendiente); 

    
    riegoPendiente=litrosRiego-volumen;
    //segunDos++;
    
    
    
    //Serial.print("Contador: ");
    //Serial.println(segunDos);
    //Serial.print("Volumen: ");
    //Serial.println(volumen);

    
     //RIEGO CANCELADO POR FALTA DE CAUDAL///"); 
    if (cop1 > 60){
      cop1=0;
     
    digitalWrite(pinRiego, LOW);
    riegoOnOff=2;
    banderaRiego=false;
    banderaRAon=false;
    flySinCaudal=true;
    
    reloj();
    minUR=minutos;
    horaUR=hora;
    diaUR=dia;
    mesUR=mes;
    
    
    EEPROM.write(15, minUR);
    EEPROM.write(16, horaUR);
    EEPROM.write(17, diaUR);
    EEPROM.write(18, mesUR);
    EEPROM.commit();
   
   
    //String regado = "Listo! ya se regó! vuelve a /Menu";
    //bot.sendMessage(jefe, regado, "Markdown");
      }
    
    if (riegoPendiente<=0 && banderaRiego==true)
    {
    digitalWrite(pinRiego, LOW);
    mensajeRiego= true;
    banderaRiego=false;
    banderaRAon=false;
    riegoXgoteo=0; 
    reloj();
    minUR=minutos;
    horaUR=hora;
    diaUR=dia;
    mesUR=mes;
    volumen=0;
    contadorSeguridad=0;
    Serial.println("Acá rebota 2");
    EEPROM.write(15, minUR);
    EEPROM.write(16, horaUR);
    EEPROM.write(17, diaUR);
    EEPROM.write(18, mesUR);
    EEPROM.commit();
    //Serial.println("///FIN DE RIEGO X LITROS///"); 
   
    //String regado = "Listo! ya se regó! vuelve a /Menu";
    //bot.sendMessage(jefe, regado, "Markdown");
    
     
      
      }  
   }
    
  if (banderaRiego==true && riegoOnOff == 2 &&  banderaRAon==false){  
    digitalWrite(pinRiego, LOW);
    //Serial.println("//////////////Riego cancelado...////////////"); 
      banderaRiego=false;
      riegoXgoteo=0; 
     mensajeRiegoCancelado= true;
    EEPROM.write(15, minutos);
    EEPROM.write(16, hora);
    EEPROM.write(17, dia);
    EEPROM.write(18, mes);
    EEPROM.commit();
    String respuesta = F("*Riego cancelado*"); 
       bot.sendMessage(chat_id, respuesta, "Markdown");
      }   


}
//// CAUDALIMETRO //////////////////////////////////////


void caudalimetro(){

  cauMillis=millis(); 
  long cicloCau = cauMillis - currentCauMillis;
  int frecuencia;
  noInterrupts(); //Deshabilitamos  las interrupciones
  frecuencia=NumPulsos; //Hz(pulsos por segundo)
  float caudal_L_m=frecuencia/factor_conversion; //calculamos el caudal en L/m
  dt=esp_timer_get_time()-t0; //calculamos la variación de tiempo
  t0=esp_timer_get_time();
  volumen=volumen+(caudal_L_m/60)*(dt/cicloCau); // volumen(L)=caudal(L/s)*tiempo(s) 
 
  
  if (NumPulsos==0){
     cop1++;
      }
      else {
        cop1=0;
        }
  NumPulsos=0;   //Ponemos a 0 el número de pulsos
  interrupts();
  currentCauMillis=millis();
  

  
  }


void sensorSuelo()
{
 unsigned long calculateMillis = millis(); // Obtiene el tiempo transcurrido desde el inicio del programa
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  if (calculateMillis - preMillis >= intervalSensor) { // Verifica si ha pasado el tiempo necesario para enviar la petición
    preMillis = calculateMillis; // Actualiza el valor de previousMillis para la próxima petición 
  if (mod.write(O2, sizeof(O2)) == 8) {
    digitalWrite(DE, LOW);
    digitalWrite(RE, LOW);
    // Leer la respuesta del sensor
      if (mod.available() >= 19) {
      for (byte i = 0; i < 19; i++) {
        values[i] = mod.read();
      }

      // Procesar los valores recibidos
      
     //humSustrato tempSustrato conductivity pH nitrogen phosphorus potassium
      Serial.println(" ............");
      Serial.print("Humedad: ");
      Serial.print(humSustrato);
      Serial.println(" %   ");
      
      Serial.print("Temperatura: ");
      Serial.print(tempSustrato);
      Serial.println(" °C   ");
      
      Serial.print("Conductividad: ");
      Serial.print(conductivity);
      Serial.println(" µS/cm   ");
      Serial.print("Conductividad ppm: ");
      Serial.print(conductivity * 0.7);
      Serial.println(" ppm   ");
      
      Serial.print("PH: ");
      Serial.print(pH);
      Serial.println(" ");
      

 
      Serial.print("Nitrogeno: ");
      Serial.print(nitrogen);
      Serial.println(" mg/kg   ");
      
      Serial.print("Fosforo: ");
      Serial.print(phosphorus);
      Serial.println(" mg/kg");
      
      Serial.print("Potasio: ");
      Serial.print(potassium);
      Serial.println(" mg/kg");
      Serial.println(" ............");
      
      
  }
 
}
} 
 
      humSustrato = ((values[3]*256)+values[4]) * 0.1;
      tempSustrato = ((values[5]*256)+values[6]) * 0.1;
      conductivity = ((values[7]*256)+values[8]);
      pH = ((values[9]*256)+values[10]) * 0.1;
      nitrogen = ((values[11]*256)+values[12]);
      phosphorus = ((values[13]*256)+values[14]);
      potassium = ((values[15]*256)+values[16]);

  if(humSustrato<4){humSustrato=0;}
  if (humSustrato>100){humSustrato=0;}
  if (humSustrato>4){
  lecturasHumedad[indiceLecturasHumedad] = humSustrato;
  indiceLecturasHumedad++;
  humedad_actual=humSustrato;
  suma=0;
  mediaLista = false;
  
    for ( byte i = 0; i < 30; i++)
  {
    suma += lecturasHumedad[i]; //Sumamos cada uno de los elementos del array
  }
  if (indiceLecturasHumedad>30)
  {
    indiceLecturasHumedad=0;
    mediaLista = true;
    promHumTierra=suma/30;
  }
  }
  


}

void sensorDHT(){
  
  if (millis() > contador3 + intervalDht) {
    
    contador3 = millis(); 
/////////////////DHT//////////////////

h = dht.getHumidity();
  // Read temperature as Celsius (the default)
t = dht.getTemperature();

  }
  }
////////////////////////////////////////////////
///////////////////////////////////////////////
//////////     AUXILIARES    //////////////
////////////////////////////////////////////////
///////////////////////////////////////////////
////////////////////////////////////////////////

void auxiliares(){
//////////////////// AUX 1 ///////////////
//////////////////// Apagado
if (aux1 == 0){
  digitalWrite(PinAux1, LOW);
  testigoA1 = false;
  //Serial.println("AUX 1, variable 0"); 
  }
//////////////////// Prendido
if (aux1 == 1){
  digitalWrite(PinAux1, HIGH);
  testigoA1 = true;
  //Serial.println("AUX 1, variable 1");
  }
//////////////////// Timer
if (aux1 == 3){
    reloj();
    //Serial.println("AUX 1, variable 3 reloj");
  //----Relay Function---- http://www.instructables.com/id/Arduino-Timer-With-OnOff-Set-Point/step6/The-Timer/
if(horaA1On == horaA1Off){
   digitalWrite(PinAux1, LOW);
   testigoA1 = false;
}


if(horaA1On < horaA1Off){
  
             if(hora >= horaA1On && hora < horaA1Off ){             //Start
             digitalWrite(PinAux1, HIGH);
             testigoA1 = true;
             }
             else if(hora >= horaA1Off) {
             digitalWrite(PinAux1, LOW);
             testigoA1 = false;
             }
             else{
             digitalWrite(PinAux1, LOW);
             testigoA1 = false;
             }
}
if (horaA1On > horaA1Off){

            if(hora >= horaA1On && hora <= 23){                  //Start
            digitalWrite(PinAux1, HIGH); 
            testigoA1 = true; 
            }
            if(hora < horaA1Off){
            digitalWrite(PinAux1, HIGH);
            testigoA1 = true;
            }
            if(hora >= horaA1Off && hora < horaA1On){
            digitalWrite(PinAux1, LOW);
            testigoA1 = false;  
            }
    }
  }
  //////////////////// Ciclos
if (aux1 == 4){
  if (contador1 == 0){contador1=millis();}

  if (millis() > contador1 + interval) {
    
    contador1 = millis();
    
    if (minutoAux1 <= cicloA1On && flyOnOffAux1==true){
    digitalWrite(PinAux1, HIGH);
    testigoA1 = true; 
    minutoAux1++;
    //Serial.println("minutos en ON aux1:");
    //Serial.println(minutoAux1);
    
    
      if (minutoAux1>cicloA1On){flyOnOffAux1=false;minutoAux1=0;}
  }
    if (minutoAux1<=cicloA1Off && flyOnOffAux1==false){
    digitalWrite(PinAux1, LOW);
    testigoA1 = false; 
    minutoAux1++;
    //Serial.println("minutos en OFF aux1:");
      if (minutoAux1>=cicloA1Off){flyOnOffAux1=true;minutoAux1=0;} 
  }

  
 }
 
}
  //////////////////// X Temp
if (aux1 == 5){
  temp=t;
  if (isnan(t)){
    //Serial.println("Problemas sensor dht"); 
  } else {
    
    if (temp>tempA1On){
      digitalWrite(PinAux1, HIGH);
      testigoA1 = true; 
      //Serial.println("AUX 1 on temp"); 
      }
    if (temp<tempA1On){
      digitalWrite(PinAux1, LOW);
      testigoA1 = false; 
      //Serial.println("AUX 1 off temp");    
      } 
  } 
}    
       //////////////////// X Humedad
if (aux1 == 6){

  hum=h;
  if (isnan(h)){
    //Serial.println("Problemas sensor dht"); 
  } else {
    
    if (hum>humA1On){
      digitalWrite(PinAux1, HIGH);
      testigoA1 = true; 
      //Serial.println("AUX 1 on temp"); 
      }
    if (hum<humA1On){
      digitalWrite(PinAux1, LOW);
      testigoA1 = false; 
      //Serial.println("AUX 1 off temp"); 
      }
  } 
}

//////////////////// AUX 2 ///////////////
//////////////////// Apagado
if (aux2 == 0){
  digitalWrite(PinAux2, LOW);
  testigoA2 = false;
  }
//////////////////// Prendido
if (aux2 == 1){
  digitalWrite(PinAux2, HIGH);
  testigoA2 = true;
  }
//////////////////// Timer
if (aux2 == 3){
    reloj();

  //----Relay Function---- http://www.instructables.com/id/Arduino-Timer-With-OnOff-Set-Point/step6/The-Timer/
if(horaA2On == horaA2Off){
   digitalWrite(PinAux2, LOW);
   testigoA2 = false;
}


if(horaA2On < horaA2Off){
  
             if(hora >= horaA2On && hora < horaA2Off ){             //Start
             digitalWrite(PinAux2, HIGH);
             testigoA2 = true;
             }
             else if(hora >= horaA2Off) {
             digitalWrite(PinAux2, LOW);
             testigoA2 = false;
             }
             else{
             digitalWrite(PinAux2, LOW);
             testigoA2 = false;
             }
}
if (horaA2On > horaA2Off){

            if(hora >= horaA2On && hora <= 23){                  //Start
            digitalWrite(PinAux2, HIGH); 
            testigoA2 = true; 
            }
            if(hora < horaA2Off){
            digitalWrite(PinAux2, HIGH);
            testigoA2 = true;
            }
            if(hora >= horaA2Off && hora < horaA2On){
            digitalWrite(PinAux2, LOW);
            testigoA2 = false;  
            }
    }
  }
  //////////////////// Ciclos
if (aux2 == 4){

  if (millis() > contador2 + interval) {
    
    contador2 = millis();
    
    if (minutoAux2 <= cicloA2On && flyOnOffAux2==true){
    digitalWrite(PinAux2, HIGH);
    testigoA2 = true; 
    minutoAux2++;
    //Serial.println("minutos en ON aux1:");
    //Serial.println(minutoAux1);
    
    
      if (minutoAux2>cicloA2On){flyOnOffAux2=false;minutoAux2=0;}
  }
    if (minutoAux2<=cicloA2Off && flyOnOffAux2==false){
    digitalWrite(PinAux2, LOW);
    testigoA2 = false; 
    minutoAux2++;
    //Serial.println("minutos en OFF aux1:");
      if (minutoAux2>cicloA2Off){flyOnOffAux2=true;minutoAux2=0;} 
  }

  
 }
 
}


 //////////////////// X Temp
if (aux2 == 5){
  temp=t;
  if (isnan(t)){
    //Serial.println("Problemas sensor dht"); 
  } else {
   
    if (temp>tempA2On){
      digitalWrite(PinAux2, HIGH);
      testigoA2 = true; 
      //Serial.println("AUX 2 on temp"); 
      }
    if (temp<tempA2On){
      digitalWrite(PinAux2, LOW);
      testigoA2 = false; 
      //Serial.println("AUX 2 off temp"); 
      }
  } 
}    
       //////////////////// X Humedad
if (aux2 == 6){

  hum=h;
  if (isnan(h)){
    //Serial.println("Problemas sensor dht"); 
  } else {
   
    if (hum>humA2On){
      digitalWrite(PinAux2, HIGH);
      testigoA2 = true; 
      //Serial.println("AUX 2 on temp"); 
      }
    if (hum<humA2On){
      digitalWrite(PinAux2, LOW);
      testigoA2 = false; 
      //Serial.println("AUX 2 off temp"); 
      } 
  } 
}
}  
///////////// FIN

///////////////////////////
//////////////////////////
//////////////////////////
void actualizarOTA()
{
  WiFiClient client;
  t_httpUpdate_return ret = httpUpdate.update(client, fwUrlBase);
    // Or:
    //t_httpUpdate_return ret = httpUpdate.update(client, "server", 80, "file.bin");

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        String act3 = "Actualización realizada!";
        
        bot.sendMessage(chat_id, act3, "Markdown");
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  
 }
void actualizarOTAServer2()
{
  WiFiClient client;
  t_httpUpdate_return ret = httpUpdate.update(client, fwUrlBase2);
    // Or:
    //t_httpUpdate_return ret = httpUpdate.update(client, "server", 80, "file.bin");

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        String act3 = "Actualización realizada!";
        
        bot.sendMessage(chat_id, act3, "Markdown");
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  
 }
  
/////////////////////// NEOPIXEL //////////////////////
//////////////////////////////////////////////////////
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(100);                           //  Pause for a moment
  }
}
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}
// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

///////////////////// FIN NEOPIXEL /////////////////////////////
////////////////////////////////////////////////////////////////

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
  
  colorWipe(strip.Color(127, 127, 127), 50); // Green PARA CONEXIÓN WIFI OK


}
void monitoreoMillis(){
  Serial.println("A ver que onda 250 millis...");
  Serial.println("Time: ");
  myTime = millis();
  Serial.println(myTime);
  }
void controlWiFiyBD(){
   
  currentMillis = millis();
  //Serial.println("Esperando reinicio...");
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if (currentMillis - previousMillis >=intervalWiFi){
    
  if (WiFi.status() != WL_CONNECTED) {
     
    colorWipe(strip.Color(255, 233, 0), 50);
     //Serial.println("REINICIANDO WIFI");
    //Serial.print(esp_timer_get_time());
    //Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    
  }
  //Enviar datos 
  if (WiFi.status() == WL_CONNECTED) {

  enviarDatos(); // ENVIA LOS DATOS A LA BASE DE DATOS
  
  }
  
  
  previousMillis = currentMillis;
  if (WiFi.status() != WL_CONNECTED){
    colorWipe(strip.Color(255, 233, 0), 50);
    }
   
  if (LuzOnOff==false){
          colorWipe(strip.Color(0, 0, 0), 0);
          }
    else{
    
    colorWipe(strip.Color(0, 128, 0), 50);
   
  }
}

}

void enviarDatos() 
{
  
    int estadoluz;
    if(LuzOnOff==false){
      estadoluz=2;
      }
      if (LuzOnOff==true)
      {
       estadoluz=1;
      }
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "api_key=";
httpRequestData += apiKeyValue;
httpRequestData += "&usr=";
httpRequestData += chat_id;
httpRequestData += "&serie=";
httpRequestData += Nserie;  // Suponiendo que tienes una variable llamada 'serie'
httpRequestData += "&humSustrato=";
httpRequestData += humSustrato;
httpRequestData += "&tempSustrato=";
httpRequestData += tempSustrato;
httpRequestData += "&conductivity=";
httpRequestData += conductivity;
httpRequestData += "&pH=";
httpRequestData += pH;
httpRequestData += "&nitrogen=";
httpRequestData += nitrogen;
httpRequestData += "&phosphorus=";
httpRequestData += phosphorus;
httpRequestData += "&potassium=";
httpRequestData += potassium;
httpRequestData += "&tempAmbiente=";
httpRequestData += t;
httpRequestData += "&humAmbiente=";
httpRequestData += h;
httpRequestData += "&riego=0";

    
    int httpResponseCode = http.POST(httpRequestData);
   
    if (httpResponseCode>0) {
      Serial.print("ENVIANDO A BD: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code BD: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  
 }



void resetXdia() {
  
  if (millis() > ONLINE_TIME){
    ESP.restart();
    }
}

///////////////// función para enviar datos de riego a la base de datos 

void riegoaBD()
{
  WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "api_key=";
httpRequestData += apiKeyValue;
httpRequestData += "&usr=";
httpRequestData += chat_id;
httpRequestData += "&serie=";
httpRequestData += Nserie;  // Suponiendo que tienes una variable llamada 'serie'
httpRequestData += "&humSustrato=";
httpRequestData += humSustrato;
httpRequestData += "&tempSustrato=";
httpRequestData += tempSustrato;
httpRequestData += "&conductivity=";
httpRequestData += conductivity;
httpRequestData += "&pH=";
httpRequestData += pH;
httpRequestData += "&nitrogen=";
httpRequestData += nitrogen;
httpRequestData += "&phosphorus=";
httpRequestData += phosphorus;
httpRequestData += "&potassium=";
httpRequestData += potassium;
httpRequestData += "&tempAmbiente=";
httpRequestData += t;
httpRequestData += "&humAmbiente=";
httpRequestData += h;
httpRequestData += "&riego=1";

    
    int httpResponseCode = http.POST(httpRequestData);
   
    if (httpResponseCode>0) {
      Serial.print("ENVIANDO A BD: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code BD: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
}


float secondsElapsed() {
  unsigned long currentTime = millis(); // tiempo actual en milisegundos
  float seconds = (currentTime - lastTime) / 1000.0; // resta el tiempo actual con el valor de la última medición y divide entre 1000 para obtener los segundos
  lastTime = currentTime; // actualiza el valor de la última medición
  return seconds;
}

void enviarDatosLive() {
 
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  String postData = "humSustrato=" + String(humSustrato) +
                 "&tempSustrato=" + String(tempSustrato) + 
                 "&conductivity=" + String(conductivity) + 
                 "&pH=" + String(pH) +
                 "&nitrogen=" + String(nitrogen) +
                 "&phosphorus=" + String(phosphorus) +
                 "&potassium=" + String(potassium) +
                 "&tempAmbiente=" + String(t) +
                 "&humAmbiente=" + String(h);

  int httpResponseCode = http.POST(postData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  
  }

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
  

void setup() {
 
  Serial.begin(115200);
  EEPROM.begin(512);
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  pinMode(pinIluminacion, OUTPUT);
  pinMode(pinRiego, OUTPUT);
  pinMode(PinLED, OUTPUT);
  pinMode(PinAux1, OUTPUT);
  pinMode(PinAux2, OUTPUT);
  pinMode(PinCaudalimetro, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(PinCaudalimetro),ContarPulsos,FALLING);//(Interrupción 0(Pin2),función,Flanco de subida)  
  
  t0=esp_timer_get_time(); 
               
  ///////////// DHT //////////////////////
  
  dht.setup(dhtPin, DHTesp::DHT22);
  
   ///////////////////// NEOPIXEL //////////////////

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  
  rainbow(3); // NEOPIXEL ARCOIRIR
  
  ///////////////// WiFiManager /////////////////
  
  WiFiManager wifiManager; 
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.setConfigPortalTimeout(timeoutportal);
  // Cremos AP y portal cautivo y comprobamos si
  // se establece la conexión
  wifiManager.autoConnect("EvaPop_S01","12345678");
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  Serial.println("Ya estás conectado!");
  // Apagamos el LED
  colorWipe(strip.Color(0, 128, 0), 50); // Green PARA CONEXIÓN WIFI OK 
  
  //server.begin();   
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  // para el bot
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  
  
  
  Serial.print("FIN SETUP WIFI CONECTADO\n");
  
  }


void loop()
{
  
    if (contadorloop == 0){contadorloop=millis();}

  if (millis() > contadorloop + intervaloop) {
    
    contadorloop = millis();
    sensorSuelo();
    leerVariables();
    buscandoMensajes();
    timerLuz();
    riego();  
    auxiliares();
    notifi();
    controlWiFiyBD();
    resetXdia();
    enviarDatosLive(); 
    sensorDHT();
    

    

    
  }
 
}
