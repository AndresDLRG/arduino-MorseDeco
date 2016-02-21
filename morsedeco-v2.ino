#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "declaraciones.h"
#include "morse.h"

//  (RS, Enable, DB4, DB5, DB6, DB7)
LiquidCrystal lcd(PIN_RS, PIN_ENABLE, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7);
SoftwareSerial swSerial(PIN_RX, PIN_TX);
String cadenaLectura;
String cadenaEscritura;
int cont = 0;
int cont2 = 0;

// cuando es true espera texto para transformarlo a código morse
// cuando es false espera botonazos para pasarlos a texto
boolean modoDecodificar = false;

void toggleLecturaEscritura() {
  // cambia el modo de decodificar a codificar y viceversa
  // el LED indicador se enciende si está en modo decodificar,
  // se apaga en caso contrario
  if  (digitalRead(PIN_LEER) == HIGH) {
    modoDecodificar = !modoDecodificar;
    if (modoDecodificar) {
      digitalWrite(PIN_INDICADOR, HIGH );
    } else {
      digitalWrite(PIN_INDICADOR, LOW );
      cadenaLectura = "";
    }
    delay(400); // para evitar debouncing
  }
}

void setup() {
  // defino cuales pines son de entrada y cuales de salida
  pinMode(PIN_BEEP, OUTPUT);
  pinMode(PIN_PUNTO, OUTPUT );
  pinMode(PIN_LINEA, OUTPUT );
  pinMode(PIN_LEER, INPUT);
  pinMode(PIN_BOTON, INPUT);
  pinMode(PIN_INDICADOR, OUTPUT);

  // valores por defecto
  digitalWrite(PIN_BEEP, LOW);
  digitalWrite(PIN_LINEA, LOW );
  digitalWrite(PIN_PUNTO, LOW );
  digitalWrite(PIN_INDICADOR, LOW );

  swSerial.begin(9600);
  // inicializar la pantalla lcd
  lcd.begin(16, 2);
}


void loop() {
  // veo si se presiono el botón para cambiar codificar/decodificar
  toggleLecturaEscritura();

  if (!modoDecodificar) {

    // espacio para traducir los botonazos a texto o intentarlo
    if (cadenaEscritura != "") {
      cadenaEscritura += " ";
      swSerial.write("Se recibio de los botones: ");
      swSerial.write('"');
      for(uint8_t i = 0; i < cadenaEscritura.length(); ++i){
        swSerial.write(cadenaEscritura[i]);
      }
      swSerial.write('"');
      swSerial.write('\n');

      cadenaEscritura = decodificar(cadenaEscritura);
      swSerial.write("Se pudo traducir a: ");
      for(uint8_t i = 0; i < cadenaEscritura.length(); ++i){
        swSerial.write(cadenaEscritura[i]);
      }
      swSerial.write('\n');
      lcd.print(cadenaEscritura);

      cadenaEscritura = "";
      swSerial.write((byte)0);
      delay(TIEMPO);
    }

    // fin del espacio


    cadenaLectura = "";

    while (swSerial.available()) {
      char c = swSerial.read();
      cadenaLectura += c;
      delay(1);
    }

    if (cadenaLectura != "") {
      lcd.noAutoscroll();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Texto recibido: ");
      lcd.setCursor(0,1);
      for(int i = 0; i < cadenaLectura.length() - 1; i++) {
        lcd.print(cadenaLectura[i]);
        delay(200);
        if( i == 15)
          lcd.autoscroll();
      }

      String palabraMorse = codificar(cadenaLectura);
      swSerial.write('"');
      for (int i = 0; i <= palabraMorse.length(); i++) {
        switch (palabraMorse[i]) {
          case '.': //punto
            digitalWrite( PIN_PUNTO, HIGH );
            tone(PIN_BEEP, 1915);
            delay(TIEMPO);
            noTone(PIN_BEEP);
            digitalWrite( PIN_PUNTO, LOW );
            delay(TIEMPO);
            break;

          case '-': //guion
            digitalWrite(PIN_LINEA, HIGH );
            tone(PIN_BEEP, 1915);
            delay(TIEMPO * 3);
            noTone(PIN_BEEP);
            digitalWrite( PIN_LINEA, LOW );
            delay(TIEMPO);
            break;

          case ' ': //espacio
            delay(TIEMPO);
        }
        swSerial.write(palabraMorse[i]);
      }
      swSerial.write('"');
      swSerial.write('\n');
      delay(1);
      swSerial.write((byte)0);
    }

  } else {
    // en modo decodificar
    // esperando que el usuario introduzca el codigo morse con el boton
    if  (digitalRead(PIN_BOTON) == HIGH) {
      // el boton se presionó
      if (cont2 <= 12 && cont2 > 6) {
        cadenaEscritura += " ";
      } else if (cont2 > 12) {
        cadenaEscritura += "     ";
      }
      cont++;
      swSerial.write('*');
      delay(TIEMPO);

      cont2 = 0;
    } else {
      // el boton se soltó

      if (cont <= 5 && cont >= 1) {
        cadenaEscritura += ".";
      } else if (cont > 6) {
        cadenaEscritura += "-";
      }
      swSerial.write(' ');
      cont2++;
      delay(TIEMPO);

      cont = 0;
    }
  }
}
