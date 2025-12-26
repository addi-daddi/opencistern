  //***********************
  //LCD Setup
  //***********************
  void ladeLCDdisplay() {
    // Wire.begin(12, 13); //SDA-D6, SCL-D7      // LCD Setup (bei beiden LCD Displays gleich)
  
    // *******************Boot mit LAN-Info für 1602 Display**************************
    if (cfg.data_display ==1) {
      timer.setInterval(1000);
    //  lcd.begin();
      lcd.init();
      lcd.backlight();
      lcd.print(cfg.SSID);
      lcd.setCursor(0, 1);
      lcd.print("Verbinde...");
      lcd.setCursor(0, 1);
      WiFi.mode ( WIFI_STA );
      WiFi.begin ( cfg.SSID, cfg.password );
    }
    // Warte kurz auf Verbindung
    while ( WiFi.status() != WL_CONNECTED ) {
      delay (2500);
      lcd.print(WiFi.localIP());
      delay (5000);
      lcd.noBacklight();
      backlstatus = 0;
    }
    // *******************Boot mit LAN-Info für 1604 Display**************************
    if (cfg.data_display ==2) {
      timer.setInterval(1000);
      lcd.init();
      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print("Ihre SSID:");
      lcd.setCursor(0, 1);
      lcd.print(cfg.SSID);
      lcd.setCursor(0, 2);
      lcd.print("Verbinde...");
      lcd.setCursor(0, 2);
      WiFi.mode ( WIFI_STA );
      WiFi.begin ( cfg.SSID, cfg.password );
    }
    while ( WiFi.status() != WL_CONNECTED ) {   // Warte kurz auf Verbindung
      delay (2500);
      lcd.print("Ihre IP Adresse:");
      lcd.setCursor(0, 3);
      lcd.print(WiFi.localIP());
      delay (5000);
      lcd.noBacklight();
    }
   }
   
  
 void ladeOLEDdisplay() {
 //*******************************Initialisierung und DEBUG OLED************************************
  int rc;
   rc = oledInit(&ssoled, OLED_128x64, -1, 0, 0, 0, 12, 13, -1, 400000L); // OLED settings
    if (rc != OLED_NOT_FOUND)
    {
        char *msgs[] =
        {
          (char *)"SSD1306 an 0x3C",
          (char *)"SSD1306 an 0x3D",
          (char *)"SH1106 an 0x3C",
          (char *)"SH1106 an 0x3D"
        };
    if (DEBUG) Serial.println(msgs[rc]);
        oledFill(&ssoled, 0, 1);
        oledWriteString(&ssoled, 0, 0, 0, (char *)"OLED gefunden:", FONT_NORMAL, 0, 1);
        oledWriteString(&ssoled, 0, 10, 2, msgs[rc], FONT_NORMAL, 0, 1);
        delay(5000);
    }
   oledSetBackBuffer(&ssoled, ucBackBuffer);                         // OLED Puffer wird gesetzt   
   // *******************Boot mit LAN-Info für OLED Display**************************
   if (cfg.data_display ==3) {
   oledFill(&ssoled, 0, 1);
   oledWriteString(&ssoled, 0,0,0,(char *)"Systemstart:", FONT_NORMAL, 0, 1);
   oledWriteString(&ssoled, 0,0,2,(char *)"Ihre SSID:", FONT_NORMAL, 0, 1);
   oledWriteString(&ssoled, 0,0,3,cfg.SSID, FONT_NORMAL, 0, 1);
   oledWriteString(&ssoled, 0,0,5,(char *)"Verbinde...", FONT_NORMAL, 0, 1);
   WiFi.mode ( WIFI_STA );
   WiFi.begin ( cfg.SSID, cfg.password );
   }
   while ( WiFi.status() != WL_CONNECTED ) {  // Warte kurz auf Verbindung
   delay (2500);
   char ipbuffer [50];
   IPAddress ip = WiFi.localIP();
   sprintf(ipbuffer, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
   oledWriteString(&ssoled, 0,0,5,(char *)"Ihre IP-Adresse:", FONT_NORMAL, 0, 1);
   oledWriteString(&ssoled, 0,0,6,ipbuffer, FONT_NORMAL, 0, 1);
   delay (6000);
   oledFill(&ssoled, 0, 1);
   }
    }  
//******************************************************** 
// Ab hier die LOOP Schleifen
//********************************************************
  //***********************
  //LCD mit Tastersteuerung für 1602 Display
  //***********************
 void loop1602() { 
    if (millis() - displayMicros >= 5000 ) {
      displayMicros = millis();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(volumen);
      lcd.print( " Liter,");
      lcd.setCursor(0,1);
      lcd.print(fuellstand);
      lcd.print( " % voll");
    }
    
    unsigned long tasterMicros = millis();
    if ((tasterMicros - previousMicros >= 90000) && backlstatus) {  
      lcd.noBacklight();
      backlstatus = false;
      return;
    }

    if ((backlstatus == false) && (tasterHigh)) {
      resetBacklightTaster();
      lcd.backlight();
      backlstatus = true;
      previousMicros = millis();
      return;
    }  
    
    if (backlstatus && tasterHigh) {
      resetBacklightTaster();
      lcd.noBacklight();
      backlstatus = false;
      return;
    }

}

//***********************
//LCD mit Tastersteuerung für 1604 Display
//***********************
void loop1604() { 
    if ( millis() - displayMicros >= 5000 ) {
      displayMicros = millis();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print( "F\365llstand:"); // Ueberschrift
      lcd.setCursor(0,1);
      lcd.print(volumen);
      lcd.print( " Liter,");
      lcd.setCursor(0,3);
      lcd.print(fuellstand);
      lcd.print( " % voll");
    }
    
    unsigned long tasterMicros = millis();
    if ((tasterMicros - previousMicros >= 90000) && backlstatus) {  
      lcd.noBacklight();
      backlstatus = false;
      return;
    }

    if ((backlstatus == false) && (tasterHigh)) {
      resetBacklightTaster();
      lcd.backlight();
      backlstatus = true;
      previousMicros = millis();
      return;
    }  
    
    if (backlstatus && tasterHigh) {
      resetBacklightTaster();
      lcd.noBacklight();
      backlstatus = false;
      return;
    }
}
//***********************
// OLED Display
//***********************

  void loopOLED() { 

      // setze Display-Timout zu 30 Sekunden
      // prüfe wenn Display an ist, ob das letzte Einschalten mehr als 30sek in der Vergangenheit liegt
      unsigned long oledTimeout = millis();
      if ((oledTimeout - previousMicros >= 30000) && backlstatus ) {
       oledFill(&ssoled, 0, 1);
       backlstatus = false;
       return;
      }
      

      //if ((backlstatus == 1) && (digitalRead(taster) == HIGH)) {
      
      // Schalte das Display aus, falls es an ist
      // setze das Flag für den GPIO-Pin zurück
      if (backlstatus && tasterHigh) { 
       resetBacklightTaster();
       oledFill(&ssoled, 0, 1);
       //delay (300); //gegen Tastenprellen
       backlstatus = false;
       return;
      }

      //if (((backlstatus == 0) && (digitalRead(taster) == HIGH)) || (backlstatus == 1)){ 
      // if ((backlstatus == 0) && (digitalRead(taster) == HIGH)) {   
      
      // schalte Display ein, falls aus
      // aktualisiere Display nach 5 sek falls an
      // setze Flag für GPIO-Pin zurück und speichere Zeitpunkt des letzten Taster-Drucks in previousMicros
      if (!backlstatus && tasterHigh) {  
        if (tasterHigh) {
          previousMicros = millis();
        }
        // delay (300); //gegen Tastenprellen
        backlstatus = true;
        resetBacklightTaster();
      }

      if (backlstatus) {
        // aktualisiere Display
        if ( millis() - displayMicros >= 5000 ) {
          displayMicros = millis();
          oledFill(&ssoled, 0, 1);
          char Volumen[10];
          sprintf(Volumen,"%.2f l", volumen);
          char Fuellstand[10];
          sprintf(Fuellstand,"%.2f%%", fuellstand);
          oledWriteString(&ssoled, 0,0,0,(char *)"Fuellstand:" , FONT_NORMAL, 0, 1);
          oledWriteString(&ssoled, 0,60,3,Volumen, FONT_SMALL, 0, 1); 
          oledWriteString(&ssoled, 0,60,5,Fuellstand, FONT_SMALL, 0, 1);
          //*********************Grafik **********************
          rest = map(fuellstand, 0, 100, 62, 13);                // Hoehe des Balkens ermitteln
          oledRectangle(&ssoled, (1), (13), (45), (62), 1, 0);   // aeussere Linie zeichnen
          oledRectangle(&ssoled, (1), (rest), (45), (62), 1, 1); // Fuellbalken
          oledDumpBuffer(&ssoled, NULL);
        }
      }
}
