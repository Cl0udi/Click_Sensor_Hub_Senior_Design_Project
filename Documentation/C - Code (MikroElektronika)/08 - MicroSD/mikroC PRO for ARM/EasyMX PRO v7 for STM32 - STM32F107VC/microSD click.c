/**************************************************************************************************
* Company: (c) mikroElektronika
* Revision History:
*     20120521:
*       - initial release (JK adn DO);
* Description:
*     This module consists of several blocks that demonstrate various aspects of
*     usage of the Mmc library. These are:
*     - Creation of new file and writing down to it;
*     - Opening existing file and re-writing it (writing from start-of-file);
*     - Opening existing file and appending data to it (writing from end-of-file);
*     - Opening a file and reading data from it (sending it to USART terminal);
*     - Creating and modifying several files at once;
*     - Reading file contents;
*     - Deleting file(s);
*     - Creating the swap file (see Help for details);
*
* Test configuration:
*       MCU:             STM32F107VC
*                        http://www.st.com/internet/com/TECHNICAL_RESOURCES/TECHNICAL_LITERATURE/DATASHEET/DM00037051.pdf
*       Dev.Board:       EasyMx PRO v7 for STM32
*                        http://www.mikroe.com/eng/products/view/852/easymx-pro-v7-for-stm32/
*       Ext. Modules:    microSD click board ac:microSDclick
*                        http://http://www.mikroe.com/eng/products/view/776/microsd-click/
*       Oscillator:      HS-PLL 72.0000 MHz, internal 8.0000 MHz RC
*       SW:              mikroC PRO for ARM
*                        http://www.mikroe.com/mikroc/arm/
*
* NOTES:
*     - Place Easy TFT into the socket.
*     - Place microSD click board into the mikroBUS socket 1.
*     - Plug USB cable in USB UART A connector.
*     - Turn on switches for UART communication RX-PA9 and TX-PA10.
*     - On TFT are displayed only messages. Context of created files are sent to UART1 module.
*     - Make sure that MMC card is properly formatted (to FAT16 or just FAT)
*       before testing it on this example.
*     - This example expects MMC card to be inserted before reset, otherwise,
*       the FAT_ERROR message is displayed.
**************************************************************************************************/
#include "resources.h"

// TFT module connections
unsigned int TFT_DataPort at GPIOE_ODR;
sbit TFT_RST  at GPIOE_ODR.B8;
sbit TFT_RS   at GPIOE_ODR.B12;
sbit TFT_CS   at GPIOE_ODR.B15;
sbit TFT_RD   at GPIOE_ODR.B10;
sbit TFT_WR   at GPIOE_ODR.B11;
sbit TFT_BLED at GPIOE_ODR.B9;
// End TFT module connections

// MMC Chip Select connection
//**************************************************************************************************
sbit Mmc_Chip_Select           at GPIOD_ODR.B13;
//**************************************************************************************************

void InitMCU(){
   // TFT config
  GPIO_Config(&GPIOE_BASE, _GPIO_PINMASK_9, _GPIO_CFG_DIGITAL_OUTPUT);
  TFT_Set_Default_Mode();

  UART1_Init(115200);
  TFT_Init(320, 240);
  Delay_ms(1000);
  TFT_BLED = 1;
}
/*************************************************************************************************/
void DrawMMCScr(){
  TFT_Fill_Screen(CL_WHITE);
  TFT_Set_Pen(CL_Black, 1);
  TFT_Line(20, 220, 300, 220);
  TFT_LIne(20,  46, 300,  46);
  TFT_Set_Font(&HandelGothic_BT21x22_Regular, CL_RED, FO_HORIZONTAL);
  TFT_Write_Text("microSD click TEST", 75, 14);
  TFT_Set_Font(&Verdana12x13_Regular, CL_BLACK, FO_HORIZONTAL);
  TFT_Write_Text("EasyMX PRO v7", 19, 223);
  TFT_Set_Font(&Verdana12x13_Regular, CL_RED, FO_HORIZONTAL);
  TFT_Write_Text("www.mikroe.com", 200, 223);
  TFT_Set_Font(&TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);
}
/*********************************************************************************************/

const LINE_LEN = 43;
char err_txt[20]       = "FAT16 not found";
char file_contents[LINE_LEN] = "XX MMC/SD FAT16 library by Anton Rieckert\n";
char           filename[14] = "MIKRO00x.TXT";          // File names
unsigned short loop, loop2;
unsigned long  i, size;
char           Buffer[512];

// UARTF write text and new line (carriage return + line feed)
void UART_Write_Line(char *uart_text) {
  UART1_Write_Text(uart_text);
  UART1_Write(13);
  UART1_Write(10);
}

// Creates new file and writes some data to it
void M_Create_New_File() {
  filename[7] = 'A';
  Mmc_Fat_Set_File_Date(2010, 4, 19, 9, 0, 0); // Set file date & time info
  Mmc_Fat_Assign(&filename, 0xA0);          // Find existing file or create a new one
  Mmc_Fat_Rewrite();                        // To clear file and start with new data

  for(loop = 1; loop <= 99; loop++) {
     UART1_Write('.');
     TFT_Write_Text("Creating New File...", 100 , 155);
     Delay_ms(10);
     TFT_Set_Font(&TFT_defaultFont,CL_WHITE, FO_HORIZONTAL);
     TFT_Write_Text("Creating New File...", 100 , 155);
     Delay_ms(10);
     TFT_Set_Font(&TFT_defaultFont,CL_BLACK,FO_HORIZONTAL);
     TFT_Write_Text("Creating New File...", 100 , 155);

    file_contents[0] = loop / 10 + 48;
    file_contents[1] = loop % 10 + 48;
    Mmc_Fat_Write(file_contents, LINE_LEN-1);   // write data to the assigned file
  }
    TFT_Set_Font(&TFT_defaultFont,CL_WHITE, FO_HORIZONTAL);
    TFT_Write_Text("Creating New File...", 100 , 155);
}

// Creates many new files and writes data to them
void M_Create_Multiple_Files() {
  for(loop2 = 'B'; loop2 <= 'Z'; loop2++) {
    UART1_Write(loop2);                  // signal the progress
    filename[7] = loop2;                 // set filename
    Mmc_Fat_Set_File_Date(2010, 4, 19, 9, 0, 0); // Set file date & time info
    Mmc_Fat_Assign(&filename, 0xA0);     // find existing file or create a new one
    Mmc_Fat_Rewrite();                   // To clear file and start with new data
    for(loop = 1; loop <= 44; loop++) {
      file_contents[0] = loop / 10 + 48;
      file_contents[1] = loop % 10 + 48;
      TFT_Write_Text("Creating Multiple Files...", 100 , 155);
      Delay_ms(10);
      TFT_Set_Font(&TFT_defaultFont,CL_WHITE, FO_HORIZONTAL);
      TFT_Write_Text("Creating Multiple Files...", 100 , 155);
      Delay_ms(10);
      TFT_Set_Font(&TFT_defaultFont,CL_BLACK,FO_HORIZONTAL);
      TFT_Write_Text("Creating Multiple Files...", 100 , 155);
      Mmc_Fat_Write(file_contents, LINE_LEN-1);  // write data to the assigned file
    }
    TFT_Set_Font(&TFT_defaultFont,CL_WHITE, FO_HORIZONTAL);
    TFT_Write_Text("Creating Multiple Files...", 100 , 155);
  }
}

// Opens an existing file and rewrites it
void M_Open_File_Rewrite() {
  filename[7] = 'C';
  Mmc_Fat_Assign(&filename, 0);
  Mmc_Fat_Rewrite();

  for(loop = 1; loop <= 55; loop++) {
    file_contents[0] = loop / 10 + 48;
    file_contents[1] = loop % 10 + 48;
    Mmc_Fat_Write(file_contents, LINE_LEN-1);    // write data to the assigned file
    TFT_Write_Text("Open File Rewrite...", 100 , 155);
    Delay_ms(10);
    TFT_Set_Font(&TFT_defaultFont,CL_BLACK, FO_HORIZONTAL);
    TFT_Write_Text("Open File Rewrite...", 100 , 155);
    Delay_ms(10);
    TFT_Set_Font(&TFT_defaultFont,CL_WHITE, FO_HORIZONTAL);
    TFT_Write_Text("Open File Rewrite...", 100 , 155);
  }
  TFT_Set_Font(&TFT_defaultFont,CL_WHITE, FO_HORIZONTAL);
  TFT_Write_Text("Open File Rewrite...", 100 , 155);
}

// Opens an existing file and appends data to it
//               (and alters the date/time stamp)
void M_Open_File_Append() {
   filename[7] = 'B';
   Mmc_Fat_Assign(&filename, 0);
   Mmc_Fat_Set_File_Date(2010, 4, 19, 9, 20, 0);
   Mmc_Fat_Append();                                    // Prepare file for append
   Mmc_Fat_Write(" for mikroElektronika 2011\n", 27);   // Write data to assigned file
    TFT_Set_Font(&TFT_defaultFont,CL_BLACK, FO_HORIZONTAL);
    TFT_Write_Text("Open File Append...", 100 , 155);
    Delay_ms(10);
    TFT_Set_Font(&TFT_defaultFont,CL_WHITE, FO_HORIZONTAL);
    TFT_Write_Text("Open File Append...", 100 , 155);
}

// Opens an existing file, reads data from it and puts it to UART
void M_Open_File_Read() {
  char character;

  filename[7] = 'B';
  Mmc_Fat_Assign(&filename, 0);
  Mmc_Fat_Reset(&size);            // To read file, procedure returns size of file
  for (i = 1; i <= size; i++) {
    Mmc_Fat_Read(&character);
    UART1_Write(character);        // Write data to UART
  }
}

// Deletes a file. If file doesn't exist, it will first be created
// and then deleted.
void M_Delete_File() {
  filename[7] = 'F';
  Mmc_Fat_Assign(filename, 0);
  Mmc_Fat_Delete();
}

// Tests whether file exists, and if so sends its creation date
// and file size via UART
void M_Test_File_Exist() {
  unsigned long  fsize;
  unsigned int   year;
  unsigned short month, day, hour, minute;
  unsigned char  outstr[12];

  filename[7] = 'B';
//  filename[7] = 'F';
  if (Mmc_Fat_Assign(filename, 0)) {
    //--- file has been found - get its create date
    Mmc_Fat_Get_File_Date(&year, &month, &day, &hour, &minute);
    UART1_Write_Text(" created: ");
    WordToStr(year, outstr);
    UART1_Write_Text(outstr);
    ByteToStr(month, outstr);
    UART1_Write_Text(outstr);
    WordToStr(day, outstr);
    UART1_Write_Text(outstr);
    WordToStr(hour, outstr);
    UART1_Write_Text(outstr);
    WordToStr(minute, outstr);
    UART1_Write_Text(outstr);

    //--- file has been found - get its modified date
    Mmc_Fat_Get_File_Date_Modified(&year, &month, &day, &hour, &minute);
    UART1_Write_Text(" modified: ");
    WordToStr(year, outstr);
    UART1_Write_Text(outstr);
    ByteToStr(month, outstr);
    UART1_Write_Text(outstr);
    WordToStr(day, outstr);
    UART1_Write_Text(outstr);
    WordToStr(hour, outstr);
    UART1_Write_Text(outstr);
    WordToStr(minute, outstr);
    UART1_Write_Text(outstr);

    //--- get file size
    fsize = Mmc_Fat_Get_File_Size();
    LongToStr((signed long)fsize, outstr);
    UART_Write_Line(outstr);
  }
  else {
    //--- file was not found - signal it
    UART1_Write(0x55);
    Delay_ms(1000);
    UART1_Write(0x55);
  }
}
// Tries to create a swap file, whose size will be at least 100
// sectors (see Help for details)
void M_Create_Swap_File() {
  unsigned int i;

  for(i=0; i<512; i++)
    Buffer[i] = i;

  size = Mmc_Fat_Get_Swap_File(5000, "mikroE.txt", 0x20);   // see help on this function for details

  if (size) {
    LongToStr((signed long)size, err_txt);
    UART_Write_Line(err_txt);

    for(i=0; i<5000; i++) {
      Mmc_Write_Sector(size++, Buffer);
      UART1_Write('.');
    }
  }
}

// Main. Uncomment the function(s) to test the desired operation(s)
void main() {
  #define COMPLETE_EXAMPLE         // comment this line to make simpler/smaller example
  InitMCU();
  DrawMMCScr();
  Delay_ms(10);

  TFT_Write_Text("1. STM32-Started",90, 55);
  TFT_Write_Text("2. Check for MMC...", 90, 75);

  // Initialize SPI3 module
  SPI3_Init_Advanced(_SPI_FPCLK_DIV64, _SPI_MASTER  | _SPI_8_BIT |
                     _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION |
                     _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1,
                     &_GPIO_MODULE_SPI3_PC10_11_12);
  TFT_BLED = 1;                           // Backlight ON

  // use fat16 quick format instead of init routine if a formatting is needed
  if (Mmc_Fat_Init() == 0) {
    TFT_Write_Text("3. MMC is ready to use!", 90, 105);
    // reinitialize spi at higher speed
    SPI3_Init_Advanced(_SPI_FPCLK_DIV2, _SPI_MASTER  | _SPI_8_BIT |
                       _SPI_CLK_IDLE_LOW | _SPI_FIRST_CLK_EDGE_TRANSITION |
                       _SPI_MSB_FIRST | _SPI_SS_DISABLE | _SPI_SSM_ENABLE | _SPI_SSI_1,
                       &_GPIO_MODULE_SPI3_PC10_11_12);
    TFT_BLED = 1;                         // Backlight ON
    //--- Test start
    TFT_Write_Text("4. Test Start", 90, 135);
    UART_Write_Line("Test Start.");
    TFT_Write_Text("Watch detailed test on UART!", 90, 205);
    //--- Test routines. Uncomment them one-by-one to test certain features
    M_Create_New_File();
    #ifdef COMPLETE_EXAMPLE
      M_Create_Multiple_Files();
      M_Open_File_Rewrite();
      M_Open_File_Append();
      M_Open_File_Read();
      M_Delete_File();
      M_Test_File_Exist();
      M_Create_Swap_File();
    #endif
    UART_Write_Line("Test End.");
    TFT_Set_Font(&TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);
    TFT_Write_Text("5. Done!", 90, 165);
  }
  else
  {
    TFT_Write_Text("3. FAT16 not found!", 90, 105);
    UART_Write_Line(err_txt); // Note: Mmc_Fat_Init tries to initialize a card more than once.
                               //       If card is not present, initialization may last longer (depending on clock speed)
  }

}