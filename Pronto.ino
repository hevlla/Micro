#include <Servo.h>
#include <LiquidCrystal.h>

//Define as os pinos do sensor de cor
#define S0 3
#define S1 4
#define S2 5
#define S3 6
#define sensorOut 7

Servo topServo;
Servo bottomServo;
LiquidCrystal lcd(53, 51, 46, 48, 50, 52);

int frequencia = 0;                 //Frequencia da intensidade da luz
int cor = 0;
int botao = 2;                      // botão no pino 2
int funcaoa = 0;                    // valor instantaneo enviado pelo botão
int funcaob = 0;                    // valor guardado
int estado = 0;                     // guarda o valor 0 ou 1 (HIGH ou LOW)

//Contadores para o display
int contAmarelo = 0;
int contVerde = 0;
int contVermelho = 0;
int contAzul = 0;
int conTotal = 0;
int cont;

int ldrPin = 0;                     //LDR no pino analígico A0
int ldrValor = 0;                   //Valor lido do LDR

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  pinMode(botao,INPUT);               //Define o botão como entrada
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  lcd.begin(16, 2);                   //2 linhas por 16 colunas
  topServo.attach(8);
  bottomServo.attach(10);
  Serial.begin(9600);

  //Configurando o que irá ser impresso no lcd
  lcd.setCursor(16, 0);
  lcd.print("Projeto");
  lcd.setCursor(16, 1);
  lcd.print("Microcontrolador"); 
   for (int posicao = 16; posicao > 0 ; posicao--)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }
  lcd.clear();
}

void loop() {                       
    funcaoa = digitalRead(botao);       // ler o valor enviado pelo botão: "HIGH" ou "LOW"

    //Condicao do botao de interrupcao
    if ((funcaoa == HIGH) && (funcaob == LOW)) {
        estado = 1 - estado;
        delay(400);                     
    }
    funcaob = funcaoa;
    
    if (estado == 1) {
      ldrValor = analogRead(ldrPin); //O valor lido será entre 0 e 1023
      Serial.print("Luminosidade: ");
      Serial.println(ldrValor); //imprime o valor lido do LDR no monitor serial  
      topServo.write(0);                  //angulo do inicial do motor
      delay(800);
    
    for(int i = 0; i < 40; i++) {         //angulo para verificar a cor
      topServo.write(i);
      delay(10);
    }
    delay(500);
    
    cor = LerCor();                       //Chamando a função 
    
    //Total ira ser impresso no lcd, assim como seu contador
    lcd.setCursor(14, 0);
    lcd.print("TT");
    lcd.setCursor(14, 1);
    conTotal = cont;
    lcd.print(conTotal);
    delay(500);
    
    delay(10);  
    bottomServo.write(30);                //Definindo uma posição inicial pro motor
    switch (cor) {
      case 1:
      bottomServo.write(0);
      break;
      
      case 2:
      bottomServo.write(20);
      break;
      
      case 3:
      bottomServo.write(40);
      break;
      
      case 4:
      bottomServo.write(60);
      break;
      
      case 0:
      break;
    }
    delay(500);
    
    for(int i = 40; i < 67; i++) {          //angulo para jogar a bolinha
      topServo.write(i);
      delay(10);
    } 
    delay(500);
    
    for(int i = 67; i > 0; i--) {           //angulo para voltar a posicao inicial  
      topServo.write(i);
      delay(10);
    }
    cor = 0;
    }
  else{
    topServo.write(0);
    bottomServo.write(0);
  }
}

// Função que ler a cores
int LerCor() {
  
  // Valores para leitura do vermelho (red)
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  
  // Lendo a frequencia da saida referente ao vermelho(red)
  frequencia = pulseIn(sensorOut, LOW);
  int R = frequencia;
  
  //Printando o valor no serial monitor (red)
  Serial.print("R= ");
  Serial.print(frequencia);
  Serial.print("  ");
  delay(50);
  
  // Valores para leitura do verde (green)
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  
  // Lendo a frequencia da saida (green)
  frequencia = pulseIn(sensorOut, LOW);
  int G = frequencia;
  
  // Printando o valor no serial monitor (green)
  Serial.print("G= ");
  Serial.print(frequencia);
  Serial.print("  ");
  delay(50);
  
  // Valores para leitura do azul (blue)
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  
  // Lendo a frequencia da saida (blue)
  frequencia = pulseIn(sensorOut, LOW);
  int B = frequencia;
  
  // Printando o valor no serial monitor (blue)
  Serial.print("B= ");
  Serial.print(frequencia);
  Serial.println("  ");
  delay(50);
 
  if(R <= 130 & R >= 50 && G <= 260 & G >= 170){       //Valores da porta serial para (red)
    cor = 1; // Red
    Serial.println("vermelho");
    contVermelho++;
    cont++;
  }
  else if(R <= 231 & R >= 160 && G <= 180 & G >= 120){    //Valores da porta serial para (green)
    cor = 2; // Green
    Serial.println("verde");
    contVerde++;
    cont++;
  }
  else if(R <= 90 & R >= 40 && G <= 115 & G >= 50){    //Valores da porta serial para (yellow)
    cor = 3; // Yellow
    Serial.println("amarelo");
    contAmarelo++;
    cont++;
  }
  else if (G <= 230  & G >= 160  && B <= 149 & B >= 90){   //Valores da porta serial para (blue)
    cor = 4; // Blue
    Serial.println("azul");
    contAzul++;
    cont++;
  }
    lcd.setCursor(7, 0);
    lcd.print("B");
    lcd.setCursor(7, 1);
    lcd.print(contAzul);

    lcd.setCursor(11, 0);
    lcd.print("Y");
    lcd.setCursor(11, 1);
    lcd.print(contAmarelo);

    lcd.setCursor(3, 0);
    lcd.print("G");
    lcd.setCursor(3, 1);
    lcd.print(contVerde);

    lcd.setCursor(0, 0);
    lcd.print("R");
    lcd.setCursor(0, 1);
    lcd.print(contVermelho);
   
  return cor;  
}

