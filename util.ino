//get a wifi id
String get_id()
{
  String id, buf_id;
  uint64_t chipid;
  char ssid[13];
  //the chip ID is essentially its MAC address(length: 6 bytes)
  chipid=ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid >> 32);
  snprintf(ssid, 13, "%04X%08X", chip, (uint32_t)chipid);
  for (int i=0; i < 12; i++)
  {
    buf_id += String(ssid[i]);
  }
  id = buf_id; buf_id="";
  return id;
}

void initAll()
{
  //serial port for debugging purposes
  Serial.begin(115200);
  M702.begin();
  initTopic();
  initOLED();    
  initStorage();
  //init FreeRTOS timer
  initMQTTTimer();
  initWiFiTimer();
  WiFi.onEvent(WiFiEvent);
  mqttHandler();
  if(initWiFi()) 
  {
    connectToMqtt();
    is_connected = true;
    displayText(1, 0, 10, "Calibrating...", true);
    delay(20000);
  }
  else 
  {
    APMode();
    is_connected = false;
  }
}
