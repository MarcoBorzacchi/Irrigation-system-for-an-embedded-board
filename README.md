# Irrigation-system-for-an-embedded-board

Il progetto riguarda l'implementazione di un sistema di controllo di un impianto di irrigazione per un giardino diviso in due
zone. Ll sistema dispone di un LCD, due pulsanti di comando e
due LED. Inoltre, il circuito acquisisce il valore della temperatura ambiente T AMB con un trasduttore
di temperatura (simulato con un trimmer collegato tra V DD e GND).
Il trasduttore fornisce in decine di mV il valore di temperatura in Celsius (p. es., a 20°C, il
trasduttore fornisce ai suoi capi 0.2V. Inoltre, per una variazione di temperatura di 1 °C, la tensione
d'uscita del trasduttore varia di 10 mV).
I due LED servono a visualizzare lo stato delle due elettrovalvole delle zone.
All'avvio, l'apparato mostra, sulla prima riga, l'ora corrente nel formato “hh:mm:ss” e, sulla
seconda riga, la temperatura misurata dal trasduttore.
Premendo in successione il tasto “MODE” è possibile visualizzare rispettivamente:
1- temperatura massima della giornata;
3- ora di inizio irrigazione della zona A;
4- ora di fine irrigazione della zona A;
5- ora di inizio irrigazione della zona B;
6- ora di fine irrigazione della zona B.
Per ogni visualizzazione di orario, ora corrente o di irrigazione, è possibile programmare il
valore voluto premendo il tasto SET. In questo caso, con MODE è possibile incrementare il valore.
Si passa da ore a minuti premendo il tasto SET. Il diagramma mostrato nella figura della pagina
successiva riassume la modalità di programmazione degli orari.
Il sensore di temperatura serve a monitorare il valore di temperatura ambiente. Se il valore
massimo ha superato i 30 °C, ciascun periodo di irrigazione deve essere incrementato del 10 %. Se il
valore massimo ha superato invece i 35 °C, l'incremento dev'essere del 20 %.
Il valore massimo di temperatura deve essere "resettato" alle ore 8:00:00 o dopo il termine
dell'ultimo periodo di irrigazione se questo è successivo alle 8:00:00
