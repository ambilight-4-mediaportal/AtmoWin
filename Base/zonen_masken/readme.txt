Wer sein Atmolight bisher mit drei Kan�len betrieben hat und
eine lange Ledleiste und zwei kurze aus der Sammelbestellung
besitzt - hat ja noch einen Atmolight Kanal brach liegen
mit der neuen Version von AtmoWin besteht die M�glichkeit
diesen Kanal so zu nutzen das man die lange Leiste in zwei
Teile zerlegt - nat�rlich macht es dann keinen Sinn mehr
die zwei H�lften mit dem gleichen Signal anzusteuern - also
muss man auch die Definition anpassen welche den Bildbereich
festlegt, und wie stark dieser auf die jeweilige Leiste wirkt. 

Die CDR Dateien stammen von einem alten CorelDraw7 und lassen
sich somit mit jeder aktuelleren Version �ffnen und nach
belieben bearbeiten. 

Es ist zu empfehlen Gradientf�llenungen von Schwarz nach Wei� 
oder umgekehrt zu verwenden. Je wei�er ein Bereich ist desto
st�rker geht der jeweilige Bildbereich sp�ter in die Berechnung
des Kanals ein.

Das Rechteck was man zu Bearbeitung verwendet - sollte im gleichen
Seitenverh�ltnis sein wie sp�ter die Bitmap (4 : 3) - ich habe
einfach 128 x 96mm verwendet... das ist auf dem Bildschirm nicht
so winzig.
Die CDR Datei exportiert man dann als unkomprimierte Bitmap,
mit 256 Graustufen und einer Aufl�sung von 64 x 48 Pixeln.

Im Ordner "Standard" liegen die Definition so wie sie im Standard
Atmowin verwendet werden - es ist nicht notwendig diese Dateien
in den Programmordner zu kopieren - sie dienen nur der Anschauung.
(und vielleicht besseren Verst�ndnis wie AtmoWin arbeitet.)

Im Ordner oben_geteilt gibt es f�r die oben beschriebene Variante
mehre Definition die man ausprobieren kann.

Die Beispieldateien m�sst ihr nur in den Ordner zu AtmoWinA.exe
kopieren - wenn das Programm neu gestartet wird - werden die Definitonen
automatisch eingelesen.

-- die aktuellen AtmoWin Versionen ben�tigen in den meisten
Anwendungsf�llen nun keine Gradient Files mehr, da sich das
Layout der Zonen zur Aufteilungs des Bildschirms in der Software
direkt einrichten l�sst - und daraus die einfachen Farbverl�ufe
selbst erzeugt werden k�nnen.  



-----
Wie ihr seht sind die Farbverl�ufe in den Bitmap linear - 
allerdings wirken sie nicht so - denn auf diesen Wert wird
noch der im AtmoWin konfigurierbare Parameter [Edge Weightning]
angewandt - und zwar als Exponent zu diesem Verlauf - d.h.
je h�her Edge Weightning ist - desto Randbezogner wird der
Algorithmus bei seiner Farbgebung! - nur wenn Edge Weightning den
Wert 1 hat - kommt auch wirklich der Lineare Verlauf zum Einsatz.

