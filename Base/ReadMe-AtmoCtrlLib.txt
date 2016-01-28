========================================================================
DYNAMIC LINK LIBRARY : AtmoCtrlLib-Projekt�bersicht
========================================================================

Ist wohl eher was f�r Programmierer die selbst Anwendungen rund
um die AtmoWin Software bauen m�chten - aber in Ihrer Programmiersprache
nicht die M�glichkeit haben den COM Server direkt einzubinden -

Wer die M�glichkeit hat den COM Server direkt anzusprechen - sei
diese auch empfohlen. Da die DLL wohl nicht immer alle Funktionen
bietet welche AtmoWin hat, sondern nur diese welche ich in meinen
Projekten gebrauche.

So nun zur Bibliothek.

Alle Funktionen sind als "C" Declare exportiert - d.h. die aufrufende
Funktion r�umt den Stack ab... 

derzeit exportierte Funktionen:

int AtmoInitialize();
=====================
- Initialisiert die DLL zur Verwendung im aktuellen Thread - wenn ihr die DLL
und Ihre Funktionen in verschiedenen Threads nutzen wollt m�sst ihr in jedem Thread
auch diese Funktion ausf�hren. Sonst geht es nicht - liegt am COM Umgebungssystem
und den Threadingmodellen... ;-)

Ein R�ckgabewert von 1: bedeute AtmoWinA.exe ist gestartet und l�uft, Initialisierung OK!
 -1: bedeutet keine AtmoWinA.exe gefunden - d.h. nicht gestartet!
 
 (nebenbei ruft diese Funktion auch f�r den Aufrufenden Thread die API OleInitialize()
  auf - im Falle des R�ckgabewertes -1 wird aber gleich wieder OleUnitialize aufgerufen)
 

void AtmoFinalize(int what);
============================
Dient dem Cleanup der evtl. eingerichten Transferbuffer etc. �ber den Paramter
what gibt es nicht viele Worte zu verlieren - der Aufruf von
AtmoFinalize(0) -> ruft lediglich OleUnitialize() f�r den aktuellen Thread auf
AtmoFinalize(1) -> r�umt auch ggf. mit AtmoCreateTransferBuffers eingerichtete Pixelbuffer
                   ab. 
Wenn man die DLL in mehreren Threads einer Anwendung verwendet sollte der Aufruf AtmoFinalize(1)
nur im Haupthread erfolgen wo auch AtmoCreateTransferBuffers(..) ausgef�hrt wurde.
Alle anderen Threads sollten lediglich vor dem Thread Ende AtmoFinalize(0) ausf�hren.                   

int AtmoSwitchEffect(int newMode);
==================================
Wechselt die AtmoLight Betriebsart - d.h. den aktiven Effekt der R�ckgabewert der 
Funktion ist der aktuelle EffectMode - so das man damit ggf. sp�ter wieder den 
Ausgangsmodus aktivieren kann.

g�ltige Werte f�r Effektmode sind:
 0 -- Disabled, Keiner
 1 -- Statische Farbe	
 2 -- Live Bild (auf GDI Basis) siehe AtmoSetLiveSource!
 3 -- Farbwechsel auf allen Kan�len	
 4 -- Farbwechsel von Links nach Rechts
 ... 

int AtmoSetLiveSource(int newSource);
=====================================
wenn der aktuelle AtmoLightEffect Live Bild (2) ist kann hier�ber die Quelle
festgelegt werden.

M�glich Werte f�r newSource sind:
 0  -- via GDI ScreenCapture (slow, high CPU usage...)
 1  -- externe Quelle - z.B. DirectShow Filter, oder VLC Modul f�r AtmoLight

Der R�ckgabewert:
 1 - Signalisiert alles Ok Modus gewechselt.
-1 - AtmoWinA.exe nicht gestartet, oder der aktuelle Effect ist nicht "Live Bild"

void AtmoCreateTransferBuffers(int FourCC,int bytePerPixel,int width,int height);
================================================================================
richtet den Pixeltransferbuffer f�r den LiveSource Modus mit externer Quelle ein.
FourCC enth�lt dabei den Code wie die Pixeldatei aufgebaut sind - derzeit sind
folgende Codes in AtmoWinA.exe implementiert - andere Codes werden einfach ignoriert.
(Was der Bauer nicht kennt...)

 FourCC  --  "HSVI"  steht f�r ein Bild was bereits in HSV Daten vorliegt...
             oder man �bergibt die in Windows.h? definierte Konstante "BI_RGB" was 
             signalisiert dass es sich um unkomprimierte Pixeldaten handelt.

bytePerPixel -- legt fest wieviel Byte ein Pixel ben�tigt - f�r HSVI wird der 
                Wert 3 erwartet.
             -- f�r BI_RGB sind die Werte 2,3 oder 4 zul�ssig. 
                (f�r 2 - ist RGB 565 definiert!)
width        -- Breite des Bildauszugs -> derzeit ist nur 64 zul�ssig!
height       -- H�he des Bildauszugs -> derzeit ist nur 48 zul�ssig!         
                
void *AtmoLockTransferBuffer();
==============================
sperrt den Transferbuffer f�r den aktuellen Thread und liefert einen Zeiger auf den
Speicherblock zur�ck.

Sollte die DLL noch nicht initialisiert oder kein Aufruf von AtmoCreateTransferBuffers
im voraus erfolgt sein - liefert die Funktion "NULL"!

void AtmoSendPixelData();
=========================
schickt den Inhalt des Buffers an den COM Server der AtmoWinA.exe und hebt die Sperre
des Buffers auf.


