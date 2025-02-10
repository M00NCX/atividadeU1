#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define BUTTON_PIN_A 5   // Pino do botão A
#define BUTTON_PIN_B 6   // Pino do botão B
#define LED_PIN_GREEN 11 // Pino do LED verde
#define LED_PIN_BLUE 12  // Pino do LED azul

#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void display_symbol_on_oled(uint8_t number)
{
  // Exemplo: exibir um número no display OLED
  ssd1306_t ssd;
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
  ssd1306_config(&ssd);
  ssd1306_fill(&ssd, false);

  // Exibe o número recebido no display
  char display_str[2] = {number + '0', '\0'};
  ssd1306_draw_string(&ssd, display_str, 8, 10);
  ssd1306_send_data(&ssd); // Atualiza o display
}

int main()
{
  // Inicializa o I2C
  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA);
  gpio_pull_up(I2C_SCL);

  // Inicializa o display OLED
  ssd1306_t ssd;
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
  ssd1306_config(&ssd);
  ssd1306_send_data(&ssd);

  // Limpa o display
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  // Configura os pinos dos botões e LEDs
  gpio_init(BUTTON_PIN_A);
  gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
  gpio_pull_up(BUTTON_PIN_A); // Configura o botão A com pull-up

  gpio_init(BUTTON_PIN_B);
  gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
  gpio_pull_up(BUTTON_PIN_B); // Configura o botão B com pull-up

  gpio_init(LED_PIN_GREEN);
  gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);

  gpio_init(LED_PIN_BLUE);
  gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);

  uart_init(UART_ID, BAUD_RATE);
  gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

  bool cor = true;
  while (true)
  {
    if (uart_is_readable(UART_ID))
    {
      char received_char = uart_getc(UART_ID);

      // Limpa o display antes de mostrar uma nova mensagem
      ssd1306_fill(&ssd, false);

      char display_str[2] = {received_char, '\0'};
      ssd1306_draw_string(&ssd, display_str, 8, 10);
      ssd1306_send_data(&ssd);
    }

    if (gpio_get(BUTTON_PIN_A) == 0)
    {
      gpio_put(LED_PIN_GREEN, true);
      gpio_put(LED_PIN_BLUE, false);

      // Exibe "Verde on" no display
      ssd1306_draw_string(&ssd, "Verde on", 8, 10);
      ssd1306_send_data(&ssd);
      sleep_ms(200);
    }
    else if (gpio_get(BUTTON_PIN_B) == 0)
    {
      gpio_put(LED_PIN_BLUE, true);
      gpio_put(LED_PIN_GREEN, false);

      // Exibe "Azul on" no display
      ssd1306_draw_string(&ssd, "Azul on", 8, 10);
      ssd1306_send_data(&ssd);
      sleep_ms(200);
    }

    sleep_ms(100);
  }
}