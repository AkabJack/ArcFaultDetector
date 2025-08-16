#define FCY 200000000
#include <stdio.h>
#include <xc.h>
#include <libpic30.h>
#include <dsp.h>

#define SAMPLE_SIZE         512         //Sample size for the FFT (power of 2)
#define LOG2_FFT_SIZE       9
#define ADC_BUFFER_SIZE     SAMPLE_SIZE //size of the adc buffer
#define ADC_CENTER          1241
#define ADC_TO_Q15_SHIFT    4
#define ARC_THRESHOLD       1       //energy
#define SAMPLE_RATE         512000      //512kHz, margin for maxim signal of 150kHz(nyquist theorem = 300kHz)
#define ARC_BIN_START       ((10000*SAMPLE_SIZE)/SAMPLE_RATE)
#define ARC_BIN_END         ((100000*SAMPLE_SIZE)/SAMPLE_RATE)//100k
#define BIN_SIZE            90
//buffers
uint32_t adc_buffer[ADC_BUFFER_SIZE];
fractcomplex fft_buffer[SAMPLE_SIZE] __attribute__((space(xmemory), aligned(512)));
fractcomplex twiddle_table[SAMPLE_SIZE/2];
fractional hammingWindow[SAMPLE_SIZE];

uint32_t power[BIN_SIZE];

uint32_t min_trigger_arc_freq  = 10000;//10kHz
uint32_t min_trigger_arc_power = 100;  //over the noise level

uint8_t sys_enable;
uint8_t arc_detect;

fractcomplex* twiddle_ptr;

int __C30_UART = 1;

void _mon_putc(char c){
    while (U1STAbits.UTXBF);
    U1TXREG = c;
}

//pin 10 | RB0 |-> Atenuare x10
//pin 12 | RB2 |-> Atenuare x5
//pin 13 | RB3 |-> Amplificare x1
//pin 14 | RB4 |-> Amplificare x5
//pin 15 | RC0 |-> Amplificare x10
//pin 16 | RC1 |-> Enable


void configure_adc();
void acquire_adc_samples();
void prepare_fft_input();
void compute_fft_and_detect();
void flash_detect();
void receive_commands_from_uart();
void init_hamming_window();

int main(void){ 
    sys_enable = 1;
    arc_detect = 0;
    SYSTEM_Initialize();
    CCP2CON1bits.ON = 0;
    __delay_ms(1500);
    configure_adc();
    while(AD1CONbits.ADRDY == 0)__delay_ms(1);//wait for the ADC to initialise
    U1CONbits.RXEN = 1;//start UART module
    U1CONbits.TXEN = 1;//start UART module
    //LATBbits.LATB0 = 1;//Atenuare x10
    LATBbits.LATB3 = 1;//Amplificare x1
    LATCbits.LATC1 = 1;//Enable
    twiddle_ptr = TwidFactorInit(LOG2_FFT_SIZE, &twiddle_table[0], 0);
    init_hamming_window();
    LATCbits.LATC4 = 1;//STATUS LED
    while(1){   
        receive_commands_from_uart();//read commands on the uart from ESP8266
        if(sys_enable == 1){//system enable for monitoring the ADC output
            acquire_adc_samples();
            prepare_fft_input();
            compute_fft_and_detect();
            __delay_ms(50);
        }
    }
}

void configure_adc(){
    AD1CONbits.ON = 0;//Turn off the ADC 
    
    ANSELAbits.ANSELA3 = 1;  //RA3 = AD1ANN1
    TRISAbits.TRISA3 = 1;    // RA3 as input
    AD1CH0CONbits.MODE = 0;// Software trigger will start a conversion.
    AD1CH0CONbits.TRG1SRC = 1;
    AD1CH0CONbits.DIFF = 0;
    //AD1CH0CONbits.SAMC = 4;
    AD1CONbits.MODE = 2;

    AD1CONbits.ON = 1;
    
    AD1CH0CONbits.TRG1SRC = 12;
    AD1CH0CONbits.TRG2SRC = 12;
}


void acquire_adc_samples(){//Function to populate the buffer with the values from the ADC
    for (int i = 0; i < ADC_BUFFER_SIZE; i++){//it's being controlled using a timer with a frequency of 2.5us (400kHz)
        while(!AD1STATbits.CH0RDY);//wait to finish sampling
        adc_buffer[i] = AD1CH0DATA;
    }
}

void prepare_fft_input(){//Convert the data to Q15 fractional and populate the fft_buffer
    for(int i = 0; i < SAMPLE_SIZE; i++){
        int32_t sample = adc_buffer[i] - ADC_CENTER;//center the ADC output
        fft_buffer[i].real = (fractional) (((int32_t)(sample << ADC_TO_Q15_SHIFT) * hammingWindow[i])>>15);
        fft_buffer[i].imag = 0;
    }
}

void compute_fft_and_detect(void) {
    FFTComplexIP(LOG2_FFT_SIZE, fft_buffer, twiddle_ptr, 0);
    BitReverseComplex(LOG2_FFT_SIZE, fft_buffer);

    uint32_t arc_energy = 0;
    uint32_t peak_power = 0;
    uint32_t peak_bin   = ARC_BIN_START;

    for (uint32_t bin = ARC_BIN_START; bin < ARC_BIN_END; bin++) {
        int32_t  re = fft_buffer[bin].real >> LOG2_FFT_SIZE;
        int32_t  im = fft_buffer[bin].imag >> LOG2_FFT_SIZE;
        uint32_t p  = (uint32_t)re*re + (uint32_t)im*im;
		
        if (arc_detect == 0) power[bin] = p;
        arc_energy  += p;          // accumulate *all* bins? energy
        if (p > peak_power) {      // whenever we see a new maximum
            peak_power = p;        //   remember its magnitude ?
            peak_bin   = bin;      //   ? and its bin index
        }
    }
    uint32_t freq = peak_bin * (uint32_t)SAMPLE_RATE / (uint32_t)SAMPLE_SIZE;
    if ((arc_energy > min_trigger_arc_power) && (freq > min_trigger_arc_freq)){
        CCP2CON1bits.ON = 0;
        flash_detect();
        //printf("ARC DETECTED! Energy=%u, bin=%u, power=%u, Freq=%u Hz\r\n" ,arc_energy ,peak_bin ,peak_power ,freq);
        CCP2CON1bits.ON = 1;
    }
}

void receive_commands_from_uart(){
    if (UART1_IsRxReady()){//receive buffer
        uint32_t command_from_uart = U1RXB;
        switch(command_from_uart){
            case 1://ATx10
                LATCbits.LATC1 = 0;//disable
                LATBbits.LATB0 = 1;//Atenuare x10
                LATBbits.LATB2 = 0;//Atenuare x5
                LATBbits.LATB3 = 0;//Amplificare x1
                LATBbits.LATB4 = 0;//Amplificare x5
                LATCbits.LATC0 = 0;//Amplificare x10
                LATCbits.LATC1 = 1;//enable
            break;
            case 2://ATx5
                LATCbits.LATC1 = 0;//disable
                LATBbits.LATB0 = 0;//Atenuare x10
                LATBbits.LATB2 = 1;//Atenuare x5
                LATBbits.LATB3 = 0;//Amplificare x1
                LATBbits.LATB4 = 0;//Amplificare x5
                LATCbits.LATC0 = 0;//Amplificare x10
                LATCbits.LATC1 = 1;//enable
            break;
            case 3://AMx1
                LATCbits.LATC1 = 0;//disable
                LATBbits.LATB0 = 0;//Atenuare x10
                LATBbits.LATB2 = 0;//Atenuare x5
                LATBbits.LATB3 = 1;//Amplificare x1
                LATBbits.LATB4 = 0;//Amplificare x5
                LATCbits.LATC0 = 0;//Amplificare x10
                LATCbits.LATC1 = 1;//enable
            break;
            case 4://AMx5
                LATCbits.LATC1 = 0;//disable
                LATBbits.LATB0 = 0;//Atenuare x10
                LATBbits.LATB2 = 0;//Atenuare x5
                LATBbits.LATB3 = 0;//Amplificare x1
                LATBbits.LATB4 = 1;//Amplificare x5
                LATCbits.LATC0 = 0;//Amplificare x10
                LATCbits.LATC1 = 1;//enable
            break;
            case 5://AMx10
                LATCbits.LATC1 = 0;//disable
                LATBbits.LATB0 = 0;//Atenuare x10
                LATBbits.LATB2 = 0;//Atenuare x5
                LATBbits.LATB3 = 0;//Amplificare x1
                LATBbits.LATB4 = 0;//Amplificare x5
                LATCbits.LATC0 = 1;//Amplificare x10
                LATCbits.LATC1 = 1;//enable
            break;
            case 6://START
                LATCbits.LATC1 = 1;
                sys_enable = 1;
            break;
            case 7://STOP
                LATCbits.LATC1 = 0;
                sys_enable = 0;
            break;
            case 8://SET_TRIGGER_FREQUENCY
                min_trigger_arc_freq = 0;
                for(int i = 0; i < 4; i++){
                    while(UART1_IsRxReady());//wait for new data
                    min_trigger_arc_freq = min_trigger_arc_freq + ((uint32_t)U1RXB<<i*8);//write the data on the dummy
                }
            break;
            case 9://SET_TRIGGER_POWER
                min_trigger_arc_power = 0;
                for(int i = 0; i < 4; i++){
                    while(UART1_IsRxReady());//wait for new data
                    min_trigger_arc_power = min_trigger_arc_power + ((uint32_t)U1RXB<<i*8);//write the data on the dummy
                }
            break;
            case 11://READ_POWER_TABLE
                    for(int i = 0; i < BIN_SIZE; i++){
                   for (int g = 0; g < 4; g++){
                        while(UART1_IsTxReady());//wait to be ready for transmitting data
                        U1TXB = power[i]<<g;    // Write the data byte to the USART.
                    }
                }
            break;
        }
    }
}

void init_hamming_window(void){
    for(int n = 0; n < SAMPLE_SIZE; n++){
        float wn = 0.54f - 0.46f * cosf((2.0f * M_PI * n) / (SAMPLE_SIZE - 1));
        hammingWindow[n] = (fractional) (wn * 32767);//convert to q15
    }
}

void flash_detect(){
    for(int i = 0; i < 10; i++){
        __delay_ms(200);
        LATCbits.LATC4 = 0;
        __delay_ms(200);
        LATCbits.LATC4 = 1;
    }
}