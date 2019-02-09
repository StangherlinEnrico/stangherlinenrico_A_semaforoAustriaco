// INIZIO ATTRIBUENDO OGNI SINGOLO PIN AD UNA VARIABILE CON IL VALORE CORRISPONDENTE E DICHIARO LE VARIABILI UTILIZZATE
int pinLedRed1 = 10;
int pinLedYellow1 = 9;
int pinLedGreen1 = 8;
int pinLedRed2 = 7;
int pinLedYellow2 = 6;
int pinLedGreen2 = 5;

long ciclo = 0;
long tmpSemaforo = 0;
long tmpGiallo = 0;
long nLampeggi = 0;
long tmpLampeggio = 0;
long tmpVerde = 0;
long tmpRosso = 0;

// DICHIARO DELLE VARIABILI BOOLEANE PER FARE LE VERIFICHE DEI DATI IN INPUT DA MONITOR SERIALE
bool cicli = true;
bool giallo = true;
bool lampeggio = true;
bool lampeggi = true;

// CREO LA FUNZIONE CHE RICHIEDE IN INPUT LE INFORMAZIONI NECESSARIE AL SEMAFORO
void infoSemaforo() {
  digitalWrite(pinLedRed1, LOW); // Spegne il rosso1
  digitalWrite(pinLedGreen2, LOW); // Spegne il verde2

  while (cicli) // Esegue fino a che la variabile cicli resta vera
  {
    Serial.print("Quanti cicli vuoi che il semaforo esegua?\n"); // Stampa nel monitor seriale
    while (Serial.available() == 0); // Attente fino a che non riceve risposta
    ciclo = Serial.readString().toInt(); // Salva la risposta
    Serial.print("Ho ricevuto "); Serial.print(ciclo); Serial.print(" volte\n"); // Stampa nel monitor seriale
    if (ciclo > 0) // Verifica una condizione per la possibilità dei dati inseriti
      cicli = false; // Pone cicli uguale a false
    else
      Serial.print("Risultato non accettabile. Riformulo richiesta\n"); // Stampa nel monitor seriale
  }
  
  Serial.print("Quanto deve durare il semaforo? (millisecondi)\n"); // Stampa nel monitor seriale
  while (Serial.available() == 0); // Attende fino a che non riceve risposta
  tmpSemaforo = Serial.readString().toInt(); // Salva la risposta
  Serial.print("Ho ricevuto "); Serial.print(tmpSemaforo); Serial.print(" millisecondi\n"); // Stampa nel monitor seriale
  
  while (giallo) // Esegue fino a che la variabile giallo resta vera
  {
    Serial.print("Quanto deve durare il giallo? (millisecondi)\n"); // Stampa nel monitor seriale
    while (Serial.available() == 0); // Attende fino a che non riceve risposta
    tmpGiallo = Serial.readString().toInt(); // Salva la risposta
    Serial.print("Ho ricevuto "); Serial.print(tmpGiallo); Serial.print(" millisecondi\n"); // Stampa nel monitor seriale
    if (tmpGiallo < tmpSemaforo) // Verifica una condizione per la possibilità dei dati inseriti
      giallo = false; // Pone giallo uguale a false
    else
      Serial.print("Risultato non accettabile. Riformulo richiesta\n"); // Stampa nel monitor seriale
  }

  while (lampeggio) // Esegue fino a che la variabile lampeggio resta vera
  {
    Serial.print("Quante volte deve lampeggiare il verde?\n"); // Stampa nel monitor seriale
    while (Serial.available() == 0); // Attende una risposta
    nLampeggi = Serial.readString().toInt(); // Salva la risposta
    Serial.print("Ho ricevuto "); Serial.print(nLampeggi); Serial.print(" volte\n"); // Stampa nel monitor seriale
    if (nLampeggi > 0) // Verifica una condizione per la possibilità dei dati inseriti
      lampeggio = false; // Pone lampeggio uguale a false
    else
      Serial.print("Risultato non accettabile. Riformulo richiesta\n"); // Stampa nel monitor seriale
  }
  
  while (lampeggi) // Esegue fino a che lampeggi resta vera
  {
    Serial.print("Quanto deve durare ogni singolo lampeggio? (millisecondi)\n"); // Stampa nel monitor seriale
    while (Serial.available() == 0); // Attende una risposta
    tmpLampeggio = Serial.readString().toInt(); // Salva la risposta
    Serial.print("Ho ricevuto "); Serial.print(tmpLampeggio); Serial.print(" millisecondi\n"); // Stampa nel monitor seriale
    if ((tmpLampeggio * nLampeggi) < (tmpSemaforo - tmpGiallo) and tmpLampeggio > 0) // Verifica una condizione per la possibilità dei dati inseriti
      lampeggi = false; // Pone lampeggi uguale a false
    else
      Serial.print("Risultato non accettabile. Riformulo richiesta\n"); // Stampa nel monitor seriale
  }
  
  tmpVerde = tmpRosso = (tmpSemaforo - tmpGiallo - (nLampeggi * tmpLampeggio)) / 4; // Calcola il valore da attribuire a tmpVerde e tmpRosso

  cicli = true; // Pone cicli uguale a true per i cicli successivi
  giallo = true; // Pone giallo uguale a true per i cicli successivi
  lampeggio = true; // Pone lampeggio uguale a true per i cicli successivi
  lampeggi = true; // Pone lampeggi uguale a true per i cicli successivi
  
  digitalWrite(pinLedRed1, HIGH); // Accende il rosso1
  digitalWrite(pinLedGreen2, HIGH); // Accende il verde2
}

// CREO LA FUNZIONE CHE PASSA DA UN SEMAFORO ALL'ALTRO
void semaforoAndata() {
  delay(tmpVerde); // Durata del verde
  for (int i = -1; i < nLampeggi; i++) { // Ciclo per i lampeggi
    digitalWrite(pinLedGreen2, HIGH); // Accende il verde2
    delay(tmpLampeggio / 2); // Aspetta il tempo per il lampeggio
    digitalWrite(pinLedGreen2, LOW); // Spegne il verde2
    delay(tmpLampeggio / 2); } // Aspetta il tempo per il lampeggio
  digitalWrite(pinLedYellow1, HIGH); // Accende il giallo1
  digitalWrite(pinLedYellow2, HIGH); // Accende il giallo2
  delay(tmpGiallo / 2); // Aspetta il tempo dei gialli
  digitalWrite(pinLedRed1, LOW); // Spegne il rosso1
  digitalWrite(pinLedYellow1, LOW); // Spegna il giallo1
  digitalWrite(pinLedGreen1, HIGH); // Accende il verde1
  digitalWrite(pinLedRed2, HIGH); // Accende il rosso2
  digitalWrite(pinLedYellow2, LOW); // Spegne il giallo2
  }

// CREO UNA FUNZIONE INVERSA CHE ESEGUE IL RITORNO ALLA SITUAZIONE INIZIALE
void semaforoRitorno() {
  delay(tmpRosso); // Durata del rosso
  for (int i = -1; i < nLampeggi; i++) { // Ciclo per i lampeggi del verde
    digitalWrite(pinLedGreen1, HIGH); // Accende il verde1
    delay(tmpLampeggio / 2); // Aspetta il tempo del lampeggio
    digitalWrite(pinLedGreen1, LOW); // Spegne il verde1
    delay(tmpLampeggio / 2); } // Aspetta il tempo del lampeggio
  digitalWrite(pinLedYellow1, HIGH); // Accende il giallo1
  digitalWrite(pinLedYellow2, HIGH); // Accende il giallo2
  delay(tmpGiallo / 2); // Aspetta il tempo del giallo
  digitalWrite(pinLedYellow2, LOW); // Spegne il giallo2
  digitalWrite(pinLedRed2, LOW); // Spegne il rosso2
  digitalWrite(pinLedGreen2, HIGH); // Accende il verde2
  digitalWrite(pinLedYellow1, LOW); // Spegne il giallo1
  digitalWrite(pinLedRed1, HIGH); // Accende il rosso1
  }
  
void setup() { // VOID SETUP CHE VIENE ESEGUITO SOLO UNA VOLTA
  // VADO A DIRE AL PROGRAMMA CHE CI SONO DEI DISPOSITIVI CHE DEVONO RICEVERE INFORMAZIONI DALLA SCHEDA E NON FORNIRLE (es.: Un diodo piuttosto che un pulsante)
  pinMode(pinLedRed1, OUTPUT);
  pinMode(pinLedYellow1, OUTPUT);
  pinMode(pinLedGreen1, OUTPUT);
  pinMode(pinLedRed2, OUTPUT);
  pinMode(pinLedYellow2, OUTPUT);
  pinMode(pinLedGreen2, OUTPUT);
  Serial.begin(9600); // Abilito il monitor seriale
  infoSemaforo(); // Richiamo la funzione che richiede le info per il semaforo
  }
  
void loop() {
  for (int i = 0; i < ciclo; i++) // Ciclo per eseguire il semaforo per un tot volte prescelto dall'utente
  {
    semaforoAndata(); // Richiamo della funzione di andare
    semaforoRitorno(); // Richiamo della funzione di ritorno
  }
  infoSemaforo(); // Richiamo della funzione per modificare il semaforo
  }
