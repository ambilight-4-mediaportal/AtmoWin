========================================================================
DYNAMIC LINK LIBRARY : AtmoCtrlLib-Projektübersicht
========================================================================

Ist wohl eher was für Programmierer die selbst Anwendungen rund
um die AtmoWin Software bauen möchten - aber in Ihrer Programmiersprache
nicht die Möglichkeit haben den COM Server direkt einzubinden -

Wer die Möglichkeit hat den COM Server direkt anzusprechen - sei
diese auch empfohlen. Da die DLL wohl nicht immer alle Funktionen
bietet welche AtmoWin hat, sondern nur diese welche ich in meinen
Projekten gebrauche.

So nun zur Bibliothek.

Alle Funktionen sind als "C" Declare exportiert - d.h. die aufrufende
Funktion räumt den Stack ab... 

derzeit exportierte Funktionen:

int AtmoInitialize();
=====================
- Initialisiert die DLL zur Verwendung im aktuellen Thread - wenn ihr die DLL
und Ihre Funktionen in verschiedenen Threads nutzen wollt müsst ihr in jedem Thread
auch diese Funktion ausführen. Sonst geht es nicht - liegt am COM Umgebungssystem
und den Threadingmodellen... ;-)

Ein Rückgabewert von 1: bedeute AtmoWinA.exe ist gestartet und läuft, Initialisierung OK!
 -1: bedeutet keine AtmoWinA.exe gefunden - d.h. nicht gestartet!
 
 (nebenbei ruft diese Funktion auch für den Aufrufenden Thread die API OleInitialize()
  auf - im Falle des Rückgabewertes -1 wird aber gleich wieder OleUnitialize aufgerufen)
 

void AtmoFinalize(int what);
============================
Dient dem Cleanup der evtl. eingerichten Transferbuffer etc. über den Paramter
what gibt es nicht viele Worte zu verlieren - der Aufruf von
AtmoFinalize(0) -> ruft lediglich OleUnitialize() für den aktuellen Thread auf
AtmoFinalize(1) -> räumt auch ggf. mit AtmoCreateTransferBuffers eingerichtete Pixelbuffer
                   ab. 
Wenn man die DLL in mehreren Threads einer Anwendung verwendet sollte der Aufruf AtmoFinalize(1)
nur im Haupthread erfolgen wo auch AtmoCreateTransferBuffers(..) ausgeführt wurde.
Alle anderen Threads sollten lediglich vor dem Thread Ende AtmoFinalize(0) ausführen.                   

int AtmoSwitchEffect(int newMode);
==================================
Wechselt die AtmoLight Betriebsart - d.h. den aktiven Effekt der Rückgabewert der 
Funktion ist der aktuelle EffectMode - so das man damit ggf. später wieder den 
Ausgangsmodus aktivieren kann.

gültige Werte für Effektmode sind:
 0 -- Disabled, Keiner
 1 -- Statische Farbe	
 2 -- Live Bild (auf GDI Basis) siehe AtmoSetLiveSource!
 3 -- Farbwechsel auf allen Kanälen	
 4 -- Farbwechsel von Links nach Rechts
 ... 

int AtmoSetLiveSource(int newSource);
=====================================
wenn der aktuelle AtmoLightEffect Live Bild (2) ist kann hierüber die Quelle
festgelegt werden.

Möglich Werte für newSource sind:
 0  -- via GDI ScreenCapture (slow, high CPU usage...)
 1  -- externe Quelle - z.B. DirectShow Filter, oder VLC Modul für AtmoLight

Der Rückgabewert:
 1 - Signalisiert alles Ok Modus gewechselt.
-1 - AtmoWinA.exe nicht gestartet, oder der aktuelle Effect ist nicht "Live Bild"

void AtmoCreateTransferBuffers(int FourCC,int bytePerPixel,int width,int height);
================================================================================
richtet den Pixeltransferbuffer für den LiveSource Modus mit externer Quelle ein.
FourCC enthält dabei den Code wie die Pixeldatei aufgebaut sind - derzeit sind
folgende Codes in AtmoWinA.exe implementiert - andere Codes werden einfach ignoriert.
(Was der Bauer nicht kennt...)

 FourCC  --  "HSVI"  steht für ein Bild was bereits in HSV Daten vorliegt...
             oder man übergibt die in Windows.h? definierte Konstante "BI_RGB" was 
             signalisiert dass es sich um unkomprimierte Pixeldaten handelt.

bytePerPixel -- legt fest wieviel Byte ein Pixel benötigt - für HSVI wird der 
                Wert 3 erwartet.
             -- für BI_RGB sind die Werte 2,3 oder 4 zulässig. 
                (für 2 - ist RGB 565 definiert!)
width        -- Breite des Bildauszugs -> derzeit ist nur 64 zulässig!
height       -- Höhe des Bildauszugs -> derzeit ist nur 48 zulässig!         
                
void *AtmoLockTransferBuffer();
==============================
sperrt den Transferbuffer für den aktuellen Thread und liefert einen Zeiger auf den
Speicherblock zurück.

Sollte die DLL noch nicht initialisiert oder kein Aufruf von AtmoCreateTransferBuffers
im voraus erfolgt sein - liefert die Funktion "NULL"!

void AtmoSendPixelData();
=========================
schickt den Inhalt des Buffers an den COM Server der AtmoWinA.exe und hebt die Sperre
des Buffers auf.


