/*
 * Project name:
     microSD click (MMC Library Example)
 * Copyright:
     (c) Mikroelektronika, 2012.
 * Revision History:
     20120615:
       - initial release (DO);
 * Description:
     MMC library test. Upon flashing, insert a microSD card into the
     module, when you should receive the "Init-OK" message.
     Then, you can experiment with MMC read and write functions,
     and observe the results through the USART Terminal.
 * Test configuration:
     MCU:             dsPIC30F4013
                      http://ww1.microchip.com/downloads/en/devicedoc/70138c.pdf
     Dev.Board:       EasyPIC v7 for dsPIC30
                      http://www.mikroe.com/eng/products/view/887/easypic-v7-for-dspic-development-system/
     Oscillator:      XT-PLL 80.0000 MHz, 10.0000 MHz Crystal
     Ext. Modules:    microSD click Board  - ac:microSDclick
                      http://www.mikroe.com/eng/products/view/776/microsd-click/
     SW:              mikroC PRO for dsPIC
                      http://www.mikroe.com/eng/products/view/231/mikroc-pro-for-dspic/
 * NOTES:
     - Make sure that microSD card is properly formatted (to FAT16 or just FAT)
       before testing it with this example.
     - Place microSD click board at the mikroBUS socket 1.
     - Put power supply jumper (J16) on the board in 3.3V position.
     - Put PORTA switches in the middle (neutral) position.
     - Turn on Rx and Tx of UART2 - UART switches (SW8.2 and SW9.2).
     - Put jumpers J20 and J21 in USB possition .
     - Connect USB cable to the respective connector (USB UART).
 */

// microSD click module connections
sbit Mmc_Chip_Select           at LATB11_bit;
sbit Mmc_Chip_Select_Direction at TRISB11_bit;
sbit Mmc_Card_Detect           at RB6_bit;
sbit Mmc_Card_Detect_Direction at TRISB6_bit;
// eof microSD click module connections

// Variables for MMC routines
unsigned char SectorData[512];        // Buffer for MMC sector reading/writing
unsigned char data_for_registers[16]; // buffer for CID and CSD registers

// UART2 write text and new line (carriage return + line feed)
void UART2_Write_Line(char *uart_text) {
  UART2_Write_Text(uart_text);
  UART2_Write(13);
  UART2_Write(10);
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

  UART2_Write(hi);
  UART2_Write(lo);
}

void main() {
  const char   FILL_CHAR = 'm';
  unsigned int i, SectorNo;
  char         mmc_error;
  bit          data_ok;

  ADPCFG = 0xFFFF;                    // Configure ports as digital I/O

  Mmc_Card_Detect_Direction = 1;      // Set card detection pin to be input

  UART2_Init(19200);                  // Initialize UART2 module
  Delay_ms(10);
  UART2_Write_Line("Insert microSD Card");

  while(Mmc_Card_Detect)              // Loop until card is detected
    ;

  Delay_ms(500);

  UART2_Write_Line("microSD Card inserted");  // microSD Card present report

  // Initialize SPI1 module
  SPI1_Init_Advanced(_SPI_MASTER, _SPI_8_BIT,_SPI_PRESCALE_SEC_8, _SPI_PRESCALE_PRI_1, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_IDLE_2_ACTIVE);

  // Initialize MMC card
  mmc_error = Mmc_Init();
  if(mmc_error == 0)
    UART2_Write_Line("MMC Init-OK");    // If MMC present report
  else
    UART2_Write_Line("MMC Init-error"); // If error report

  // Fill MMC buffer with same characters
  for(i=0; i<=511; i++)
    SectorData[i] = FILL_CHAR;

  // Choose sector
  SectorNo = 590;

  // Write sector
  mmc_error = Mmc_Write_Sector(SectorNo, SectorData);
  if(mmc_error == 0)
    UART2_Write_Line("Write-OK");
  else  // if there are errors.....
    UART2_Write_Line("Write-Error");

  // Reading of CID register
  mmc_error = Mmc_Read_Cid(data_for_registers);
  if(mmc_error == 0) {
    UART2_Write_Text("CID : ");
    for(i=0; i<=15; i++)
      PrintHex(data_for_registers[i]);
    UART2_Write_Line("");
  }
  else
    UART2_Write_Line("CID-error");

  // Reading of CSD register
  mmc_error = Mmc_Read_Csd(data_for_registers);
  if(mmc_error == 0) {
    UART2_Write_Text("CSD : ");
    for(i=0; i<=15; i++)
      PrintHex(data_for_registers[i]);
    UART2_Write_Line("");
  }
  else
    UART2_Write_Line("CSD-error");

  // Read sector
  mmc_error = Mmc_Read_Sector(SectorNo, SectorData);
  if(mmc_error == 0) {
    UART2_Write_Line("Read-OK");
    // Chech data match
    data_ok = 1;
    for(i=0; i<=511; i++) {
      UART2_Write(SectorData[i]);
      if (SectorData[i] != FILL_CHAR) {
        data_ok = 0;
        break;
      }
    }
    UART2_Write_Line("");
    if (data_ok)
      UART2_Write_Line("Content-OK");
    else
      UART2_Write_Line("Content-Error");
  }
  else  // if there are errors.....
    UART2_Write_Line("Read-Error");

  // Signal test end
  UART2_Write_Line("Test End.");
}