/*
 * bitmap.h
 *
 *  Created on: 23 wrz 2022
 *      Author: ppp
 */

#ifndef SRC_BITMAP_H_
#define SRC_BITMAP_H_

  unsigned char intro [] =
  {
		  0xff,0xff,0x03,0x03,0x03,0x03,0x03,0xe3,0xe3,0x03,0x03,0x03,0x83,0xc3,0x63,0x23,
		  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
		  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,
		  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xc3,0xc3,0x03,0x03,0x03,0x03,0x03,
		  0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0xe3,0xf3,0x33,0x33,0x33,0x03,
		  0x03,0x03,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x06,0x0e,0x1b,
		  0x31,0x60,0xc0,0x80,0x00,0x00,0xff,0xff,0x03,0x03,0x03,0x00,0xc3,0xf3,0xfb,0xdf,
		  0xcf,0xc3,0x00,0x01,0x0f,0x3f,0xf8,0xe0,0xf8,0x3f,0x0f,0x01,0x00,0xce,0xdf,0xdb,
		  0xdb,0xfb,0x71,0x00,0x00,0xc3,0xf3,0xfb,0xdf,0xcf,0xc3,0x00,0x00,0x7f,0xff,0xc3,
		  0xc3,0xc3,0x00,0x00,0x3c,0x7e,0xe7,0xc3,0xc3,0xe7,0x7e,0x3c,0x00,0x00,0xff,0xff,
		  0x03,0x03,0x03,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
		  0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,
		  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x06,0xc7,0xc3,0x00,0x00,0x00,0x00,
		  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,
		  0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0xc0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x00,0x00,
		  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x19,0x19,0x19,0x19,
		  0x9f,0xf7,0xe0,0x00,0x00,0xc0,0xec,0x6c,0x6c,0x6c,0xfc,0xf8,0x00,0x00,0xfd,0xfd,
		  0x00,0x00,0xf0,0xf8,0x9c,0x0c,0x0c,0x9c,0xf8,0xf0,0x00,0x18,0xff,0xff,0x04,0x04,
		  0x00,0xf0,0xf8,0x6c,0x6c,0x6c,0x78,0x70,0x00,0x00,0xff,0xff,0x20,0x70,0xd8,0x8c,
		  0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
		  0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,
		  0x03,0x03,0x03,0x03,0x03,0x01,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x03,0x03,
		  0x18,0x18,0x1f,0x0f,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x01,0x00,0x00,0x00,
		  0x01,0x03,0x03,0x00,0x00,0x00,0x01,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x03,0x03,
		  0x00,0x00,0x00,0x03,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		  0x00,0x00,0xff,0xff,0xff,0xff,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
		  0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
		  0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
		  0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
		  0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,
		  0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xff,0xff
  };

  unsigned char digits [][6] = {
		   {0x00,0x7c,0x82,0x82,0x82,0x7c} //0
		  ,{0x00,0x00,0x82,0xfe,0x80,0x00} //1
		  ,{0x00,0x84,0xc2,0xa2,0x92,0x8c} //2
		  ,{0x00,0x44,0x92,0x92,0x92,0x6c} //3
		  ,{0x00,0x30,0x28,0x24,0xfe,0x20} //4
		  ,{0x00,0x9e,0x92,0x92,0x92,0x72} //5
		  ,{0x00,0x7c,0x92,0x92,0x92,0x64} //6
		  ,{0x00,0x02,0x82,0x62,0x1a,0x06} //7
		  ,{0x00,0x6c,0x92,0x92,0x92,0x6c} //8
		  ,{0x00,0x4c,0x92,0x92,0x92,0x7c} //9
  };

  unsigned char enter [] =
  {0xff,0x80,0x80,0x90,0x88,0xfc,0x08,0x10};

  unsigned char escape [] =
  {0x08,0x1c,0x2a,0x08,0x08,0x08,0x08,0x08};

  unsigned char program_data[][5] = {
		   {1, 1, 1, 1, 1}
		  ,{1, 1, 3, 10, 22}
		  ,{1, 0, 3, 13, 52}
		  ,{0, 1, 7, 23, 51}
		  ,{1, 1, 3, 17, 52}
		  ,{0, 0, 2, 14, 53}
		  ,{1, 0, 1, 5, 44}
		  ,{1, 0, 6, 12, 12}
		  ,{0, 1, 4, 10, 23}
		  ,{0, 1, 6, 11, 54}
		  ,{1, 0, 1, 9, 22}
		  ,{0, 1, 2, 7, 27}
		  ,{1, 0, 5, 8, 26}
		  ,{1, 1, 7, 2, 36}

  };



#endif /* SRC_BITMAP_H_ */
