int led1 = 2; // red
int led2 = 13; // green

int buttonRed = 3;
int buttonGreen = 4;

int start_bit = 0;
int* ledOrder = new int[3];
int newCapacity = 3;
int pressedCounter = 0;
int* ledOrderUser = new int[3];

int parpadeos = 3;
int newLed = 0;

bool win = true;

void setup() {
  Serial.begin(9600);
  
  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);
  
  pinMode(buttonRed, INPUT);
  pinMode(buttonGreen, INPUT);
}

void loop() {
  //1. Generamos una secuencia aleatoria de 3 parpadeos
  if(start_bit == 0){
      secuenciaParpadeos();//Solo se ejcuta la primera vez
  }
  start_bit = 1;
  //Imprimimos los leds que se han iluminado por orden
//  Serial.println("value: " + String(ledOrder[0]));
//  Serial.println("value: " + String(ledOrder[1]));
//  Serial.println("value: " + String(ledOrder[2]));
  
  //2. Registramos los botones pulsados por el usuario
  registrarLedsPulsadosUsuario();

  //3. Se comprueba que el usuario acertó
  if(win){
    delay(1500);
    Serial.println("Current game status: " + String(win));
    //Aumentamos el tamaño de las listas
    if(parpadeos >= newCapacity){
        resizeListas(); 
    }
    //Pasamos al siguiente nivel
    parpadeos++;
  }
  else{//El usuario ha fallado
    //Volvemos al inicio
    //Borramos listas
    borrarLedsEnListas();
    parpadeos = 3;
    win = true;
    newLed = 0;
  }
  start_bit = 0;
}

void secuenciaParpadeos(){
   int aux = 0;
   int led = 0;
   while(aux < parpadeos) {
      if (newLed == aux) {
      led = random(2,4);//me puede salir 2 o 3
      Serial.println("Led selector: " + String(led));
      ledOrder[aux] = led;//Registramos el led que se va a iluminar
      newLed++;
      Serial.println("newLed " + String(newLed));
      }

      if (ledOrder[aux] == 2) {
       digitalWrite(led1, HIGH);
       digitalWrite(led2, LOW);
      }
     if (ledOrder[aux] == 3) {
       digitalWrite(led2, HIGH);
       digitalWrite(led1, LOW);
     }
      
//     Serial.println("Led value: " + String(ledOrder[aux]));
//     Serial.println("aux: " + String(aux));
      
     aux++;
      
     delay(500);     
     digitalWrite(led2, LOW);
     digitalWrite(led1, LOW);
     delay(500);     
   }  

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  
}

void registrarLedsPulsadosUsuario(){
  int aux = 0;
  while(aux < parpadeos){
    if(digitalRead(buttonRed) == HIGH){
      Serial.println("Red pressed!");
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      ledOrderUser[aux]=2;
      delay(1000);//No se pone fuera del if porque si no se pulsa ningún botón va a hacer el delay
      digitalWrite(led1, LOW);
//      Serial.println("ledOrderUser " + String(ledOrderUser[aux]));      
    
//      Serial.println("ledOrderC " + String(ledOrder[aux]));
//      Serial.println("ledOrderUserC " + String(ledOrderUser[aux]));
       if(ledOrder[aux] != ledOrderUser[aux]){
          win=false;
          aux = parpadeos; // rompemos el bucle
       }       
      aux++;
    }
    else if(digitalRead(buttonGreen) == HIGH){
      Serial.println("Green pressed!");
      digitalWrite(led2, HIGH);
      digitalWrite(led1, LOW);
      ledOrderUser[aux]=3;
      delay(1000);
      digitalWrite(led2, LOW);
//      Serial.println("ledOrderUser " + String(ledOrderUser[aux]));
    
//      Serial.println("ledOrderC " + String(ledOrder[aux]));
//      Serial.println("ledOrderUserC " + String(ledOrderUser[aux]));
       if(ledOrder[aux] != ledOrderUser[aux]){
          win=false;
          aux = parpadeos; // rompemos el bucle
       }
       
      aux++;
    }
   }
}

void resizeListas(){
   newCapacity = parpadeos * 2;
   int* newList = new int[newCapacity];
//   memmove(newList, ledOrder, parpadeos  * sizeof(int));
   memmove(newList, ledOrder, sizeof(ledOrder[0])*newCapacity);
   delete[] ledOrder;
   ledOrder = newList;
   ledOrderUser = new int[newCapacity]; 
}

void borrarLedsEnListas(){
  ledOrder = new int[3];
  ledOrderUser = new int[3];
}
