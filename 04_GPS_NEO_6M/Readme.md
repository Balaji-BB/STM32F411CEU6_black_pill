Interfacing NEO-6M GPS module with STM32.
-  Vcc - 5V for NEO-6M
-  TX & RX pins of NEO-6M are 3.3V tolerant pins
-  LED Blinks in NEO-6M GPS module for every 1 second if GPS position fix opted.
-  Parsed only RMC type sentence for getting required minimal GPS data
-  Transmit Date, Time, GPS data obtained from NEO-6M GPS module.
-  PA9   -> RX Pin of NEO-6M GPS
-  PA10  -> RX Pin of NEO-6M GPS
-  PA2   -> TX
-  PA3   -> RX 
