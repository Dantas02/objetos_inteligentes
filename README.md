﻿# Sistema de Irrigação Automático
Este projeto consiste em um sistema de irrigação automática de baixo custo. 

## Funcionamento
Utiliza um sensor de umidade do solo para realizar a leitura da umidade.
Caso a umidade esteja abaixo do ideal, o sistema aciona um relé, que por sua vez ativa uma válvula, liberando o fluxo de água para irrigar o solo. Além disso, o sistema se conecta a uma rede WiFi e envia o valor da umidade do solo para um broker MQTT.

## Aplicações
O sistema de irrigação pode ser utilizado para realizar a irrigação de plantas, jardins e hortas.

## Hardware
O hardware utilizado consiste nos seguintes itens:
- Uma placa NodeMCU ESP 12-E.
- Um sensor de umidade do solo Montimport
- Um relé Módulo Relé 5V 1 Canal 10A
- Um protoboard de 830 pontos
- Fonte De Alimentação 3.3v 5v Mb102 Protoboard
- Fios jumpers
- Uma válvula solenoide Marcoval VA03 12Vcc
