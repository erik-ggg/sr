int led1 = 2;
int led2 = 3;

int button1 = 4;
int button2 = 5;

int start_bit = 0;
int* ledOrder = new int[3];
int newCapacity = 3;
int pressedCounter = 0;
int* ledOrderUser = new int[3];

int parpadeos = 3;

bool win = true;

void setup() {
  Serial.begin(9600);
  
  pinMode(led1, OUTPUT);
  digitalWrite(led1, LOW);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, LOW);
  
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
}

void loop() {
  //1. Generamos una secuencia aleatoria de 3 parpadeos
  if(start_bit == 0){
      secuenciaParpadeos();//Solo se ejcuta la primera vez
  }
  start_bit = 1;
  //Imprimimos los leds que se han iluminado por orden
  Serial.println("value: " + String(ledOrder[0]));
  Serial.println("value: " + String(ledOrder[1]));
  Serial.println("value: " + String(ledOrder[2]));
  
  //2. Registramos los botones pulsados por el usuario
  registrarLedsPulsadosUsuario();

  //3. Se comprueba que el usuario acertó
  if(win){
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
  }
  start_bit = 0;
}

void secuenciaParpadeos(){
   int aux = 0;
   int led = 0;
   while(aux < parpadeos) {
     if(sizeof(ledOrder) == aux){
        led = random(1,4);//me puede salir 2 o 3
        Serial.println("Led selector: " + String(led));
        ledOrder[aux] = led;//Registramos el led que se va a iluminar
     }
     
     if (ledOrder[aux] == led1) {
       digitalWrite(led1, HIGH);
       digitalWrite(led2, LOW);
     }
     if (ledOrder[aux] == led2) {
       digitalWrite(led2, HIGH);
       digitalWrite(led1, LOW);
     }
      
     Serial.println("Led value: " + String(ledOrder[aux]));
     Serial.println("aux: " + String(aux));
      
     aux++;
      
     delay(1000);
   }  
  
}

void registrarLedsPulsadosUsuario(){
  int aux = 0;
  while(aux < parpadeos){
    int button1State = digitalRead(button1);
    int button2State = digitalRead(button2);
    if(button1State == HIGH){
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
      ledOrderUser[aux]=led1;
      aux++;
      delay(1000);//No se pone fuera del if porque si no se pulsa ningún botón va a hacer el delay
    }
    else if(button2State == HIGH){
      digitalWrite(led2, HIGH);
      digitalWrite(led1, LOW);
      ledOrderUser[aux]=led2;
      aux++;
      delay(1000);
    }
   }

   if(ledOrder[aux] != ledOrderUser[aux]){
      win=false;
   }
}

void resizeListas(){
   newCapacity = parpadeos * 2;
   int* newList = new int[newCapacity];
   memmove(newList, ledOrder, parpadeos  * sizeof(int));
   delete[] ledOrder;
   ledOrder = newList;
   ledOrderUser = ledOrder; 
}

void borrarLedsEnListas(){
  ledOrder = new int[3];
  ledOrderUser = new int[3];
}
