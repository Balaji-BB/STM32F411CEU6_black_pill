/******************************************************************************
 * @file    neo_6m_gps_parser.c
 * @author  Balaji-BB
 * @date    Aug 16, 2025
 * @brief   [Short description of what this file does]
 *
 * @note    [Any important notes if applicable]
 * @version 1.0
 *****************************************************************************/
#include "neo_6m_gps_parser.h"

uint8_t neo_6m_gps_data[500];
uint16_t size;


data_struct_typedef data_struct;
char send_str[200];

RMCStruct_Typedef RMC_Data;

extern bool gps_data_flag;
extern UART_HandleTypeDef huart2;


void NMEA_parser(void)
{
	if(gps_data_flag)
	{
		gps_data_flag = false;
		char *sentence_ptr = strtok((char*)neo_6m_gps_data, "$");
		while(sentence_ptr!=NULL)
		{
			NMEA_sentence_parser(sentence_ptr);
			sentence_ptr = strtok(NULL, "$");
		}
		memset(neo_6m_gps_data, 0, sizeof(neo_6m_gps_data));
	}
}

void NMEA_sentence_parser(char *msg)
{
	uint32_t len = strlen(msg) + 1;
	//	char *msg_ptr;
	char msg_ptr[len];
	//	msg_ptr = (char *)malloc(len * sizeof(char));
	if(msg_ptr!=NULL)
	{
		memcpy(msg_ptr, msg, len);
		if(strncmp(msg_ptr, "GPRMC", 5) == 0)
		{
			char *data = strchr(msg_ptr, (int)',');
			RMC_parser(data+1);
			uart_serial_monitor();
		}
		else if(strncmp(msg_ptr, "GPGLL", 5) == 0)
		{
			__NOP();
		}
		else if(strncmp(msg_ptr, "GPGSV", 5) == 0)
		{
			__NOP();
		}
		else if(strncmp(msg_ptr, "GPGSA", 5) == 0)
		{
			__NOP();
		}
		else if(strncmp(msg_ptr, "GPGGA", 5) == 0)
		{
			__NOP();
		}
		else if(strncmp(msg_ptr, "GPVTG", 5) == 0)
		{
			__NOP();
		}
		else
		{
			__NOP();
		}

	}
	//	free(msg_ptr);
}

void RMC_parser(char *data)
{
	char buffer[14][10] = {0};
	uint8_t ind = 0;
	uint32_t base_addr = (uint32_t)&data[0];
	char *data_ptr;
	data_ptr = strchr(data, ',');

	while(data_ptr!= NULL)
	{
		uint32_t diff = (uint32_t)data_ptr - base_addr;
		if(diff!=0)
		{
			strncpy(buffer[ind++], (char*)(base_addr), diff);
		}
		else
		{
			buffer[ind++][0] = 0;
		}
		base_addr += (diff+1);
		data_ptr = strchr(++data_ptr, ',');
	}
//	if(ind == 11)
	{
		data_ptr = strchr(data, '*');
		if(data_ptr!=NULL)
		{
			buffer[ind++][0] = (*(data_ptr-1));
			strncpy(buffer[ind], data_ptr+1, 2);
		}
	}

	RMC_Data.UTC_Time = atoi(buffer[0]);
	data_struct.hours = ((buffer[0][0]-48) * 10) + (buffer[0][1]-48) + 5U;
	data_struct.minutes = ((buffer[0][2]-48) * 10) + (buffer[0][3]-48) + 30U;
	if(data_struct.minutes > 59)
	{
		data_struct.hours += 1;
		data_struct.minutes -= 60;
	}
	data_struct.seconds = ((buffer[0][4]-48) * 10) + (buffer[0][5]-48);
	RMC_Data.position_status = buffer[1][0];

	RMC_Data.knots = atof(&buffer[6][0]);
	RMC_Data.heading = atof(&buffer[7][0]);
	RMC_Data.date = atoi(&buffer[8][0]);
	data_struct.date = (((buffer[8][0]-48) * 10) + (buffer[8][1]-48));
	data_struct.month = (((buffer[8][2]-48) * 10) + (buffer[8][3]-48));
	data_struct.year = ((buffer[8][4]-48) * 10) + (buffer[8][5]-48);
	RMC_Data.magnetic_variation = atof(&buffer[9][0]);
	RMC_Data.magnetic_variation_dir = buffer[10][0];
	RMC_Data.mode = buffer[11][0];
	RMC_Data.checksum = atoi(buffer[12]);

	if(RMC_Data.position_status == 'A')
	{
		double min_to_deg = 0;
		RMC_Data.latitude = ((buffer[2][0] - 48) * 10) + (buffer[2][1] - 48);
		min_to_deg = atof(&buffer[2][2]);
		min_to_deg = min_to_deg / 60; // minutes to degree conversion
		RMC_Data.latitude += min_to_deg;
		RMC_Data.latitude_dir = buffer[3][0];
		RMC_Data.longitude = ((buffer[4][0] - 48) * 100) + ((buffer[4][1] - 48) * 10) + (buffer[4][2] - 48);
		min_to_deg = atof(&buffer[4][3]);
		min_to_deg = min_to_deg / 60; // minutes to degree conversion
		RMC_Data.longitude += min_to_deg;
		RMC_Data.longitude_dir = buffer[5][0];
		snprintf(data_struct.map_link, sizeof(data_struct.map_link), "https://www.google.com/maps/search/?api=1&query=%f,%f", RMC_Data.latitude, RMC_Data.longitude);
	}
	else
	{
		RMC_Data.latitude = 0;
		RMC_Data.latitude_dir = 0;
		RMC_Data.longitude = 0;
		RMC_Data.longitude_dir = 0;
		snprintf(data_struct.map_link, sizeof(data_struct.map_link), "GPS Position fixing in progress...");
	}
}

void uart_serial_monitor(void)
{
	int len = snprintf(send_str, sizeof(send_str), "Date : %u/%u/%lu, Time : %u:%u:%u (GMT:+5:30 ZONE)\r\n%s\r\n",\
			data_struct.date, data_struct.month, data_struct.year,\
			data_struct.hours, data_struct.minutes, data_struct.seconds, data_struct.map_link);
	HAL_UART_Transmit_IT(&huart2, (const uint8_t *)send_str, len);
}
