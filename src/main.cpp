#include <Arduino.h>
#include <AccelStepper.h>
#include <EEPROM.h>
#define MOTOR_A_ENABLE_PIN 8
#define MOTOR_A_STEP_PIN 2
#define MOTOR_A_DIR_PIN 5

#define MOTOR_B_ENABLE_PIN 1
#define MOTOR_B_STEP_PIN 3
#define MOTOR_B_DIR_PIN 6
const int recbutton = 11;
const int startformeeprom = 10;
AccelStepper stepper1(1, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN);
AccelStepper stepper2(1, MOTOR_B_STEP_PIN, MOTOR_B_DIR_PIN);
// Setando variaveis
int vm1 = 0;
int vm2 = 0;
long int stpm1 = 0;
long int stpm2 = 0;
int acm1 = 0;
int acm2 = 0;
int espera = 0;
int replay = 0;
bool eeprom = false;
bool executar = false;
bool rec = false;
bool start = false;
bool setrecebido = false;
bool moveisset = false;
bool setgravados = false;
int executeReplay = 0;
int endereMemory = 0;
bool flag = false;
String movimento = "";
String recInput = "";
String input = "";
String movnumber = "";

void setup()
{
  Serial.begin(9600);
  stepper1.setAcceleration(1000.0);
  
  stepper2.setAcceleration(1000.0);
  stepper1.setMinPulseWidth(20);
  stepper2.setMinPulseWidth(20);
  
  pinMode(recbutton, INPUT_PULLUP);
  pinMode(startformeeprom, INPUT_PULLUP);

  // put your setup code here, to run once:
}
// ------------------------------------------------------------Gravando na eeprom---------------------------

void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}
//-----------------------------------------------------------Fim da gravacao----------------------------------
// ------------------------------------------------------------Lendo da eeprom---------------------------
String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)
  return String(data);
}

//-----------------------------------------------------------Fim da leitura----------------------------------

// ------------------------------------------------------------Inicio do codigo---------------------------

void setmoves(String mov)
{

  while (mov.indexOf(";") != -1)
  {
    String aux = mov.substring(0, mov.indexOf(";"));
    mov = mov.substring(mov.indexOf(";") + 1);
    if (aux.indexOf("vm1") != -1)
    {
      vm1 = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("vm1=" + String(vm1));
    }
    if (aux.indexOf("vm2") != -1)
    {
      vm2 = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("vm2=" + String(vm2));
    }
    if (aux.indexOf("stpm1") != -1)
    {
      stpm1 = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("stpm1=" + String(stpm1));
    }
    if (aux.indexOf("stpm2") != -1)
    {
      stpm2 = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("stpm2=" + String(stpm2));
    }
    if (aux.indexOf("acm1") != -1)
    {
      acm1 = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("acm1=" + String(acm1));
    }
    if (aux.indexOf("acm2") != -1)
    {
      acm2 = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("acm2=" + String(acm2));
    }
    if (aux.indexOf("delay") != -1)
    {
      espera = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("delay=" + String(espera));
    }
    if (aux.indexOf("replay") != -1)
    {
      if (flag == false)
      {
        replay = aux.substring(aux.indexOf("=") + 1).toInt();
        //Serial.println("replay=" + String(replay));
        flag = true;
      }
    }
    if (aux.indexOf("eeprom") != -1)
    {
      eeprom = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("eeprom=" + String(eeprom));
    }
    if (aux.indexOf("executar") != -1)
    {
      executar = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("executar=" + String(executar));
    }
    if (aux.indexOf("rec") != -1)
    {
      rec = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("rec=" + String(rec));
    }
    if (aux.indexOf("start") != -1)
    {
      start = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("start=" + String(start));
    }
    if (aux.indexOf("movnumber") != -1)
    {
      movnumber = aux.substring(aux.indexOf("=") + 1).toInt();
      //Serial.println("movnumber=" + String(movnumber));
    }
  }
  moveisset = true;
}

void executarMovimento(String movimento)
{
  stepper1.setSpeed(vm1);
  stepper2.setSpeed(vm2);
  stepper1.setMaxSpeed(vm1);
  stepper2.setMaxSpeed(vm2);
  stepper1.setAcceleration(acm1);
  stepper2.setAcceleration(acm2);
  stepper1.moveTo(stpm1);
  stepper2.moveTo(stpm2);
  while ((stepper1.distanceToGo() != 0) || (stepper2.distanceToGo() != 0))
  {
    stepper1.run();
    stepper2.run();
  }
  delay(espera);
  Serial.println("fim");
  setrecebido = false;
};
// ------------------------------------------------------------Set replay  function---------------------------
void repetir(int repetir)
{
  for (int i = 0; i < repetir; i++)
  {
    setmoves(movimento);
    executarMovimento(movimento);
  }
  Serial.println("fim");
}
// ------------------------------------------------------------Lendo arquivos gravados---------------------------
void lergravados()
{
  movimento = readStringFromEEPROM(endereMemory);
  Serial.println("Movimento lido da memoria:  " + String(movimento));
  Serial.println("endereco da memoria:  " + String(endereMemory));
  setmoves(movimento);
  while (replay != 0)
  {
    setmoves(movimento);
    executarMovimento(movimento);
    endereMemory = endereMemory + movimento.length() + 1;
    movimento = readStringFromEEPROM(endereMemory);
    Serial.println("Movimento lido da Memoria:  " + String(movimento));
    Serial.println("endereco da memoria:  " + String(endereMemory));

    if (movimento.length() == 0)
    {
      Serial.println("Sem movimentos gravados");
    }
    if (movimento.startsWith("fim"))
    {
      Serial.println("Fim dos movimentos gravados");
      endereMemory = -4;
      replay--;
      Serial.println("replay=" + String(replay));
    }
  }
  String retorno = "vm1=1800;vm2=1200;stpm1=0;stpm2=0;acm1=5000;acm2=5000;espera=0;";
  Serial.println("Executando retorno");
  setmoves(retorno);
  executarMovimento(retorno);
  setgravados = false;
}
void loop()
{
  // gravacao dos movimentos na memoria
  if ((digitalRead(recbutton) == LOW) && (Serial.available() > 0))
  {
    Serial.println("Preparando para gravar");
    String movimento = Serial.readStringUntil('\n');
    Serial.println("Gravando");
    writeStringToEEPROM(endereMemory, movimento);
    endereMemory = endereMemory + movimento.length() + 1;
    Serial.println("movimento: " + movimento);
    Serial.println("endereco: " + String(endereMemory));
    Serial.println(movimento);
    Serial.println("aguardando");
    if (movimento.startsWith("fim"))
    {
      Serial.println("Todos os movimentos foram gravados.");
      Serial.println("Fim da gravacao.");
      endereMemory = 0;
    }
  }

  if (setgravados)
  {
    lergravados();
    // setgravados = false;
  }
  if ((digitalRead(recbutton) == HIGH) && (Serial.available() > 0))
  {
    movimento = Serial.readStringUntil('\n');
    setmoves(movimento);
    executarMovimento(movimento);
  }
  if (digitalRead(startformeeprom) == LOW)
  {
    setgravados = !setgravados;
    Serial.println("Movimento iniciado: " + String(setgravados));
    flag = false;
    delay(1000);
    endereMemory = 0;
  }
  
}

/*
vm1=1800;vm2=1200;stpm1=100;stpm2=100;acm1=100;acm2=100;espera=10;replay=2;
vm1=1800;vm2=1200;stpm1=0;stpm2=0;acm1=50;acm2=100;espera=1000;
vm1=13000;vm2=5000;stpm1=500000;stpm2=500000;acm1=50000;acm2=1000000; delay=2010;executar=1;
vm1=10;vm2=10;stpm1=50;stpm2=20;acm1=5000;acm2=1000; delay=20;executar=1;
fim
vm1=1800;vm2=1200;stpm1=10;stpm2=10;acm1=500;acm2=100;espera=10;replay=2;executar=1;
vm1=130;vm2=1300;stpm1=5000;stpm2=5000;acm1=500;acm2=10000; delay=2010;executar=1;
vm1=140;vm2=1400;stpm1=500;stpm2=200;acm1=5000;acm2=1000; delay=20;executar=1;
vm1=1800;vm2=1200;stpm1=10;stpm2=10;acm1=500;acm2=100;espera=10;replay=2;executar=1;
vm1=130;vm2=1300;stpm1=500;stpm2=500;acm1=500;acm2=10000; delay=2010;executar=1;
vm1=140;vm2=1400;stpm1=500;stpm2=200;acm1=5000;acm2=1000; delay=20;executar=1;
vm1=1800;vm2=1200;stpm1=10;stpm2=10;acm1=500;acm2=100;espera=10;replay=2;executar=1;
vm1=130;vm2=1300;stpm1=5000;stpm2=5000;acm1=500;acm2=10000; delay=2010;executar=1;
vm1=140;vm2=1400;stpm1=500;stpm2=200;acm1=5000;acm2=1000; delay=20;executar=1;

vm1=150;vm2=1500;stpm1=0;stpm2=30;acm1=5000;acm2=100; delay=30;
vm1=150;vm2=1500;stpm1=500;stpm2=0;acm1=5000;acm2=100;delay=40;


vm1=6000;vm2=9000;stpm1=200;stpm2=300;acm1=100;acm2=5400;delay=0;
vm1=6000;vm2=8000;stpm1=00;stpm2=0;acm1=3000;acm2=4000;delay=0;
fim;
*/