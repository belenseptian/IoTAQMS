//captive portal (auto redirect when user is connected to access point)
class CaptiveRequestHandler : public AsyncWebHandler 
{
  public:
      CaptiveRequestHandler() 
      {
        //this is where you can place the calls
        server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) 
        {
          AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/style.css", "text/css");
          request->send(response);
        });
      }
    virtual ~CaptiveRequestHandler() {}
  
    bool canHandle(AsyncWebServerRequest *request)
    {
      return true;
    }
  
    void handleRequest(AsyncWebServerRequest *request) 
    {
      request->send(SPIFFS, "/wifimanager.html", "text/html");
    }
};

//initialize WiFi
bool initWiFi() {
  if(ssid=="")
  {
    Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Connecting to WiFi...");
  displayText(1, 0, 10, "Connecting to WiFi...", true);

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) 
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) 
    {
      Serial.println("Failed to connect.");
      return false;
    }
  }

  Serial.println(WiFi.localIP());
  displayText(1, 0, 10, "Connected", true);
  return true;
}

void APMode()
{
  //connect to Wi-Fi network with SSID and password
  displayText(1, 0, 10, "Access point mode", true);
  WiFi.softAP(ssid_ap);
  Serial.println(WiFi.softAPIP()); 
  //web server root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/wifimanager.html", "text/html");
  });
  
  server.serveStatic("/", SPIFFS, "/");
  
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) 
  {
    int params = request->params();
    for(int i=0;i<params;i++)
    {
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost())
      {
        //HTTP POST ssid value
        if (p->name() == PARAM_INPUT_1) 
        {
          ssid = p->value().c_str();
          Serial.print("SSID set to: ");
          Serial.println(ssid);
          //write file to save value
          writeFile(SPIFFS, ssidPath, ssid.c_str());
        }
        //HTTP POST pass value
        if (p->name() == PARAM_INPUT_2) 
        {
          pass = p->value().c_str();
          Serial.print("Password set to: ");
          Serial.println(pass);
          //write file to save value
          writeFile(SPIFFS, passPath, pass.c_str());
        }
        //HTTP POST lat value
        if (p->name() == PARAM_INPUT_3) 
        {
          lat = p->value().c_str();
          Serial.print("Latitude set to: ");
          Serial.println(lat);
          //write file to save value
          writeFile(SPIFFS, latPath, lat.c_str());
        }
        //HTTP POST lng value
        if (p->name() == PARAM_INPUT_4) 
        {
          lng = p->value().c_str();
          Serial.print("Longitude set to: ");
          Serial.println(lng);
          //write file to save value
          writeFile(SPIFFS, lngPath, lng.c_str());
        }
      }
    }
    request->send(200, "text/plain", "Done. ESP will restart, you are ready to go.");
    delay(3000);
    ESP.restart();
  });
  dnsServer.start(53, "*", WiFi.softAPIP());
  displayText(1, 0, 10, "Access point mode", false);
  displayText(1, 0, 30, IpAddress2String(WiFi.softAPIP()), false);
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP
  server.begin();
}

String IpAddress2String(const IPAddress& ipAddress)
{
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}

void connectToWifi() 
{
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid.c_str(), pass.c_str());
}

void WiFiEvent(WiFiEvent_t event) 
{
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) 
  {
    case SYSTEM_EVENT_STA_GOT_IP:
      conn_status = 1;
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      conn_status = 0;
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); 
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void initWiFiTimer()
{
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
}
