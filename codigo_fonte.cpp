#include <NewPing.h>  // Inclui a biblioteca NewPing para lidar com o sensor de ultrassom
#include <LiquidCrystal_I2C.h>  // Inclui a biblioteca LiquidCrystal_I2C para lidar com o display LCD

#define TRIGGER_PIN 7       // Define o pino de trigger (disparo) para o sensor de ultrassom como pino digital 7
#define ECHO_PIN 8          // Define o pino de eco para o sensor de ultrassom como pino digital 8
#define MAX_DISTANCE 430   // Define a distância máxima de medição do sensor de ultrassom como 430 centímetros

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);  // Cria um objeto NewPing chamado "sonar" com os pinos definidos acima

// Inicialização do display LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Inicializa o display LCD com endereço I2C 0x27, 16 colunas e 2 linhas

// Definição da matriz de bytes para a logo da Mahindra
byte logoMahindra[6][8] = {
  {B00000, B10000, B11000, B10100, B10010, B10001, B10000, B10000},  // Linha 1 da logo
  {B00000, B00000, B00000, B00000, B00000, B00000, B10001, B01010},  // Linha 2 da logo
  {B00000, B00001, B00011, B00101, B01001, B10001, B00001, B00001},  // Linha 3 da logo
  {B10000, B10000, B10000, B10000, B10000, B10000, B01111, B00000},  // Linha 4 da logo
  {B01010, B01010, B01010, B01010, B01010, B10001, B00000, B00000},  // Linha 5 da logo
  {B00001, B00001, B00001, B00001, B00001, B00001, B11110, B00000}   // Linha 6 da logo
};

float mediaD;  // Variável para armazenar a média das distâncias medidas pelo sensor

void setup() {
  Serial.begin(9600);  // Inicializa a comunicação serial com uma taxa de transmissão de 9600 bps
  lcd.init();                      // Inicializa o display LCD
  lcd.backlight();                 // Ativa a luz de fundo do display
  
  // Cria os caracteres personalizados da logo da Mahindra no display LCD
  for (int i = 0; i < 6; i++) {
    lcd.createChar(i, logoMahindra[i]);
  }
  
  mostrarInicio();  // Chama a função para exibir a inicialização do display
}

void loop() {
  LerVariaveis();  // Chama a função para ler as variáveis do sensor de ultrassom
  mostrar();       // Chama a função para mostrar as informações no display LCD
  delay(1000);     // Atraso de 1 segundo entre cada iteração do loop
}

void LerVariaveis(){
  float distancia = 0; // Soma de 5 leituras da distância
  float S[5]; // Vetor para armazenar as 5 leituras

  // Realiza 5 leituras da distância pelo sensor de ultrassom e calcula a média
  for(int i =0; i<5;i++){
    S[i] = sonar.ping_cm();
    distancia += S[i];
  }
  mediaD = distancia/5;  // Calcula a média das distâncias medidas
  mediaD /= 100;         // Converte a média para metros
}

void mostrar(){
  lcd.clear();  // Limpa o display LCD
  
  // Verifica se a distância é menor ou igual a 1 metro e mostra a mensagem correspondente no display
  if (mediaD<=1){
    lcd.setCursor(0,0);
    lcd.print(mediaD);
    lcd.setCursor(4,0);
    lcd.print("m");
    lcd.setCursor(0,1);
    lcd.print("MUITO PERTO!");
  }
  else {  // Se a distância for maior que 1 metro, mostra a mensagem correspondente no display
    lcd.setCursor(0,0);
    lcd.print(mediaD);
    lcd.setCursor(4,0);
    lcd.print("m");
    lcd.setCursor(0,1);
    lcd.print("BOA DISTANCIA!");
  }
}

void mostrarInicio() {
  lcd.clear(); // Limpa o display antes de atualizar
  
  // Exibe "Mahindra" na primeira linha, uma letra por vez
  for (int i = 0; i < 8; i++) {
    lcd.setCursor(i, 0);
    lcd.print("Mahindra"[i]);
    delay(100); // Atraso de 100 ms entre cada letra
  }
  
  // Exibe "Race" na segunda linha, uma letra por vez
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(8 + i, 1); // Ajusta a posição para a palavra "Race"
    lcd.print("Race"[i]);
    delay(100); // Atraso de 100 ms entre cada letra
  }

  // Exibe a logo da Mahindra nas posições adequadas no display
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      lcd.setCursor(13 + j, i); // Ajusta a posição para a exibição do logo
      lcd.write(logoMahindra[i][j]);
      delay(100); // Atraso de 100 ms entre cada coluna do logo
    }
  }

  delay(500); // Atraso de 500 ms antes de finalizar a função
}
