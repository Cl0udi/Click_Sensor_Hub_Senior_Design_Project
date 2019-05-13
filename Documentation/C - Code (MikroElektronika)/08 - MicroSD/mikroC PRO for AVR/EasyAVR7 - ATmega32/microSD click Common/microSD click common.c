/*
 * Project name:
     microSD click (MMC Library Example)
 * Copyright:
     (c) Mikroelektronika, 2011.
 * Revision History:
     20111114:
       - initial release (FJ);
     20130627:
       - modified for EasyAVR v7 (MV);
 * Description:
     MMC library test. Upon flashing, insert a microSD card into the
     module, when you should receive the "Init-OK" message.
     Then, you can experiment with MMC read and write functions,
     and observe the results through the USART Terminal.
  * Test configuration:
     MCU:             ATmega32
                      http://www.atmel.com/Images/doc2503.pdf
     Dev.Board:       EasyAVR7
                      ac:EasyAVR7
                      http://www.mikroe.com/easyavr/
     Oscillator:      Oscillator Frequency 8.0000 MHz, 8.0000 MHz Crystal
     ext. modules:    microSD Click
                      ac:microSD_Click
                      http://www.mikroe.com/click/microsd/
     SW:              mikroC PRO for AVR
                      http://www.mikroe.com/mikroc/avr/
 * NOTES:
     - Make sure that microSD card is properly formatted (to FAT16 or just FAT)
       before testing it with this example.
     - Place microSD click board at the mikroBUS socket 1 on the EasyAVR7 board.
     - Put power supply jumper (J5) on the EasyPIC7 board in 3.3V position.
     - Make sure all pull down / pull up switches are in neutral position.
     - Turn on Rx and Tx switches (SW10.1 and SW10.2) and place appropriate jumpers
       in the RS-232 position or USB UART position
     - Connect RS-232 or USB cable to the respective UART connectors.
     - Turn ON SW5.4 , SW5.5 , SW5.6
 */

// microSD click module connections
sbit Mmc_Chip_Select           at PORTA5_bit;
sbit Mmc_Chip_Select_Direction at DDA5_bit;
sbit Mmc_Card_Detect           at PORTA7_bit;
sbit Mmc_Card_Detect_Direction at DDA7_bit;
// eof microSD click module connections

// Variables for MMC routines
unsigned char SectorData[512];        // Buffer for MMC sector reading/writing
unsigned char data_for_registers[16]; // buffer for CID and CSD registers

// UART1 write text and new line (carriage return + line feed)
void UART1_Write_Line(char *uart_text) {
  UART1_Write_Text(uart_text);
  UART1_Write(13);
  UART1_Write(10);
}

// Display byte in hex
void PrintHex(unsigned char i) {
  unsigned char hi,lo;

  hi = i & 0xF0;               // High nibble
  hi = hi >> 4;
  hi = hi + '0';
  if (hi > '9')
    hi = hi + 7;
  lo = (i & 0x0F) + '0';       // Low nibble
  if (lo > '9')
    lo = lo + 7;

  UART1_Write(hi);
  UART1_Write(lo);
}

void main() {
  const char   FILL_CHAR = 'm';
  unsigned int i, SectorNo;
  char         mmc_error;
  bit          data_ok;

  Mmc_Card_Detect_Direction = 0;   // Set card detection pin to be input

  UART1_Init(19200);               // Initialize UART1 module
  Delay_ms(10);
  UART1_Write_Line("Insert microSD Card");

  while(Mmc_Card_Detect)           // Loop until card is detected
    ;

  Delay_ms(500);

  UART1_Write_Line("microSD Card inserted");  // microSD Card present report

  // Initialize SPI1 module
  SPI1_Init_Advanced(_SPI_MASTER, _SPI_FCY_DIV4, _SPI_CLK_LO_LEADING);

  // Initialize MMC card
  mmc_error = Mmc_Init();
  if(mmc_error == 0)
    UART1_Write_Line("MMC Init-OK");    // If MMC present report
  else
    UART1_Write_Line("MMC Init-error"); // If error report

  // Fill MMC buffer with same characters
  for(i=0; i<=511; i++)
    SectorData[i] = FILL_CHAR;

  // Choose sector
  SectorNo = 590;

  // Write sector
  mmc_error = Mmc_Write_Sector(SectorNo, SectorData);
  if(mmc_error == 0)
    UART1_Write_Line("Write-OK");
  else  // if there are errors.....
    UART1_Write_Line("Write-Error");

  // Reading of CID register
  mmc_error = Mmc_Read_Cid(data_for_registers);
  if(mmc_error == 0) {
    UART1_Write_Text("CID : ");
    for(i=0; i<=15; i++)
      PrintHex(data_for_registers[i]);
    UART1_Write_Line("");
  }
  else
    UART1_Write_Line("CID-error");

  // Reading of CSD register
  mmc_error = Mmc_Read_Csd(data_for_registers);
  if(mmc_error == 0) {
    UART1_Write_Text("CSD : ");
    for(i=0; i<=15; i++)
      PrintHex(data_for_registers[i]);
    UART1_Write_Line("");
  }
  else
    UART1_Write_Line("CSD-error");

  // Read sector
  mmc_error = Mmc_Read_Sector(SectorNo, SectorData);
  if(mmc_error == 0) {
    UART1_Write_Line("Read-OK");
    // Chech data match
    data_ok = 1;
    for(i=0; i<=511; i++) {
      UART1_Write(SectorData[i]);
      if (SectorData[i] != FILL_CHAR) {
        data_ok = 0;
        break;
      }
    }
    UART1_Write_Line("");
    if (data_ok)
      UART1_Write_Line("Content-OK");
    else
      UART1_Write_Line("Content-Error");
  }
  else  // if there are errors.....
    UART1_Write_Line("Read-Error");

  // Signal test end
  UART1_Write_Line("Test End.");
}