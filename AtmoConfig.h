/*
* AtmoConfig.h: Class for holding all configuration values of AtmoWin
*
* See the README.txt file for copyright information and how to reach the author(s).
*
* $Id: 62848dd3622ba418f03ee9d738a769f6bc6d12e9 $
*/

#ifndef _AtmoConfig_h_
#define _AtmoConfig_h_

#include <windows.h>
#include <stdlib.h>
#include "AtmoDefs.h"
#include "AtmoZoneDefinition.h"
#include "AtmoChannelAssignment.h"
#include <string>
#include <vector>
#include <fstream>

#if defined(_ATMO_VLC_PLUGIN_)
#   include <string.h>
#endif



class CAtmoConfig 
{

protected:
	int m_IsShowConfigDialog;
#if defined(_ATMO_VLC_PLUGIN_)
	char *m_devicename;
	char *m_devicenames[3]; // additional Devices ?
#else
	int m_Comport;
	int m_ArduComport;
	int m_Comports[3]; // additional Comports
#endif
	enum AtmoConnectionType m_eAtmoConnectionType;
	enum EffectMode m_eEffectMode;

	ATMO_BOOL m_IgnoreConnectionErrorOnStartup;

protected:
	ATMO_BOOL m_UseSoftwareWhiteAdj;
	ATMO_BOOL m_UseSoftware2WhiteAdj;
	ATMO_BOOL m_UseColorKWhiteAdj;
	ATMO_BOOL m_Use3dlut;
	ATMO_BOOL m_Useinvert;

	int m_WhiteAdjustment_Red;
	int m_WhiteAdjustment_Green;
	int m_WhiteAdjustment_Blue;
	int m_ColorK_Red[3];
	int m_ColorK_Green[3];
	int m_ColorK_Blue[3];

	ATMO_BOOL m_WhiteAdjPerChannel;
	int  m_chWhiteAdj_Count;
	int *m_chWhiteAdj_Red;
	int *m_chWhiteAdj_Green;
	int *m_chWhiteAdj_Blue;

protected:
	int m_IsSetShutdownColor;
	int m_ShutdownColor_Red;
	int m_ShutdownColor_Green;
	int m_ShutdownColor_Blue;

protected:
	/* Config Values for Color Changer */
	int m_ColorChanger_iSteps;
	int m_ColorChanger_iDelay;

protected:
	/* Config  values for the primitive Left Right Color Changer */
	int m_LrColorChanger_iSteps;
	int m_LrColorChanger_iDelay;

protected:
	/* the static background color */
	int m_StaticColor_Red;
	int m_StaticColor_Green;
	int m_StaticColor_Blue;

protected:
	/*
	one for System + 9 for userdefined channel
	assignments (will it be enough?)
	*/
	CAtmoChannelAssignment *m_ChannelAssignments[10];
	int m_CurrentChannelAssignment;

protected:
	CAtmoZoneDefinition **m_ZoneDefinitions;
	int m_AtmoZoneDefCount;


	/*
	zone layout description for generating the default Zone weightning
	*/
	// count of zone on the top of the screen
	int m_ZonesTopCount;
	// count of zone on the bottom of the screen
	int m_ZonesBottomCount;
	// count of zones on left and right (the same count)
	int m_ZonesLRCount;

	// does a summary Zone exists (Fullscreen)
	int m_computed_zones_count;
	ATMO_BOOL m_ZoneSummary;

public:
	int getZoneCount();
	ATMO_BOOL m_3dlut;
	std::string profile;
	std::string d_profile;


protected:
	/* Live View Parameters (most interesting) */
	AtmoFilterMode m_LiveViewFilterMode;
	int m_LiveViewFilter_PercentNew;
	int m_LiveViewFilter_MeanLength;
	int m_LiveViewFilter_MeanThreshold;

	ATMO_BOOL m_show_statistics;

	// number of rows to process each frame
	int m_LiveView_RowsPerFrame;

	// weighting of distance to edge
	int m_LiveView_EdgeWeighting; //  = 8;
	// brightness correction
	int m_LiveView_BrightCorrect; //  = 100;
	// darkness limit (pixels below this value will be ignored)
	int m_LiveView_DarknessLimit; //  = 5;
	// Windowing size for hue histogram building
	int m_LiveView_HueWinSize;    //  = 3;
	// Windowing size for sat histogram building
	int m_LiveView_SatWinSize;    //  = 3;
	// Blur Zone Overlap
	int m_LiveView_Overlap;       //  = 2;
	// daturation
	int m_LiveView_Saturation;
	// Sensitivity Filter low color Values
	int m_LiveView_Sensitivity;

	/*
	special (hack) for ignorning black borders durring
	playback of letterboxed material on a 16:9 output device
	*/
	int m_LiveView_WidescreenMode; // = 0

	// border from source image which should be ignored
	// the values are only used by the Win32 GDI Screen capture
	int m_LiveView_HOverscanBorder;
	int m_LiveView_VOverscanBorder;
	int m_LiveView_DisplayNr;

	/*
	a special delay to get the light in sync with the video
	was required because the frames will pass my VLC filter some [ms]
	before they become visible on screen with this delay - screenoutput
	and light timing could be "synchronized"
	*/
	int m_LiveView_FrameDelay;

	int m_LiveView_GDI_FrameRate;

protected:
	/* values of the last hardware white adjustment (only for hardware with new firmware) */
	int m_Hardware_global_gamma;
	int m_Hardware_global_contrast;
	int m_Hardware_contrast_red;
	int m_Hardware_contrast_green;
	int m_Hardware_contrast_blue;
	int m_Hardware_gamma_red;
	int m_Hardware_gamma_green;
	int m_Hardware_gamma_blue;

protected:
	char *m_DMX_BaseChannels;
	int m_DMX_RGB_Channels;
	int m_DMX_BaudrateIndex;
	int m_Ardu_BaudrateIndex;

protected:
	int m_MoMo_Channels;

protected:
	int m_Fnordlicht_Amount;

protected:
	AtmoGammaCorrect m_Software_gamma_mode;

	int m_Software_gamma_red;
	int m_Software_gamma_green;
	int m_Software_gamma_blue;
	int m_Software_gamma_global;
	int m_hAtmoClLeds;

public:
	volatile int m_UpdateEdgeWeightningFlag;

	//calib
	int red_grid[11];
	int green_grid[11];
	int blue_grid[11];
	int red_whiteAdj[256];
	int green_whiteAdj[256];
	int blue_whiteAdj[256];
	int red_ColorK[258][3];
	int green_ColorK[258][3];
	int blue_ColorK[258][3];
	int little_ColorCube[17][17][17][3];
	int ColorCube[2][2][2][3];

	std::vector<std::string> profiles;

public:
	CAtmoConfig();
	virtual ~CAtmoConfig();
	virtual void SaveSettings(std::string profile) {};
	virtual void LoadSettings(std::string profile) {};
	virtual void ReadXMLStringList(char *section, std::string, char *default_value) {};
  virtual void WriteXMLStringList(char *section, char *default_value){};
	void LoadDefaults();

	/*
	function to copy  the values of one configuration object to another
	will be used in windows settings dialog as backup if the user
	presses cancel
	*/
	void Assign(CAtmoConfig *pAtmoConfigSrc);

	void UpdateZoneDefinitionCount();

public:
	int isShowConfigDialog()            { return m_IsShowConfigDialog; }
	void setShowConfigDialog(int value) { m_IsShowConfigDialog = value; }

#if defined(_ATMO_VLC_PLUGIN_)
	char *getSerialDevice()               { return m_devicename; }
	void setSerialDevice(const char *newdevice) { free(m_devicename); if(newdevice) m_devicename = strdup(newdevice); else m_devicename = NULL; }
	char *getSerialDevice(int i);
	void setSerialDevice(int i,const char *pszNewDevice);
#else
	int getComport()                    { return m_Comport; }
	void setComport(int value)          { m_Comport = value; }
	int getComport(int i);
	void setComport(int i, int nr);

	int getArduComport()                    { return m_ArduComport; }
	void setArduComport(int value)          { m_ArduComport = value; }
	int getArduComport(int i);
	void setArduComport(int i, int nr);
#endif

	ATMO_BOOL getIgnoreConnectionErrorOnStartup() { return m_IgnoreConnectionErrorOnStartup; }
	void setIgnoreConnectionErrorOnStartup(ATMO_BOOL ignore) { m_IgnoreConnectionErrorOnStartup = ignore; }

	int getWhiteAdjustment_Red() { return m_WhiteAdjustment_Red;  }
	void setWhiteAdjustment_Red(int value) { m_WhiteAdjustment_Red = value; }
	int getWhiteAdjustment_Green() { return m_WhiteAdjustment_Green;  }
	void setWhiteAdjustment_Green(int value) { m_WhiteAdjustment_Green = value; }
	int getWhiteAdjustment_Blue() { return m_WhiteAdjustment_Blue;  }
	void setWhiteAdjustment_Blue(int value) { m_WhiteAdjustment_Blue = value; }
	ATMO_BOOL isUseSoftwareWhiteAdj() { return m_UseSoftwareWhiteAdj; }
	void setUseSoftwareWhiteAdj(ATMO_BOOL value) { m_UseSoftwareWhiteAdj = value; }

	ATMO_BOOL isUseSoftware2WhiteAdj() { return m_UseSoftware2WhiteAdj; }
	void setUseSoftware2WhiteAdj(ATMO_BOOL value) { m_UseSoftware2WhiteAdj = value; }
	ATMO_BOOL isUseColorKWhiteAdj() { return m_UseColorKWhiteAdj; }
	void setUseColorKWhiteAdj(ATMO_BOOL value) { m_UseColorKWhiteAdj = value; }
	ATMO_BOOL isUse3dlut() { return m_Use3dlut; }
	void setUse3dlut(ATMO_BOOL value) { m_Use3dlut = value; }
	ATMO_BOOL isUseinvert() { return m_Useinvert; }
	void setUseinvert(ATMO_BOOL value) { m_Useinvert = value; }

	/* White ADJ per Channel settings */
	ATMO_BOOL isWhiteAdjPerChannel() { return  m_WhiteAdjPerChannel; }
	void setWhiteAdjPerChannel( ATMO_BOOL value) { m_WhiteAdjPerChannel = value; }

	void setChannelWhiteAdj(int channel,int red,int green,int blue);
	void getChannelWhiteAdj(int channel,int &red,int &green,int &blue);

	int isSetShutdownColor()     { return m_IsSetShutdownColor; }
	void SetSetShutdownColor(int value) { m_IsSetShutdownColor = value; }
	int getShutdownColor_Red()   { return m_ShutdownColor_Red; }
	void setShutdownColor_Red(int value) { m_ShutdownColor_Red = value; }
	int getShutdownColor_Green() { return m_ShutdownColor_Green; }
	void setShutdownColor_Green(int value) { m_ShutdownColor_Green = value; }
	int getShutdownColor_Blue()  { return m_ShutdownColor_Blue; }
	void setShutdownColor_Blue(int value) { m_ShutdownColor_Blue=value; }

	int getColorChanger_iSteps() { return m_ColorChanger_iSteps; }
	void setColorChanger_iSteps(int value) { m_ColorChanger_iSteps = value; }
	int getColorChanger_iDelay() { return m_ColorChanger_iDelay; }
	void setColorChanger_iDelay(int value) { m_ColorChanger_iDelay = value; }

	int getLrColorChanger_iSteps() { return m_LrColorChanger_iSteps; }
	void setLrColorChanger_iSteps(int value) { m_LrColorChanger_iSteps = value; }
	int getLrColorChanger_iDelay() { return m_LrColorChanger_iDelay; }
	void setLrColorChanger_iDelay(int value) { m_LrColorChanger_iDelay = value; }

	int getStaticColor_Red()   { return m_StaticColor_Red;   }
	void setStaticColor_Red(int value)  { m_StaticColor_Red=value; }
	int getStaticColor_Green() { return m_StaticColor_Green; }
	void setStaticColor_Green(int value) { m_StaticColor_Green=value; }
	int getStaticColor_Blue()  { return m_StaticColor_Blue;  }
	void  setStaticColor_Blue(int value) { m_StaticColor_Blue=value; }


	AtmoConnectionType getConnectionType() { return m_eAtmoConnectionType; }
	void setConnectionType(AtmoConnectionType value) { m_eAtmoConnectionType = value; }

	EffectMode getEffectMode() { return m_eEffectMode; }
	void setEffectMode(EffectMode value) { m_eEffectMode = value; }

	ATMO_BOOL getShow_statistics() { return m_show_statistics; }

	AtmoFilterMode getLiveViewFilterMode() { return m_LiveViewFilterMode; }
	void setLiveViewFilterMode(AtmoFilterMode value) { m_LiveViewFilterMode = value; }

	int getLiveViewFilter_PercentNew() { return m_LiveViewFilter_PercentNew; }
	void setLiveViewFilter_PercentNew(int value) { m_LiveViewFilter_PercentNew=value; }
	int getLiveViewFilter_MeanLength() { return m_LiveViewFilter_MeanLength; }
	void setLiveViewFilter_MeanLength(int value) { m_LiveViewFilter_MeanLength = value; }
	int getLiveViewFilter_MeanThreshold() { return m_LiveViewFilter_MeanThreshold; }
	void setLiveViewFilter_MeanThreshold(int value) { m_LiveViewFilter_MeanThreshold = value; }

	int getLiveView_EdgeWeighting() { return m_LiveView_EdgeWeighting; }
	void setLiveView_EdgeWeighting(int value) { m_LiveView_EdgeWeighting=value; }

	int getLiveView_RowsPerFrame() { return m_LiveView_RowsPerFrame; }
	void setLiveView_RowsPerFrame(int value) { m_LiveView_RowsPerFrame=value; }

	int getLiveView_BrightCorrect() { return m_LiveView_BrightCorrect; }
	void setLiveView_BrightCorrect(int value) { m_LiveView_BrightCorrect=value; }

	int getLiveView_DarknessLimit() { return m_LiveView_DarknessLimit; }
	void setLiveView_DarknessLimit(int value) { m_LiveView_DarknessLimit=value; }

	int getLiveView_Saturation() { return m_LiveView_Saturation; }
	void setLiveView_Saturation(int value) { m_LiveView_Saturation=value; }

	int getLiveView_Sensitivity() { return m_LiveView_Sensitivity; }
	void setLiveView_Sensitivity(int value) { m_LiveView_Sensitivity=value; }


	int getLiveView_HueWinSize() { return m_LiveView_HueWinSize; }
	void setLiveView_HueWinSize(int value) { m_LiveView_HueWinSize=value; }

	int getLiveView_SatWinSize() { return m_LiveView_SatWinSize; }
	void setLiveView_SatWinSize(int value) { m_LiveView_SatWinSize=value; }

	int getLiveView_Overlap() { return m_LiveView_Overlap; }
	void setLiveView_Overlap(int value) { m_LiveView_Overlap=value; }

	int getLiveView_WidescreenMode() { return m_LiveView_WidescreenMode; }
	void setLiveView_WidescreenMode(int value) { m_LiveView_WidescreenMode=value; }

	int getLiveView_HOverscanBorder() { return m_LiveView_HOverscanBorder; }
	void setLiveView_HOverscanBorder(int value) { m_LiveView_HOverscanBorder = value; }

	int getLiveView_VOverscanBorder() { return m_LiveView_VOverscanBorder; }
	void setLiveView_VOverscanBorder(int value) { m_LiveView_VOverscanBorder = value; }

	int getLiveView_DisplayNr() { return m_LiveView_DisplayNr; }
	void setLiveView_DisplayNr(int value) { m_LiveView_DisplayNr = value; }

	int getLiveView_FrameDelay() { return m_LiveView_FrameDelay; }
	void setLiveView_FrameDelay(int delay) { m_LiveView_FrameDelay = delay; }

	int getLiveView_GDI_FrameRate() { return m_LiveView_GDI_FrameRate; }
	void setLiveView_GDI_FrameRate(int value) { m_LiveView_GDI_FrameRate=value; }

	int getHardware_global_gamma() { return m_Hardware_global_gamma ; }
	void setHardware_global_gamma(int value) { m_Hardware_global_gamma=value; }

	int getHardware_global_contrast() { return m_Hardware_global_contrast; }
	void setHardware_global_contrast(int value) { m_Hardware_global_contrast=value; }

	int getHardware_contrast_red() { return m_Hardware_contrast_red; }
	void setHardware_contrast_red(int value) { m_Hardware_contrast_red=value; }

	int getHardware_contrast_green() { return m_Hardware_contrast_green; }
	void setHardware_contrast_green(int value) { m_Hardware_contrast_green=value; }

	int getHardware_contrast_blue() { return m_Hardware_contrast_blue; }
	void setHardware_contrast_blue(int value) { m_Hardware_contrast_blue=value; }

	int getHardware_gamma_red() { return m_Hardware_gamma_red; }
	void setHardware_gamma_red(int value) { m_Hardware_gamma_red=value; }

	int getHardware_gamma_green() { return m_Hardware_gamma_green; }
	void setHardware_gamma_green(int value) { m_Hardware_gamma_green=value; }

	int getHardware_gamma_blue() { return m_Hardware_gamma_blue; }
	void setHardware_gamma_blue(int value) { m_Hardware_gamma_blue=value; }

	//atmoduino
	int getAtmoClLeds() { return m_hAtmoClLeds; }
	void setAtmoClLeds(int leds) { m_hAtmoClLeds = leds; } 



	AtmoGammaCorrect getSoftware_gamma_mode() { return m_Software_gamma_mode; }
	int getSoftware_gamma_red() { return m_Software_gamma_red; }
	int getSoftware_gamma_green() { return m_Software_gamma_green; }
	int getSoftware_gamma_blue() { return m_Software_gamma_blue; }
	int getSoftware_gamma_global() { return m_Software_gamma_global; }

	void setSoftware_gamma_mode(AtmoGammaCorrect value) { m_Software_gamma_mode = value; }
	void setSoftware_gamma_red(int value)    { m_Software_gamma_red = value; }
	void setSoftware_gamma_green(int value)  { m_Software_gamma_green = value; }
	void setSoftware_gamma_blue(int value)   { m_Software_gamma_blue = value; }
	void setSoftware_gamma_global(int value) { m_Software_gamma_global = value; }

	CAtmoChannelAssignment *getChannelAssignment(int nummer) {
		return this->m_ChannelAssignments[nummer];
	}
	int getCurrentChannelAssignment() { return m_CurrentChannelAssignment; }
	void setCurrentChannelAssignment(int index) { m_CurrentChannelAssignment = index; }

	int getNumChannelAssignments();
	void clearChannelMappings();
	void clearAllChannelMappings();
	void AddChannelAssignment(CAtmoChannelAssignment *ta);
	void SetChannelAssignment(int index, CAtmoChannelAssignment *ta);

	CAtmoZoneDefinition *getZoneDefinition(int zoneIndex);

	void UpdateZoneCount();

	void setZonesTopCount(int zones) { m_ZonesTopCount = zones; UpdateZoneCount(); };
	int getZonesTopCount() { return m_ZonesTopCount; }
	void setZonesBottomCount(int zones) { m_ZonesBottomCount = zones; UpdateZoneCount(); };
	int getZonesBottomCount() { return m_ZonesBottomCount; }
	void setZonesLRCount(int zones) { m_ZonesLRCount = zones; UpdateZoneCount(); };
	int getZonesLRCount() { return m_ZonesLRCount; }
	ATMO_BOOL getZoneSummary() { return m_ZoneSummary; }
	void setZoneSummary(ATMO_BOOL summary) { m_ZoneSummary = summary; UpdateZoneCount(); }

	char *getDMX_BaseChannels() { return m_DMX_BaseChannels; }
	void setDMX_BaseChannels(const char *channels);

	int getDMX_RGB_Channels() { return m_DMX_RGB_Channels; }
	void setDMX_RGB_Channels(int ch) { m_DMX_RGB_Channels = ch; }

	int getDMX_BaudrateIndex() { return m_DMX_BaudrateIndex; }
	void setDMX_BaudrateIndex(int baudrateIndex) { m_DMX_BaudrateIndex = baudrateIndex; }

	int getArdu_BaudrateIndex() { return m_Ardu_BaudrateIndex; }
	void setArdu_BaudrateIndex(int baudrateIndex) { m_Ardu_BaudrateIndex = baudrateIndex; }

	int getMoMo_Channels() { return m_MoMo_Channels; }
	void setMoMo_Channels(int chCount) { m_MoMo_Channels = chCount; }

	int getFnordlicht_Amount() { return m_Fnordlicht_Amount; }
	void setFnordlicht_Amount(int fnordlichtAmount) { m_Fnordlicht_Amount = fnordlichtAmount; }

};

#endif
