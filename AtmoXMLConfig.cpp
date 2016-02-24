
#include "StdAfx.h"
#include "ObjectModel.h"
#include "AtmoXMLConfig.h"

using namespace std;

CAtmoXMLConfig::CAtmoXMLConfig(char *section) : CAtmoConfig()
{
	// Speicher Strukturen wenns welche gibt mit new holen
	configSection[0] = 0;

	if(section!=NULL)
		strcpy(this->configSection, section);

	m_IsShowConfigDialog = 0;
}

CAtmoXMLConfig::~CAtmoXMLConfig(void)
{
}

void CAtmoXMLConfig::SaveSettings(std::string Profile1) 
{
	char XMLSectionName[100];
	char valueName[32];

	std::string path(configSection);
	CUtils *Utils = new CUtils;

	if (Profile1 != "")
	{
		path.assign(Profile1);
		strcpy(this->newconfigSection, lastprofile.data());
	}
	if (Profile1 == "")
		strcpy(this->newconfigSection, "AtmoWinX");

	strcpy(this->configSection, "Default");

	// don't destroy config in that case..
	if(m_eAtmoConnectionType != actNUL) 
		GetProfile().SetConfig(configSection, "ConnectionType", (int)m_eAtmoConnectionType);

	GetProfile().SetConfig(configSection, "lastprofile", (char*)lastprofile.data());
	GetProfile().SetConfig(configSection, "defaultprofile", (char*)defaultprofile.data());
	GetProfile().SetConfig(configSection, "EffectMode", (int)m_eEffectMode);
	GetProfile().SetConfig(configSection, "comport", this->m_Comport);
	GetProfile().SetConfig(configSection, "Arducomport", this->m_ArduComport);
	GetProfile().SetConfig(configSection, "AtmoV2comport", this->m_AtmoV2Comport);
	GetProfile().SetConfig(configSection, "comport_1", m_Comports[0]);
	GetProfile().SetConfig(configSection, "comport_2",  m_Comports[1]);
	GetProfile().SetConfig(configSection, "comport_3", m_Comports[2]);

	GetProfile().SetConfig(newconfigSection, "WhiteAdjustment_Red", m_WhiteAdjustment_Red);
	GetProfile().SetConfig(newconfigSection, "WhiteAdjustment_Green", m_WhiteAdjustment_Green);
	GetProfile().SetConfig(newconfigSection, "WhiteAdjustment_Blue", m_WhiteAdjustment_Blue);
	GetProfile().SetConfig(newconfigSection, "UseSoftwareWhiteAdj", (int)m_UseSoftwareWhiteAdj);
	GetProfile().SetConfig(newconfigSection, "UseSoftware2WhiteAdj", (int)m_UseSoftware2WhiteAdj);
	GetProfile().SetConfig(newconfigSection, "UseColorK", (int)m_UseColorKWhiteAdj);
	GetProfile().SetConfig(newconfigSection, "Use3dlut", (int)m_Use3dlut);

	GetProfile().SetConfig(newconfigSection, "UsePerChWhiteAdj", (int)m_WhiteAdjPerChannel);	
	sprintf(XMLSectionName,"%sWhiteAdjPerChannel", "");
	GetProfile().SetConfig(XMLSectionName, "count", (int)m_chWhiteAdj_Count);

	for(int i=0; (i<m_chWhiteAdj_Count) && m_chWhiteAdj_Red && m_chWhiteAdj_Blue && m_chWhiteAdj_Green; i++) 
	{
		sprintf(valueName,"red_%d",i);
		GetProfile().SetConfig(XMLSectionName, valueName, m_chWhiteAdj_Red[i]);

		sprintf(valueName,"green_%d",i);
		GetProfile().SetConfig(XMLSectionName, valueName, m_chWhiteAdj_Green[i]);

		sprintf(valueName,"blue_%d",i);
		GetProfile().SetConfig(XMLSectionName, valueName, m_chWhiteAdj_Blue[i]);
	}

	GetProfile().SetConfig(newconfigSection, "ColorChanger_iSteps", this->m_ColorChanger_iSteps);
	GetProfile().SetConfig(newconfigSection, "ColorChanger_iDelay", this->m_ColorChanger_iDelay);
	GetProfile().SetConfig(newconfigSection, "LrColorChanger_iSteps", this->m_LrColorChanger_iSteps);
	GetProfile().SetConfig(newconfigSection, "LrColorChanger_iDelay", this->m_LrColorChanger_iDelay);
	GetProfile().SetConfig(newconfigSection, "StaticColor_Red", this->m_StaticColor_Red);
	GetProfile().SetConfig(newconfigSection, "StaticColor_Green", this->m_StaticColor_Green);
	GetProfile().SetConfig(newconfigSection, "StaticColor_Blue", this->m_StaticColor_Blue);
	GetProfile().SetConfig(newconfigSection, "isSetShutdownColor", m_IsSetShutdownColor);
	GetProfile().SetConfig(newconfigSection, "ShutdownColor_red", m_ShutdownColor_Red);
	GetProfile().SetConfig(newconfigSection, "ShutdownColor_green", m_ShutdownColor_Green);
	GetProfile().SetConfig(newconfigSection, "ShutdownColor_blue", m_ShutdownColor_Blue);
	GetProfile().SetConfig(newconfigSection, "LiveViewFilterMode", (int)m_LiveViewFilterMode);
	GetProfile().SetConfig(newconfigSection, "LiveViewFilter_PercentNew", m_LiveViewFilter_PercentNew);
	GetProfile().SetConfig(newconfigSection, "LiveViewFilter_MeanLength", m_LiveViewFilter_MeanLength);
	GetProfile().SetConfig(newconfigSection, "LiveViewFilter_MeanThreshold", m_LiveViewFilter_MeanThreshold);
	GetProfile().SetConfig(newconfigSection, "LiveView_EdgeWeighting", m_LiveView_EdgeWeighting);
	GetProfile().SetConfig(newconfigSection, "LiveView_RowsPerFrame", m_LiveView_RowsPerFrame);
	GetProfile().SetConfig(newconfigSection, "LiveView_BrightCorrect", m_LiveView_BrightCorrect);
	GetProfile().SetConfig(newconfigSection, "LiveView_DarknessLimit", m_LiveView_DarknessLimit);
	GetProfile().SetConfig(newconfigSection, "LiveView_HueWinSize", m_LiveView_HueWinSize);
	GetProfile().SetConfig(newconfigSection, "LiveView_SatWinSize", m_LiveView_SatWinSize);
	GetProfile().SetConfig(newconfigSection, "LiveView_Overlap", m_LiveView_Overlap);
	GetProfile().SetConfig(newconfigSection, "LiveView_WidescreenMode", m_LiveView_WidescreenMode);
	GetProfile().SetConfig(newconfigSection, "LiveView_Mode",m_LiveView_Mode);
	GetProfile().SetConfig(newconfigSection, "LiveView_Saturation", m_LiveView_Saturation);
	GetProfile().SetConfig(newconfigSection, "LiveView_Sensitivity", m_LiveView_Sensitivity);
	GetProfile().SetConfig(newconfigSection, "LiveView_invert", (int)m_Useinvert);
	GetProfile().SetConfig(newconfigSection, "LiveView_HOverscanBorder", m_LiveView_HOverscanBorder);
	GetProfile().SetConfig(newconfigSection, "LiveView_VOverscanBorder", m_LiveView_VOverscanBorder);
	GetProfile().SetConfig(newconfigSection, "LiveView_DisplayNr", m_LiveView_DisplayNr);
	GetProfile().SetConfig(newconfigSection, "LiveView_FrameDelay", m_LiveView_FrameDelay);
	GetProfile().SetConfig(newconfigSection, "LiveView_GDI_FrameRate", m_LiveView_GDI_FrameRate);
	GetProfile().SetConfig(newconfigSection, "ZonesTopCount", m_ZonesTopCount);
	GetProfile().SetConfig(newconfigSection, "ZonesBottomCount", m_ZonesBottomCount);
	GetProfile().SetConfig(newconfigSection, "ZonesLRCount", m_ZonesLRCount);
	GetProfile().SetConfig(newconfigSection, "ZoneSummary", m_ZoneSummary);

	GetProfile().SetConfig(newconfigSection, "Hardware_global_gamma", m_Hardware_global_gamma);
	GetProfile().SetConfig(newconfigSection, "Hardware_global_contrast", m_Hardware_global_contrast);
	GetProfile().SetConfig(newconfigSection, "Hardware_contrast_red", m_Hardware_contrast_red);
	GetProfile().SetConfig(newconfigSection, "Hardware_contrast_green", m_Hardware_contrast_green);
	GetProfile().SetConfig(newconfigSection, "Hardware_contrast_blue", m_Hardware_contrast_blue);
	GetProfile().SetConfig(newconfigSection, "Hardware_gamma_red", m_Hardware_gamma_red);
	GetProfile().SetConfig(newconfigSection, "Hardware_gamma_green", m_Hardware_gamma_green);
	GetProfile().SetConfig(newconfigSection, "Hardware_gamma_blue", m_Hardware_gamma_blue);

	GetProfile().SetConfig(newconfigSection, "Software_gamma_mode", m_Software_gamma_mode);
	GetProfile().SetConfig(newconfigSection, "Software_gamma_red", m_Software_gamma_red);
	GetProfile().SetConfig(newconfigSection, "Software_gamma_green", m_Software_gamma_green);
	GetProfile().SetConfig(newconfigSection, "Software_gamma_blue", m_Software_gamma_blue);
	GetProfile().SetConfig(newconfigSection, "Software_gamma_global", m_Software_gamma_global);
	GetProfile().SetConfig(newconfigSection, "hAtmoClLeds", m_hAtmoClLeds);

	//calib
	GetProfile().SetConfig(newconfigSection, "Software_ColK_rr", red_ColorK[256][0]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_rg", red_ColorK[256][1]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_rb", red_ColorK[256][2]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_gr", green_ColorK[256][0]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_gg", green_ColorK[256][1]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_gb", green_ColorK[256][2]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_br", blue_ColorK[256][0]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_bg", blue_ColorK[256][1]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_bb", blue_ColorK[256][2]);

	GetProfile().SetConfig(newconfigSection, "Software_ColK_yr", red_ColorK[257][0]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_yg", red_ColorK[257][1]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_yb", red_ColorK[257][2]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_mr", green_ColorK[257][0]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_mg", green_ColorK[257][1]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_mb", green_ColorK[257][2]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_cr", blue_ColorK[257][0]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_cg", blue_ColorK[257][1]);
	GetProfile().SetConfig(newconfigSection, "Software_ColK_cb", blue_ColorK[257][2]);

	GetProfile().SetConfig(newconfigSection, "Software_red_g_5", red_grid[0]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_10", red_grid[1]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_20", red_grid[2]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_30", red_grid[3]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_40", red_grid[4]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_50", red_grid[5]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_60", red_grid[6]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_70", red_grid[7]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_80", red_grid[8]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_90", red_grid[9]);
	GetProfile().SetConfig(newconfigSection, "Software_red_g_100", red_grid[10]);

	GetProfile().SetConfig(newconfigSection, "Software_green_g_5", green_grid[0]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_10", green_grid[1]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_20", green_grid[2]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_30", green_grid[3]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_40", green_grid[4]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_50", green_grid[5]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_60", green_grid[6]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_70", green_grid[7]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_80", green_grid[8]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_90", green_grid[9]);
	GetProfile().SetConfig(newconfigSection, "Software_green_g_100", green_grid[10]);

	GetProfile().SetConfig(newconfigSection, "Software_blue_g_5", blue_grid[0]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_10", blue_grid[1]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_20", blue_grid[2]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_30", blue_grid[3]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_40", blue_grid[4]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_50", blue_grid[5]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_60", blue_grid[6]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_70", blue_grid[7]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_80", blue_grid[8]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_90", blue_grid[9]);
	GetProfile().SetConfig(newconfigSection, "Software_blue_g_100", blue_grid[10]);

	// AtmoduinoV2 proper setting 
	GetProfile().SetConfig(newconfigSection, "hAtmoV2ClLeds", m_AtmoV2ClLeds);

	GetProfile().SetConfig(newconfigSection, "DMX_BaseChannels", m_DMX_BaseChannels);
	GetProfile().SetConfig(newconfigSection, "DMX_RGB_Channels", m_DMX_RGB_Channels);
	GetProfile().SetConfig(newconfigSection, "DMX_BaudrateIndex", m_DMX_BaudrateIndex);
	GetProfile().SetConfig(newconfigSection, "Ardu_BaudrateIndex", m_Ardu_BaudrateIndex);
	GetProfile().SetConfig(newconfigSection, "AtmoV2_BaudrateIndex", m_AtmoV2_BaudrateIndex);
	GetProfile().SetConfig(newconfigSection, "MoMo_Channels", m_MoMo_Channels);
	GetProfile().SetConfig(newconfigSection, "Fnordlicht_Amount", m_Fnordlicht_Amount);

	GetProfile().SetConfig(newconfigSection, "CurrentChannelAssignment", m_CurrentChannelAssignment);

	if (!ChannelDelete())
	{
		GetProfile().SetConfig(newconfigSection, "NumChannelAssignments", getNumChannelAssignments());
		for(int i=1;i<10;i++) 
		{
			CAtmoChannelAssignment *ta = this->m_ChannelAssignments[i];
			if(ta!=NULL) 
			{
				string name = string(newconfigSection) + "_ChannelAssignment_" + string(ta->getName());				
	      strcpy(XMLSectionName, name.c_str());
				
				GetProfile().SetConfig(XMLSectionName, "name", ta->getName());
				GetProfile().SetConfig(XMLSectionName, "count", ta->getSize());
				for(int c=0;c<ta->getSize();c++)
				{
					sprintf(valueName,"channel_%d", c);
					GetProfile().SetConfig(XMLSectionName, valueName, ta->getZoneIndex(c));
				}
			}
		}
		m_ChannelDelete = false;
	}

	GString strXMLStreamDestinationBuffer = "<?xml version=\"1.0\" encoding='ISO-8859-1'?>\r\n";
	GetProfile().WriteCurrentConfig(&strXMLStreamDestinationBuffer, true);
	strXMLStreamDestinationBuffer.ToFile(Utils->szTemp);
}

int CAtmoXMLConfig::trilinear(int x, int y, int z, int col)
{

	int x0 = (int)x/17;
	int x1 = x0 + 1;
	if (x1 > 255) 
	{
		x1 = x0;
	}
	int y0 = (int)y/17;
	int y1 = y0 + 1;
	if (y1 > 255) 
	{
		y1 = y0;
	}
	int z0 = (int)z/17;
	int z1 = z0 + 1;
	if (z1 > 255)
	{
		z1 = z0;
	}

	int xd = (x1-x/17);
	int yd = (y1-y/17);
	int zd = (z1-z/17);

	return little_ColorCube[x0][y0][z0][col]*(1-xd)*(1-yd)*(1-zd)+ 
		little_ColorCube[x1][y0][z0][col]*(1-yd)*(1-zd)*xd+
		little_ColorCube[x0][y1][z0][col]*(1-xd)*(1-zd)*yd+
		little_ColorCube[x0][y0][z1][col]*(1-xd)*(1-yd)*zd+
		little_ColorCube[x1][y0][z1][col]*xd*(1-yd)*zd+
		little_ColorCube[x0][y1][z1][col]*(1-xd)*yd*zd+
		little_ColorCube[x1][y1][z0][col]*(1-zd)*xd*yd+
		little_ColorCube[x1][y1][z1][col]*xd*yd*zd;

};

void CAtmoXMLConfig::LoadSettings(std::string profile1) 
{
	// alle Variabel etc. aus Registry lesen
	char XMLSectionName[100], valueName[32];
	CUtils *Utils = new CUtils;

	//profilenames
	strcpy(this->configSection, "Default");

	profile1 = GetProfile().GetStringOrDefault(configSection, "profiles", "");	

	char *buffer = new char[profile1.length()+1];
	strcpy(buffer, profile1.c_str());

	ReadXMLStringList(configSection, buffer);

	if (profile1 == "startup") 
	{
		lastprofile = GetProfile().GetStringOrDefault(configSection, "lastprofile", "");
		defaultprofile = GetProfile().GetStringOrDefault(configSection, "defaultprofile", "");
		if (defaultprofile!="") 
			lastprofile = defaultprofile;

		profile1 = lastprofile;
	}

	if (profile1 == "")
		strcpy(this->newconfigSection, "AtmoWinX");

	if (Utils->firststart)
		lastprofile = GetProfile().GetStringOrDefault(configSection, "lastprofile", "");
	else
		lastprofile = lastprofile.data();

	Utils->firststart = false;

	if (lastprofile != "")
	{
		buffer = new char[lastprofile.length()+1];
		strcpy(buffer, lastprofile.c_str());
		strcpy(this->newconfigSection, buffer);
	}

	defaultprofile = GetProfile().GetStringOrDefault(configSection, "defaultprofile", "");

	if (GetProfile().GetIntOrDefault(configSection, "IgnoreConnectionErrorOnStartup", 0) ==1)
		m_IgnoreConnectionErrorOnStartup = ATMO_TRUE;  

	m_eAtmoConnectionType = (AtmoConnectionType)(GetProfile().GetIntOrDefault(configSection, "ConnectionType", (int)actClassicAtmo));

	m_Comport  = GetProfile().GetIntOrDefault(configSection, "comport", -1); // -1 als Indikator ob Setup noch erfolgen muss!
	if(m_Comport < 1) 
	{ 
		m_IsShowConfigDialog = 1; 
		m_Comport=1; 
	} 

	m_Comports[0] = GetProfile().GetIntOrDefault(configSection, "comport_1", -1);
	m_Comports[1] = GetProfile().GetIntOrDefault(configSection, "comport_2", -1);
	m_Comports[2] = GetProfile().GetIntOrDefault(configSection, "comport_3", -1);

	m_ArduComport = GetProfile().GetIntOrDefault(configSection, "Arducomport", -1); // -1 als Indikator ob Setup noch erfolgen muss!
	if(m_ArduComport < 1) 
	{
		m_IsShowConfigDialog = 1; 
		m_ArduComport=1; 
	} 
	m_AtmoV2Comport = GetProfile().GetIntOrDefault(configSection, "AtmoV2comport", -1); // -1 als Indikator ob Setup noch erfolgen muss!
	if (m_AtmoV2Comport < 1)
	{
		m_IsShowConfigDialog=1;
		m_AtmoV2Comport=1;
	}
	m_eEffectMode = (EffectMode)GetProfile().GetIntOrDefault(configSection, "EffectMode", (int)m_eEffectMode);

	m_WhiteAdjustment_Red    = CheckByteValue(GetProfile().GetIntOrDefault(newconfigSection, "WhiteAdjustment_Red", m_WhiteAdjustment_Red));
	m_WhiteAdjustment_Green  = CheckByteValue(GetProfile().GetIntOrDefault(newconfigSection, "WhiteAdjustment_Green", m_WhiteAdjustment_Green));
	m_WhiteAdjustment_Blue   = CheckByteValue(GetProfile().GetIntOrDefault(newconfigSection, "WhiteAdjustment_Blue", m_WhiteAdjustment_Blue));

	m_UseSoftwareWhiteAdj    = (GetProfile().GetIntOrDefault(newconfigSection, "UseSoftwareWhiteAdj", m_UseSoftwareWhiteAdj) != 0);
	m_UseSoftware2WhiteAdj   = 0;
	m_UseColorKWhiteAdj		   = (GetProfile().GetIntOrDefault(newconfigSection, "UseColorK", m_UseColorKWhiteAdj) != 0);
	m_Use3dlut		           = (GetProfile().GetIntOrDefault(newconfigSection, "Use3dlut", m_Use3dlut) != 0);

	m_WhiteAdjPerChannel     = (GetProfile().GetIntOrDefault(newconfigSection, "UsePerChWhiteAdj", m_WhiteAdjPerChannel) != 0);
	sprintf(XMLSectionName,"%sWhiteAdjPerChannel", "");
	m_chWhiteAdj_Count       = GetProfile().GetIntOrDefault(XMLSectionName, "count", m_WhiteAdjPerChannel);

	delete []m_chWhiteAdj_Red;
	delete []m_chWhiteAdj_Green;
	delete []m_chWhiteAdj_Blue; 

	if(m_chWhiteAdj_Count > 0) 
	{ 
		m_chWhiteAdj_Red   = new int[ m_chWhiteAdj_Count ];
		m_chWhiteAdj_Green = new int[ m_chWhiteAdj_Count ];
		m_chWhiteAdj_Blue  = new int[ m_chWhiteAdj_Count ];
	} 
	else 
	{
		m_chWhiteAdj_Red   = NULL;
		m_chWhiteAdj_Green = NULL;
		m_chWhiteAdj_Blue  = NULL;
	}

	for(int i=0; (i<m_chWhiteAdj_Count); i++) 
	{
		sprintf(valueName,"red_%d",i);
		m_chWhiteAdj_Red[i]   = Check8BitValue(GetProfile().GetIntOrDefault(XMLSectionName, "valueName", 256));

		sprintf(valueName,"green_%d",i);
		m_chWhiteAdj_Green[i] = Check8BitValue(GetProfile().GetIntOrDefault(XMLSectionName, "valueName", 256));

		sprintf(valueName,"blue_%d",i);
		m_chWhiteAdj_Blue[i]  = Check8BitValue(GetProfile().GetIntOrDefault(XMLSectionName, "valueName", 256));
	}

	m_ColorChanger_iSteps            = GetProfile().GetIntOrDefault(newconfigSection, "ColorChanger_iSteps", m_ColorChanger_iSteps);
	m_ColorChanger_iDelay            = GetProfile().GetIntOrDefault(newconfigSection, "ColorChanger_iDelay", m_ColorChanger_iDelay);
	m_LrColorChanger_iSteps          = GetProfile().GetIntOrDefault(newconfigSection, "LrColorChanger_iSteps", m_LrColorChanger_iSteps);
	m_LrColorChanger_iDelay          = GetProfile().GetIntOrDefault(newconfigSection, "LrColorChanger_iDelay", m_LrColorChanger_iDelay);
	m_IsSetShutdownColor             = GetProfile().GetIntOrDefault(newconfigSection, "isSetShutdownColor", m_IsSetShutdownColor);
	m_ShutdownColor_Red              = CheckByteValue(GetProfile().GetIntOrDefault(newconfigSection, "ShutdownColor_red", m_ShutdownColor_Red));
	m_ShutdownColor_Green            = CheckByteValue(GetProfile().GetIntOrDefault(newconfigSection, "ShutdownColor_green", m_ShutdownColor_Green));
	m_ShutdownColor_Blue             = CheckByteValue(GetProfile().GetIntOrDefault(newconfigSection, "ShutdownColor_blue", m_ShutdownColor_Blue));
	m_StaticColor_Red                = GetProfile().GetIntOrDefault(newconfigSection, "StaticColor_Red", m_StaticColor_Red);
	m_StaticColor_Green              = GetProfile().GetIntOrDefault(newconfigSection, "StaticColor_Green", m_StaticColor_Green);
	m_StaticColor_Blue               = GetProfile().GetIntOrDefault(newconfigSection, "StaticColor_Blue", m_StaticColor_Blue);
	m_LiveViewFilterMode             = (AtmoFilterMode)GetProfile().GetIntOrDefault(newconfigSection, "LiveViewFilterMode", (int)m_LiveViewFilterMode);
	m_LiveViewFilter_PercentNew      = GetProfile().GetIntOrDefault(newconfigSection, "LiveViewFilter_PercentNew", m_LiveViewFilter_PercentNew);
	m_LiveViewFilter_MeanLength      = GetProfile().GetIntOrDefault(newconfigSection, "LiveViewFilter_MeanLength", m_LiveViewFilter_MeanLength);
	m_LiveViewFilter_MeanThreshold   = GetProfile().GetIntOrDefault(newconfigSection, "LiveViewFilter_MeanThreshold", m_LiveViewFilter_MeanThreshold);
	m_show_statistics                = (GetProfile().GetIntOrDefault(newconfigSection, "show_statistics", ATMO_FALSE) !=0);

	m_LiveView_RowsPerFrame          = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_RowsPerFrame", m_LiveView_RowsPerFrame);
	if(m_LiveView_RowsPerFrame < 1) m_LiveView_RowsPerFrame = 1;
	if(m_LiveView_RowsPerFrame >= CAP_HEIGHT) m_LiveView_RowsPerFrame = CAP_HEIGHT - 1;

	m_LiveView_EdgeWeighting         = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_EdgeWeighting", m_LiveView_EdgeWeighting);
	m_LiveView_BrightCorrect         = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_BrightCorrect", m_LiveView_BrightCorrect);
	m_LiveView_DarknessLimit         = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_DarknessLimit", m_LiveView_DarknessLimit);
	m_LiveView_HueWinSize            = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_HueWinSize", m_LiveView_HueWinSize);
	m_LiveView_SatWinSize            = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_SatWinSize", m_LiveView_SatWinSize);
	m_LiveView_Overlap               = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_Overlap", m_LiveView_Overlap);
	m_LiveView_Saturation            = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_Saturation", 100);
	m_LiveView_Sensitivity           = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_Sensitivity", 0);
	m_Useinvert                      = (GetProfile().GetIntOrDefault(newconfigSection,"LiveView_invert", m_Useinvert) != 0);
	m_LiveView_WidescreenMode        = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_WidescreenMode", m_LiveView_WidescreenMode);
	m_LiveView_Mode                  = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_Mode", m_LiveView_Mode);
	m_LiveView_HOverscanBorder       = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_HOverscanBorder", m_LiveView_HOverscanBorder);
	m_LiveView_VOverscanBorder       = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_VOverscanBorder", m_LiveView_VOverscanBorder);
	m_LiveView_DisplayNr             = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_DisplayNr", m_LiveView_DisplayNr);
	m_LiveView_FrameDelay            = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_FrameDelay", m_LiveView_FrameDelay);
	m_LiveView_GDI_FrameRate         = GetProfile().GetIntOrDefault(newconfigSection, "LiveView_GDI_FrameRate", m_LiveView_GDI_FrameRate);

	m_ZonesTopCount                  = GetProfile().GetIntOrDefault(newconfigSection, "ZonesTopCount", m_ZonesTopCount);
	m_ZonesBottomCount               = GetProfile().GetIntOrDefault(newconfigSection, "ZonesBottomCount", m_ZonesBottomCount);
	m_ZonesLRCount                   = GetProfile().GetIntOrDefault(newconfigSection, "ZonesLRCount", m_ZonesLRCount);
	m_ZoneSummary                    = (GetProfile().GetIntOrDefault(newconfigSection, "ZoneSummary", m_ZoneSummary) != 0);

	UpdateZoneCount();
	m_Hardware_global_gamma          = GetProfile().GetIntOrDefault(newconfigSection, "Hardware_global_gamma", m_Hardware_global_gamma);
	m_Hardware_global_contrast       = GetProfile().GetIntOrDefault(newconfigSection, "Hardware_global_contrast", m_Hardware_global_contrast);
	m_Hardware_contrast_red          = GetProfile().GetIntOrDefault(newconfigSection, "Hardware_contrast_red", m_Hardware_contrast_red);
	m_Hardware_contrast_green        = GetProfile().GetIntOrDefault(newconfigSection, "Hardware_contrast_green", m_Hardware_contrast_green);
	m_Hardware_contrast_blue         = GetProfile().GetIntOrDefault(newconfigSection, "Hardware_contrast_blue", m_Hardware_contrast_blue);
	m_Hardware_gamma_red             = GetProfile().GetIntOrDefault(newconfigSection, "Hardware_gamma_red", m_Hardware_gamma_red);
	m_Hardware_gamma_green           = GetProfile().GetIntOrDefault(newconfigSection, "Hardware_gamma_green", m_Hardware_gamma_green);
	m_Hardware_gamma_blue            = GetProfile().GetIntOrDefault(newconfigSection, "Hardware_gamma_blue", m_Hardware_gamma_blue);

	m_Software_gamma_mode            = (AtmoGammaCorrect)GetProfile().GetIntOrDefault(newconfigSection, "Software_gamma_mode", (int)m_Software_gamma_mode);
	m_Software_gamma_red             = GetProfile().GetIntOrDefault(newconfigSection, "Software_gamma_red", m_Software_gamma_red);
	m_Software_gamma_green           = GetProfile().GetIntOrDefault(newconfigSection, "Software_gamma_green", m_Software_gamma_green);
	m_Software_gamma_blue            = GetProfile().GetIntOrDefault(newconfigSection, "Software_gamma_blue", m_Software_gamma_blue);
	m_Software_gamma_global          = GetProfile().GetIntOrDefault(newconfigSection, "Software_gamma_global", m_Software_gamma_global);
	m_Software_gamma_red             = min(max(m_Software_gamma_red, 10), 350);  // 0,5 - 3,5
	m_Software_gamma_green           = min(max(m_Software_gamma_green, 10), 350);  // 0,5 - 3,5
	m_Software_gamma_blue            = min(max(m_Software_gamma_blue, 10), 350);  // 0,5 - 3,5
	m_Software_gamma_global          = min(max(m_Software_gamma_global, 10), 350);  // 0,5 - 3,5

	char workdir[MAX_PATH];
	char _lut[10] = "3Dlut.3dl";

	GetModuleFileName(GetModuleHandle(NULL),workdir,MAX_PATH);

	// strip of everything after last "\"
	for(size_t i=(strlen(workdir)-1); i > 1 ; i--) 
	{  /*  c:\*/
		if(workdir[i] == '\\')  
		{
			workdir[i+1] = 0;
			break;
		}
	}

	int pathlength = strlen(workdir);
	for(size_t i=0; i <10 ; ++i)
	{
		workdir[pathlength+i]=_lut[i];
	}

	// loading 3D LUT
	m_3dlut = false;
	little_ColorCube[16][16][16][2] = -1;
	using namespace std;
	char *inname = workdir; // +"./3Dlut.3dl";
	std::string strInput;
	ifstream _3dlut(inname);
	if (_3dlut)
	{ 
		for (int i1=0; i1<4; i1++) getline(_3dlut, strInput);
		for( int i1=0 ; i1<17 ; i1++)
			for( int i2=0 ; i2<17 ; i2++)
				for( int i3=0 ; i3<17 ; i3++)
					(_3dlut>>little_ColorCube[i1][i2][i3][0]>>little_ColorCube[i1][i2][i3][1]>>little_ColorCube[i1][i2][i3][2]);

	}	

	if (little_ColorCube[16][16][16][2] != -1) 
		m_3dlut = true;

	red_ColorK[256][0]    = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_rr", 255);
	red_ColorK[256][1]    = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_rg", 0);
	red_ColorK[256][2]    = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_rb", 0);

	green_ColorK[256][0]  = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_gr", 0);
	green_ColorK[256][1]  = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_gg", 255);
	green_ColorK[256][2]  = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_gb", 0);

	blue_ColorK[256][0]   = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_br", 0);
	blue_ColorK[256][1]   = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_bg", 0);
	blue_ColorK[256][2]   = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_bb", 255);

	red_ColorK[257][0]    = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_yr", 255);
	red_ColorK[257][1]    = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_yg", 255);
	red_ColorK[257][2]    = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_yb", 0);

	green_ColorK[257][0]  = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_mr", 255);
	green_ColorK[257][1]  = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_mg", 0);
	green_ColorK[257][2]  = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_mb", 255);

	blue_ColorK[257][0]   = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_cr", 0);
	blue_ColorK[257][1]   = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_cg", 255);
	blue_ColorK[257][2]   = GetProfile().GetIntOrDefault(newconfigSection, "Software_ColK_cb", 255);

	red_grid[0]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_5", 12);
	red_grid[1]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_10", 25);
	red_grid[2]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_20", 51);
	red_grid[3]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_30", 76);
	red_grid[4]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_40", 102);
	red_grid[5]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_50", 127);
	red_grid[6]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_60", 153);
	red_grid[7]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_70", 178);
	red_grid[8]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_80", 204);
	red_grid[9]           = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_90", 229);
	red_grid[10]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_red_g_100", 255);

	green_grid[0]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_5", 12);
	green_grid[1]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_10", 25);
	green_grid[2]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_20", 51);
	green_grid[3]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_30", 76);
	green_grid[4]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_40", 102);
	green_grid[5]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_50", 127);
	green_grid[6]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_60", 153);
	green_grid[7]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_70", 178);
	green_grid[8]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_80", 204);
	green_grid[9]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_90", 229);
	green_grid[10]        = GetProfile().GetIntOrDefault(newconfigSection, "Software_green_g_100", 255);

	blue_grid[0]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_5", 12);
	blue_grid[1]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_10", 25);
	blue_grid[2]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_20", 51);
	blue_grid[3]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_30", 76);
	blue_grid[4]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_40", 102);
	blue_grid[5]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_50", 127);
	blue_grid[6]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_60", 153);
	blue_grid[7]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_70", 178);
	blue_grid[8]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_80", 204);
	blue_grid[9]          = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_90", 229);
	blue_grid[10]         = GetProfile().GetIntOrDefault(newconfigSection, "Software_blue_g_100", 255);

	ColorCube[0][0][0][0]=0;ColorCube[0][0][0][1]=0;ColorCube[0][0][0][2]=0; //black
	ColorCube[1][0][0][0]=red_ColorK[256][0];ColorCube[1][0][0][1]=red_ColorK[256][1];ColorCube[1][0][0][2]=red_ColorK[256][2]; //red
	ColorCube[0][1][0][0]=green_ColorK[256][0];ColorCube[0][1][0][1]=green_ColorK[256][1];ColorCube[0][1][0][2]=green_ColorK[256][2]; //green
	ColorCube[0][0][1][0]=blue_ColorK[256][0];ColorCube[0][0][1][1]=blue_ColorK[256][1];ColorCube[0][0][1][2]=blue_ColorK[256][2]; //blue
	ColorCube[1][1][0][0]=red_ColorK[257][0];ColorCube[1][1][0][1]=red_ColorK[257][1];ColorCube[1][1][0][2]=red_ColorK[257][2]; //yellow
	ColorCube[1][0][1][0]=green_ColorK[257][0];ColorCube[1][0][1][1]=green_ColorK[257][1];ColorCube[1][0][1][2]=green_ColorK[257][2]; //magenta
	ColorCube[0][1][1][0]=blue_ColorK[257][0];ColorCube[0][1][1][1]=blue_ColorK[257][1];ColorCube[0][1][1][2]=blue_ColorK[257][2]; //cyan
	ColorCube[1][1][1][0]=red_grid[10] ;ColorCube[1][1][1][1]=green_grid[10];ColorCube[1][1][1][2]=blue_grid[10]; //white

	GetProfile().GetIntOrDefault(newconfigSection, "DMX_BaseChannel", 0);
	int tmpChannel          = GetProfile().GetIntOrDefault(newconfigSection, "DMX_BaseChannel", 0);
	if((tmpChannel < 0) || (tmpChannel>253)) tmpChannel = 0;
	char buf[16];
	sprintf(buf,"%d",tmpChannel);
	const char *dmx_ch;
	dmx_ch = GetProfile().GetStringOrDefault(newconfigSection, "DMX_BaseChannels", buf);
	this->setDMX_BaseChannels( dmx_ch );

	//atmoduino
	m_hAtmoClLeds            = GetProfile().GetIntOrDefault(newconfigSection, "hAtmoClLeds", m_hAtmoClLeds);
	m_DMX_RGB_Channels       = GetProfile().GetIntOrDefault(newconfigSection, "DMX_RGB_Channels", m_DMX_RGB_Channels);
	m_DMX_BaudrateIndex      = GetProfile().GetIntOrDefault(newconfigSection, "DMX_BaudrateIndex", m_DMX_BaudrateIndex);
	m_Ardu_BaudrateIndex     = GetProfile().GetIntOrDefault(newconfigSection, "Ardu_BaudrateIndex", m_Ardu_BaudrateIndex);

	// AtmoduinoV2
	m_AtmoV2ClLeds             = GetProfile().GetIntOrDefault(newconfigSection, "hAtmoV2ClLeds", m_AtmoV2ClLeds );
	  if (m_AtmoV2ClLeds > 256) m_AtmoV2ClLeds = 256;
    if (m_AtmoV2ClLeds < 1)   m_AtmoV2ClLeds = 1;
	m_AtmoV2_BaudrateIndex = GetProfile().GetIntOrDefault(newconfigSection, "AtmoV2_BaudrateIndex", m_AtmoV2_BaudrateIndex);

	m_MoMo_Channels          = GetProfile().GetIntOrDefault(newconfigSection, "MoMo_Channels", m_MoMo_Channels);	
	if(m_MoMo_Channels > 254) 
		m_MoMo_Channels = 254;

	if(m_MoMo_Channels < 0)   
		m_MoMo_Channels = 2;

	m_Fnordlicht_Amount      = GetProfile().GetIntOrDefault(newconfigSection, "Fnordlicht_Amount", m_Fnordlicht_Amount);
	if(m_Fnordlicht_Amount > 254) 
		m_Fnordlicht_Amount = 254;

	if(m_Fnordlicht_Amount < 0)  
		m_Fnordlicht_Amount = 1;

	clearChannelMappings(); // clear channel mappings except default!
	m_CurrentChannelAssignment = GetProfile().GetIntOrDefault(newconfigSection, "CurrentChannelAssignment", m_CurrentChannelAssignment);
	setCurrentChannelAssignment(m_CurrentChannelAssignment);
	int numChannels = GetProfile().GetIntOrDefault(newconfigSection, "NumChannelAssignments", 1);

	if(m_CurrentChannelAssignment>=numChannels)
		m_CurrentChannelAssignment = 0;


	GStringList *lstFind = new GStringList;
	GStringList *lstFound = new GStringList;
	GString *gsfound = new GString;

	GString name = GString(newconfigSection) + "_ChannelAssignment_";

	GetProfile().GetSectionNames(lstFind);

	__int64 count = lstFind->GetCount();	
	for(__int64 i=0;i<count;i++) 
	{
		gsfound = lstFind->GetStrAt(i);

		if (gsfound->Left(strlen(name._str)) == name._str)
			lstFound->AddLast(gsfound->_str);
	}

	count = lstFound->GetCount();
	for(__int64 i=0;i<count;i++) 
	{
		gsfound = lstFound->GetStrAt(i);

		if ( gsfound->Left(strlen(name._str)) == name._str)
		{
			CAtmoChannelAssignment *ta = this->m_ChannelAssignments[i+1];
			if(ta==NULL) 
			{
				ta = new CAtmoChannelAssignment();
				this->m_ChannelAssignments[i+1] = ta;
			}		

			strcpy(XMLSectionName, gsfound->_str);
			const char *secname;
			secname = GetProfile().GetStringOrDefault(XMLSectionName, "name", "?");

			ta->setName(secname);
			ta->system = ATMO_FALSE;

			int chCount = GetProfile().GetIntOrDefault(XMLSectionName, "count", 0);

			ta->setSize(chCount);

			for(int c=0;c<chCount;c++) 
			{
				sprintf(valueName,"channel_%d",c);
				ta->setZoneIndex(c, GetProfile().GetIntOrDefault(XMLSectionName, valueName, c));
			}
		}
	}
  lstFind->RemoveAll();
	lstFound->RemoveAll();

	UpdateZoneDefinitionCount();
}

int CAtmoXMLConfig::CheckByteValue(int value) 
{
	if(value < 0) value = 0;
	if(value > 255) value = 255;
	return value;
}

int CAtmoXMLConfig::Check8BitValue(int value) 
{
	if(value < 0) value = 0;
	if(value > 256) value = 256;
	return value;
}

void CAtmoXMLConfig::ReadXMLStringList(char *section, char *default_value)
{
	__int64 count;
	CUtils *Utils = new CUtils;
	GString rslt;

	string Profile1 = GetProfile().GetStringOrDefault(configSection, "profiles", default_value);
	char *buffer = new char[Profile1.length()+1];
	strcpy(buffer, Profile1.c_str());

	GStringList lst("|", buffer);
	count = lst.GetCount();

	if (count >> 0)
	{
		Utils->profiles.RemoveAll();

		for (int i=0; i<count;i++)
		{		
			rslt = lst.Serialize("|", i, 0);
			Utils->profiles.AddFirst(rslt, rslt._len);
		}
	}

	count = Utils->profiles.GetCount();
	if(count == 0)
		default_value = strdup(default_value);

	return;

}