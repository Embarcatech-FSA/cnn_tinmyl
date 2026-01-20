#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "tflm_wrapper.h"
#include "libs/include/all_samples.h"
#include "libs/include/ssd1306.h"

// Configurações do Botão
#define BUTTON_PIN 5

// Configurações do Display
ssd1306_t ssd;
#define I2C_PORT i2c1 
#define I2C_SDA 14 
#define I2C_SCL 15
#define ENDERECO 0x3C

// Função auxiliar para limpar e atualizar o display com mensagens simples
void display_message(const char *line1, const char *line2, const char *line3, const char *line4) {
    ssd1306_fill(&ssd, false);
    if (line1) ssd1306_draw_string(&ssd, line1, 0, 0);
    if (line2) ssd1306_draw_string(&ssd, line2, 0, 16);
    if (line3) ssd1306_draw_string(&ssd, line3, 0, 32);
    if (line4) ssd1306_draw_string(&ssd, line4, 0, 48);
    ssd1306_send_data(&ssd);
}

// Função auxiliar: retorna o índice do maior valor (predição)
static int argmax_i8(const int8_t* v, int n) {
    int best = 0;
    int8_t bestv = v[0];
    for (int i = 1; i < n; i++) {
        if (v[i] > bestv) { bestv = v[i]; best = i; }
    }
    return best;
}

// Função auxiliar: quantiza float para int8
static int8_t quantize_f32_to_i8(float x, float scale, int zp) {
    long q = lroundf(x / scale) + zp;
    if (q < -128) q = -128;
    if (q > 127) q = 127;
    return (int8_t)q;
}

int main() {
    stdio_init_all();
    
    // --- Configuração do Botão ---
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // --- Configuração I2C e Display ---
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);
    
    // Mensagem inicial no display
    display_message("AGUARDANDO", "ACIONAMENTO", "DO BOTAO", "PARA COMECAR");

    printf("\n[SISTEMA]: Pressione o botao A para iniciar...\n");

    // Loop de espera do botão (executado apenas uma vez)
    while (gpio_get(BUTTON_PIN)) {
        tight_loop_contents();
    }

    display_message("INICIALIZANDO", "SISTEMA...", NULL, NULL);
    printf("Botao pressionado! Inicializando TFLM...\n");

    if (tflm_init() != 0) {
        display_message("ERRO TFLM", "FALHA CRITICA", NULL, NULL);
        while (1) tight_loop_contents();
    }

    // Parâmetros do modelo
    int8_t* in = tflm_input_ptr(NULL);
    int8_t* out = tflm_output_ptr(NULL);
    float in_scale = tflm_input_scale();
    int in_zp = tflm_input_zero_point();
    float out_scale = tflm_output_scale();
    int out_zp = tflm_output_zero_point();

    display_message("DIGITE O NUMERO", "NA SERIAL", "PARA REALIZAR", "A INFERENCIA");
    printf("Aguardando comando... Digite um numero de 0 a 9:\n");
    while (1) {
        // Mensagem padrão de espera por entrada serial

        int c = getchar();

        if (c >= '0' && c <= '9') {
            int escolhido = c - '0';
            display_message("PROCESSANDO...", NULL, NULL, NULL);

            // 1. Carrega e quantiza a amostra
            const uint8_t* imagem = samples_db[escolhido];
            for (int i = 0; i < 28 * 28; i++) {
                float x = (float)imagem[i] / 255.0f;
                in[i] = quantize_f32_to_i8(x, in_scale, in_zp);
            }

            // 2. Inferência
            tflm_invoke();

            // 3. Resultados
            int pred = argmax_i8(out, 10);
            float y = (float)(out[pred] - out_zp) * out_scale; // Confiança do valor escolhido


            // Formatação de strings para o display
            char str_input[20], str_pred[20], str_status[20], str_conf[20];
            sprintf(str_input, "DIGITADO: %d", escolhido);
            sprintf(str_pred,  "INFERIDO: %d", pred);
            sprintf(str_status, (pred == escolhido) ? "ACERTOU" : "ERROU");
            sprintf(str_conf,  "CONF: %.2f", y);

            // Atualiza Display com resultados
            display_message(str_input, str_pred, str_status, str_conf);

            // Log Serial
            printf("Input: %d | Pred: %d | Status: %s | Conf: %f\n", 
                    escolhido, pred, (pred == escolhido) ? "OK" : "ERRO", y);


            sleep_ms(200);
        }
    }
}