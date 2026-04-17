S.E. MILEAGE 2026 - Módulo Central (Receptor & Web Server)

Este repositório contém o firmware para o Hub Central do projeto S.E. MILEAGE 2026. Esta unidade atua como um gateway: ela recebe dados via ESP-NOW de múltiplos transmissores (até 3 sensores) e disponibiliza essas informações em tempo real através de um Dashboard Web acessível via Wi-Fi.

🚀 Funcionalidades

Agregador de Dados: Centraliza as leituras de até 3 placas transmissoras simultaneamente.

Servidor Web Assíncrono: Interface web fluida utilizando a biblioteca ESPAsyncWebServer, permitindo múltiplos acessos sem travar o processamento.

Modo Híbrido (AP_STA): Atua como um Ponto de Acesso (Access Point), permitindo que celulares ou PCs se conectem diretamente à rede do receptor.

API JSON Interna: Fornece um endpoint /status que entrega os dados em formato JSON, facilitando integrações com outros sistemas.

Atualização Dinâmica: O Dashboard utiliza AJAX (fetch) para atualizar os valores na tela a cada 200ms sem necessidade de recarregar a página.

🛠️ Hardware Necessário

Microcontrolador: ESP32 (DevKit V1 ou similar).

Dispositivo de Visualização: Qualquer dispositivo com Wi-Fi e navegador (Smartphone, Tablet ou Notebook).

📋 Pré-requisitos
Para compilar este código, instale as seguintes bibliotecas:

ESPAsyncWebServer

AsyncTCP

ArduinoJson (Versão 6 ou superior)
