Trying out the software switch debouncing concept for reading an push buttons
- Configure pins as external interrupt
- Configure pins mode either PULL UP/PULL DOWN based on the hardware circuit
- Configure the interrupt mode - either RISING/FALLING/BOTH
- Configure the timer in overflow interrupt mode to introduce DEBOUNCE DELAY for calculation
- In case of interrupt trigger -> Read the pin, start the timer and read the same pin after DEBOUNCE DELAY.
- In my case, I had configured 5 pins to act like reading navigation buttons
- DOWN -> PA0, RIGHT -> PA1, CENTER -> PA2, LEFT -> PA3, UP -> PA4
