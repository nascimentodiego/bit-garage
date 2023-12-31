/*            
  Project: Bit-Garage Door
  Author: Diego Figueredo do Nascimento
  Date: 23/12/2023

  Description:
  
> Pre-Setup
    - Carrega o evento salvo no storage para determinar qual será o modo 
    de execução: READY_MODE / SETUP_MODE / RECORD_MODE

> Idle event:
    - Sempre fica escutando o sinal do PULL_UP para habilitar o modo de configuração ou voltar para READY_MODE, 
    ao ser contabilizado 3 eventos de pull_up.
      1. Se o mode atual for READY
        1.1 Deve registrar o funcionamento como SETUP_MODE
        1.2 Deve reiniciar o disposítivo.
      2. Se o mode atual for SETUP ou RECORD
        2.1 Deve registrar o funcionamento como READY
        2.2 Deve reiniciar o dispositivo.  

> Operation Mode: 

  1. Ready mode:
    1.1 Carrega as credenciais do WiFi e do firebase
    1.2 Se connecta a Wifi
    1.3 Se registra no firebase para escutar o nó da chave do dispositivo.
    1.4 Se receber um valor diferente do que tem salvo para o evento A ou B. 
      1.4.1 Envia o sinal de RF 433 mhz anteriomente registrado no device.
  2. Setup Mode:
    2.1 Fica piscando o led piscando Lentamente(1s).
    2.2 Habilitar o WiFi como AP e fica aguardando o envio das requests de setup.
    2.3 Ao receber a request bitconfig/wifi, 
      2.3.1 Deve salvar o SSID e PASSWORD enviados na request na memória fisica.
      2.3.1 Enviar o UUID do device como sucesso da request para que o Mobile App persista o device.
    2.4 Ao receber a request bitconfig/record 
      2.4.1 Deve olhar o response para pegar o slot que deve ser salvo.
      2.4.2 Seta o record para o Slot recebido no response.
      2.4.3 Habilta o modo para RECORD_MODE.
    2.5 Ao receber a request bitconfig/finish
      2.5.1 Habilta o modo para READY_MODE.
      2.5.2 Reset o device.
     
  3. Record Mode:
    3.1 Fica piscando o led com maior frequencia
    3.2 Ao receber um sinal do RF deve gravar na memoria fisica na posição do evento anteriomente recebido no modo de setup.
    3.3 Após a gravação do sinal RF, deve voltar para o SETUPMODE.
    3.3 Fica aguardando por 5 segundos um sinal RF, caso não haja, deve voltar para SETUP_MODE.

*/

#include <Arduino.h>
#include <Scheduler.h>
#include <Task.h>
#include "./button/ButtonControlTask.h"
#include "./led/BlinkTask.h"
#include "./network/webservice/WebServiceTask.h"
#include "./network/firebase/RTDBTask.h"
#include "./rf/RfReceiverTask.h"
#include "./storage/Preferences.h"
#include "./rf/RFReceiveControl.h"
#include "GlobalVars.h"

void biTlog();
void preSetup();

void setup() {
  preSetup();
  biTlog();  
  delay(1000);

  Scheduler.start(&blinkTask);
  Scheduler.start(&buttonTask);
  Scheduler.start(&webServiceTask);
  Scheduler.start(&realTimeDataBaseTask);
  Scheduler.start(&rfReceiverTask);
  
  Scheduler.begin();
}

void loop(){}

void preSetup(){
  Serial.begin(115200);
  biT.setMode(storage.getOperationMode());
}

void biTlog(){
  if(!debug)
    return;

  Serial.println("");
 
  if(biT.isReadyMode()){
    Serial.println("SSID: ");
    Serial.println(storage.getSSID());
    Serial.println("WiFi-PASS: ");
    Serial.println(storage.getWifiPass());
  }
 
  Serial.println("DOOR_CODE_A: ");
  Serial.println(storage.getDoorARFCode());

  Serial.println("DOOR_CODE_B: ");
  Serial.println(storage.getDoorBRFCode());

  if(biT.isReadyMode()){
    Serial.println("Em modo de execução !");
  } else if(biT.isSetupMode()) {
    Serial.println("Em modo de configuração !"); 
  } else{
     Serial.println("Em modo de gravação !"); 
  }
}