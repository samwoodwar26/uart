# Program: UART LED Controller  
**Author:** Intel/Zephyr

**Date:** 01/24/2025  

**Edited by:** Samuel Woodward, James Lee

**Date:** 01/24/2025  

**Purpose:**  

This program demonstrates UART communication with a microcontroller using the Zephyr RTOS framework. It allows the user to toggle LEDs by pressing specific keys ('1', '2', and '3') on their keyboard, which are received via UART. It supports boards with either two or three LEDs configured as GPIO outputs.  

The program uses a callback function to handle UART events, enabling asynchronous behavior.  

---

## Configuration  

### Devicetree Overlay  

No overlay required

### Kernel Configuration via prj.conf

CONFIG_LOG=y \
CONFIG_UART_INTERRUPT_DRIVEN=y \
CONFIG_GPIO=y \
CONFIG_SERIAL=y

## Configuration
### External
Plugged into PC

### Internal
Leds 1, 2, and 3

```mermaid
graph TD
    A[Start] --> B{Is UART Ready?}
    B -- Yes --> C[Configure GPIO LEDs]
    B -- No --> D[Log Error and Exit]
    C --> E[Initialize UART Callback]
    E --> F[Enable UART RX]
    F --> G[Transmit Welcome Message]
    G --> H[System Running]

    subgraph UART Callback Thread
        H1[Wait for UART RX Event] --> H2[Process Received Data]
        H2 -- Key '1' --> H3[Toggle LED0]
        H2 -- Key '2' --> H4[Toggle LED1]
        H2 -- Key '3' --> H5[Toggle LED2]
        H3 --> H1
        H4 --> H1
        H5 --> H1
    end
