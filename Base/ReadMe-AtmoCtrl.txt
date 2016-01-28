========================================================================
KONSOLENANWENDUNG : AtmoCtrl-Projektübersicht
========================================================================

AtmoCtrl.exe ist eine Steuerprogramm um von der Befehlsteile aus
bestimmte AtmoLight Parameter zu verändern. Wer nicht programmieren
möchte kann dieses Programm z.B. in Girder, EventGhost oder in Batch-
dateien verwenden, um die Farben zu wechseln die Betriebsart... etc...

Das Programm unterstützt folgende Parameter.

AtmoCtrl.exe [-effect {none,live,static,lrchanger,changer}] [-color red green blue] [-sleep delay]

-effect {none,live,static,lrchanger,changer}  gefolgt von einer der Optionen in {} schaltet
 den aktuellen Effekt um.
 * none -- kein Effekt aktiv 
 * live -- um schalten in Screencapture Modus (GDI) 
 * static -- aktiviert die als statisch voreingestellte Farbe
 * lrchange -- Farbwechsel von Links nach Rechts...
 * changer -- wechselt die Farben auf allen Kanäle 
 
 -color red green blue    - stellt die aktuelle Farbe ein wobei "red green blue" durch Zahlen
  im Bereich 0..255 zu ersetzen sind (RGB Werte) Diese Funktion noch nur Sinn wenn man sich im
  effect none oder static befindet, sonst blitzt die Farbe nur kurz auf und verschwindet
  gleich wieder.
  
 -sleep delay   -- fügt eine Pause zwischen der Verarbeitung von Parametern ein, die Angabe erfolgt in
  Millisekunden.
  
  Jeder der Parameter kann beliebig oft in einer Befehlszeile wiederholt werden...
  
z.B. bewirkt folgende Befehlzeile...
AtmoCtrl.exe -effect none -color 0 0 0 -sleep 500 -color 64 0 0 -sleep 500 -color 128 0 0 -sleep 500 -color 255 0 0 -sleep 2000 -effect changer
 1. Umschalten auf keinen aktiven Effekt. 
 2. Farbe Schwarz (0,0,0) ausgeben auf allen Kanälen
 3. 0,5s warten
 4. ein leichtes Rot ausgeben auf allen Kanälen
 5. 0,5s warten
 6. ein mittleres Rot ausgeben auf allen Kanälen
 7. 0,5s warten
 8. ein stattes Rot ausgeben auf allen Kanälen 
 9. 2s warten
10. in den Farbwechselmodus gehen und Programm Ende...

statt dessen könnte man in einer Batchdatei auch folgende Sequenz schreiben...
AtmoCtrl.exe -effect none 
AtmoCtrl.exe -color 0 0 0 -sleep 500 
AtmoCtrl.exe -color 64 0 0 -sleep 500 
AtmoCtrl.exe -color 128 0 0 -sleep 500 
AtmoCtrl.exe -color 255 0 0 -sleep 2000 
AtmoCtrl.exe -effect changer

was den gleichen Effect hätte...

weitere Funktionen gibts auf Anfrage ... oder ihr programmiert sie euch selbst,
was meint ihr warum der Source sonst verfügbar ist?  **g**

Euer Igor


 