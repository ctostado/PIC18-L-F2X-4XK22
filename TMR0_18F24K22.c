/* ****************************************************************************
 * File:            TMR0_PIC18F24K22.c
 * Author:          Carlos Tostado
 * Fecha:           28 de junio de 2016
 * ****************************************************************************/

#include <xc.h>
#include <stdint.h>

// <editor-fold defaultstate="collapsed" desc="Bits de configuracion">

// CONFIG1H
#pragma config FOSC = INTIO67    // Oscillator Selection bits->Internal oscillator block
#pragma config PLLCFG = OFF    // 4X PLL Enable->Oscillator used directly
#pragma config PRICLKEN = ON    // Primary clock enable bit->Primary clock is always enabled
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable bit->Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF    // Internal/External Oscillator Switchover bit->Oscillator Switchover mode disabled

// CONFIG2L
#pragma config PWRTEN = OFF    // Power-up Timer Enable bit->Power up timer disabled
#pragma config BOREN = SBORDIS    // Brown-out Reset Enable bits->Brown-out Reset enabled in hardware only (SBOREN is disabled)
#pragma config BORV = 190    // Brown Out Reset Voltage bits->VBOR set to 1.90 V nominal

// CONFIG2H
#pragma config WDTEN = SWON    // Watchdog Timer Enable bits->WDT is controlled by SWDTEN bit of the WDTCON register
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits->1:32768

// CONFIG3H
#pragma config CCP2MX = PORTC1    // CCP2 MUX bit->CCP2 input/output is multiplexed with RC1
#pragma config PBADEN = ON    // PORTB A/D Enable bit->PORTB<5:0> pins are configured as analog input channels on Reset
#pragma config CCP3MX = PORTB5    // P3A/CCP3 Mux bit->P3A/CCP3 input/output is multiplexed with RB5
#pragma config HFOFST = ON    // HFINTOSC Fast Start-up->HFINTOSC output and ready status are not delayed by the oscillator stable status
#pragma config T3CMX = PORTC0    // Timer3 Clock input mux bit->T3CKI is on RC0
#pragma config P2BMX = PORTD2    // ECCP2 B output mux bit->P2B is on RD2
#pragma config MCLRE = EXTMCLR    // MCLR Pin Enable bit->MCLR pin enabled, RE3 input pin disabled

// CONFIG4L
#pragma config STVREN = ON    // Stack Full/Underflow Reset Enable bit->Stack full/underflow will cause Reset
#pragma config LVP = OFF    // Single-Supply ICSP Enable bit->Single-Supply ICSP disabled
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
#pragma config DEBUG = OFF    // Background Debug->Disabled

// CONFIG5L
#pragma config CP0 = OFF    // Code Protection Block 0->Block 0 (000800-001FFFh) not code-protected
#pragma config CP1 = OFF    // Code Protection Block 1->Block 1 (002000-003FFFh) not code-protected

// CONFIG5H
#pragma config CPB = OFF    // Boot Block Code Protection bit->Boot block (000000-0007FFh) not code-protected
#pragma config CPD = OFF    // Data EEPROM Code Protection bit->Data EEPROM not code-protected

// CONFIG6L
#pragma config WRT0 = OFF    // Write Protection Block 0->Block 0 (000800-001FFFh) not write-protected
#pragma config WRT1 = OFF    // Write Protection Block 1->Block 1 (002000-003FFFh) not write-protected

// CONFIG6H
#pragma config WRTC = OFF    // Configuration Register Write Protection bit->Configuration registers (300000-3000FFh) not write-protected
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block (000000-0007FFh) not write-protected
#pragma config WRTD = OFF    // Data EEPROM Write Protection bit->Data EEPROM not write-protected

// CONFIG7L
#pragma config EBTR0 = OFF    // Table Read Protection Block 0->Block 0 (000800-001FFFh) not protected from table reads executed in other blocks
#pragma config EBTR1 = OFF    // Table Read Protection Block 1->Block 1 (002000-003FFFh) not protected from table reads executed in other blocks

// CONFIG7H
#pragma config EBTRB = OFF    // Boot Block Table Read Protection bit->Boot Block (000000-0007FFh) not protected from table reads executed in other blocks

// </editor-fold>

// Frecuencia de trabajo del microcontrolador
#define _XTAL_FREQ 16000000

//Variable global que servira como contador
volatile uint8_t counter = 0;

// Función donde se alojan las rutinas que seran ejecutadas durante la interrupción
void interrupt interrupciones ()
{
    //La interrupcion debe ocurrir cada 0.010048 segundos
    if ( (1 == INTCONbits.TMR0IF) && (1 == INTCONbits.TMR0IE) )
    {
        if (++counter >= 100){
           counter = 0;
           PORTBbits.RB0 ^= 1;                                                  //Cambia el estado del PIN
        }
        INTCONbits.TMR0IF = 0;                                                  //Limpia la bandera
        TMR0L= 0x63;                                                            //Recarga el valor del TMR0L
    }
}

void main(void) {
   
    //Configuracion de oscilador
    OSCCONbits.IDLEN = 0;                                                       //Equipo entra en MODO SLEEP con la instruccion Sleep
    OSCCONbits.IRCF = 0b111;                                                    //Frecuencia de oscilador a MHz
    OSCCONbits.OSTS = 0;                                                        //Oscilador interno
    OSCCONbits.HFIOFS = 0;                                                      //Frecuencia de oscilador no estable
    OSCCONbits.SCS = 0b10;                                                      //Bloque de osciladror interno
    
    //Configuracion inicial de los pines
    ANSELA = 0;                                                                 //Configuracion de pines como digitales 
    ANSELB = 0;                                                                 //Configuracion de pines como digitales 
    ANSELC = 0;                                                                 //Configuracion de pines como digitales 
    ANSELD = 0;                                                                 //Configuracion de pines como digitales 
    
    //COnfiguracion de entradas y salidas
    TRISA = 0;                                                                  //Todos como salida  
    TRISB = 0;                                                                  //Todos como salida  
    TRISC = 0;                                                                  //Todos como salida  
    TRISE = 0;                                                                  //Todos como salida  
    TRISD = 0;                                                                  //Todos como salida  
    
    //Estado inicial del pin
    PORTA = 0;                                                                  //Todos EN CERO   
    PORTB = 0;                                                                  //Todos EN CERO   
    PORTC = 0;                                                                  //Todos EN CERO   
    PORTE = 0;                                                                  //Todos EN CERO   
    PORTD = 0;                                                                  //Todos EN CERO    
    
    //Configuracion para el TIMER 0
    //T0CON = 0b11000011;                                                         //Timer0 10ms, Timer a 8bit, Prescaler como fuente de reloj, Prescaler 1:16, Configuración como oscilador interno, Habilita e timer 8
    
    //Configuracion para el TIMER 0
    T0CONbits.TMR0ON = 1;                                                       //Habilita el TRM0
    T0CONbits.T08BIT = 1;                                                       //TMR0 a 8bits
    T0CONbits.T0CS   = 0;                                                       //Proviene de oscilador
    T0CONbits.PSA    = 0;                                                       //Se asigna prescaler
    T0CONbits.T0PS   = 0b111;                                                   //Prescaler a 256
    
    TMR0H = 0x00;                                                               //Parte alta del registro en 0
    TMR0L = 0x63;                                                               //Parte baja del registro en 99 (0x63)
    
    //Configracion de los registros de interrupcion
    INTCONbits.GIE    = 1;                                                      //Interrupciones globales habilitadas
    INTCONbits.PEIE   = 1;                                                      //Habilita interrupciones por perifericos
    INTCONbits.TMR0IE = 1;                                                      //Habilita interrupciones por TMR0
    
    // Ciclo infiito, no hara nada hasta que la interrupcion sea generada
    while(1)
    {}
    
}
