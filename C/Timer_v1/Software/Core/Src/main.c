/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bitmap.h"
#include "font.h"

#define LED_ON HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET)
#define LED_OFF HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_RESET)
#define RELAY_ON HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_SET)
#define RELAY_OFF HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_RESET);

#define ENTER_SET HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_SET
#define ENTER_RST HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET
#define ESCAPE_SET HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_SET
#define ESCAPE_RST HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET

#define RTC_STATUS_ENABLE HAL_GPIO_WritePin(GPIOB, RTC_STATUS_Pin, GPIO_PIN_SET)
#define RTC_STATUS_DISABLE HAL_GPIO_WritePin(GPIOB, RTC_STATUS_Pin, GPIO_PIN_RESET)
#define RTC_CLK_ON HAL_GPIO_WritePin(GPIOB, RTC_CLK_Pin, GPIO_PIN_SET)
#define RTC_CLK_OFF HAL_GPIO_WritePin(GPIOB, RTC_CLK_Pin, GPIO_PIN_RESET)
#define RTC_DATA_ON HAL_GPIO_WritePin(GPIOB, RTC_DATA_Pin, GPIO_PIN_SET)
#define RTC_DATA_OFF HAL_GPIO_WritePin(GPIOB, RTC_DATA_Pin, GPIO_PIN_RESET
#define RTC_DATA_SET HAL_GPIO_ReadPin(GPIOB, DATA_Pin) == GPIO_PIN_SET
#define RTC_DATA_RST HAL_GPIO_ReadPin(GPIOB, DATA_Pin) == GPIO_PIN_RESET

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  void RTC_WRITE_DATA (uint8_t data)
  {
	  GPIO_InitTypeDef data_out;                   // data pin out
	  data_out.Pin = RTC_DATA_Pin;                 //
	  data_out.Mode = GPIO_MODE_OUTPUT_PP;         //
	  data_out.Pull = GPIO_NOPULL;                 //
	  data_out.Speed = GPIO_SPEED_FREQ_LOW;        //
	  HAL_GPIO_Init(GPIOB, &data_out);             //

	  RTC_STATUS_ENABLE;
	  HAL_Delay(1);

  }

  uint8_t RTC_READ_DATA (void)
  {
	  GPIO_InitTypeDef data_in;          // data pin input
	  data_in.Pin = RTC_DATA_Pin;        //
	  data_in.Mode = GPIO_MODE_INPUT;    //
	  data_in.Pull = GPIO_NOPULL;        //
	  HAL_GPIO_Init(GPIOB, &data_in);    //

	  RTC_STATUS_ENABLE;
	  HAL_Delay(1);
  }


  void WRITE_EEPROM_PROGRAM_DATA (void)
  {
	  uint8_t mem_address = 0;
	  uint8_t value = 0;
	  uint8_t program_number = 0;
	  uint8_t a = 0;
	  uint8_t b = 0;
	  uint8_t c = 0;

	  while(a < 14)
	  {
		  for(b = 0; b < 5; b++)
		  {
			  value = program_data[program_number][c];
			  HAL_I2C_Mem_Write(&hi2c1, 0xa0, mem_address, 1, (uint8_t*)&value, 1, HAL_MAX_DELAY);
			  mem_address ++;
			  c++;
			  HAL_Delay(5);
		  }

		  program_number ++;
		  a++;
		  c = 0;
	  }
  }

  void READ_EEPROM_PROGRAM_DATA (void)
  {
	  uint8_t mem_address = 0;
	  uint8_t value = 0;
	  uint8_t program_number = 0;
	  uint8_t a = 0;
	  uint8_t b = 0;
	  uint8_t c = 0;

	  while(a < 14)
	  {
		  for(b = 0; b < 5; b++)
		  {

			  HAL_I2C_Mem_Read(&hi2c1, 0xa0, mem_address, 1, (uint8_t*)&value, 1, HAL_MAX_DELAY);
			  program_data[program_number][c] = value;
			  mem_address ++;
			  c++;
			  HAL_Delay(5);
		  }

		  program_number ++;
		  a++;
		  c = 0;
	  }
  }




  void LCD_DATA (uint8_t data)
  {
	  HAL_GPIO_WritePin(GPIOA, LCD_CE_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA, LCD_DC_Pin, GPIO_PIN_SET);
	  HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOA, LCD_CE_Pin, GPIO_PIN_SET);

  }

  void LCD_CMD (uint8_t cmd)
  {
	  HAL_GPIO_WritePin(GPIOA, LCD_CE_Pin | LCD_DC_Pin, GPIO_PIN_RESET);
	  HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
	  HAL_GPIO_WritePin(GPIOA, LCD_CE_Pin | LCD_DC_Pin, GPIO_PIN_SET);

  }

  void LCD_ON (void)
  {
	  HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_RESET);  // LCD RST ACTIVE
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin, GPIO_PIN_SET);    // LCD ACTIVE
	  HAL_GPIO_WritePin(GPIOA, LCD_LIGHT_Pin, GPIO_PIN_SET);   // Light active

	  LCD_CMD(0x21);  // enable extended instructions
	  LCD_CMD(0xc8);  // contrast
	  LCD_CMD(0x06);  // set Temperature Coefficient
	  LCD_CMD(0x11);     // BIAS
	  LCD_CMD(0x20); // enable standard instructions
	  LCD_CMD(0x08);  // display blank
	  LCD_CMD(0x09); // all display segments on
	  LCD_CMD(0x0d); // inverse video
	  HAL_Delay(999);


	  HAL_Delay(100);
	  LCD_CMD(0x0c);   // normal mode
	  HAL_Delay(100);

  }

  void LCD_CLEAR (void)
  {
	  LCD_CMD(0x20); // enable standard instructions
	  uint8_t position_y = 0x40;
	  uint8_t y = 0;



	  LCD_CMD(0x80);
	  LCD_CMD(position_y);

	  while(y < 8)
	  {
		  for (uint8_t a = 0; a < 84; a++ )
		  	  {
		  		  LCD_DATA(0x00);
		  	  }
		  LCD_CMD(0x80);
		  y ++;
		  position_y ++;
		  LCD_CMD(position_y);

	  }
  }

  void LCD_SEND_PICTURE (uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t picture[])        // location x, location y, width pixels, height pixels, picture
  {
	  uint8_t address_x;
	  uint8_t address_y;
	  uint8_t width_1;
	  uint8_t end_width;
	  uint16_t element;
	  uint16_t end;      //o

	  element=0;

	  end_width=x+width;
	  height=height/8;

	  	width_1=x;


	  	koniec=height*width;


	  	address_x=0x80;             // stała wartoć którš LCD interpretuje jako adres
	  	address_y=0x40;             // jak wyżej
	  	address_x=address_x+x;
	  	address_y=address_y+y;
	  	LCD_CMD(0x20); // enable standard instructions
	  	LCD_CMD(address_x);
	  	LCD_CMD(address_y);


	  while(1)
	  {
		  LCD_DATA(picture[element]);
		  element++;
		  width_1++;
		  if(element==end) break;
		  if(width_1==end_width) {width_1=x; address_y++; LCD_CMD(address_x); LCD_CMD(address_y); };
	  }
  }

  void LCD_SEND_DIGIT (uint8_t x, uint8_t y, int digit) // location x, location y, data
  {
	  uint8_t address_x;
	  uint8_t address_y;
	  unsigned char argument_number;
	     // rest of the division
	  uint16_t rest;
	  uint8_t a;
	  a = 0;




	  address_x=0x80;             // static value
	  address_y=0x40;             // static value
	  address_x=address_x+x;
	  address_y=address_y+y;
	  LCD_CMD(0x20); // enable standard instructions
	  LCD_CMD(address_x);   // location of cursor
	  LCD_CMD(address_y);   //

	  rest = digit % 10;
	  digit = digit - rest;
	  while(digit > 9) {digit = digit / 10;}
	  argument_number = 0;
	  for(a = 6; a>0; a--){LCD_DATA(digits[digit][argument_number]); argument_number ++;}
	  argument_number = 0;
	  for(a = 6; a>0; a--){LCD_DATA(digits[rest][argument_number]); argument_number ++;}
  }

  void LCD_SEND_TEXT (uint8_t x, uint8_t y, const char* text) // location x, location y, data
  {
	  uint8_t address_x;
	  uint8_t address_y;
	  char a, c;
	  unsigned char argument_number;




	  address_x=0x80;             // stable value
	  address_y=0x40;             // stable value
	  address_x=address_x+x;
	  address_y=address_y+y;
	  LCD_CMD(0x20); // enable standard instructions
	  LCD_CMD(address_x);   // location of cursor
	  LCD_CMD(address_y);   //

	  c = *text;

	  while(c)
	  {
		  argument_number = 0;
		  for(a = 5; a > 0; a--) {LCD_DATA(font_ASCII[*text - 32][argument_number]); argument_number ++; }
		  LCD_DATA(0x00);
		  text ++;
		  c = *text;
	  }

  }

  void send_time_data (void)
  {
	  uint8_t week_day;


	  RTC_TimeTypeDef time;     // variable of time
	  RTC_DateTypeDef date;     // variable of date
	  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
	  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date

	  week_day = date.WeekDay;



	  LCD_SEND_TEXT(0, 0, "*** Today ***");


	  switch (week_day)
	  {
	  case 1: LCD_SEND_TEXT(0, 1, "Monday :,(   ");
	  break;
	  case 2: LCD_SEND_TEXT(0, 1, "Tuesday :-[  ");
	  break;
	  case 3: LCD_SEND_TEXT(0, 1, "Wednesday :-(");
	  break;
	  case 4: LCD_SEND_TEXT(0, 1, "Thursday :-| ");
	  break;
	  case 5: LCD_SEND_TEXT(0, 1, "Friday :-)   ");
	  break;
	  case 6: LCD_SEND_TEXT(0, 1, "Saturday :-D ");
	  break;
	  case 7: LCD_SEND_TEXT(0, 1, "Sunday :-)   ");
	  break;
	  }

	  LCD_SEND_DIGIT(0, 2, time.Hours);
	  LCD_DATA(0x00);
	  LCD_DATA(0x44);
	  LCD_SEND_DIGIT(15, 2, time.Minutes);
	  LCD_DATA(0x00);
	  LCD_DATA(0x44);
	  LCD_SEND_DIGIT(29, 2, time.Seconds);

	  LCD_SEND_DIGIT(0, 3, date.Date);
	  LCD_DATA(0x00);
	  LCD_DATA(0x80);

	  LCD_SEND_DIGIT(15, 3, date.Month);
	  LCD_DATA(0x00);
	  LCD_DATA(0x80);

	  LCD_SEND_DIGIT(30, 3, 20);
	  LCD_SEND_DIGIT(42, 3, date.Year);







  }




  int8_t ENCODER_ROTATION_DIRECTION (void)
   {

 	  int tim1_value = htim1.Instance->CNT;
 	  static int last_value;
 	  signed char direction;
 	  tim1_value = tim1_value / 4;
 	  direction = last_value - tim1_value;

 	  last_value = tim1_value;



 	  return direction;

   }

  void SET_HOUR_DATE (void)
  {
	  uint8_t menu;
	  int8_t a;
	  int8_t week_day;

	  RTC_TimeTypeDef time;     // value of time
	  RTC_DateTypeDef date;     // value of date

	  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
	  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date
	  menu = 1;
	  LCD_CLEAR();

	  while(1)
	  {
		  while(menu == 1)   // set hour
		  {

			  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
			  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date

		 	  RTC_TimeTypeDef sTime = {0};   // set time


			  a = time.Hours;
			  a = a + ENCODER_ROTATION_DIRECTION();
			  if(a > 23) a = 23;
			  if(a < 0) a = 0;

			  sTime.Hours = a;
			  sTime.Minutes = time.Minutes;
			  sTime.Seconds = time.Seconds;
			  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

			  LCD_SEND_TEXT(15, 0, "SET TIME:");
			  LCD_SEND_TEXT(0, 2, "Hrs:");
			  LCD_SEND_TEXT(0, 3, "Mins:");
			  LCD_SEND_TEXT(0, 4, "Secs:");

			  LCD_SEND_TEXT(39, 2, ">");
			  LCD_SEND_TEXT(59, 2, "<");
			  LCD_SEND_TEXT(39, 3, " ");
			  LCD_SEND_TEXT(59, 3, " ");
			  LCD_SEND_TEXT(39, 4, " ");
			  LCD_SEND_TEXT(59, 4, " ");

			  LCD_SEND_DIGIT(45, 2, a);
			  LCD_SEND_DIGIT(45, 3, time.Minutes);
			  LCD_SEND_DIGIT(45, 4, time.Seconds);

			  if(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET)	{menu = 2;}

		  }

		  while(menu == 2)   // set minutes
		  {
			  while(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET);
			  while(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET);
			  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
			  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date

		 	  RTC_TimeTypeDef sTime = {0};   // set time


			  a = time.Minutes;
			  a = a + ENCODER_ROTATION_DIRECTION();
			  if(a > 59) a = 59;
			  if(a < 0) a = 0;

			  sTime.Hours = time.Hours;
			  sTime.Minutes = a;
			  sTime.Seconds = time.Seconds;
			  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

			  LCD_SEND_TEXT(15, 0, "SET TIME:");
			  LCD_SEND_TEXT(0, 2, "Hrs:");
			  LCD_SEND_TEXT(0, 3, "Mins:");
			  LCD_SEND_TEXT(0, 4, "Secs:");

			  LCD_SEND_TEXT(39, 2, " ");
			  LCD_SEND_TEXT(59, 2, " ");
			  LCD_SEND_TEXT(39, 3, ">");
			  LCD_SEND_TEXT(59, 3, "<");
			  LCD_SEND_TEXT(39, 4, " ");
			  LCD_SEND_TEXT(59, 4, " ");

			  LCD_SEND_DIGIT(45, 2, time.Hours);
			  LCD_SEND_DIGIT(45, 3, a);
			  LCD_SEND_DIGIT(45, 4, time.Seconds);

			  if(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET)	{menu = 1;}
			  if(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET)	{menu = 3;}

		  }
		  while(menu == 3)   // set seconds
		  {
			  while(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET);
			  while(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET);
			  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
			  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date

		 	  RTC_TimeTypeDef sTime = {0};   // set time


			  a = time.Seconds;
			  a = a + ENCODER_ROTATION_DIRECTION();
			  if(a > 59) a = 59;
			  if(a < 0) a = 0;

			  sTime.Hours = time.Hours;
			  sTime.Minutes = time.Minutes;
			  sTime.Seconds = a;
			  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

			  LCD_SEND_TEXT(15, 0, "SET TIME:");
			  LCD_SEND_TEXT(0, 2, "Hrs:");
			  LCD_SEND_TEXT(0, 3, "Mins:");
			  LCD_SEND_TEXT(0, 4, "Secs:");

			  LCD_SEND_TEXT(39, 2, " ");
			  LCD_SEND_TEXT(59, 2, " ");
			  LCD_SEND_TEXT(39, 3, " ");
			  LCD_SEND_TEXT(59, 3, " ");
			  LCD_SEND_TEXT(39, 4, ">");
			  LCD_SEND_TEXT(59, 4, "<");

			  LCD_SEND_DIGIT(45, 2, time.Hours);
			  LCD_SEND_DIGIT(45, 3, time.Minutes);
			  LCD_SEND_DIGIT(45, 4, a);

			  if(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET)	{menu = 2;}
			  if(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET)	{menu = 4; LCD_CLEAR();}

		  }
		  while(menu == 4)   // set year
		  {
			  while(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET);
			  while(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET);
			  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
			  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date


		 	  RTC_DateTypeDef sDate = {0};   // set date
		 	  week_day = date.WeekDay;


			  a = date.Year;
			  a = a + ENCODER_ROTATION_DIRECTION();
			  if(a > 99) a = 99;
			  if(a < 0) a = 0;

			  sDate.Year = a;
			  sDate.Month = date.Month;
			  sDate.Date = date.Date;
			  sDate.WeekDay = date.WeekDay;
			  HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

			  LCD_SEND_TEXT(15, 0, "SET DATE:");
			  LCD_SEND_TEXT(0, 2, "Year:");
			  LCD_SEND_TEXT(0, 3, "Month:");
			  LCD_SEND_TEXT(0, 4, "Day:");

			  LCD_SEND_TEXT(40, 2, ">");
			  LCD_SEND_TEXT(72, 2, "<");
			  LCD_SEND_TEXT(39, 3, " ");
			  LCD_SEND_TEXT(59, 3, " ");
			  LCD_SEND_TEXT(39, 4, " ");
			  LCD_SEND_TEXT(59, 4, " ");
			  LCD_SEND_TEXT(0, 5, " ");
			  LCD_SEND_TEXT(0, 55, " ");

			  LCD_SEND_DIGIT(45, 2, 20);
			  LCD_SEND_DIGIT(57, 2, a);
			  LCD_SEND_DIGIT(45, 3, date.Month);
			  LCD_SEND_DIGIT(45, 4, date.Date);
			  switch (week_day)
			  {
			  case 1: LCD_SEND_TEXT(15, 5, "Monday   ");
			  break;
			  case 2: LCD_SEND_TEXT(15, 5, "Tuesday  ");
			  break;
			  case 3: LCD_SEND_TEXT(15, 5, "Wednesday");
			  break;
			  case 4: LCD_SEND_TEXT(15, 5, "Thursday ");
			  break;
			  case 5: LCD_SEND_TEXT(15, 5, "Friday   ");
			  break;
			  case 6: LCD_SEND_TEXT(15, 5, "Saturday ");
			  break;
			  case 7: LCD_SEND_TEXT(15, 5, "Sunday   ");
			  break;
			  }

			  if(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET)	{menu = 3; LCD_CLEAR();}
			  if(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET)	{menu = 5;}
		  }
		  while(menu == 5)   // set month
		  {
			  while(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET);
			  while(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET);
			  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
			  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date


		 	  RTC_DateTypeDef sDate = {0};   // set date
		   	  week_day = date.WeekDay;

			  a = date.Month;
			  a = a + ENCODER_ROTATION_DIRECTION();
			  if(a > 12) a = 12;
			  if(a < 0) a = 0;

			  sDate.Year = date.Year;
			  sDate.Month = a;
			  sDate.Date = date.Date;
			  sDate.WeekDay = date.WeekDay;
			  HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

			  LCD_SEND_TEXT(15, 0, "SET DATE:");
			  LCD_SEND_TEXT(0, 2, "Year:");
			  LCD_SEND_TEXT(0, 3, "Month:");
			  LCD_SEND_TEXT(0, 4, "Day:");

			  LCD_SEND_TEXT(40, 2, " ");
			  LCD_SEND_TEXT(71, 2, " ");
			  LCD_SEND_TEXT(39, 3, ">");
			  LCD_SEND_TEXT(59, 3, "<");
			  LCD_SEND_TEXT(39, 4, " ");
			  LCD_SEND_TEXT(59, 4, " ");
			  LCD_SEND_TEXT(0, 5, " ");
			  LCD_SEND_TEXT(0, 55, " ");

			  LCD_SEND_DIGIT(45, 2, 20);
			  LCD_SEND_DIGIT(57, 2, date.Year);
			  LCD_SEND_DIGIT(45, 3, a);
			  LCD_SEND_DIGIT(45, 4, date.Date);
			  switch (week_day)
			  {
			  case 1: LCD_SEND_TEXT(15, 5, "Monday   ");
			  break;
			  case 2: LCD_SEND_TEXT(15, 5, "Tuesday  ");
			  break;
			  case 3: LCD_SEND_TEXT(15, 5, "Wednesday");
			  break;
			  case 4: LCD_SEND_TEXT(15, 5, "Thursday ");
			  break;
			  case 5: LCD_SEND_TEXT(15, 5, "Friday   ");
			  break;
			  case 6: LCD_SEND_TEXT(15, 5, "Saturday ");
			  break;
			  case 7: LCD_SEND_TEXT(15, 5, "Sunday   ");
			  break;
			  }
			  if(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET)	{menu = 4;}
			  if(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET)	{menu = 6;}
		  }
		  while(menu == 6)   // set day of month
		  {
			  uint8_t b;
			  while(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET);
			  while(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET);
			  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
			  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date

			  switch (date.Month)  // set the max number of days of a given month
			  {
			  case 1: b = 31;
			  break;
			  case 2: b = 28;
			  break;
			  case 3: b = 31;
			  break;
			  case 4: b = 30;
			  break;
			  case 5: b = 31;
			  break;
			  case 6: b = 30;
			  break;
			  case 7: b = 31;
			  break;
			  case 8: b = 31;
			  break;
			  case 9: b = 30;
			  break;
			  case 10: b = 31;
			  break;
			  case 11: b = 30;
			  break;
			  case 12: b = 31;
			  }

				  if(date.Month == 2)    // check if the year is leap
				  {
					  a = date.Year % 4;
					  if (a == 0) b = 29;
				  }



		 	  RTC_DateTypeDef sDate = {0};   // set date

		 	  week_day = date.WeekDay;

			  a = date.Date;
			  a = a + ENCODER_ROTATION_DIRECTION();
			  if(a > b) a = b;
			  if(a < 1) a = 1;

			  sDate.Year = date.Year;
			  sDate.Month = date.Month;
			  sDate.Date = a;
			  sDate.WeekDay = date.WeekDay;
			  HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

			  LCD_SEND_TEXT(15, 0, "SET DATE:");
			  LCD_SEND_TEXT(0, 2, "Year:");
			  LCD_SEND_TEXT(0, 3, "Month:");
			  LCD_SEND_TEXT(0, 4, "Day:");

			  LCD_SEND_TEXT(40, 2, " ");
			  LCD_SEND_TEXT(71, 2, " ");
			  LCD_SEND_TEXT(39, 3, " ");
			  LCD_SEND_TEXT(59, 3, " ");
			  LCD_SEND_TEXT(39, 4, ">");
			  LCD_SEND_TEXT(59, 4, "<");
			  LCD_SEND_TEXT(0, 5, " ");
			  LCD_SEND_TEXT(80, 5, " ");

			  LCD_SEND_DIGIT(45, 2, 20);
			  LCD_SEND_DIGIT(57, 2, date.Year);
			  LCD_SEND_DIGIT(45, 3, date.Month);
			  LCD_SEND_DIGIT(45, 4, a);
			  switch (week_day)
			  {
			  case 1: LCD_SEND_TEXT(15, 5, "Monday   ");
			  break;
			  case 2: LCD_SEND_TEXT(15, 5, "Tuesday  ");
			  break;
			  case 3: LCD_SEND_TEXT(15, 5, "Wednesday");
			  break;
			  case 4: LCD_SEND_TEXT(15, 5, "Thursday ");
			  break;
			  case 5: LCD_SEND_TEXT(15, 5, "Friday   ");
			  break;
			  case 6: LCD_SEND_TEXT(15, 5, "Saturday ");
			  break;
			  case 7: LCD_SEND_TEXT(15, 5, "Sunday   ");
			  break;
			  }
			  if(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET)	{menu = 5;}
			  if(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET)	{menu = 7;}
		  }
		  while(menu == 7)   // set day of week
		  {


			  while(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET);
			  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
			  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date


		 	  RTC_DateTypeDef sDate = {0};   // set date

			  week_day = date.WeekDay;
			  week_day = week_day + ENCODER_ROTATION_DIRECTION();
			  if(week_day > 7) week_day = 7;
			  if(week_day < 1) week_day = 1;

			  sDate.Year = date.Year;
			  sDate.Month = date.Month;
			  sDate.Date = date.Date;
			  sDate.WeekDay = week_day;
			  HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

			  LCD_SEND_TEXT(15, 0, "SET DATE:");
			  LCD_SEND_TEXT(0, 2, "Year:");
			  LCD_SEND_TEXT(0, 3, "Month:");
			  LCD_SEND_TEXT(0, 4, "Day:");

			  LCD_SEND_TEXT(40, 2, " ");
			  LCD_SEND_TEXT(71, 2, " ");
			  LCD_SEND_TEXT(39, 3, " ");
			  LCD_SEND_TEXT(59, 3, " ");
			  LCD_SEND_TEXT(39, 4, " ");
			  LCD_SEND_TEXT(59, 4, " ");
			  LCD_SEND_TEXT(0, 5, ">");
			  LCD_SEND_TEXT(80, 5, "<");

			  LCD_SEND_DIGIT(45, 2, 20);
			  LCD_SEND_DIGIT(57, 2, date.Year);
			  LCD_SEND_DIGIT(45, 3, date.Month);
			  LCD_SEND_DIGIT(45, 4, date.Date);
			  switch (week_day)
			  {
			  case 1: LCD_SEND_TEXT(15, 5, " Monday  ");
			  break;
			  case 2: LCD_SEND_TEXT(15, 5, " Tuesday ");
			  break;
			  case 3: LCD_SEND_TEXT(15, 5, "Wednesday");
			  break;
			  case 4: LCD_SEND_TEXT(15, 5, "Thursday ");
			  break;
			  case 5: LCD_SEND_TEXT(15, 5, " Friday  ");
			  break;
			  case 6: LCD_SEND_TEXT(15, 5, "Saturday ");
			  break;
			  case 7: LCD_SEND_TEXT(15, 5, " Sunday  ");
			  break;
			  }
			  if(HAL_GPIO_ReadPin(GPIOF, ESCAPE_Pin) == GPIO_PIN_RESET)	{menu = 6;}
			  if(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET)	{menu = 8; LCD_CLEAR();}
		  }
		  if(menu == 8) {break;}
	  }
  }

  void SET_RELAY_PROGRAM (void)
  {

		  static int8_t number_program = 0;
		  int8_t activity_program = 0;
		  int8_t relay = 0;
		  int8_t day_of_week = 0;
		  int8_t hour = 0;
		  int8_t minute = 0;
		  uint8_t menu = 0;
		  while(menu < 7)
		  {
			  	  	  	  	  	  	  	  	  while(ENTER_RST)
			  		  	  					  {
			  		  	  						  HAL_Delay(100);
			  		  	  						  LCD_SEND_TEXT(12, 0, "Program:");
			  		  	  						  LCD_SEND_DIGIT(57, 0, number_program);                                                 // number program
			  		  	  						  if(program_data[number_program][0] == 0) LCD_SEND_TEXT(12, 1, "Disable");              // program status
			  		  	  						  if(program_data[number_program][0] == 1) LCD_SEND_TEXT(12, 1, "Enable ");               // program status
			  		  	  						  LCD_SEND_TEXT(12, 2, "Relay:");
			  		  	  						  if(program_data[number_program][1] == 0) LCD_SEND_TEXT(48, 2, "OFF");                  // relay status
			  		  	  						  if(program_data[number_program][1] == 0) LCD_SEND_TEXT(48, 2, "ON ");                  // relay status
			  		  	  						  LCD_SEND_TEXT(12, 3, "At:");
			  		  	  						  if(program_data[number_program][2] == 0) program_data[number_program][2] = 1;
			  		  	  						  day_of_week = program_data[number_program][2];

			  		  	  						  switch (day_of_week)                                                                 //  day of week
			  		  	  						  {
			  		  	  						  case 1: LCD_SEND_TEXT(12, 4, "Monday   ");
			  		  	  						  break;
			  		  	  						  case 2: LCD_SEND_TEXT(12, 4, "Tuesday  ");
			  		  	  						  break;
			  		  	  						  case 3: LCD_SEND_TEXT(12, 4, "Wednesday");
			  		  	  						  break;
			  		  	  						  case 4: LCD_SEND_TEXT(12, 4, "Thursday ");
			  		  	  						  break;
			  		  	  						  case 5: LCD_SEND_TEXT(12, 4, "Friday   ");
			  		  	  						  break;
			  		  	  						  case 6: LCD_SEND_TEXT(12, 4, "Saturday ");
			  		  	  						  break;
			  		  	  						  case 7: LCD_SEND_TEXT(12, 4, "Sunday   ");
			  		  	  						  break;
			  		  	  						  }
			  		  	  						  LCD_SEND_DIGIT(12, 5, program_data[number_program][3]);                             // hour
			  		  	  						  LCD_SEND_TEXT(25, 5, ":");
			  		  	  						  LCD_SEND_DIGIT(29, 5, program_data[number_program][4]);                             // minute
			  		  	  					  }
			  	  	  	  	  	  	  	  	  while(ESCAPE_RST) {HAL_Delay(99);}
			  		  	  					  while(menu == 0)                            // ENTER SET
			  		  	  					  {

			  		  	  						  number_program = number_program + ENCODER_ROTATION_DIRECTION();
			  		  	  						  if(number_program < 0) number_program = 13;
			  		  	  						  if(number_program > 13) number_program = 0;
			  		  	  						  HAL_Delay(10);
			  		  	  						  LCD_SEND_TEXT(0, 0, "  Program:");
			  		  	  						  LCD_SEND_TEXT(0, 0, "->");
			  		  	  						  LCD_SEND_DIGIT(59, 0, number_program);                                                 // number program
			  		  	  						  if(program_data[number_program][0] == 0) LCD_SEND_TEXT(12, 1, "Disable");              // program status
			  		  	  						  if(program_data[number_program][0] == 1) LCD_SEND_TEXT(12, 1, "Enable ");               // program status
			  		  	  						  if(program_data[number_program][1] == 0) LCD_SEND_TEXT(12, 2, "Relay: OFF");                  // relay status
			  		  	  						  if(program_data[number_program][1] == 1) LCD_SEND_TEXT(12, 2, "Relay: ON ");                  // relay status
			  		  	  						  LCD_SEND_TEXT(12, 3, "At:");
			  		  	  						  if(program_data[number_program][2] == 0) program_data[number_program][2] = 1;
			  		  	  						  day_of_week = program_data[number_program][2];

			  		  	  						  switch (day_of_week)                                                                 //  day of week
			  		  	  						  {
			  		  	  						  case 1: LCD_SEND_TEXT(12, 4, "Monday   ");
			  		  	  						  break;
			  		  	  						  case 2: LCD_SEND_TEXT(12, 4, "Tuesday  ");
			  		  	  						  break;
			  		  	  						  case 3: LCD_SEND_TEXT(12, 4, "Wednesday");
			  		  	  						  break;
			  		  	  						  case 4: LCD_SEND_TEXT(12, 4, "Thursday ");
			  		  	  						  break;
			  		  	  						  case 5: LCD_SEND_TEXT(12, 4, "Friday   ");
			  		  	  						  break;
			  		  	  						  case 6: LCD_SEND_TEXT(12, 4, "Saturday ");
			  		  	  						  break;
			  		  	  						  case 7: LCD_SEND_TEXT(12, 4, "Sunday   ");
			  		  	  						  break;
			  		  	  						  }
			  		  	  						  LCD_SEND_DIGIT(12, 5, program_data[number_program][3]);                             // hour
			  		  	  						  LCD_SEND_TEXT(25, 5, ":");
			  		  	  						  LCD_SEND_DIGIT(29, 5, program_data[number_program][4]);                             // minute
			  		  	  						  if(ENTER_RST){HAL_Delay(100); menu = 1;}
			  		  	  						  if(ESCAPE_RST) menu = 7;
			  		  	  					  }

			  		  	  					  while(ENTER_RST)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  LCD_SEND_TEXT(0, 0, "  Program:");
			  		  	  						  LCD_SEND_TEXT(0, 1, "->");
			  		  	  					  }

			  		  	  					  while(menu == 1)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  LCD_SEND_TEXT(0, 0, "  Program:");
			  		  	  						  LCD_SEND_TEXT(0, 1, "->");
			  		  	  						  activity_program = program_data[number_program][0];
			  		  	  						  activity_program = activity_program + ENCODER_ROTATION_DIRECTION();
			  		  	  						  if(activity_program < 0) activity_program = 1;
			  		  	  						  if(activity_program > 1) activity_program = 0;
			  		  	  						  if(activity_program == 0) LCD_SEND_TEXT(12, 1, "Disable");
			  		  	  						  if(activity_program == 1) LCD_SEND_TEXT(12, 1, "Enable ");
			  		  	  						  program_data[number_program][0] = activity_program;
			  		  	  						  if(ENTER_RST){HAL_Delay(100); menu = 2;}
			  		  	  						  if(ESCAPE_RST){menu = 0; LCD_SEND_TEXT(0, 1, "  "); LCD_SEND_TEXT(0, 0, "->");}
			  		  	  					  }
			  		  	  					  while(ENTER_RST)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  if(program_data[number_program][0] == 0) LCD_SEND_TEXT(0, 1, "  Disable");
			  		  	  						  else {LCD_SEND_TEXT(0, 1, "  Enable ");};
			  		  	  						  LCD_SEND_TEXT(0, 2, "->");
			  		  	  					  }

			  		  	  					  while(menu == 2)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  if(program_data[number_program][0] == 0) LCD_SEND_TEXT(0, 1, "  Disable");
			  		  	  						  else {LCD_SEND_TEXT(0, 1, "  Enable ");};
			  		  	  						  LCD_SEND_TEXT(0, 2, "->");
			  		  	  						  relay = program_data[number_program][1];
			  		  	  						  relay = relay + ENCODER_ROTATION_DIRECTION();
			  		  	  						  if(relay < 0) relay = 1;
			  		  	  						  if(relay > 1) relay = 0;
			  		  	  						  if(relay == 0) LCD_SEND_TEXT(12, 2, "Relay: OFF");
			  		  	  						  if(relay == 1) LCD_SEND_TEXT(12, 2, "Relay: ON ");
			  		  	  						  program_data[number_program][1] = relay;
			  		  	  						  if(ENTER_RST) {HAL_Delay(100); menu = 3;}
			  		  	  						  if(ESCAPE_RST) {menu = 1; LCD_SEND_TEXT(0, 2, "  "); LCD_SEND_TEXT(0, 1, "->");}
			  		  	  					  }
			  		  	  					  while(ENTER_RST)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  if(program_data[number_program][1] == 0) LCD_SEND_TEXT(0, 2, "  Relay: OFF");
			  		  	  						  else {LCD_SEND_TEXT(0, 2, "  Relay: ON ");};
			  		  	  						  LCD_SEND_TEXT(0, 4, "->");
			  		  	  					  }

			  		  	  					  while(menu == 3)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  if(program_data[number_program][1] == 0) LCD_SEND_TEXT(0, 2, "  Relay: OFF");
			  		  	  						  else {LCD_SEND_TEXT(0, 2, "  Relay: ON ");};
			  		  	  						  LCD_SEND_TEXT(0, 4, "->");
			  		  	  						  day_of_week = program_data[number_program][2];
			  		  	  						  day_of_week = day_of_week + ENCODER_ROTATION_DIRECTION();
			  		  	  						  if(day_of_week < 1) day_of_week = 7;
			  		  	  						  if(day_of_week > 7) day_of_week = 1;
			  		  	  						  switch (day_of_week)                                                                 //  day of week
			  		  	  						  {
			  		  	  						  case 1: LCD_SEND_TEXT(12, 4, "Monday   ");
			  		  	  						  break;
			  		  	  						  case 2: LCD_SEND_TEXT(12, 4, "Tuesday  ");
			  		  	  						  break;
			  		  	  						  case 3: LCD_SEND_TEXT(12, 4, "Wednesday");
			  		  	  						  break;
			  		  	  						  case 4: LCD_SEND_TEXT(12, 4, "Thursday ");
			  		  	  						  break;
			  		  	  						  case 5: LCD_SEND_TEXT(12, 4, "Friday   ");
			  		  	  						  break;
			  		  	  						  case 6: LCD_SEND_TEXT(12, 4, "Saturday ");
			  		  	  						  break;
			  		  	  						  case 7: LCD_SEND_TEXT(12, 4, "Sunday   ");
			  		  	  						  break;
			  		  	  						  }
			  		  	  						  program_data[number_program][2] = day_of_week;
			  		  	  						  if(ENTER_RST) {HAL_Delay(100); menu = 4;}
			  		  	  						  if(ESCAPE_RST) {menu = 2; LCD_SEND_TEXT(0, 4, "  "); LCD_SEND_TEXT(0, 2, "->");}
			  		  	  					  }
			  		  	  					  while(ENTER_RST)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  switch (day_of_week)                                                                 //  day of week
			  		  	  						  {
			  		  	  						  case 1: LCD_SEND_TEXT(0, 4, "  Monday   ");
			  		  	  						  break;
			  		  	  						  case 2: LCD_SEND_TEXT(0, 4, "  Tuesday  ");
			  		  	  						  break;
			  		  	  						  case 3: LCD_SEND_TEXT(0, 4, "  Wednesday");
			  		  	  						  break;
			  		  	  						  case 4: LCD_SEND_TEXT(0, 4, "  Thursday ");
			  		  	  						  break;
			  		  	  						  case 5: LCD_SEND_TEXT(0, 4, "  Friday   ");
			  		  	  						  break;
			  		  	  						  case 6: LCD_SEND_TEXT(0, 4, "  Saturday ");
			  		  	  						  break;
			  		  	  						  case 7: LCD_SEND_TEXT(0, 4, "  Sunday   ");
			  		  	  						  break;
			  		  	  						  }
			  		  	  						  LCD_SEND_TEXT(0, 5, "->");
			  		  	  					  }

			  		  	  					  while(menu == 4)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  switch (day_of_week)                                                                 //  day of week
			  		  	  						  {
			  		  	  						  case 1: LCD_SEND_TEXT(0, 4, "  Monday   ");
			  		  	  						  break;
			  		  	  						  case 2: LCD_SEND_TEXT(0, 4, "  Tuesday  ");
			  		  	  						  break;
			  		  	  						  case 3: LCD_SEND_TEXT(0, 4, "  Wednesday");
			  		  	  						  break;
			  		  	  						  case 4: LCD_SEND_TEXT(0, 4, "  Thursday ");
			  		  	  						  break;
			  		  	  						  case 5: LCD_SEND_TEXT(0, 4, "  Friday   ");
			  		  	  						  break;
			  		  	  						  case 6: LCD_SEND_TEXT(0, 4, "  Saturday ");
			  		  	  						  break;
			  		  	  						  case 7: LCD_SEND_TEXT(0, 4, "  Sunday   ");
			  		  	  						  break;
			  		  	  						  }
			  		  	  						  LCD_SEND_TEXT(0, 5, "->");
			  		  	  						  hour = program_data[number_program][3];
			  		  	  						  hour = hour + ENCODER_ROTATION_DIRECTION();
			  		  	  						  if(hour < 0) hour = 23;                            // min value
			  		  	  						  if(hour > 23) hour = 0;            // max value
			  		  	  						  LCD_SEND_DIGIT(12, 5, program_data[number_program][3]);                             // hour
			  		  	  						  program_data[number_program][3] = hour;
			  		  	  						  if(ENTER_RST) {HAL_Delay(100); menu = 5;}
			  		  	  						  if(ESCAPE_RST) {menu = 3; LCD_SEND_TEXT(0, 5, "  "); LCD_SEND_TEXT(0, 4, "->");}
			  		  	  					  }
			  		  	  					  while(ENTER_RST)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  LCD_SEND_TEXT(0, 5, "  ");
			  		  	  						  LCD_SEND_TEXT(43, 5, "<-");
			  		  	  					  }

			  		  	  					  while(menu == 5)
			  		  	  					  {
			  		  	  						  HAL_Delay(10);
			  		  	  						  LCD_SEND_TEXT(0, 5, "  ");
			  		  	  						  LCD_SEND_TEXT(43, 5, "<-");
			  		  	  						  minute = program_data[number_program][4];
			  		  	  						  minute = minute + ENCODER_ROTATION_DIRECTION();
			  		  	  						  if(minute < 0) minute = 59;                            // min value
			  		  	  						  if(minute > 59) minute = 0;            // max value
			  		  	  						  LCD_SEND_DIGIT(29, 5, program_data[number_program][4]);                             // minute
			  		  	  						  program_data[number_program][4] = minute;
			  		  	  						  if(ENTER_RST) {HAL_Delay(100); LCD_CLEAR(); menu = 6;}
			  		  	  						  if(ESCAPE_RST) {menu = 4; LCD_SEND_TEXT(43, 5, "  "); LCD_SEND_TEXT(0, 5, "->");}

			  		  	  					 }
			  		  	  					  while(menu == 6)
			  		  	  					  {
			  		  	  					  LCD_CLEAR();
			  		  	  					  LCD_SEND_TEXT(0, 1, "Saving data...");
			  		  	  					  WRITE_EEPROM_PROGRAM_DATA();
			  		  	  					  LCD_CLEAR();
			  		  	  					  LCD_SEND_TEXT(12, 1, "Data saved");
			  		  	  					  HAL_Delay(999);
			  		  	  					  LCD_CLEAR();
			  		  	  					  menu = 0;
			  		  	  					  }

		  }
		  while(menu == 7)
		  {
			  READ_EEPROM_PROGRAM_DATA();
			  LCD_CLEAR();
			  break;

		  }

  }

  void SET_LCD (void)
  {
	  static int8_t contrast_value = 72;
	  static int8_t BIAS_value = 2;
	  static int8_t temperature_coefficient = 1;
	  uint8_t a;
	  uint8_t b;
	  uint8_t menu = 0;

	  while(menu < 3)
	  {
		  while(ENTER_RST)
		  {
			  LCD_SEND_TEXT(18, 0, "Contrast");
			  HAL_Delay(99);
		  }

		  while(menu == 0)
		  {
			  LCD_SEND_TEXT(18, 0, "Contrast");
			  contrast_value = contrast_value + ENCODER_ROTATION_DIRECTION();
			  if(contrast_value > 92) contrast_value = 92;
			  if(contrast_value < 60) contrast_value = 60;

			  a = 128 + contrast_value;   // 128 is minimum value of contrast

			  LCD_CMD(0x21);  // enable extended instructions
			  LCD_CMD(a);
			  LCD_CMD(0x20); // enable standard instructions
			  a = 0x80 + 10;
			  LCD_CMD(a);  // set address x
			  a = 0x40 + 2;
			  LCD_CMD(a);  // set address y
			  b = contrast_value - 60;
			  for(a = 0; a < 32; a++)
			  {
				  if(a < b) {LCD_DATA(255); LCD_DATA(255);}
				  else
					  {LCD_DATA(0); LCD_DATA(0);}
			  }
			  if(ENTER_RST) {LCD_CLEAR(); menu = 1;}
			  if(ESCAPE_RST) {LCD_CLEAR(); menu = 3;}
		  }
		  while(ENTER_RST)
		  {
			  LCD_SEND_TEXT(10, 0, "BIAS System");
			  HAL_Delay(99);
		  }
		  while(menu == 1)
		  {
			  LCD_SEND_TEXT(10, 0, "BIAS System");
			  BIAS_value = BIAS_value + ENCODER_ROTATION_DIRECTION();
			  if(BIAS_value < 0) BIAS_value = 0;
			  if(BIAS_value > 4) BIAS_value = 4;
			  LCD_CMD(0x21);  // enable extended instructions
			  a = 0x10;
			  a = a + BIAS_value;
			  LCD_CMD(a);
			  LCD_CMD(0x20); // enable standard instructions
			  a = 0x80 + 10;
			  LCD_CMD(a);  // set address x
			  a = 0x40 + 2;
			  LCD_CMD(a);  // set address y

			  for(a = 0; a < 4; a++)
			  {
				  if(a < BIAS_value)
				  {
					  for(b = 0; b < 14; b++)
					  {LCD_DATA(255);}
				  }
				  else
				  {
					  for(b = 0; b < 14; b++)
					  {LCD_DATA(0);}
				  }
			  }
			  if(ENTER_RST) {LCD_CLEAR(); menu = 2;}
			  if(ESCAPE_RST) {LCD_CLEAR(); menu = 0; LCD_SEND_TEXT(18, 0, "Contrast");}

		  }
		  while(ENTER_RST)
		  {
			  LCD_SEND_TEXT(11, 0, "Temperature");
			  LCD_SEND_TEXT(11, 1, "Coefficient");
			  HAL_Delay(99);
		  }
		  while(menu == 2)
		  {
			  LCD_SEND_TEXT(11, 0, "Temperature");
			  LCD_SEND_TEXT(11, 1, "Coefficient");
			  temperature_coefficient = temperature_coefficient + ENCODER_ROTATION_DIRECTION();
			  if(temperature_coefficient < 0) temperature_coefficient = 0;
			  if(temperature_coefficient > 3) temperature_coefficient = 3;
			  LCD_CMD(0x21);  // enable extended instructions
			  a = 0x04;
			  a = a + temperature_coefficient;
			  LCD_CMD(a);
			  LCD_CMD(0x20); // enable standard instructions
			  a = 0x80 + 10;
			  LCD_CMD(a);  // set address x
			  a = 0x40 + 3;
			  LCD_CMD(a);  // set address y

			  for(a = 0; a < 3; a++)
			  {
				  if(a < temperature_coefficient)
				  {
					  for(b = 0; b < 20; b++)
					  {LCD_DATA(255);}
				  }
				  else
				  {
					  for(b = 0; b < 20; b++)
					  {LCD_DATA(0);}
				  }
			  }
			  if(ENTER_RST) {LCD_CLEAR(); menu = 3;}
			  if(ESCAPE_RST) {LCD_CLEAR(); menu = 1; LCD_SEND_TEXT(10, 0, "BIAS System");}

		  }
		  while(ESCAPE_RST)
		  {
			  HAL_Delay(99);
		  }
	  }
	  while(menu == 3)
	  {
		  break;
	  }





  }

  void relay_control_program (void)
  {
	  static uint8_t program_number = 0;

	  uint8_t day_of_week = 0;
	  uint8_t hour = 0;
	  uint8_t minute = 0;
	  char a = 0;

	  RTC_TimeTypeDef time;     // variable of time
	  RTC_DateTypeDef date;     // variable of date
	  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);  // get time
	  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);  // get date

	  hour = time.Hours;
	  minute = time.Minutes;
	  day_of_week = date.WeekDay;





	  if(program_data[program_number][0] == 1)    // program enable? disable??
	  {
		 if(program_data[program_number][2] == day_of_week) a++;
		 if(program_data[program_number][3] == hour) a++;
		 if(program_data[program_number][4] == minute) a++;
		 if(a == 3)
		 {
			 if(program_data[program_number][1] == 1) {LED_ON; RELAY_ON;}
			 else{LED_OFF; RELAY_OFF};

		 }

	  }

	  program_number++;
	  if (program_number > 13) program_number = 0;



  }





  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */




	  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);   // encoder start
	  READ_EEPROM_PROGRAM_DATA();

	  LCD_ON();
	  LCD_CLEAR();
	  LCD_SEND_PICTURE(0, 0, 84, 48, intro);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LCD_LIGHT_Pin, GPIO_PIN_RESET);

	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);

	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_RESET);
	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);

	  HAL_GPIO_WritePin(GPIOC, RELAY_Pin, GPIO_PIN_RESET);
	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);

	  HAL_Delay(199);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_SET);
	  HAL_Delay(199);

	  HAL_Delay(99);
	  LCD_CLEAR();
	  HAL_Delay(99);
	  HAL_GPIO_WritePin(GPIOA, LED_Pin, GPIO_PIN_RESET);
	  SET_HOUR_DATE();


	  while (1)
	  {

		  static uint8_t main_menu;                             // HOME SCREEN



		  if(ENTER_SET) main_menu = 0;                          //
		  while(ENTER_RST)                                    //
		  {
			  send_time_data();
			  LCD_SEND_TEXT(59, 5, "MENU");
			  main_menu = 0;
		  }
		  while(main_menu == 0)    // HOME SCREEN
		  {
			  relay_control_program();
			  send_time_data();
			  LCD_SEND_TEXT(59, 5, "MENU");
			  if(HAL_GPIO_ReadPin(GPIOF, ENTER_Pin) == GPIO_PIN_RESET) {LCD_CLEAR(); main_menu = 1; break;}
		  }                                                                                                          // end home screen





		  while(main_menu == 1)
		  {
			  static int8_t menu;
			  while(ENTER_RST)                                // Main menu screen enter push
			  {
				  LCD_SEND_TEXT(29, 0, "MENU:");
				  LCD_SEND_TEXT(0, 5, "ESC");
				  LCD_SEND_TEXT(65, 5, "SEL");

				  switch (menu)
				  {
				  case 0: LCD_SEND_TEXT(8, 2, "Time & Date ");
				  break;
				  case 1: LCD_SEND_TEXT(8, 2, "Programming ");
				  break;
				  case 2: LCD_SEND_TEXT(8, 2, "LCD Settings");
				  break;
				  }
			 }
			  while(1)
			  {

				  menu = menu + ENCODER_ROTATION_DIRECTION();
				  if(menu < 0) menu = 2;
				  if(menu > 2) menu = 0;

				  LCD_SEND_TEXT(29, 0, "MENU:");
				  LCD_SEND_TEXT(0, 5, "ESC");
				  LCD_SEND_TEXT(65, 5, "SEL");

				  switch (menu)
				  {
				  case 0: LCD_SEND_TEXT(8, 2, "Time & Date ");
				  break;
				  case 1: LCD_SEND_TEXT(8, 2, "Programming ");
				  break;
				  case 2: LCD_SEND_TEXT(8, 2, "LCD Settings");
				  break;
				  }
				  if(ESCAPE_RST){LCD_CLEAR(); main_menu = 0; break;} // jump to HOME SCREEN
				  break;
			  }																								// end main menu screen



			  if(ENTER_RST)
			  {
				  LCD_CLEAR();

				  while(menu==0)                                                             // set hour and date
				  {
					  SET_HOUR_DATE();
					  HAL_Delay(100);
					  while(ENTER_RST);
					  break;
				  }
				  while(menu==1)
				  {
					  SET_RELAY_PROGRAM();
					  break;


				  }
				  while(menu==2)
				  {


					  while(1)
					  {

						  SET_LCD();
						  break;
					  }
					  break;


				  }
			  }
		  }

	  }


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_HIGH);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 1;
  sTime.Minutes = 10;
  sTime.Seconds = 23;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_MAY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 15;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 15;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_Pin|LCD_RST_Pin|LCD_CE_Pin|LCD_DC_Pin
                          |LCD_LIGHT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RELAY_Pin */
  GPIO_InitStruct.Pin = RELAY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RELAY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_Pin LCD_RST_Pin LCD_CE_Pin LCD_DC_Pin
                           LCD_LIGHT_Pin */
  GPIO_InitStruct.Pin = LED_Pin|LCD_RST_Pin|LCD_CE_Pin|LCD_DC_Pin
                          |LCD_LIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ESCAPE_Pin ENTER_Pin */
  GPIO_InitStruct.Pin = ESCAPE_Pin|ENTER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : RTC_STATUS_Pin RTC_CLK_PIN*/
  GPIO_InitStruct.Pin = RTC_STATUS_Pin|RTC_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
