/*
 * Project name:
     microSD click (MMC Library Example)
 * Copyright:
     (c) Mikroelektronika, 2014.
 * Revision History:
     20140704:
       - initial release (FJ);
 * Description:
     MMC library test. Upon flashing, insert a microSD card into the
     module, when you should receive the "Init-OK" message.
     Then, you can experiment with MMC read and write functions,
     and observe the results through the USART Terminal.
 * Test configuration:
     MCU:             P32MX795F512L
                      http://ww1.microchip.com/downloads/en/DeviceDoc/61156F.pdf
     Dev.Board:       EasyPIC Fusion v7
                      http://www.mikroe.com/easypic-fusion/
     Oscillator:      XT-PLL, 80.000MHz
     Ext. Modules:    microSD click Board  - ac:microSDclick
                      http://www.mikroe.com/click/microsd/
     SW:              mikroC PRO for PIC32
                      http://www.mikroe.com/mikroc/pic32/
 * NOTES:
     - Make sure that microSD card is properly formatted (to FAT16 or just FAT)
       before testing it with this example.
     - Place microSD click board at the mikroBUS socket 1 on the EasyPIC Fusion v7 board.
     - Put PORTB switches in the middle (neutral) position.
     - Turn on SPI switches at SW13 and microSD card switches at SW14.
     - Turn on Rx and Tx switches (SW12.1 and SW12.1).
 */

// microSD click module connections
sbit Mmc_Chip_Select           at LATC2_bit;
sbit Mmc_Chip_Select_Direction at TRISC2_bit;
sbit Mmc_Card_Detect           at RB8_bit;
sbit Mmc_Card_Detect_Direction at TRISB8_bit;
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

  AD1PCFG = 0xFFFF;                // Configure AN pins as digital I/O
  JTAGEN_bit = 0;                  // Disable JTAG

  Mmc_Card_Detect_Direction = 1;   // Set card detection pin to be input

  UART2_Init(19200);               // Initialize UART2 module
  Delay_ms(10);
  UART2_Write_Line("Insert microSD Card");

  while(Mmc_Card_Detect)           // Loop until card is detected
    ;

  Delay_ms(500);

  UART2_Write_Line("microSD Card inserted");  // microSD Card present report

  // Initialize SPI3 module
  SPI3_Init_Advanced(_SPI_MASTER, _SPI_8_BIT, 80, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_HIGH, _SPI_ACTIVE_2_IDLE);

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