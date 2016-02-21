//Estructura para mapear todos los caracteres de codigo morse
static const struct {
  const char letra, *codigo;
}

MAPA_MORSE[] = {
  { 'A', ".-"    },
  { 'B', "-..."  },
  { 'C', "-.-."  },
  { 'D', "-.."   },
  { 'E', "."     },
  { 'F', "..-."  },
  { 'G', "--."   },
  { 'H', "...."  },
  { 'I', ".."    },
  { 'J', ".---"  },
  { 'K', ".-.-"  },
  { 'L', ".-.."  },
  { 'M', "--"    },
  { 'N', "-."    },
  { 'O', "---"   },
  { 'P', ".--."  },
  { 'Q', "--.-"  },
  { 'R', ".-."   },
  { 'S', "..."   },
  { 'T', "-"     },
  { 'U', "..-"   },
  { 'V', "...-"  },
  { 'W', ".--"   },
  { 'X', "-..-"  },
  { 'Y', "-.--"  },
  { 'Z', "--.."  },
  { ' ', "     " }, //El espacio se representara con 5 tiempos sin  luz y sonido
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { '0', "-----" }
};

// pasa una palabra a notacion de puntos y guiones
String codificar(String cadena) {
  String palabraMorse = "";
  for (int i = 0; cadena[i]; i++) {
    for (int j = 0; j < sizeof MAPA_MORSE / sizeof *MAPA_MORSE; j++) {
      if ( toupper(cadena[i]) == MAPA_MORSE[j].letra) {
        palabraMorse += MAPA_MORSE[j].codigo;
        break;
      }
    }
    palabraMorse += " "; //Se agrega un espacio para separar los caracteres
  }
  return palabraMorse;
}

// pasa de notación de puntos y guiones hacia una palabra
String decodificar(String morse) {
  String mensaje = "";
  int ultPos = 0;
  int pos = morse.indexOf(' ');
  while ( ultPos <= morse.lastIndexOf(' ') ) {
    for ( int i = 0; i < sizeof MAPA_MORSE / sizeof *MAPA_MORSE; i++) {
      if ( morse.substring(ultPos, pos) == MAPA_MORSE[i].codigo ) {
        mensaje += MAPA_MORSE[i].letra;
      }
    }
    ultPos = pos + 1;
    pos = morse.indexOf(' ', ultPos);
    // maneja espacios entre palabras (5 espacios)
    while ( morse[ultPos] == ' ' && morse[pos + 1] == ' ' ) {
      pos ++;
    }
  }
  return mensaje;
}
