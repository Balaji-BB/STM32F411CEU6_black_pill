Configured UART peripheral for echo mode

- Baud rate : 115200
- Parity : None
- Interrupt : TX & RX
- Utilized IDLE Line detection concept for receiving the data of unknown size
- Transmit the received data in the same UART Peripheral (Echoes back the data)
- UART Pins : TX -> PA9, RX -> PA10
