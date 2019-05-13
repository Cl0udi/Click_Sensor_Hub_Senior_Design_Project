/*
 * Project name:
     microSD click FAT16 (Demonstration of usage of Mmc_Fat16 library)
 * Copyright:
     (c) Mikroelektronika, 2012.
 * Revision History:
     20120615:
       - initial release (DO);
 * Description:
     This project consists of several blocks that demonstrate various aspects of
     usage of the Mmc_Fat16 library. These are:
     - Creation of new file and writing down to it;
     - Opening existing file and re-writing it (writing from start-of-file);
     - Opening existing file and appending data to it (writing from end-of-file);
     - Opening a file and reading data from it (sending it to USART terminal);
     - Creating and modifying several files at once;
     - Reading file contents;
     - Deleting file(s);
     - Creating the swap file (see Help for details);
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

const LINE_LEN = 43;
char err_txt[20]       = "FAT16 not found";
char file_contents[LINE_LEN] = "XX MMC/SD FAT16 library by Anton Rieckert\n";
char           filename[14] = "MIKRO00x.TXT";   // File names
unsigned short loop, loop2;
unsigned long  i, size;

// UART2 write text and new line (carriage return + line feed)
void UART2_Write_Line(char *uart_text) {
  UART2_Write_Text(uart_text);
  UART2_Write(13);
  UART2_Write(10);
}

// Creates new file and writes some data to it
void M_Create_New_File() {
  filename[7] = 'A';
  Mmc_Fat_Set_File_Date(2010, 4, 19, 9, 0, 0);  // Set file date & time info
  Mmc_Fat_Assign(&filename, 0xA0);              // Find existing file or create a new one
  Mmc_Fat_Rewrite();                            // To clear file and start with new data
  for(loop = 1; loop <= 99; loop++) {
    UART2_Write('.');
    file_contents[0] = loop / 10 + 48;
    file_contents[1] = loop % 10 + 48;
    Mmc_Fat_Write(file_contents, LINE_LEN-1);   // write data to the assigned file
  }
}

// Creates many new files and writes data to them
void M_Create_Multiple_Files() {
  for(loop2 = 'B'; loop2 <= 'Z'; loop2++) {
    UART2_Write(loop2);                          // signal the progress
    filename[7] = loop2;                         // set filename
    Mmc_Fat_Set_File_Date(2010, 4, 19, 9, 0, 0); // Set file date & time info
    Mmc_Fat_Assign(&filename, 0xA0);             // find existing file or create a new one
    Mmc_Fat_Rewrite();                           // To clear file and start with new data
    for(loop = 1; loop <= 44; loop++) {
      file_contents[0] = loop / 10 + 48;
      file_contents[1] = loop % 10 + 48;
      Mmc_Fat_Write(file_contents, LINE_LEN-1);  // write data to the assigned file
    }
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
  }
}

// Opens an existing file and appends data to it
//               (and alters the date/time stamp)
void M_Open_File_Append() {
   filename[7] = 'B';
   Mmc_Fat_Assign(&filename, 0);
   Mmc_Fat_Set_File_Date(2010, 4, 19, 9, 20, 0);
   Mmc_Fat_Append();                                    // Prepare file for append
   Mmc_Fat_Write(" for mikroElektronika 2010\n", 27);   // Write data to assigned file
}

// Opens an existing file, reads data from it and puts it to UART
void M_Open_File_Read() {
  char character;

  filename[7] = 'B';
  Mmc_Fat_Assign(&filename, 0);
  Mmc_Fat_Reset(&size);            // To read file, procedure returns size of file
  for (i = 1; i <= size; i++) {
    Mmc_Fat_Read(&character);
    UART2_Write(character);        // Write data to UART
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

  filename[7] = 'B';           // Uncomment this line to search for file that DOES exists
  //filename[7] = 'F';         // Uncomment this line to search for file that DOES NOT exist
  if (Mmc_Fat_Assign(filename, 0)) {
    // File has been found - get its create date
    Mmc_Fat_Get_File_Date(&year, &month, &day, &hour, &minute);
    UART2_Write_Text(" created: ");
    WordToStr(year, outstr);
    UART2_Write_Text(outstr);
    ByteToStr(month, outstr);
    UART2_Write_Text(outstr);
    WordToStr(day, outstr);
    UART2_Write_Text(outstr);
    WordToStr(hour, outstr);
    UART2_Write_Text(outstr);
    WordToStr(minute, outstr);
    UART2_Write_Text(outstr);

    // File has been found - get its modified date
    Mmc_Fat_Get_File_Date_Modified(&year, &month, &day, &hour, &minute);
    UART2_Write_Text(" modified: ");
    WordToStr(year, outstr);
    UART2_Write_Text(outstr);
    ByteToStr(month, outstr);
    UART2_Write_Text(outstr);
    WordToStr(day, outstr);
    UART2_Write_Text(outstr);
    WordToStr(hour, outstr);
    UART2_Write_Text(outstr);
    WordToStr(minute, outstr);
    UART2_Write_Text(outstr);

    // Get file size
    fsize = Mmc_Fat_Get_File_Size();
    LongToStr((signed long)fsize, outstr);
    UART2_Write_Line(outstr);
  }
  else {
    // File was not found - signal it
    UART2_Write(0x55);
    Delay_ms(1000);
    UART2_Write(0x55);
  }
}

// Tries to create a swap file, whose size will be at least 100
// sectors (see Help for details)
void M_Create_Swap_File() {
  unsigned int i;

  size = Mmc_Fat_Get_Swap_File(5000, "mikroE.txt", 0x20);   // see help on this function for details

  for(i=0; i<512; i++)
    f16_sector.fSect[i] = i;

  if (size) {
    LongToStr((signed long)size, err_txt);
    UART2_Write_Line(err_txt);

    for(i=0; i<5000; i++) {
      Mmc_Write_Sector(size++, f16_sector.fSect);
      UART2_Write('.');
    }
  }
}

void main() {
  ADPCFG = 0xFFFF;                 // Configure ports as digital I/O

  Mmc_Card_Detect_Direction = 1;   // Set card detection pin to be input

  UART2_Init(19200);               // Initialize UART2 module
  Delay_ms(10);
  UART2_Write_Line("Insert microSD Card");

  while(Mmc_Card_Detect)           // Loop until card is detected
    ;
  Delay_ms(500);

  UART2_Write_Line("microSD Card inserted");  // microSD Card present report

  // Initialize SPI1 module
  SPI1_Init_Advanced(_SPI_MASTER, _SPI_8_BIT,_SPI_PRESCALE_SEC_8, _SPI_PRESCALE_PRI_1, _SPI_SS_DISABLE, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_IDLE_2_ACTIVE);

  // Use fat16 quick format instead of init routine if a formatting is needed
  if (Mmc_Fat_Init() == 0) {
    // Test start
    UART2_Write_Line("Test Start.");
    
    // Test routines. Uncomment them one-by-one to test certain features
    M_Create_New_File();
    M_Create_Multiple_Files();
    M_Open_File_Rewrite();
    M_Open_File_Append();
    M_Open_File_Read();
    M_Delete_File();
    M_Test_File_Exist();
    M_Create_Swap_File();
    UART2_Write_Line("Test End.");
  }
  else {
    UART2_Write_Line(err_txt); // Note: Mmc_Fat_Init tries to initialize a card more than once.
                               //       If card is not present, initialization may last longer (depending on clock speed)
  }
}