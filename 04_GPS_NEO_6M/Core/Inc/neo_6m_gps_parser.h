/******************************************************************************
 * @file    neo_6m_gps_parser.h
 * @author  Balaji-BB
 * @date    Aug 16, 2025
 * @brief   [Short description of what this file does]
 *
 * @note    [Any important notes if applicable]
 * @version 1.0
 *****************************************************************************/

#ifndef INC_NEO_6M_GPS_PARSER_H_
#define INC_NEO_6M_GPS_PARSER_H_

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdlib.h"


typedef struct
{
	uint32_t UTC_Time;
	char position_status;
	float latitude;
	char latitude_dir;
	float longitude;
	char longitude_dir;
	float knots; // Speed over ground
	float heading; // Course over ground
	uint32_t date;
	float magnetic_variation;
	char magnetic_variation_dir;
	char mode;
	uint32_t checksum;
}RMCStruct_Typedef;

typedef struct
{
	uint8_t date;
	uint8_t month;
	uint32_t year;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	char map_link[200];
}data_struct_typedef;


void NMEA_parser(void);
void NMEA_sentence_parser(char *msg);
void RMC_parser(char *data);
void uart_serial_monitor(void);

#endif /* INC_NEO_6M_GPS_PARSER_H_ */
