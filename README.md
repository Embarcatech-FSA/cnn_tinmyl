# Reconhecimento de Dígitos com TinyML no RP2040

Este projeto desenvolve um sistema embarcado para o reconhecimento de dígitos numéricos (0 a 9) utilizando uma Rede Neural Convolucional (CNN) treinada com a base de dados MNIST. O modelo foi convertido e quantizado para execução eficiente em microcontroladores utilizando o **TensorFlow Lite Micro (TFLM)**.

O objetivo principal é validar o uso de técnicas de **TinyML** em dispositivos de baixo custo e recursos limitados, como o Raspberry Pi Pico (microcontrolador RP2040).

**Equipe:** Camila de Araújo Bastos, Jackson Silva dos Santos, Lucas Carneiro de Araújo Lima, Luis Felipe Pereira de Carvalho, Mariana da Silva Lima Santos.

## 🚀 Funcionalidades

- **Inferência Local (Edge Computing):** Todo o processamento da rede neural ocorre diretamente no RP2040, sem dependência de nuvem.
- **Modelo Quantizado (INT8):** Utilização de quantização para reduzir o uso de memória RAM e Flash, mantendo alta precisão.
- **Interface Interativa:** Entrada de dados via Serial e feedback visual através de um display OLED.
- **Processamento de Imagens 28x28:** Compatível com o formato padrão do dataset MNIST.

## 🛠️ Hardware Necessário

- **Placa:** Raspberry Pi Pico (RP2040).
- **Display:** OLED SSD1306 (128x64 pixels) via I2C.
- **Botão:** Botão táctil para controle de fluxo.
- **Conectividade:** Cabo Micro-USB para alimentação e comunicação serial.

## 📌 Pinagem e Conexões

| Componente | Pino Pico (GPIO) | Função |
| :--- | :--- | :--- |
| **SSD1306 SDA** | GPIO 14 | I2C Data |
| **SSD1306 SCL** | GPIO 15 | I2C Clock |
| **Botão (A)** | GPIO 5 | Início do Sistema |


## 📂 Estrutura do Projeto

- `cnn_tinyml.c`: Arquivo principal contendo a lógica de controle, interface com display e loop de interação.
- `tflm_wrapper.cpp/h`: Wrapper em C++ para facilitar a integração do TensorFlow Lite Micro com código C.
- `mnist_cnn_int8_model.h`: O modelo da rede neural convertido em um array C quantizado.
- `CNN_MNIST.ipynb`: Notebook Jupyter contendo o treinamento, conversão e quantização do modelo.
- `libs/`: Bibliotecas de suporte para o display SSD1306 e o núcleo do TFLM.

## ⚙️ Como Compilar e Rodar

### Pré-requisitos
- [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado e configurado.
- Ferramenta `CMake` e compilador `arm-none-eabi-gcc`.

### Passo a Passo
1. Clone este repositório:
   ```bash
   git clone --recursive https://github.com/seu-usuario/cnn_tinyml.git
   cd cnn_tinyml
   ```
2. Inicialize os submódulos (se houver):
   ```bash
   git submodule update --init
   ```
3. Crie um diretório de build e compile:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
4. Transfira o arquivo `cnn_tinyml.uf2` gerado para o seu Raspberry Pi Pico (modo BOOTSEL).

## 🎮 Como Usar

1. Após ligar o sistema, o display exibirá "AGUARDANDO ACIONAMENTO".
2. Pressione o **Botão A** (GPIO 5) para inicializar o motor de inferência.
3. Abra um monitor serial (ex: Minicom, PuTTY ou o VS Code Serial Monitor) a 115200 bps.
4. Digite um número de **0 a 9** no terminal.
5. O sistema carregará uma imagem de teste correspondente ao dígito, executará a inferência e exibirá o resultado no display e na serial:
   - **Digitado:** O número enviado.
   - **Inferido:** A predição do modelo.
   - **Conf:** O nível de confiança da predição.
