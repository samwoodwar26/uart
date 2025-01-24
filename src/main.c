#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>

#define SLEEP_TIME_MS 1000

#define RECEIVE_BUFF_SIZE 10

#define RECEIVE_TIMEOUT 100

#if defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP) ||                                              \
	defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
#else
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
#endif

const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

static uint8_t tx_buf[] = {
	"nRF Connect SDK Fundamentals Course\r\n"
	"Press 1-3 on your keyboard to toggle LEDS 1-3 on your development kit\r\n"};


static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};


static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type) {

	case UART_RX_RDY:
#if defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP) ||                                              \
	defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
		if ((evt->data.rx.len) == 1) {

			if (evt->data.rx.buf[evt->data.rx.offset] == '1') {
				gpio_pin_toggle_dt(&led0);
			} else if (evt->data.rx.buf[evt->data.rx.offset] == '2') {
				gpio_pin_toggle_dt(&led1);
			}
		}
#else
		if ((evt->data.rx.len) == 1) {

			if (evt->data.rx.buf[evt->data.rx.offset] == '1') {
				gpio_pin_toggle_dt(&led0);
			} else if (evt->data.rx.buf[evt->data.rx.offset] == '2') {
				gpio_pin_toggle_dt(&led1);
			} else if (evt->data.rx.buf[evt->data.rx.offset] == '3') {
				gpio_pin_toggle_dt(&led2);
			}
		}
#endif
		break;
	case UART_RX_DISABLED:
		uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
		break;

	default:
		break;
	}
}

int main(void)
{
	int ret;

	if (!device_is_ready(uart)) {
		printk("UART device not ready\n");
		return 1;
	}

	if (!device_is_ready(led0.port)) {
		printk("GPIO device is not ready\n");
		return 1;
	}

#if defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP) ||                                              \
	defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
#else
	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
#endif


	ret = uart_callback_set(uart, uart_cb, NULL);
	if (ret) {
		return 1;
	}

	ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
	if (ret) {
		return 1;
	}

	ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
	if (ret) {
		return 1;
	}
	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
}
