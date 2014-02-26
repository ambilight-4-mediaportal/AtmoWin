
#include "AtmoNulConnection.h"

CAtmoNulConnection::CAtmoNulConnection(CAtmoConfig *cfg) : CAtmoConnection(cfg)
{
}

CAtmoNulConnection::~CAtmoNulConnection(void)
{
}

  	
ATMO_BOOL CAtmoNulConnection::OpenConnection()
{
  return ATMO_TRUE;
}

void CAtmoNulConnection::CloseConnection()
{
}

ATMO_BOOL CAtmoNulConnection::isOpen(void)
{
  return ATMO_TRUE;
}

ATMO_BOOL CAtmoNulConnection::SendData(pColorPacket data)
{
  return ATMO_TRUE;
}

ATMO_BOOL CAtmoNulConnection::HardwareWhiteAdjust(int global_gamma,
                                             int global_contrast,
                                             int contrast_red,
                                             int contrast_green,
                                             int contrast_blue,
                                             int gamma_red,
                                             int gamma_green,
                                             int gamma_blue,
                                             ATMO_BOOL storeToEeprom) 
{
  return ATMO_TRUE;
}

