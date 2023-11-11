void encodeJSON()
{
  //put your main code here, to run repeatedly:
  //station
  station["id"] = id_;
  station["lat"] = strtod(lat.c_str(), NULL);
  station["lng"] = strtod(lng.c_str(), NULL);
  station["description"] = "M702 AQMS";
  //payload
  payload["co2"] = 0;
  payload["ch20"] = M702.getCH20();
  payload["tvoc"] = M702.getTVOC();
  payload["pm2.5"] = M702.getPM2_5();
  payload["pm10"] = M702.getPM10();
  payload["temperature"] = 0;
  payload["humidity"] = 0;
  //encode
  root["station"] = station;
  root["payload"] = payload;
  root.printTo(JSONrootmessageBuffer, sizeof(JSONrootmessageBuffer));
}
