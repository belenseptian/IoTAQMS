//adjust the alignment of text
void align2(int q)
{
  if (q < 100) display.print(" ");
  if (q < 10) display.print(" ");   
}
void align3(int q)
{
  if (q < 1000) display.print(" ");
  align2(q);
}

void initOLED()
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  
  display.clearDisplay();
  display.drawBitmap(30, 0, umdp, 80, 60, WHITE);
  display.display();
  delay(2000);
}

void displayText(int text_size, int cursor_x, int cursor_y, String text, bool clear_)
{
  if(clear_ == true) display.clearDisplay(); 
  display.setTextSize(text_size);
  display.setTextColor(WHITE);
  display.setCursor(cursor_x, cursor_y);
  // Display static text
  display.println(text);
  display.display(); 
}

void displayVal(int pm_25, int status_)
{
  int pm25 = pm_25;
  float pm25_max;
  if(pm25 > 500) pm25_max = 500; 
  else pm25_max = pm25;
  pm25_max = (pm25_max/500)*69;
  
  //online status
  display.clearDisplay(); 
  display.setTextSize(1); //normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); //draw white text
  switch(status_) 
  {
    case 0:
      display.drawBitmap(75, 45, nowifi, 16, 16, WHITE);     
      break;
    case 1:
      display.drawBitmap(75, 45, wifisymb, 16, 16, WHITE); 
      break;
    case 2:
      display.drawBitmap(75, 45, onlinesymb, 16, 16, WHITE); 
      break;
    case 3:
      display.drawBitmap(75, 45, offlinesymb, 16, 16, WHITE); 
      break;  
  }
  //health status
  if(pm25 <= 100)
  {
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(1);
    display.setCursor(74,25);
    display.println("Healthy");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(74,25);
    display.println("Healthy");  
    display.drawBitmap(100, 40, smileEmoji, 25, 25, WHITE);     
  }
  else
  {
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(1);
    display.setCursor(74,25);
    display.println("Unhealthy");
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(74,25);
    display.println("Unhealthy");
    display.drawBitmap(100, 40, sadEmoji, 25, 25, WHITE);    
    
  }
  //pollutant unit
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(74,10);  
  display.drawRect(0, 10, 70, 35, SSD1306_WHITE);
  display.println("PM2.5");
  //percentage
  display.drawLine(0,47,0,62, SSD1306_WHITE);
  display.fillRect(1, 50, 69, 10, SSD1306_BLACK);
  display.fillRect(1, 50, pm25_max, 10, SSD1306_WHITE);
  //sensor value
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(10,20);
  display.setTextSize(2);
  align3(pm25); 
  display.println(pm25);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,20);
  display.setTextSize(2);
  align3(pm25); 
  display.println(pm25);
  display.display();  
}
