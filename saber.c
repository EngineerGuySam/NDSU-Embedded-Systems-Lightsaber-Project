#include <pic18.h>

// Global Variables
int Time = 0;
int Na, Nb, Nc;

const int SINEWAVE_STEPS = 302;
int sinWaveLookup[302] = {0,77,-5622,-4,-7134,-819,784,-301,-1494,-18,-5720,-2020,2483,-1886,3721,4,-2289,-2203,3570,-3566,7840,106,1549,-597,2658,-3851,8973,268,4231,2423,-505,-1851,6402,378,4882,5406,-4692,2030,1002,285,3574,6578,-7753,6105,-5059,-95,1150,4845,-7780,8219,-9217,-681,-1242,505,-4240,6910,-9662,-1206,-2676,-4745,1596,2314,-6163,-1314,-2793,-8543,7177,-3747,-206,-754,-1836,-9004,9933,-8576,5668,434,-434,-5668,8576,-9933,9004,1836,754,206,3747,-7177,8543,2793,1314,6163,-2314,-1596,4745,2676,1206,9662,-6910,4240,-505,1242,681,9217,-8219,7780,-4845,-1150,95,5059,-6105,7753,-6578,-3574,-285,-1002,-2030,4692,-5406,-4882,-378,-6402,1851,505,-2423,-4231,-268,-8973,3851,-2658,597,-1549,-106,-7840,3566,-3570,2203,2289,-4,-3721,1886,-2483,2020,5720,18,1494,301,-784,819,7134,4,5622,-77,1,-77,5622,4,7134,819,-784,301,1494,18,5720,2020,-2483,1886,-3721,-4,2289,2203,-3570,3566,-7840,-106,-1549,597,-2658,3851,-8973,-268,-4231,-2423,505,1851,-6402,-378,-4882,-5406,4692,-2030,-1002,-285,-3574,-6578,7753,-6105,5059,95,-1150,-4845,7780,-8219,9217,681,1242,-505,4240,-6910,9662,1206,2676,4745,-1596,-2314,6163,1314,2793,8543,-7177,3747,206,754,1836,9004,-9933,8576,-5668,-434,434,5668,-8576,9933,-9004,-1836,-754,-206,-3747,7177,-8543,-2793,-1314,-6163,2314,1596,-4745,-2676,-1206,-9662,6910,-4240,505,-1242,-681,-9217,8219,-7780,4845,1150,-95,-5059,6105,-7753,6578,3574,285,1002,2030,-4692,5406,4882,378,6402,-1851,-505,2423,4231,268,8973,-3851,2658,-597,1549,106,7840,-3566,3570,-2203,-2289,4,3721,-1886,2483,-2020,-5720,-18,-1494,-301,784,-819,-7134,-4,-5622,77
 };

void setupPWM() {
    // Example setup for a PWM on a PIC18F
    PR2 = 0xFF; // Set the PWM period
    CCPR1L = 0x7F; // Set the initial duty cycle
    T2CON = 0x04; // Enable Timer2 (prescaler 1:1)
    CCP1CON = 0x0C; // PWM mode
   
    // Enable PWM output pin
    TRISC2 = 0; // Set RC2 as output
}

void setupTimer() {
    // Configure Timer0 for periodic interrupts
    T0CON = 0x07; // Timer0 on, 1:256 prescale
    TMR0L = 0x00; // Low byte of the timer count
    TMR0H = 0x00; // High byte of the timer count

    // Enable Timer0 interrupt
    INTCONbits.TMR0IE = 1;
    INTCONbits.GIE = 1; // Global interrupt enable
}

void interrupt HighISR(void) {
    static int step = 0;
    if(INTCONbits.TMR0IF) {
        // Update PWM duty cycle
        CCPR1L = sinWaveLookup[step] & 0xFF; // Ensure duty cycle is 8-bit
        step = (step + 1) % SINEWAVE_STEPS; // Loop through your sinewave steps
        // Reset Timer0 (Optional: Adjust values based on desired timing)
        TMR0L = 0x00;
        INTCONbits.TMR0IF = 0; // Clear Timer0 interrupt flag
    }
}

void main(void) {
    // IO Configuration
    TRISA = 0;
    TRISB = 0xFF;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0;
    ADCON1 = 0x0F; // Configure all PORTA & PORTB pins as digital

    // PWM Configuration
    setupPWM();

    // Timer Configuration
    setupTimer();
    
    while(1) {
        // Main loop, all the work is done in ISR
    }
}
