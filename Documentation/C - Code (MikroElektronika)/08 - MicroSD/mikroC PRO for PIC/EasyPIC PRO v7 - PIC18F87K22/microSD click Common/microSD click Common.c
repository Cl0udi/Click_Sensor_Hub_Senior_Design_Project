/*
 * Project name:
     microSD click (MMC Library Example)
 * Copyright:
     (c) Mikroelektronika, 2012.
 * Revision History:
     20120404:
       - initial release (FJ);
 * Description:
     MMC library test. Upon flashing, insert a microSD card into the
     module, when you should receive the "Init-OK" message.
     Then, you can experiment with MMC read and write functions,
     and observe the results through the USART Terminal.
 * Test configuration:
     MCU:             PIC18F87K22
                      http://ww1.microchip.com/downloads/en/DeviceDoc/39960d.pdf
     Dev.Board:       EasyPIC PRO v7
                      http://www.mikroe.com/eng/products/view/815/easypic-pro-v7-development-system/
     Oscillator:      HS-PLL 64.0000 MHz, 16.0000 MHz Crystal
     Ext. Modules:    microSD click Board  - ac:microSDclick
                      http://www.mikroe.com/eng/products/view/776/microsd-click/
     SW:              mikroC PRO for PIC
                      http://www.mikroe.com/eng/products/view/7/mikroc-pro-for-pic/
 * NOTES:
     - Make sure that microSD card is properly formatted (to FAT16 or just FAT)
       before testing it with this example.
     - Place microSD click board at the mikroBUS socket 1 on the EasyPIC PRO v7 board.
     - Put power supply jumper (J1) on the EasyPIC PRO v7 board in 3.3V position.
     - Put PORTA switches in the middle (neutral) position.
     - Turn on Rx and Tx  USB UART switches (SW5.1 and SW5.2).
     - Connect USB cable to the respective connector.

 */

// microSD click module connections
sbit Mmc_Chip_Select           at LATE0_bit;
sbit Mmc_Chip_Select_Direction at TRISE0_bit;
sbit Mmc_Card_Detect           at RA0_bit;
sbit Mmc_Card_Detect_Direction at TRISA0_bit;
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

  ANCON0 = 0;                      // Configure ports as digital I/O
  ANCON1 = 0;
  ANCON2 = 0;

  Mmc_Card_Detect_Direction = 1;   // Set card detection pin to be input

  UART1_Init(19200);               // Initialize UART1 module
  Delay_ms(10);
  UART1_Write_Line("Insert microSD Card");

  while(Mmc_Card_Detect)           // Loop until card is detected
    ;

  Delay_ms(500);

  UART1_Write_Line("microSD Card inserted");  // microSD Card present report

  // Initialize SPI1 module
  SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV4, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);

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