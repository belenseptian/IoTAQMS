//list of tasks
void task1()
{
  now_task1 = millis();
  if (now_task1 - lastMeasure_task1 > interval_task1) 
  {
    lastMeasure_task1 = now_task1;
    switch(conn_status) 
    {
      case 0:
        displayVal(M702.getPM2_5(), 0);
        break;
      case 1:
        displayVal(M702.getPM2_5(), 1);
        break;
      case 2:
        publishSensor();
        displayVal(M702.getPM2_5(), 2);
        break;
      case 3:
        displayVal(M702.getPM2_5(), 3);
        break;  
    }
  }  
}
