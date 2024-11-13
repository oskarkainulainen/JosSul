/*
 * Pixel.c
 *
 *  Created on: -----
 *      Author: -----
 */

#include "Pixel.h"

#define CONTROL_SIGNAL   *(uint32_t*) 0x41220008
#define CHANNEL_SIGNAL  *(uint32_t*) 0x41220000



//Table for pixel dots.
//				 dots[X][Y][COLOR]
volatile uint8_t dots[8][8][3]={0};


// Here the setup operations for the LED matrix will be performed
// Setup function for initializing the LED matrix
void setup() {
    // 1. Initialize control and channel signals to 0
    CONTROL_SIGNAL = 0;
    CHANNEL_SIGNAL = 0;

    // Resetting screen at start (asynchronous reset by toggling RST pin)
    CONTROL_SIGNAL |= (1 << 0); // Set bit 0 to 1
    usleep(500); // Wait 500 µs
    CONTROL_SIGNAL &= ~(1 << 0); // Clear bit 0
    usleep(500); // Wait 500 µs
    CONTROL_SIGNAL |= (1 << 0); // Set bit 0 to 1 again

    // Set SDA bit to 1
    CHANNEL_SIGNAL |= (1 << 4);

    // Define gamma correction vector with {63, 63, 63} for RGB channels
    uint8_t gamma_vector[3] = {63, 63, 63};

    // Write gamma correction values to DM163 register
    for (int i = 0; i < 8; i++) {           // Loop over 8 RGB LEDs
        for (int color = 0; color < 3; color++) {  // Loop over RGB colors
            uint8_t t = gamma_vector[color];
            for (int bit = 5; bit >= 0; bit--) {   // Loop over 6 bits
                if (t & (1 << bit)) {
                    CHANNEL_SIGNAL = 1; // Write 1 to the register
                }else {
                    CONTROL_SIGNAL &=~0x10; //SET only BIT4 to 0 in control signal (SDA bit)
                    CONTROL_SIGNAL &=~0x08; //SET only BIT3 to 0 in control signal (CLK bit)
                    t <<= 1; //shift one to left
                    CONTROL_SIGNAL|=0x08; //SET only BIT3 to 1 in control signal (CLK bit)

                }

                // Toggle clock to write each bit
                CONTROL_SIGNAL |= (1 << 1); // CLK = 1
                CONTROL_SIGNAL &= ~(1 << 1); // CLK = 0
            }
        }
    }

    // Final step: set SB bit to 1 to enable transmission to 8-bit register
    CONTROL_SIGNAL |= (1 << 2); // Set bit 2 (SB) to 1
}

//Change value of one pixel at led matrix. This function is only used for changing values of dots array
void SetPixel(uint8_t x,uint8_t y, uint8_t r, uint8_t g, uint8_t b){

	//Hint: you can invert Y-axis quite easily with 7-y
	dots[x][y][0]=b;
  dots[x][y][1] = g;
	dots[x][y][2] = r;

}


//Put new data to led matrix. Hint: This function is supposed to send 24-bytes and parameter x is for channel x-coordinate.
void run(uint8_t x){



	//Write code that writes data to led matrix driver (8-bit data). Use values from dots array
	//Hint: use nested loops (loops inside loops)
	//Hint2: loop iterations are 8,3,8 (pixels,color,8-bitdata)


}

//Latch signal. See colorsshield.pdf or DM163.pdf in project folder on how latching works
void latch(){
 // Step 1: Set the LAT bit to 1 to signal the end of data writing
    CONTROL_SIGNAL |= (1 << 1); // Assuming bit 1 in CONTROL_SIGNAL is the LAT bit

    // Step 2: Clear the LAT bit to 0 to complete the latch signal
    CONTROL_SIGNAL &= ~(1 << 1);

}


//Set one line (channel) as active, one at a time.
void open_line(uint8_t x){
 // Clear all channels first to ensure only the selected channel is active
    CHANNEL_SIGNAL = 0;

    // Use switch-case to activate only the specified channel
    switch (channel) {
        case 0:
            CHANNEL_SIGNAL |= (1 << 0); // Activate channel 0
            break;
        case 1:
            CHANNEL_SIGNAL |= (1 << 1); // Activate channel 1
            break;
        case 2:
            CHANNEL_SIGNAL |= (1 << 2); // Activate channel 2
            break;
        case 3:
            CHANNEL_SIGNAL |= (1 << 3); // Activate channel 3
            break;
        case 4:
            CHANNEL_SIGNAL |= (1 << 4); // Activate channel 4
            break;
        case 5:
            CHANNEL_SIGNAL |= (1 << 5); // Activate channel 5
            break;
        case 6:
            CHANNEL_SIGNAL |= (1 << 6); // Activate channel 6
            break;
        case 7:
            CHANNEL_SIGNAL |= (1 << 7); // Activate channel 7
            break;
        default:
            // Default case to clear all channels if an invalid channel number is provided
            CHANNEL_SIGNAL = 0;
            break;
    }

}



