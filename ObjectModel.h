//#pragma once
#ifndef _OBJECT_MODEL_H__
#define _OBJECT_MODEL_H__

#include "xmlObject.h"
#include "GString.h"
#include "ObjectPointer.h"
#include "ObjectDataHandler.h"
#include "GArray.h"

extern int g_verify0ref;

class MySection : public XMLObject
{
	virtual void MapXMLTagsToMembers();

public:							    // All public example simplicity - not required
	GString m_strSection; // An attribute, not an element

	GString profile;
	GString d_profile;
	int m_eAtmoConnectionType;
	int m_eEffectMode;
	int m_Comport;
	int m_ArduComport;
	//int m_Comports[0];
  //int m_Comports[1];
	//int m_Comports[2]; 
	int m_WhiteAdjustment_Red;
	int m_WhiteAdjustment_Green;
	int m_WhiteAdjustment_Blue;
	int m_UseSoftwareWhiteAdj;
	int m_UseSoftware2WhiteAdj;
	int m_UseColorKWhiteAdj;
	int m_Use3dlut;
	int m_WhiteAdjPerChannel;
//  bool m_IgnoreConnectionErrorOnStartup;
	int m_ColorChanger_iSteps;
	int m_ColorChanger_iDelay;
	int m_LrColorChanger_iSteps;
	int m_LrColorChanger_iDelay;
	int m_StaticColor_Red;
	int m_StaticColor_Green;
	int m_StaticColor_Blue;
	int m_IsSetShutdownColor;
	int m_ShutdownColor_Red;
	int m_ShutdownColor_Green;
	int m_ShutdownColor_Blue;
	int m_LiveViewFilterMode;
	int m_LiveViewFilter_PercentNew;
	int m_LiveViewFilter_MeanLength;
	int m_LiveViewFilter_MeanThreshold;
	int m_LiveView_EdgeWeighting;
	int m_LiveView_RowsPerFrame;
	int m_LiveView_BrightCorrect;
	int m_LiveView_DarknessLimit;
	int m_LiveView_HueWinSize;
	int m_LiveView_SatWinSize;
	int m_LiveView_Overlap;
	int m_LiveView_WidescreenMode;
	int m_LiveView_Saturation;
	int m_LiveView_Sensitivity;
	int m_Useinvert;
	int m_LiveView_HOverscanBorder;
	int m_LiveView_VOverscanBorder;
	int m_LiveView_DisplayNr;
	int m_LiveView_FrameDelay;
	int m_LiveView_GDI_FrameRate;
	int m_ZonesTopCount;
	int m_ZonesBottomCount;
	int m_ZonesLRCount;
	int m_ZoneSummary;
	int m_Hardware_global_gamma;
	int m_Hardware_global_contrast;
	int m_Hardware_contrast_red;
	int m_Hardware_contrast_green;
	int m_Hardware_contrast_blue;
	int m_Hardware_gamma_red;
	int m_Hardware_gamma_green;
	int m_Hardware_gamma_blue;
	int m_Software_gamma_mode;
	int m_Software_gamma_red;
	int m_Software_gamma_green;
	int m_Software_gamma_blue;
	int m_Software_gamma_global;
	int m_hAtmoClLeds; 

/*
	int red_ColorK[256][0];
	int red_ColorK[256][1];
	int red_ColorK[256][2];
	int green_ColorK[256][0];
	int green_ColorK[256][1];
	int green_ColorK[256][2];
	int blue_ColorK[256][0];
	int blue_ColorK[256][1];
	int blue_ColorK[256][2];	

	int red_ColorK[257][0];
	int red_ColorK[257][1];
	int red_ColorK[257][2];
	int green_ColorK[257][0];
	int green_ColorK[257][1];
	int green_ColorK[257][2];
	int blue_ColorK[257][0];
	int blue_ColorK[257][1];
	int blue_ColorK[257][2];

	int red_grid[0];
	int red_grid[1];
	int red_grid[2];
	int red_grid[3];
	int red_grid[4];
	int red_grid[5];
	int red_grid[6];
	int red_grid[7];
	int red_grid[8];
	int red_grid[9];
	int red_grid[10];

	int green_grid[0];
	int green_grid[1];
	int green_grid[2];
	int green_grid[3];
	int green_grid[4];
	int green_grid[5];
	int green_grid[6];
	int green_grid[7];
	int green_grid[8];
	int green_grid[9];
	int green_grid[10];

	int blue_grid[0];
	int blue_grid[1];
	int blue_grid[2];
	int blue_grid[3];
	int blue_grid[4];
	int blue_grid[5];
	int blue_grid[6];
	int blue_grid[7];
	int blue_grid[8];
	int blue_grid[9];
	int blue_grid[10];
*/

	GString m_DMX_BaseChannels;
	int m_DMX_RGB_Channels;
	int m_DMX_BaudrateIndex;  
	int m_Ardu_BaudrateIndex; 
	int m_MoMo_Channels;
	int m_Fnordlicht_Amount;
	int getNumChannelAssignments;
	int m_CurrentChannelAssignment;	
	
	// 'this' type, followed by the XML Element name, normally DECLARE_FACTORY() is in an .h file
	DECLARE_FACTORY(MySection, section) 

	MySection(){} // keep one constructor with no arguments
	~MySection(){};
};

class MyConfiguration : public XMLObject
{

	virtual void MapXMLTagsToMembers();
public:

	// This list will hold instances of type MyCustomObject
	GList m_lstMyObjects;

	virtual long IncRef(StackFrameCheck *pStack =0, int nDeep=1)
	{
		return XMLObject::IncRef(pStack, nDeep);
	}

	DECLARE_FACTORY(MyConfiguration, configuration);

	MyConfiguration(){};
	~MyConfiguration(){};

};

class MyWhiteAdjPerChannel : public XMLObject
{
	virtual void MapXMLTagsToMembers();
public:
	int m_chWhiteAdj_Count; 
	int m_chWhiteAdj_Red;
	int m_chWhiteAdj_Green;
	int m_chWhiteAdj_Blue;

public:

	DECLARE_FACTORY(MyWhiteAdjPerChannel, WhiteAdjPerChannel);

	MyWhiteAdjPerChannel(){}
	~MyWhiteAdjPerChannel(){}

};

#endif //_OBJECT_MODEL_H__

