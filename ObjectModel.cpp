#include "stdafx.h"
#include "ObjectModel.h"
#include "MemberDescriptor.h"
#include "AtmoDefs.h"

IMPLEMENT_FACTORY(MyConfiguration, configuration)
IMPLEMENT_FACTORY(MySection, section)
IMPLEMENT_FACTORY(MyWhiteAdjPerChannel,	section)


int g_verify0ref = 0;

void MyConfiguration::MapXMLTagsToMembers()
{
	
	// All of the <Section>'s under <Configuration> goes into this list
	MapMember(&m_lstMyObjects, MySection::GetStaticTag());

}


void MyWhiteAdjPerChannel::MapXMLTagsToMembers()
{
	MapMember(&m_chWhiteAdj_Count, "count");
	MapMember(&m_chWhiteAdj_Red, "red_%d");
	MapMember(&m_chWhiteAdj_Green, "green_%d");
	MapMember(&m_chWhiteAdj_Blue, "blue_%d");
}

void MySection::MapXMLTagsToMembers()
{
	MapAttribute(&m_strSection,	"name");
	
	MapMember(&profile, "lastprofile");
	MapMember(&d_profile, "defaultprofile");
	MapMember(&m_eAtmoConnectionType, "ConnectionType");
MapMember(&m_IgnoreConnectionErrorOnStartup, "IgnoreConnectionErrorOnStartup");	
	MapMember(&m_eEffectMode, "EffectMode");
	MapMember(&m_Comport, "comport");
	MapMember(&m_ArduComport, "Arducomport");
	//MapMember(&m_Comports[0], "comport_1");
  //MapMember(&m_Comports[1], "comport_2");
	//MapMember(&m_Comports[2], "comport_3");
	MapMember(&m_WhiteAdjustment_Red, "WhiteAdjustment_Red");
	MapMember(&m_WhiteAdjustment_Green, "WhiteAdjustment_Green");
	MapMember(&m_WhiteAdjustment_Blue, "WhiteAdjustment_Blue");
	MapMember(&m_UseSoftwareWhiteAdj, "UseSoftwareWhiteAdj");
	MapMember(&m_UseSoftware2WhiteAdj, "UseSoftware2WhiteAdj");
	MapMember(&m_UseColorKWhiteAdj, "UseColorK");
	MapMember(&m_Use3dlut, "Use3dlut");
	MapMember(&m_WhiteAdjPerChannel, "UsePerChWhiteAdj");
	MapMember(&m_ColorChanger_iSteps, "ColorChanger_iSteps");
	MapMember(&m_ColorChanger_iDelay, "ColorChanger_iDelay");
	MapMember(&m_LrColorChanger_iSteps, "LrColorChanger_iSteps");
	MapMember(&m_LrColorChanger_iDelay, "LrColorChanger_iDelay");
	MapMember(&m_StaticColor_Red, "StaticColor_Red");
	MapMember(&m_StaticColor_Green, "StaticColor_Green");
	MapMember(&m_StaticColor_Blue, "StaticColor_Blue");
	MapMember(&m_IsSetShutdownColor, "isSetShutdownColor");
	MapMember(&m_ShutdownColor_Red, "ShutdownColor_red");
	MapMember(&m_ShutdownColor_Green, "ShutdownColor_green");
	MapMember(&m_ShutdownColor_Blue, "ShutdownColor_blue");
	MapMember(&m_LiveViewFilterMode, "LiveViewFilterMode");
	MapMember(&m_LiveViewFilter_PercentNew, "LiveViewFilter_PercentNew");
	MapMember(&m_LiveViewFilter_MeanLength, "LiveViewFilter_MeanLength");
	MapMember(&m_LiveViewFilter_MeanThreshold, "LiveViewFilter_MeanThreshold");
	MapMember(&m_LiveView_EdgeWeighting, "LiveView_EdgeWeighting");
	MapMember(&m_LiveView_RowsPerFrame, "LiveView_RowsPerFrame");
	MapMember(&m_LiveView_BrightCorrect, "LiveView_BrightCorrect");
	MapMember(&m_LiveView_DarknessLimit, "LiveView_DarknessLimit");
	MapMember(&m_LiveView_HueWinSize, "LiveView_HueWinSize");
	MapMember(&m_LiveView_SatWinSize, "LiveView_SatWinSize");
	MapMember(&m_LiveView_Overlap, "LiveView_Overlap");
	MapMember(&m_LiveView_WidescreenMode, "LiveView_WidescreenMode");
	MapMember(&m_LiveView_Saturation, "LiveView_Saturation");
	MapMember(&m_LiveView_Sensitivity, "LiveView_Sensitivity");
	MapMember(&m_Useinvert, "LiveView_invert");
	MapMember(&m_LiveView_HOverscanBorder, "LiveView_HOverscanBorder");
	MapMember(&m_LiveView_VOverscanBorder, "LiveView_VOverscanBorder");
	MapMember(&m_LiveView_DisplayNr, "LiveView_DisplayNr");
	MapMember(&m_LiveView_FrameDelay, "LiveView_FrameDelay");
	MapMember(&m_LiveView_GDI_FrameRate, "LiveView_GDI_FrameRate");
	MapMember(&m_ZonesTopCount, "ZonesTopCount");
	MapMember(&m_ZonesBottomCount, "ZonesBottomCount");
	MapMember(&m_ZonesLRCount, "ZonesLRCount");
	MapMember(&m_ZoneSummary, "ZoneSummary");
	MapMember(&m_Hardware_global_gamma, "Hardware_global_gamma");
	MapMember(&m_Hardware_global_contrast, "Hardware_global_contrast");
	MapMember(&m_Hardware_contrast_red, "Hardware_contrast_red");
	MapMember(&m_Hardware_contrast_green, "Hardware_contrast_green");
	MapMember(&m_Hardware_contrast_blue, "Hardware_contrast_blue");
	MapMember(&m_Hardware_gamma_red, "Hardware_gamma_red");
	MapMember(&m_Hardware_gamma_green, "Hardware_gamma_green");
	MapMember(&m_Hardware_gamma_blue, "Hardware_gamma_blue");
	MapMember(&m_Software_gamma_mode, "Software_gamma_mode");
	MapMember(&m_Software_gamma_red, "Software_gamma_red");
	MapMember(&m_Software_gamma_green, "Software_gamma_green");
	MapMember(&m_Software_gamma_blue, "Software_gamma_blue");
	MapMember(&m_Software_gamma_global, "Software_gamma_global");
	MapMember(&m_hAtmoClLeds, "hAtmoClLeds");

/*
	MapMember(&red_ColorK[256][0], "Software_ColK_rr");
	MapMember(&red_ColorK[256][1], "Software_ColK_rg");
	MapMember(&red_ColorK[256][2], "Software_ColK_rb");
	MapMember(&green_ColorK[256][0], "Software_ColK_gr");
	MapMember(&green_ColorK[256][1], "Software_ColK_gg");
	MapMember(&green_ColorK[256][2], "Software_ColK_gb");
	MapMember(&blue_ColorK[256][0], "Software_ColK_br");
	MapMember(&blue_ColorK[256][1], "Software_ColK_bg");
	MapMember(&blue_ColorK[256][2], "Software_ColK_bb");

	MapMember(&red_ColorK[257][0], "Software_ColK_yr");
	MapMember(&red_ColorK[257][1], "Software_ColK_yg");
	MapMember(&red_ColorK[257][2], "Software_ColK_yb");
	MapMember(&green_ColorK[257][0], "Software_ColK_mr");
	MapMember(&green_ColorK[257][1], "Software_ColK_mg");
	MapMember(&green_ColorK[257][2], "Software_ColK_mb");
	MapMember(&blue_ColorK[257][0], "Software_ColK_cr");
	MapMember(&blue_ColorK[257][1], "Software_ColK_cg");
	MapMember(&blue_ColorK[257][2], "Software_ColK_cb");

	MapMember(&red_grid[0], "Software_red_g_5");
	MapMember(&red_grid[1], "Software_red_g_10");
	MapMember(&red_grid[2], "Software_red_g_20");
	MapMember(&red_grid[3], "Software_red_g_30");
	MapMember(&red_grid[4], "Software_red_g_40");
	MapMember(&red_grid[5], "Software_red_g_50");
	MapMember(&red_grid[6], "Software_red_g_60");
	MapMember(&red_grid[7], "Software_red_g_70");
	MapMember(&red_grid[8], "Software_red_g_80");
	MapMember(&red_grid[9], "Software_red_g_90");
	MapMember(&red_grid[10], "Software_red_g_100");

	MapMember(&green_grid[0], "Software_green_g_5");
	MapMember(&green_grid[1], "Software_green_g_10");
	MapMember(&green_grid[2], "Software_green_g_20");
	MapMember(&green_grid[3], "Software_green_g_30");
	MapMember(&green_grid[4], "Software_green_g_40");
	MapMember(&green_grid[5], "Software_green_g_50");
	MapMember(&green_grid[6], "Software_green_g_60");
	MapMember(&green_grid[7], "Software_green_g_70");
	MapMember(&green_grid[8], "Software_green_g_80");
	MapMember(&green_grid[9], "Software_green_g_90");
	MapMember(&green_grid[10], "Software_green_g_100");

	MapMember(&blue_grid[0], "Software_blue_g_5");
	MapMember(&blue_grid[1], "Software_blue_g_10");
	MapMember(&blue_grid[2], "Software_blue_g_20");
	MapMember(&blue_grid[3], "Software_blue_g_30");
	MapMember(&blue_grid[4], "Software_blue_g_40");
	MapMember(&blue_grid[5], "Software_blue_g_50");
	MapMember(&blue_grid[6], "Software_blue_g_60");
	MapMember(&blue_grid[7], "Software_blue_g_70");
	MapMember(&blue_grid[8], "Software_blue_g_80");
	MapMember(&blue_grid[9], "Software_blue_g_90");
	MapMember(&blue_grid[10], "Software_blue_g_100");

*/
	MapMember(&m_DMX_BaseChannels, "DMX_BaseChannels");
	MapMember(&m_DMX_RGB_Channels, "DMX_RGB_Channels");
	MapMember(&m_DMX_BaudrateIndex, "DMX_BaudrateIndex");  
	MapMember(&m_Ardu_BaudrateIndex, "Ardu_BaudrateIndex");
	MapMember(&m_MoMo_Channels, "MoMo_Channels");
	MapMember(&m_Fnordlicht_Amount, "Fnordlicht_Amount");
	MapMember(&getNumChannelAssignments, "NumChannelAssignments");
	MapMember(&m_CurrentChannelAssignment, "CurrentChannelAssignment");

	// All of the <Section>'s under <Configuration> goes into this list
	MapMember(&m_lstMyObjects, MyWhiteAdjPerChannel::GetStaticTag());
}
