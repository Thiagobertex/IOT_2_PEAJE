#include <Servo.h> //librería para controlar servos
#define SERVO 3
#define SENSOR 4
#define BUTTON 2
#define PERIODO 5000  
#define CMAX_AUTOS 30
#define BARRERA_ALTA 0
#define BARRERA_BAJA 90

unsigned long TiempoAhora=0; 
int Cant = 0;

Servo MiServo;
int angulo=0;


void abrir_barrera(void);
void cerrar_barrera(void);

void setup(){
  
  pinMode(BUTTON,INPUT); //inicializacion del puerto BUTTON como entrada.
  pinMode(SENSOR,INPUT); //inicializacion del puerto SENSOR como entrada.
  MiServo.attach(SERVO); //inicialización del servo que controlará la barrera.
  Serial.begin(9600); //inicializacion del puerto serie.
   
}

void loop(){
  
  if(Cant<=CMAX_AUTOS){ //como la cantidad maxima de autos por fila en una cabina de peaje es 30, siempre que cant sea menor o igual a 30 se mantendra dentro del if.
    if(digitalRead(SENSOR)==HIGH){ //leo al sensor de movimiento digitalmente.
      abrir_barrera(); //abro la barrera para que pase el auto ingresante.
      while(digitalRead(SENSOR)==HIGH); // avanzará cuando el sensor deje de enviar un 1.
      cerrar_barrera(); //se cerrará la barrera cuando el sensor cambie de estado de 1 a 0.
      TiempoAhora=millis(); //guardo este preciso momento en dicha variable.
      Cant++; //sumo 1 a la variable cant, que si llega a 31 del tiempo definido, saldrá del if entrando en else.
    }
  }else{
    if(MiServo.read()==BARRERA_BAJA)abrir_barrera(); //si la barrera se encuentra baja se abrirá.
    else{
        while(digitalRead(BUTTON)==HIGH); //hasta que aprete el botón manualmente la barrera seguirá alta.
        while(digitalRead(BUTTON)==LOW);
        cerrar_barrera(); //cierro la barrera luego de presionar el boton.
        Cant=0; //reinicio la variable para cuando el programa vuelva al inicio.
    }
  }
  if(Cant!=0){
    if(millis()>TiempoAhora+PERIODO){ //si la cantidad de autos es distinta de 0 y el tiempo en este instante es mayor al tiempo registrado anteriormente mas 5 segundos se produce lo siguiente:
      Cant=0; //se reiniciará la variable Cant para poder iniciar nuevamente el conteo.
      TiempoAhora=millis(); //se registrará un nuevo momento.
    } //esto sirve para que si pasan 30 autos en 5 horas (por ejemplo) no se abran las barreras innecesariamente.
  }
  Serial.println(Cant);
}

void abrir_barrera(void){
  MiServo.write(BARRERA_ALTA); //creo la función abrir_barrera
}

void cerrar_barrera(void){
  MiServo.write(BARRERA_BAJA); //creo la funcion cerrar_barrera
}
