
//infrapale/feeds/home-tampere.tampere-indoor-temperature

// Adafruit_MQTT_Subscribe *aio_subs[AIO_SUBS_NBR_OF] =
// {
//   [AIO_SUBS_TIME]           = &timefeed,
//   [AIO_SUBS_TRE_ID_TEMP]    = &tre_id_temp_feed,
//   [AIO_SUBS_TRE_ID_HUM]     = &tre_id_hum_feed,
//   [AIO_SUBS_LA_ID_TEMP]     = &lilla_astrid_id_temp,
//   [AIO_SUBS_VA_OD_TEMP]     = &villa_astrid_od_temp,
//   [AIO_SUBS_VA_OD_HUM]      = &villa_astrid_od_hum,
//   [AIO_SUBS_RUUVI_E6_TEMP]  = &ruuvi_e6_temp,
//   [AIO_SUBS_RUUVI_EA_TEMP]  = &ruuvi_ea_temp,
//   [AIO_SUBS_RUUVI_ED_TEMP]  = &ruuvi_ed_temp,
// };





  // First we test them with a background colour set
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
  tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
  int xpos = 0;
  xpos += tft.drawString("xyz{|}~", 0, 64, 2);
  tft.drawChar(127, xpos, 64, 2);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  tft.drawString("/0123456789:;", 0, 26, 4);
  tft.drawString("<=>?@ABCDE", 0, 52, 4);
  tft.drawString("FGHIJKLMNO", 0, 78, 4);
  tft.drawString("PQRSTUVWX", 0, 104, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  tft.drawString("defghijklmno", 0, 26, 4);
  tft.drawString("pqrstuvwxyz", 0, 52, 4);
  xpos = 0;
  xpos += tft.drawString("{|}~", 0, 78, 4);
  tft.drawChar(127, xpos, 78, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);

  tft.drawString("012345", 0, 0, 6);
  tft.drawString("6789", 0, 40, 6);
  tft.drawString("apm-:.", 0, 80, 6);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);

  tft.drawString("0123", 0, 0, 7);
  tft.drawString("4567", 0, 60, 8);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 7);
  tft.drawString("", 0, 60, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.drawString("0123", 0, 0, 8);
  tft.drawString("4567", 0, 72, 8);
  delay(WAIT);;

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 72, 8);
  delay(WAIT);;

  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
  tft.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
  xpos = 0;
  xpos += tft.drawString("xyz{|}~", 0, 128, 2);
  tft.drawChar(127, xpos, 128, 2);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);

  tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  tft.drawString("/0123456789:;", 0, 52, 4);
  tft.drawString("<=>?@ABCDE", 0, 104, 4);
  tft.drawString("FGHIJKLMNO", 0, 156, 4);
  tft.drawString("PQRSTUVWX", 0, 208, 4);
  delay(WAIT);
  tft.fillScreen(TFT_BLACK);
  tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  tft.drawString("defghijklmno", 0, 52, 4);
  tft.drawString("pqrstuvwxyz", 0, 104, 4);
  xpos = 0;
  xpos += tft.drawString("{|}~", 0, 156, 4);
  tft.drawChar(127, xpos, 156, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_BLUE, TFT_BLACK);

  tft.drawString("01234", 0, 0, 6);
  tft.drawString("56789", 0, 80, 6);
  tft.drawString("font 6", 0, 160, 6);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);

  tft.drawString("0123", 0, 0, 7);
  tft.drawString("4567", 0, 120, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 7);
  tft.drawString("", 0, 120, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  tft.drawString("0123", 0, 0, 8);
  tft.drawString("4567", 0, 144, 8);
  delay(WAIT);;

  tft.fillScreen(TFT_BLACK);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 144, 8);
  delay(WAIT);;

  tft.setTextColor(TFT_MAGENTA, TFT_BROWN);

  tft.drawNumber(millis() - targetTime, 0, 180, 4);
  delay(4000);;

  // Now test them with transparent background
  targetTime = millis();

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW);


  uint8_t text_size = 1;
  while(true)
  {
      tft.setTextSize(text_size);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("001", 0, 0, 1);
      delay(WAIT);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("002", 0, 0, 2);
      delay(WAIT);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("003", 0, 0, 3);
      delay(WAIT);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("004", 0, 0, 4);
      delay(WAIT);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("005", 0, 0, 5);
      delay(WAIT);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("006", 0, 0, 6);
      delay(WAIT);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("007", 0, 0, 7);
      delay(WAIT);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("008", 0, 0, 8);
      delay(WAIT);
      tft.fillScreen(TFT_BLACK);
      tft.drawString("009", 0, 0, 9);
      delay(WAIT);
      //text_size++;

  }

  tft.setTextSize(1);
  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_GREEN);

  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
  tft.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
  xpos = 0;
  xpos += tft.drawString("xyz{|}~", 0, 64, 2);
  tft.drawChar(127, xpos, 64, 2);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_GREEN);

  tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  tft.drawString("/0123456789:;", 0, 26, 4);
  tft.drawString("<=>?@ABCDE", 0, 52, 4);
  tft.drawString("FGHIJKLMNO", 0, 78, 4);
  tft.drawString("PQRSTUVWX", 0, 104, 4);

  delay(WAIT);
  tft.fillScreen(TFT_BROWN);
  tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  tft.drawString("defghijklmno", 0, 26, 4);
  tft.drawString("pqrstuvwxyz", 0, 52, 4);
  xpos = 0;
  xpos += tft.drawString("{|}~", 0, 78, 4);
  tft.drawChar(127, xpos, 78, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_BLUE);

  tft.drawString("012345", 0, 0, 6);
  tft.drawString("6789", 0, 40, 6);
  tft.drawString("apm-:.", 0, 80, 6);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_RED);

  tft.drawString("0123", 0, 0, 7);
  tft.drawString("4567", 0, 60, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.drawString("890:.", 0, 0, 7);
  tft.drawString("", 0, 60, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_YELLOW);

  tft.drawString("0123", 0, 0, 8);
  tft.drawString("4567", 0, 72, 8);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 72, 8);
  delay(WAIT);

  tft.setTextSize(2);
  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_GREEN);

  tft.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  tft.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
  tft.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
  tft.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
  xpos = 0;
  xpos += tft.drawString("xyz{|}~", 0, 128, 2);
  tft.drawChar(127, xpos, 128, 2);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_GREEN);

  tft.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  tft.drawString("/0123456789:;", 0, 52, 4);
  tft.drawString("<=>?@ABCDE", 0, 104, 4);
  tft.drawString("FGHIJKLMNO", 0, 156, 4);
  tft.drawString("PQRSTUVWX", 0, 208, 4);
  delay(WAIT);
  tft.fillScreen(TFT_BROWN);
  tft.drawString("YZ[\\]^_`abc", 0, 0, 4);
  tft.drawString("defghijklmno", 0, 52, 4);
  tft.drawString("pqrstuvwxyz", 0, 104, 4);
  xpos = 0;
  xpos += tft.drawString("{|}~", 0, 156, 4);
  tft.drawChar(127, xpos, 156, 4);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_BLUE);

  tft.drawString("01234", 0, 0, 6);
  tft.drawString("56789", 0, 80, 6);
  tft.drawString("apm-:.", 0, 160, 6);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_RED);

  tft.drawString("0123", 0, 0, 7);
  tft.drawString("4567", 0, 120, 8);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 120, 7);
  delay(WAIT);

  tft.fillScreen(TFT_BROWN);
  tft.setTextColor(TFT_YELLOW);

  tft.drawString("01 23", 0, 0, 8);
  tft.drawString("4567", 0, 144, 8);
  delay(WAIT);;

  tft.fillScreen(TFT_BROWN);
  tft.drawString("890:.", 0, 0, 8);
  tft.drawString("", 0, 144, 8);
  delay(WAIT);;

  tft.setTextColor(TFT_MAGENTA);

  tft.drawNumber(millis() - targetTime, 0, 180, 4);
