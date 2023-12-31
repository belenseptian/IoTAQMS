//initialize SPIFFS
void initSPIFFS() 
{
  if (!SPIFFS.begin(true)) Serial.println("An error has occurred while mounting SPIFFS");
  Serial.println("SPIFFS mounted successfully");
}

void initStorage()
{
  initSPIFFS();
  
  //load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  lat = readFile(SPIFFS, latPath);
  lng = readFile(SPIFFS, lngPath);
}

//read File from SPIFFS
String readFile(fs::FS &fs, const char * path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(file.available())
  {
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}

//write file to SPIFFS
void writeFile(fs::FS &fs, const char * path, const char * message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)) Serial.println("- file written");
  else Serial.println("- frite failed");
}
