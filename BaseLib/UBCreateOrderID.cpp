#include "UBCreateOrderID.h"

uint64_t CreateOrderID(int64_t iTimeStamp,uint32_t uiAutoIncID,uint16_t uiServerID)
{
	uint64_t orderID = iTimeStamp;
	orderID <<= 32;
	uiAutoIncID <<= 8;
	orderID += uiAutoIncID;
	orderID += uiServerID;
	return orderID;
}
