//trying to establish mqtt connection
void connectToMqtt() 
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) 
{
  conn_status = 2;
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) 
{
  
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) 
  {
    conn_status = 3;
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttPublish(uint16_t packetId) 
{
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void initMQTTTimer()
{
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
}

void mqttHandler()
{
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);  
}

void initTopic()
{
  strcpy(topic_, topic);
  strcpy(topic_ + strlen(topic), id_);  
}

void publishSensor()
{
  M702.readAll();
  encodeJSON();
  mqttClient.publish(topic_, 1, true, JSONrootmessageBuffer); 
}
