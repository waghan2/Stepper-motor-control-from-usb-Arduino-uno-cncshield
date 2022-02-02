# How to Control steper Motor over usb

# Includes: include <AccelStepper.h>
# Components: 
- Arduino uno
- cnc shield for arduino uno (cnc shield v3.0)
- DRV8825 or A4988 
- 
### Very simple program that gets the value form a String and execute or save it on EEprom
- This program was writen on visual studio code using platformIO
## Recived string 
 - vm1=200;vm2=200;stpm1=5000;stpm2=5500;acm1=50;acm2=10000;delay=100;

So it means that:
- vm1 = Speed  motor one
- vm2 = Speed  motor two
- stpm1 = Steps motor one
- stpm2 = Steps motor two
- acm1 = Acceleration motor one
- acm2 = Acceleration motor two
- delay = The delay between each movement
- fim = indicate the end of a move
