
#include "StdAfx.h"
#include "atmoregistryconfig.h"
#include "AtmoTools.h"



CAtmoRegistryConfig::CAtmoRegistryConfig(char *regKey) : CAtmoConfig()
{
  // Speicher Strukturen wenns welche gibt mit new holen
  configRootKey[0] = 0;
  if(regKey!=NULL)
     strcpy(this->configRootKey, regKey);
  m_IsShowConfigDialog = 0;
  psz_language = NULL;
}

CAtmoRegistryConfig::~CAtmoRegistryConfig(void)
{
  delete []psz_language;
}

void CAtmoRegistryConfig::SaveSettings(HKEY mykey, std::string profile1) {
    
	// alle Variabel etc. in Registry speichern
    char regKeyName[100],valueName[32];

	WriteRegistryString(mykey, configRootKey,"lastprofile",(char*)profile1.data());
	WriteRegistryString(mykey, configRootKey,"defaultprofile",(char*)d_profile.data());
	
	//profilenames
	WriteRegistryStringList(mykey, configRootKey,"profiles","-1");

	

	if (configRootKey!=NULL) strcpy(newconfigRootKey, configRootKey);

	if (profile1 != ""){
	    std::string key(configRootKey);
		std::string path(configRootKey);
		path.append(profile1);
		path.append("\\");
		strcpy(this->newconfigRootKey, path.data());
		}
	
	// don't destroy config in that case..
    if(m_eAtmoConnectionType != actNUL) WriteRegistryInt(mykey, configRootKey,"ConnectionType",(int)m_eAtmoConnectionType);
	WriteRegistryInt(mykey, configRootKey,"EffectMode",(int)m_eEffectMode);
	WriteRegistryInt(mykey, configRootKey,"comport",this->m_Comport);
	WriteRegistryInt(mykey, configRootKey,"Arducomport",this->m_ArduComport);
    WriteRegistryInt(mykey, configRootKey,"comport_1", m_Comports[0] );
    WriteRegistryInt(mykey, configRootKey,"comport_2", m_Comports[1] );
    WriteRegistryInt(mykey, configRootKey,"comport_3", m_Comports[2] );

	WriteRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Red",m_WhiteAdjustment_Red);
	WriteRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Green",m_WhiteAdjustment_Green);
	WriteRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Blue",m_WhiteAdjustment_Blue);
	WriteRegistryInt(mykey, newconfigRootKey,"UseSoftwareWhiteAdj",(int)m_UseSoftwareWhiteAdj);
	WriteRegistryInt(mykey, newconfigRootKey,"UseSoftware2WhiteAdj",(int)m_UseSoftware2WhiteAdj);
	WriteRegistryInt(mykey, newconfigRootKey,"UseColorK",(int)m_UseColorKWhiteAdj);
	WriteRegistryInt(mykey, newconfigRootKey,"Use3dlut",(int)m_Use3dlut);

    
    WriteRegistryInt(mykey, newconfigRootKey,"UsePerChWhiteAdj",(int)m_WhiteAdjPerChannel);

    sprintf(regKeyName,"%sWhiteAdjPerChannel\\",newconfigRootKey);
    WriteRegistryInt(mykey, regKeyName,"count",(int)m_chWhiteAdj_Count);

    for(int i=0; (i<m_chWhiteAdj_Count) && m_chWhiteAdj_Red && m_chWhiteAdj_Blue && m_chWhiteAdj_Green; i++) 
    {
        sprintf(valueName,"red_%d",i);
        WriteRegistryInt(mykey, regKeyName, valueName, m_chWhiteAdj_Red[i] );

        sprintf(valueName,"green_%d",i);
        WriteRegistryInt(mykey, regKeyName, valueName, m_chWhiteAdj_Green[i] );

        sprintf(valueName,"blue_%d",i);
        WriteRegistryInt(mykey, regKeyName, valueName, m_chWhiteAdj_Blue[i] );
    }


    WriteRegistryInt(mykey, newconfigRootKey,"ColorChanger_iSteps",this->m_ColorChanger_iSteps);
    WriteRegistryInt(mykey, newconfigRootKey,"ColorChanger_iDelay",this->m_ColorChanger_iDelay);

    WriteRegistryInt(mykey, newconfigRootKey,"LrColorChanger_iSteps",this->m_LrColorChanger_iSteps);
    WriteRegistryInt(mykey, newconfigRootKey,"LrColorChanger_iDelay",this->m_LrColorChanger_iDelay);

    WriteRegistryInt(mykey, newconfigRootKey,"StaticColor_Red",this->m_StaticColor_Red);
    WriteRegistryInt(mykey, newconfigRootKey,"StaticColor_Green",this->m_StaticColor_Green);
    WriteRegistryInt(mykey, newconfigRootKey,"StaticColor_Blue",this->m_StaticColor_Blue);

    WriteRegistryInt(mykey, newconfigRootKey,"isSetShutdownColor",m_IsSetShutdownColor);
	WriteRegistryInt(mykey, newconfigRootKey,"ShutdownColor_red",m_ShutdownColor_Red);
	WriteRegistryInt(mykey, newconfigRootKey,"ShutdownColor_green",m_ShutdownColor_Green);
	WriteRegistryInt(mykey, newconfigRootKey,"ShutdownColor_blue",m_ShutdownColor_Blue);

    WriteRegistryInt(mykey, newconfigRootKey,"LiveViewFilterMode",(int)m_LiveViewFilterMode);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_PercentNew",m_LiveViewFilter_PercentNew);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_MeanLength",m_LiveViewFilter_MeanLength);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_MeanThreshold",m_LiveViewFilter_MeanThreshold);

    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_EdgeWeighting",m_LiveView_EdgeWeighting);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_RowsPerFrame",m_LiveView_RowsPerFrame);

    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_BrightCorrect",m_LiveView_BrightCorrect);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_DarknessLimit",m_LiveView_DarknessLimit);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_HueWinSize",m_LiveView_HueWinSize);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_SatWinSize",m_LiveView_SatWinSize);
      WriteRegistryInt(mykey, newconfigRootKey,"LiveView_Overlap",m_LiveView_Overlap);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_WidescreenMode",m_LiveView_WidescreenMode);
	WriteRegistryInt(mykey, newconfigRootKey,"LiveView_Mode",m_LiveView_Mode);
//
 WriteRegistryInt(mykey, newconfigRootKey,"LiveView_Saturation",m_LiveView_Saturation);
 WriteRegistryInt(mykey, newconfigRootKey,"LiveView_Sensitivity",m_LiveView_Sensitivity);
 WriteRegistryInt(mykey, newconfigRootKey,"LiveView_invert",m_Useinvert);

    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_HOverscanBorder",m_LiveView_HOverscanBorder);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_VOverscanBorder",m_LiveView_VOverscanBorder);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_DisplayNr",m_LiveView_DisplayNr);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_FrameDelay",m_LiveView_FrameDelay);
    WriteRegistryInt(mykey, newconfigRootKey,"LiveView_GDI_FrameRate",m_LiveView_GDI_FrameRate);

    WriteRegistryInt(mykey, newconfigRootKey,"ZonesTopCount",m_ZonesTopCount);
    WriteRegistryInt(mykey, newconfigRootKey,"ZonesBottomCount",m_ZonesBottomCount);
    WriteRegistryInt(mykey, newconfigRootKey,"ZonesLRCount",m_ZonesLRCount);
    WriteRegistryInt(mykey, newconfigRootKey,"ZoneSummary",m_ZoneSummary);
    


    WriteRegistryInt(mykey, newconfigRootKey,"Hardware_global_gamma",m_Hardware_global_gamma);
    WriteRegistryInt(mykey, newconfigRootKey,"Hardware_global_contrast",m_Hardware_global_contrast);
    WriteRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_red",m_Hardware_contrast_red);
    WriteRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_green",m_Hardware_contrast_green);
    WriteRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_blue",m_Hardware_contrast_blue);
    WriteRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_red",m_Hardware_gamma_red);
    WriteRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_green",m_Hardware_gamma_green);
    WriteRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_blue",m_Hardware_gamma_blue);

    WriteRegistryInt(mykey, newconfigRootKey,"Software_gamma_mode",(int)m_Software_gamma_mode);
    WriteRegistryInt(mykey, newconfigRootKey,"Software_gamma_red",m_Software_gamma_red);
    WriteRegistryInt(mykey, newconfigRootKey,"Software_gamma_green",m_Software_gamma_green);
    WriteRegistryInt(mykey, newconfigRootKey,"Software_gamma_blue",m_Software_gamma_blue);
    WriteRegistryInt(mykey, newconfigRootKey,"Software_gamma_global",m_Software_gamma_global);

	WriteRegistryInt(mykey, newconfigRootKey,"hAtmoClLeds", m_hAtmoClLeds); 

//calib
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_rr",red_ColorK[256][0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_rg",red_ColorK[256][1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_rb",red_ColorK[256][2]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_gr",green_ColorK[256][0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_gg",green_ColorK[256][1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_gb",green_ColorK[256][2]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_br",blue_ColorK[256][0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_bg",blue_ColorK[256][1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_bb",blue_ColorK[256][2]);

	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_yr",red_ColorK[257][0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_yg",red_ColorK[257][1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_yb",red_ColorK[257][2]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_mr",green_ColorK[257][0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_mg",green_ColorK[257][1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_mb",green_ColorK[257][2]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_cr",blue_ColorK[257][0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_cg",blue_ColorK[257][1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_ColK_cb",blue_ColorK[257][2]);


	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_5",red_grid[0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_10",red_grid[1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_20",red_grid[2]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_30",red_grid[3]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_40",red_grid[4]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_50",red_grid[5]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_60",red_grid[6]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_70",red_grid[7]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_80",red_grid[8]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_90",red_grid[9]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_red_g_100",red_grid[10]);

	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_5",green_grid[0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_10",green_grid[1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_20",green_grid[2]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_30",green_grid[3]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_40",green_grid[4]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_50",green_grid[5]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_60",green_grid[6]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_70",green_grid[7]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_80",green_grid[8]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_90",green_grid[9]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_green_g_100",green_grid[10]);

	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_5",blue_grid[0]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_10",blue_grid[1]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_20",blue_grid[2]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_30",blue_grid[3]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_40",blue_grid[4]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_50",blue_grid[5]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_60",blue_grid[6]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_70",blue_grid[7]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_80",blue_grid[8]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_90",blue_grid[9]);
	WriteRegistryInt(mykey, newconfigRootKey,"Software_blue_g_100",blue_grid[10]);

//

    WriteRegistryString(mykey,  newconfigRootKey, "DMX_BaseChannels", m_DMX_BaseChannels );
    WriteRegistryInt(mykey, newconfigRootKey,"DMX_RGB_Channels",m_DMX_RGB_Channels);
    WriteRegistryInt(mykey, newconfigRootKey,"DMX_BaudrateIndex", m_DMX_BaudrateIndex );  
	WriteRegistryInt(mykey, newconfigRootKey,"Ardu_BaudrateIndex", m_Ardu_BaudrateIndex ); 

    WriteRegistryInt(mykey, newconfigRootKey,"MoMo_Channels", m_MoMo_Channels );

    WriteRegistryInt(mykey, newconfigRootKey,"Fnordlicht_Amount", m_Fnordlicht_Amount );
    

    WriteRegistryInt(mykey, configRootKey,"NumChannelAssignments",getNumChannelAssignments());
    WriteRegistryInt(mykey, newconfigRootKey,"CurrentChannelAssignment",m_CurrentChannelAssignment);
    for(int i=1;i<10;i++) {
        CAtmoChannelAssignment *ta = this->m_ChannelAssignments[i];
        if(ta!=NULL) {
           sprintf(regKeyName,"%sChannelAssignment_%d\\",configRootKey,i);
           WriteRegistryString(mykey, regKeyName,"name", ta->getName() );
           WriteRegistryInt(mykey, regKeyName,"count", ta->getSize() );
           for(int c=0;c<ta->getSize();c++) {
               sprintf(valueName,"channel_%d",c);
               WriteRegistryInt(mykey, regKeyName,valueName,ta->getZoneIndex(c));
           }
        }
    }
}

int CAtmoRegistryConfig::trilinear(int x, int y, int z, int col)
{

int x0 = (int)x/17;
int x1 = x0 + 1;
if (x1 > 255) {x1 = x0;}
int y0 = (int)y/17;
int y1 = y0 + 1;
if (y1 > 255) {y1 = y0;}
int z0 = (int)z/17;
int z1 = z0 + 1;
if (z1 > 255) {z1 = z0;}

double xd = (x1-x/17.0f);
double yd = (y1-y/17.0f);
double zd = (z1-z/17.0f);

//double c00 = little_ColorCube[x0][y0][z0][col]*(1-xd) + little_ColorCube[x1][y0][z0][col]*xd;
//double c10 = little_ColorCube[x0][y1][z0][col]*(1-xd) + little_ColorCube[x1][y1][z0][col]*xd;
//double c01 = little_ColorCube[x0][y0][z1][col]*(1-xd) + little_ColorCube[x1][y0][z1][col]*xd;
//double c11 = little_ColorCube[x0][y1][z1][col]*(1-xd) + little_ColorCube[x1][y1][z1][col]*xd;
//
//double c0 = c00*(1-yd) + c10*yd;
//double c1 = c01*(1-yd) + c11*yd;
//
//return c0*(1-zd) + c1*zd;

return little_ColorCube[x0][y0][z0][col]*(1-xd)*(1-yd)*(1-zd)+ 
       little_ColorCube[x1][y0][z0][col]*(1-yd)*(1-zd)*xd+
	   little_ColorCube[x0][y1][z0][col]*(1-xd)*(1-zd)*yd+
	   little_ColorCube[x0][y0][z1][col]*(1-xd)*(1-yd)*zd+
	   little_ColorCube[x1][y0][z1][col]*xd*(1-yd)*zd+
	   little_ColorCube[x0][y1][z1][col]*(1-xd)*yd*zd+
	   little_ColorCube[x1][y1][z0][col]*(1-zd)*xd*yd+
	   little_ColorCube[x1][y1][z1][col]*xd*yd*zd;

};

void CAtmoRegistryConfig::LoadSettings(HKEY mykey, std::string profile1) 
{
     // alle Variabel etc. aus Registry lesen
    char regKeyName[100],valueName[32];

	//profilenames
	ReadRegistryStringList(mykey, configRootKey,"profiles","-1");

	if (profile1 == "startup") {
		profile = ReadRegistryString(mykey, configRootKey,"lastprofile","");
		d_profile = ReadRegistryString(mykey, configRootKey,"defaultprofile","");
		if (d_profile!="") profile=d_profile;
		profile1=profile;}
	
	if (configRootKey!=NULL) strcpy(newconfigRootKey, configRootKey);
	
	
	    std::string key(configRootKey);
		key.append(profile1);
	if (profile1 != "" && RegistryKeyExists(mykey,(char*)key.data())==1) {
		std::string path(configRootKey);
		path.append(profile1);
		path.append("\\");
		strcpy(this->newconfigRootKey, path.data());
	    }
	    else profile="";


	//if (profile != "") 	sprintf(newconfigRootKey,(profile.append("\\")).data(),configRootKey);

    // m_IgnoreConnectionErrorOnStartup 
    if(ReadRegistryInt(mykey, configRootKey,"IgnoreConnectionErrorOnStartup",0) == 1)
       m_IgnoreConnectionErrorOnStartup = ATMO_TRUE;  

    m_eAtmoConnectionType = (AtmoConnectionType)ReadRegistryInt(mykey, configRootKey,"ConnectionType",(int)actClassicAtmo);


	m_Comport  = ReadRegistryInt(mykey, configRootKey,"comport",-1); // -1 als Indikator ob Setup noch erfolgen muss!
    if(m_Comport < 1) { m_IsShowConfigDialog = 1; m_Comport=1; } // die Variable beim Laden der Settings immer setzen wenn ein key uns nicht gefällt - so das der Setup Dialog
	                                          // zwanghaft gezeigt wird - dafür sparen wir uns das Flag minimiert - solange die Config Ok ist - brauchen
	                                          // wir den Dialog ja nicht - dafür gibts ja das Trayicon?

    m_Comports[0] = ReadRegistryInt(mykey, configRootKey,"comport_1",-1);
    m_Comports[1] = ReadRegistryInt(mykey, configRootKey,"comport_2",-1);
    m_Comports[2] = ReadRegistryInt(mykey, configRootKey,"comport_3",-1);

	m_ArduComport  = ReadRegistryInt(mykey, configRootKey,"Arducomport",-1); // -1 als Indikator ob Setup noch erfolgen muss!
    if(m_ArduComport < 1) { m_IsShowConfigDialog = 1; m_ArduComport=1; } // die Variable beim Laden der Settings immer setzen wenn ein key uns nicht gefällt - so das der Setup Dialog
	

    psz_language    = ReadRegistryString(mykey,  configRootKey, "language", "de");  

    m_eEffectMode = (EffectMode)ReadRegistryInt(mykey, configRootKey,"EffectMode",(int)m_eEffectMode);

    m_WhiteAdjustment_Red    = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Red",m_WhiteAdjustment_Red) );
    m_WhiteAdjustment_Green  = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Green",m_WhiteAdjustment_Green) );
    m_WhiteAdjustment_Blue   = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Blue",m_WhiteAdjustment_Blue) );
	m_UseSoftwareWhiteAdj    = (ReadRegistryInt(mykey, newconfigRootKey,"UseSoftwareWhiteAdj",m_UseSoftwareWhiteAdj) != 0);
	  //m_UseSoftware2WhiteAdj    = (ReadRegistryInt(mykey, newconfigRootKey,"UseSoftware2WhiteAdj",m_UseSoftware2WhiteAdj) != 0);
	  m_UseSoftware2WhiteAdj    = 0;
	m_UseColorKWhiteAdj		  = (ReadRegistryInt(mykey, newconfigRootKey,"UseColorK",m_UseColorKWhiteAdj) != 0);
	m_Use3dlut		  = (ReadRegistryInt(mykey, newconfigRootKey,"Use3dlut",m_Use3dlut) != 0);


    m_WhiteAdjPerChannel     = (ReadRegistryInt(mykey, newconfigRootKey,"UsePerChWhiteAdj",m_WhiteAdjPerChannel) != 0);
    sprintf(regKeyName,"%sWhiteAdjPerChannel\\",newconfigRootKey);
    m_chWhiteAdj_Count       = ReadRegistryInt(mykey, regKeyName,"count",0);
    delete []m_chWhiteAdj_Red;
    delete []m_chWhiteAdj_Green;
    delete []m_chWhiteAdj_Blue; 
    if(m_chWhiteAdj_Count > 0) 
    { 
       m_chWhiteAdj_Red   = new int[ m_chWhiteAdj_Count ];
       m_chWhiteAdj_Green = new int[ m_chWhiteAdj_Count ];
       m_chWhiteAdj_Blue  = new int[ m_chWhiteAdj_Count ];
    } else {
       m_chWhiteAdj_Red   = NULL;
       m_chWhiteAdj_Green = NULL;
       m_chWhiteAdj_Blue  = NULL;
    }
    for(int i=0; (i<m_chWhiteAdj_Count); i++) 
    {
        sprintf(valueName,"red_%d",i);
        m_chWhiteAdj_Red[i] = Check8BitValue( ReadRegistryInt(mykey, regKeyName, valueName, 256 ) );

        sprintf(valueName,"green_%d",i);
        m_chWhiteAdj_Green[i] = Check8BitValue( ReadRegistryInt(mykey, regKeyName, valueName, 256 ) );

        sprintf(valueName,"blue_%d",i);
        m_chWhiteAdj_Blue[i] = Check8BitValue( ReadRegistryInt(mykey, regKeyName, valueName, 256 ) );
    }


	m_ColorChanger_iSteps    = ReadRegistryInt(mykey, newconfigRootKey,"ColorChanger_iSteps",m_ColorChanger_iSteps);
	m_ColorChanger_iDelay    = ReadRegistryInt(mykey, newconfigRootKey,"ColorChanger_iDelay",m_ColorChanger_iDelay);

	m_LrColorChanger_iSteps    = ReadRegistryInt(mykey, newconfigRootKey,"LrColorChanger_iSteps",m_LrColorChanger_iSteps);
	m_LrColorChanger_iDelay    = ReadRegistryInt(mykey, newconfigRootKey,"LrColorChanger_iDelay",m_LrColorChanger_iDelay);

    m_IsSetShutdownColor   = ReadRegistryInt(mykey, newconfigRootKey,"isSetShutdownColor",m_IsSetShutdownColor);
	m_ShutdownColor_Red    = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"ShutdownColor_red",m_ShutdownColor_Red  ));
	m_ShutdownColor_Green  = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"ShutdownColor_green",m_ShutdownColor_Green));
	m_ShutdownColor_Blue   = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"ShutdownColor_blue",m_ShutdownColor_Blue) );

    m_StaticColor_Red   = ReadRegistryInt(mykey, newconfigRootKey,"StaticColor_Red",m_StaticColor_Red);
    m_StaticColor_Green = ReadRegistryInt(mykey, newconfigRootKey,"StaticColor_Green",m_StaticColor_Green);
    m_StaticColor_Blue  = ReadRegistryInt(mykey, newconfigRootKey,"StaticColor_Blue",m_StaticColor_Blue);

    m_LiveViewFilterMode       = (AtmoFilterMode)ReadRegistryInt(mykey, newconfigRootKey,"LiveViewFilterMode",(int)m_LiveViewFilterMode);
    m_LiveViewFilter_PercentNew      = ReadRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_PercentNew",m_LiveViewFilter_PercentNew);
    m_LiveViewFilter_MeanLength      = ReadRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_MeanLength",m_LiveViewFilter_MeanLength);
    m_LiveViewFilter_MeanThreshold   = ReadRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_MeanThreshold",m_LiveViewFilter_MeanThreshold);

    m_show_statistics                = (ReadRegistryInt(mykey, newconfigRootKey,"show_statistics",ATMO_FALSE) != 0);  

    m_LiveView_RowsPerFrame   = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_RowsPerFrame",m_LiveView_RowsPerFrame);
    if(m_LiveView_RowsPerFrame < 1) m_LiveView_RowsPerFrame = 1;
    if(m_LiveView_RowsPerFrame >= CAP_HEIGHT) m_LiveView_RowsPerFrame = CAP_HEIGHT - 1;

    m_LiveView_EdgeWeighting  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_EdgeWeighting",m_LiveView_EdgeWeighting);
    m_LiveView_BrightCorrect  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_BrightCorrect",m_LiveView_BrightCorrect);
    m_LiveView_DarknessLimit  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_DarknessLimit",m_LiveView_DarknessLimit);
    m_LiveView_HueWinSize     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_HueWinSize",m_LiveView_HueWinSize);
    m_LiveView_SatWinSize     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_SatWinSize",m_LiveView_SatWinSize);
       m_LiveView_Overlap     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_Overlap",m_LiveView_Overlap);
	   m_LiveView_Saturation     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_Saturation",100);
	   m_LiveView_Sensitivity     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_Sensitivity",0);
	   m_Useinvert          = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_invert",0);
	
	m_LiveView_WidescreenMode = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_WidescreenMode",m_LiveView_WidescreenMode);
	m_LiveView_Mode = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_Mode",m_LiveView_Mode);
	
    m_LiveView_HOverscanBorder  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_HOverscanBorder",m_LiveView_HOverscanBorder);
    m_LiveView_VOverscanBorder  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_VOverscanBorder",m_LiveView_VOverscanBorder);
    m_LiveView_DisplayNr        = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_DisplayNr",m_LiveView_DisplayNr);

    m_LiveView_FrameDelay      = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_FrameDelay",m_LiveView_FrameDelay);

    m_LiveView_GDI_FrameRate   = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_GDI_FrameRate",m_LiveView_GDI_FrameRate);

    m_ZonesTopCount            = ReadRegistryInt(mykey, newconfigRootKey,"ZonesTopCount",m_ZonesTopCount);
    m_ZonesBottomCount         = ReadRegistryInt(mykey, newconfigRootKey,"ZonesBottomCount",m_ZonesBottomCount);
    m_ZonesLRCount             = ReadRegistryInt(mykey, newconfigRootKey,"ZonesLRCount",m_ZonesLRCount);
    m_ZoneSummary              = (ReadRegistryInt(mykey, newconfigRootKey,"ZoneSummary",m_ZoneSummary) != 0);
    UpdateZoneCount();


    m_Hardware_global_gamma    = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_global_gamma",m_Hardware_global_gamma); // 0..255
    m_Hardware_global_contrast = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_global_contrast",m_Hardware_global_contrast);
    m_Hardware_contrast_red    = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_red",m_Hardware_contrast_red);
    m_Hardware_contrast_green  = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_green",m_Hardware_contrast_green);
    m_Hardware_contrast_blue   = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_blue",m_Hardware_contrast_blue);

    m_Hardware_gamma_red       = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_red",m_Hardware_gamma_red);
    m_Hardware_gamma_green     = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_green",m_Hardware_gamma_green);
    m_Hardware_gamma_blue      = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_blue",m_Hardware_gamma_blue);


    m_Software_gamma_mode   = (AtmoGammaCorrect)ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_mode",(int)m_Software_gamma_mode);
    m_Software_gamma_red    = ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_red",m_Software_gamma_red);
    m_Software_gamma_green  = ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_green",m_Software_gamma_green);
    m_Software_gamma_blue   = ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_blue",m_Software_gamma_blue);
    m_Software_gamma_global = ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_global",m_Software_gamma_global);
    
    m_Software_gamma_red    = min(max(m_Software_gamma_red, 10), 350);  // 0,5 - 3,5
    m_Software_gamma_green  = min(max(m_Software_gamma_green, 10), 350);  // 0,5 - 3,5
    m_Software_gamma_blue   = min(max(m_Software_gamma_blue, 10), 350);  // 0,5 - 3,5
    m_Software_gamma_global = min(max(m_Software_gamma_global, 10), 350);  // 0,5 - 3,5

//calib
	
	//pfad holen
	char workdir[MAX_PATH];
	char _lut[10] = "3Dlut.3dl";
    GetModuleFileName(GetModuleHandle(NULL),workdir,MAX_PATH);
    // strip of everything after last "\"
    for(size_t i=(strlen(workdir)-1); i > 1 ; i--) {  /*  c:\*/
        if(workdir[i] == '\\')  {
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
	 if (little_ColorCube[16][16][16][2] != -1) m_3dlut = true;
	 
	 red_ColorK[256][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_rr",255);
	 red_ColorK[256][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_rg",0);
	 red_ColorK[256][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_rb",0);
	  
	 green_ColorK[256][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_gr",0);
	 green_ColorK[256][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_gg",255);
	 green_ColorK[256][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_gb",0);
	  
	 blue_ColorK[256][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_br",0);
	 blue_ColorK[256][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_bg",0);
	 blue_ColorK[256][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_bb",255);
	
	 red_ColorK[257][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_yr",255);
	 red_ColorK[257][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_yg",255);
	 red_ColorK[257][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_yb",0);
	  
	 green_ColorK[257][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_mr",255);
	 green_ColorK[257][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_mg",0);
	 green_ColorK[257][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_mb",255);
	  
	 blue_ColorK[257][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_cr",0);
	 blue_ColorK[257][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_cg",255);
	 blue_ColorK[257][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_cb",255);
  	 
	 red_grid[0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_5",12);
	 red_grid[1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_10",25);
	 red_grid[2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_20",51);
	 red_grid[3]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_30",76);
	 red_grid[4]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_40",102);
	 red_grid[5]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_50",127);
	 red_grid[6]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_60",153);
	 red_grid[7]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_70",178);
	 red_grid[8]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_80",204);
	 red_grid[9]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_90",229);
	 red_grid[10]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_100",255);

	 green_grid[0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_5",12);
	 green_grid[1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_10",25);
	 green_grid[2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_20",51);
	 green_grid[3]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_30",76);
	 green_grid[4]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_40",102);
	 green_grid[5]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_50",127);
	 green_grid[6]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_60",153);
	 green_grid[7]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_70",178);
	 green_grid[8]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_80",204);
	 green_grid[9]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_90",229);
	 green_grid[10]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_100",255);

	 blue_grid[0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_5",12);
	 blue_grid[1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_10",25);
	 blue_grid[2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_20",51);
	 blue_grid[3]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_30",76);
	 blue_grid[4]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_40",102);
	 blue_grid[5]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_50",127);
	 blue_grid[6]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_60",153);
	 blue_grid[7]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_70",178);
	 blue_grid[8]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_80",204);
	 blue_grid[9]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_90",229);
	 blue_grid[10]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_100",255);

	 ColorCube[0][0][0][0]=0;ColorCube[0][0][0][1]=0;ColorCube[0][0][0][2]=0; //black
	 ColorCube[1][0][0][0]=red_ColorK[256][0];ColorCube[1][0][0][1]=red_ColorK[256][1];ColorCube[1][0][0][2]=red_ColorK[256][2]; //red
	 ColorCube[0][1][0][0]=green_ColorK[256][0];ColorCube[0][1][0][1]=green_ColorK[256][1];ColorCube[0][1][0][2]=green_ColorK[256][2]; //green
	 ColorCube[0][0][1][0]=blue_ColorK[256][0];ColorCube[0][0][1][1]=blue_ColorK[256][1];ColorCube[0][0][1][2]=blue_ColorK[256][2]; //blue
	 ColorCube[1][1][0][0]=red_ColorK[257][0];ColorCube[1][1][0][1]=red_ColorK[257][1];ColorCube[1][1][0][2]=red_ColorK[257][2]; //yellow
	 ColorCube[1][0][1][0]=green_ColorK[257][0];ColorCube[1][0][1][1]=green_ColorK[257][1];ColorCube[1][0][1][2]=green_ColorK[257][2]; //magenta
	 ColorCube[0][1][1][0]=blue_ColorK[257][0];ColorCube[0][1][1][1]=blue_ColorK[257][1];ColorCube[0][1][1][2]=blue_ColorK[257][2]; //cyan
	 ColorCube[1][1][1][0]=red_grid[10] ;ColorCube[1][1][1][1]=green_grid[10];ColorCube[1][1][1][2]=blue_grid[10]; //white
	
    int tmpChannel          = ReadRegistryInt(mykey, newconfigRootKey, "DMX_BaseChannel", 0 );
    if((tmpChannel < 0) || (tmpChannel>253)) tmpChannel = 0;
    char buf[16];
    sprintf(buf,"%d",tmpChannel);
    char *dmx_ch = ReadRegistryString(mykey,  newconfigRootKey, "DMX_BaseChannels", buf);
    this->setDMX_BaseChannels( dmx_ch );
    free( dmx_ch );

	//atmoduino
	m_hAtmoClLeds      = ReadRegistryInt(mykey, newconfigRootKey,"hAtmoClLeds", m_hAtmoClLeds ); 

    m_DMX_RGB_Channels      = ReadRegistryInt(mykey, newconfigRootKey,"DMX_RGB_Channels", m_DMX_RGB_Channels );
    m_DMX_BaudrateIndex     = ReadRegistryInt(mykey, newconfigRootKey,"DMX_BaudrateIndex", m_DMX_BaudrateIndex );  
	m_Ardu_BaudrateIndex     = ReadRegistryInt(mykey, newconfigRootKey,"Ardu_BaudrateIndex", m_Ardu_BaudrateIndex );  

    m_MoMo_Channels         = ReadRegistryInt(mykey, newconfigRootKey,"MoMo_Channels", m_MoMo_Channels );
    if(m_MoMo_Channels > 254) m_MoMo_Channels = 254;
    if(m_MoMo_Channels < 0)   m_MoMo_Channels = 2;

    m_Fnordlicht_Amount     = ReadRegistryInt(mykey, newconfigRootKey,"Fnordlicht_Amount", m_Fnordlicht_Amount );
    if(m_Fnordlicht_Amount > 254) m_Fnordlicht_Amount = 254;
    if(m_Fnordlicht_Amount < 0)   m_Fnordlicht_Amount = 1;

    clearChannelMappings(); // clear channel mappings except default!
    m_CurrentChannelAssignment = ReadRegistryInt(mykey, newconfigRootKey,"CurrentChannelAssignment",m_CurrentChannelAssignment);
    int numChannels = ReadRegistryInt(mykey, configRootKey,"NumChannelAssignments",0);
    if(m_CurrentChannelAssignment>=numChannels)
       m_CurrentChannelAssignment = 0;

    for(int i=1;i<numChannels;i++) {
        CAtmoChannelAssignment *ta = this->m_ChannelAssignments[i];
        if(ta==NULL) {
           ta = new CAtmoChannelAssignment();
           this->m_ChannelAssignments[i] = ta;
        }

        sprintf(regKeyName,"%sChannelAssignment_%d\\",configRootKey,i);
        char *name = ReadRegistryString(mykey, regKeyName,"name","?");
        ta->setName(name);
        ta->system = ATMO_FALSE;
        free(name);
        int chCount = ReadRegistryInt(mykey, regKeyName,"count",0);

        ta->setSize(chCount);

        for(int c=0;c<chCount;c++) {
            sprintf(valueName,"channel_%d",c);
            ta->setZoneIndex(c, ReadRegistryInt(mykey, regKeyName,valueName,c) );
        }
    }
	
    UpdateZoneDefinitionCount();
}

void CAtmoRegistryConfig::fastLoadSettings(HKEY mykey, std::string profile1) 
{
     // alle Variabel etc. aus Registry lesen
    char regKeyName[100],valueName[32];
	
	if (configRootKey!=NULL) strcpy(newconfigRootKey, configRootKey);
	
	if (profile1 != "") {
		std::string key(configRootKey);
		std::string path(configRootKey);
		path.append(profile1);
		path.append("\\");
		strcpy(this->newconfigRootKey, path.data());
	}
///
	//m_eAtmoConnectionType = (AtmoConnectionType)ReadRegistryInt(mykey, newconfigRootKey,"ConnectionType",(int)actClassicAtmo);
	//
	//m_Comport  = ReadRegistryInt(mykey, newconfigRootKey,"comport",-1); // -1 als Indikator ob Setup noch erfolgen muss!
 //   if(m_Comport < 1) { m_IsShowConfigDialog = 1; m_Comport=1; } // die Variable beim Laden der Settings immer setzen wenn ein key uns nicht gefällt - so das der Setup Dialog
	//                                          // zwanghaft gezeigt wird - dafür sparen wir uns das Flag minimiert - solange die Config Ok ist - brauchen
	//                                          // wir den Dialog ja nicht - dafür gibts ja das Trayicon?

 //   m_Comports[0] = ReadRegistryInt(mykey, newconfigRootKey,"comport_1",-1);
 //   m_Comports[1] = ReadRegistryInt(mykey, newconfigRootKey,"comport_2",-1);
 //   m_Comports[2] = ReadRegistryInt(mykey, newconfigRootKey,"comport_3",-1);

	//m_ArduComport  = ReadRegistryInt(mykey, newconfigRootKey,"Arducomport",-1); // -1 als Indikator ob Setup noch erfolgen muss!
 //   if(m_ArduComport < 1) { m_IsShowConfigDialog = 1; m_ArduComport=1; } // die Variable beim Laden der Settings immer setzen wenn ein key uns nicht gefällt - so das der Setup Dialog
///	

    m_WhiteAdjustment_Red    = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Red",m_WhiteAdjustment_Red) );
    m_WhiteAdjustment_Green  = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Green",m_WhiteAdjustment_Green) );
    m_WhiteAdjustment_Blue   = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"WhiteAdjustment_Blue",m_WhiteAdjustment_Blue) );
	m_UseSoftwareWhiteAdj    = (ReadRegistryInt(mykey, newconfigRootKey,"UseSoftwareWhiteAdj",m_UseSoftwareWhiteAdj) != 0);
	  //m_UseSoftware2WhiteAdj    = (ReadRegistryInt(mykey, newconfigRootKey,"UseSoftware2WhiteAdj",m_UseSoftware2WhiteAdj) != 0);
	  m_UseSoftware2WhiteAdj    = 0;
	m_UseColorKWhiteAdj		  = (ReadRegistryInt(mykey, newconfigRootKey,"UseColorK",m_UseColorKWhiteAdj) != 0);
	m_Use3dlut		  = (ReadRegistryInt(mykey, newconfigRootKey,"Use3dlut",m_Use3dlut) != 0);


    m_WhiteAdjPerChannel     = (ReadRegistryInt(mykey, newconfigRootKey,"UsePerChWhiteAdj",m_WhiteAdjPerChannel) != 0);
    sprintf(regKeyName,"%sWhiteAdjPerChannel\\",newconfigRootKey);
    m_chWhiteAdj_Count       = ReadRegistryInt(mykey, regKeyName,"count",0);
    delete []m_chWhiteAdj_Red;
    delete []m_chWhiteAdj_Green;
    delete []m_chWhiteAdj_Blue; 
    if(m_chWhiteAdj_Count > 0) 
    { 
       m_chWhiteAdj_Red   = new int[ m_chWhiteAdj_Count ];
       m_chWhiteAdj_Green = new int[ m_chWhiteAdj_Count ];
       m_chWhiteAdj_Blue  = new int[ m_chWhiteAdj_Count ];
    } else {
       m_chWhiteAdj_Red   = NULL;
       m_chWhiteAdj_Green = NULL;
       m_chWhiteAdj_Blue  = NULL;
    }
    for(int i=0; (i<m_chWhiteAdj_Count); i++) 
    {
        sprintf(valueName,"red_%d",i);
        m_chWhiteAdj_Red[i] = Check8BitValue( ReadRegistryInt(mykey, regKeyName, valueName, 256 ) );

        sprintf(valueName,"green_%d",i);
        m_chWhiteAdj_Green[i] = Check8BitValue( ReadRegistryInt(mykey, regKeyName, valueName, 256 ) );

        sprintf(valueName,"blue_%d",i);
        m_chWhiteAdj_Blue[i] = Check8BitValue( ReadRegistryInt(mykey, regKeyName, valueName, 256 ) );
    }


	m_ColorChanger_iSteps    = ReadRegistryInt(mykey, newconfigRootKey,"ColorChanger_iSteps",m_ColorChanger_iSteps);
	m_ColorChanger_iDelay    = ReadRegistryInt(mykey, newconfigRootKey,"ColorChanger_iDelay",m_ColorChanger_iDelay);

	m_LrColorChanger_iSteps    = ReadRegistryInt(mykey, newconfigRootKey,"LrColorChanger_iSteps",m_LrColorChanger_iSteps);
	m_LrColorChanger_iDelay    = ReadRegistryInt(mykey, newconfigRootKey,"LrColorChanger_iDelay",m_LrColorChanger_iDelay);

    m_IsSetShutdownColor   = ReadRegistryInt(mykey, newconfigRootKey,"isSetShutdownColor",m_IsSetShutdownColor);
	m_ShutdownColor_Red    = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"ShutdownColor_red",m_ShutdownColor_Red  ));
	m_ShutdownColor_Green  = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"ShutdownColor_green",m_ShutdownColor_Green));
	m_ShutdownColor_Blue   = CheckByteValue( ReadRegistryInt(mykey, newconfigRootKey,"ShutdownColor_blue",m_ShutdownColor_Blue) );

    m_StaticColor_Red   = ReadRegistryInt(mykey, newconfigRootKey,"StaticColor_Red",m_StaticColor_Red);
    m_StaticColor_Green = ReadRegistryInt(mykey, newconfigRootKey,"StaticColor_Green",m_StaticColor_Green);
    m_StaticColor_Blue  = ReadRegistryInt(mykey, newconfigRootKey,"StaticColor_Blue",m_StaticColor_Blue);

    m_LiveViewFilterMode       = (AtmoFilterMode)ReadRegistryInt(mykey, newconfigRootKey,"LiveViewFilterMode",(int)m_LiveViewFilterMode);
    m_LiveViewFilter_PercentNew      = ReadRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_PercentNew",m_LiveViewFilter_PercentNew);
    m_LiveViewFilter_MeanLength      = ReadRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_MeanLength",m_LiveViewFilter_MeanLength);
    m_LiveViewFilter_MeanThreshold   = ReadRegistryInt(mykey, newconfigRootKey,"LiveViewFilter_MeanThreshold",m_LiveViewFilter_MeanThreshold);

    m_show_statistics                = (ReadRegistryInt(mykey, newconfigRootKey,"show_statistics",ATMO_FALSE) != 0);  

    m_LiveView_RowsPerFrame   = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_RowsPerFrame",m_LiveView_RowsPerFrame);
    if(m_LiveView_RowsPerFrame < 1) m_LiveView_RowsPerFrame = 1;
    if(m_LiveView_RowsPerFrame >= CAP_HEIGHT) m_LiveView_RowsPerFrame = CAP_HEIGHT - 1;

    m_LiveView_EdgeWeighting  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_EdgeWeighting",m_LiveView_EdgeWeighting);
    m_LiveView_BrightCorrect  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_BrightCorrect",m_LiveView_BrightCorrect);
    m_LiveView_DarknessLimit  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_DarknessLimit",m_LiveView_DarknessLimit);
    m_LiveView_HueWinSize     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_HueWinSize",m_LiveView_HueWinSize);
    m_LiveView_SatWinSize     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_SatWinSize",m_LiveView_SatWinSize);
       m_LiveView_Overlap     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_Overlap",m_LiveView_Overlap);
	   m_LiveView_Saturation     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_Saturation",100);
	   m_LiveView_Sensitivity     = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_Sensitivity",0);
	   m_Useinvert          = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_invert",0);
	
	m_LiveView_WidescreenMode = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_WidescreenMode",m_LiveView_WidescreenMode);
	m_LiveView_Mode = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_Mode",m_LiveView_Mode);
	
    m_LiveView_HOverscanBorder  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_HOverscanBorder",m_LiveView_HOverscanBorder);
    m_LiveView_VOverscanBorder  = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_VOverscanBorder",m_LiveView_VOverscanBorder);
    m_LiveView_DisplayNr        = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_DisplayNr",m_LiveView_DisplayNr);

    m_LiveView_FrameDelay      = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_FrameDelay",m_LiveView_FrameDelay);

    m_LiveView_GDI_FrameRate   = ReadRegistryInt(mykey, newconfigRootKey,"LiveView_GDI_FrameRate",m_LiveView_GDI_FrameRate);

    m_ZonesTopCount            = ReadRegistryInt(mykey, newconfigRootKey,"ZonesTopCount",m_ZonesTopCount);
    m_ZonesBottomCount         = ReadRegistryInt(mykey, newconfigRootKey,"ZonesBottomCount",m_ZonesBottomCount);
    m_ZonesLRCount             = ReadRegistryInt(mykey, newconfigRootKey,"ZonesLRCount",m_ZonesLRCount);
    m_ZoneSummary              = (ReadRegistryInt(mykey, newconfigRootKey,"ZoneSummary",m_ZoneSummary) != 0);
    UpdateZoneCount();


    m_Hardware_global_gamma    = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_global_gamma",m_Hardware_global_gamma); // 0..255
    m_Hardware_global_contrast = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_global_contrast",m_Hardware_global_contrast);
    m_Hardware_contrast_red    = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_red",m_Hardware_contrast_red);
    m_Hardware_contrast_green  = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_green",m_Hardware_contrast_green);
    m_Hardware_contrast_blue   = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_contrast_blue",m_Hardware_contrast_blue);

    m_Hardware_gamma_red       = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_red",m_Hardware_gamma_red);
    m_Hardware_gamma_green     = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_green",m_Hardware_gamma_green);
    m_Hardware_gamma_blue      = ReadRegistryInt(mykey, newconfigRootKey,"Hardware_gamma_blue",m_Hardware_gamma_blue);


    m_Software_gamma_mode   = (AtmoGammaCorrect)ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_mode",(int)m_Software_gamma_mode);
    m_Software_gamma_red    = ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_red",m_Software_gamma_red);
    m_Software_gamma_green  = ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_green",m_Software_gamma_green);
    m_Software_gamma_blue   = ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_blue",m_Software_gamma_blue);
    m_Software_gamma_global = ReadRegistryInt(mykey, newconfigRootKey,"Software_gamma_global",m_Software_gamma_global);
    
    m_Software_gamma_red    = min(max(m_Software_gamma_red, 10), 350);  // 0,5 - 3,5
    m_Software_gamma_green  = min(max(m_Software_gamma_green, 10), 350);  // 0,5 - 3,5
    m_Software_gamma_blue   = min(max(m_Software_gamma_blue, 10), 350);  // 0,5 - 3,5
    m_Software_gamma_global = min(max(m_Software_gamma_global, 10), 350);  // 0,5 - 3,5

//calib
	
	 red_ColorK[256][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_rr",255);
	 red_ColorK[256][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_rg",0);
	 red_ColorK[256][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_rb",0);
	  
	 green_ColorK[256][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_gr",0);
	 green_ColorK[256][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_gg",255);
	 green_ColorK[256][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_gb",0);
	  
	 blue_ColorK[256][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_br",0);
	 blue_ColorK[256][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_bg",0);
	 blue_ColorK[256][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_bb",255);
	
	 red_ColorK[257][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_yr",255);
	 red_ColorK[257][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_yg",255);
	 red_ColorK[257][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_yb",0);
	  
	 green_ColorK[257][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_mr",255);
	 green_ColorK[257][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_mg",0);
	 green_ColorK[257][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_mb",255);
	  
	 blue_ColorK[257][0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_cr",0);
	 blue_ColorK[257][1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_cg",255);
	 blue_ColorK[257][2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_ColK_cb",255);
  	 
	 red_grid[0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_5",12);
	 red_grid[1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_10",25);
	 red_grid[2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_20",51);
	 red_grid[3]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_30",76);
	 red_grid[4]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_40",102);
	 red_grid[5]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_50",127);
	 red_grid[6]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_60",153);
	 red_grid[7]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_70",178);
	 red_grid[8]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_80",204);
	 red_grid[9]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_90",229);
	 red_grid[10]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_red_g_100",255);

	 green_grid[0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_5",12);
	 green_grid[1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_10",25);
	 green_grid[2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_20",51);
	 green_grid[3]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_30",76);
	 green_grid[4]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_40",102);
	 green_grid[5]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_50",127);
	 green_grid[6]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_60",153);
	 green_grid[7]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_70",178);
	 green_grid[8]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_80",204);
	 green_grid[9]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_90",229);
	 green_grid[10]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_green_g_100",255);

	 blue_grid[0]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_5",12);
	 blue_grid[1]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_10",25);
	 blue_grid[2]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_20",51);
	 blue_grid[3]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_30",76);
	 blue_grid[4]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_40",102);
	 blue_grid[5]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_50",127);
	 blue_grid[6]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_60",153);
	 blue_grid[7]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_70",178);
	 blue_grid[8]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_80",204);
	 blue_grid[9]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_90",229);
	 blue_grid[10]    = ReadRegistryInt(mykey, newconfigRootKey,"Software_blue_g_100",255);

	 ColorCube[0][0][0][0]=0;ColorCube[0][0][0][1]=0;ColorCube[0][0][0][2]=0; //black
	 ColorCube[1][0][0][0]=red_ColorK[256][0];ColorCube[1][0][0][1]=red_ColorK[256][1];ColorCube[1][0][0][2]=red_ColorK[256][2]; //red
	 ColorCube[0][1][0][0]=green_ColorK[256][0];ColorCube[0][1][0][1]=green_ColorK[256][1];ColorCube[0][1][0][2]=green_ColorK[256][2]; //green
	 ColorCube[0][0][1][0]=blue_ColorK[256][0];ColorCube[0][0][1][1]=blue_ColorK[256][1];ColorCube[0][0][1][2]=blue_ColorK[256][2]; //blue
	 ColorCube[1][1][0][0]=red_ColorK[257][0];ColorCube[1][1][0][1]=red_ColorK[257][1];ColorCube[1][1][0][2]=red_ColorK[257][2]; //yellow
	 ColorCube[1][0][1][0]=green_ColorK[257][0];ColorCube[1][0][1][1]=green_ColorK[257][1];ColorCube[1][0][1][2]=green_ColorK[257][2]; //magenta
	 ColorCube[0][1][1][0]=blue_ColorK[257][0];ColorCube[0][1][1][1]=blue_ColorK[257][1];ColorCube[0][1][1][2]=blue_ColorK[257][2]; //cyan
	 ColorCube[1][1][1][0]=red_grid[10] ;ColorCube[1][1][1][1]=green_grid[10];ColorCube[1][1][1][2]=blue_grid[10]; //white

	 m_CurrentChannelAssignment = ReadRegistryInt(mykey, newconfigRootKey,"CurrentChannelAssignment",m_CurrentChannelAssignment);

	 //setConnectionType(m_eAtmoConnectionType);

     
}

int CAtmoRegistryConfig::RegistryKeyExists(HKEY mykey, char *path) {
	HKEY keyHandle;
	
	if( RegOpenKeyEx(mykey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
	    RegCloseKey(keyHandle);	
		return 1;
	}
    return 0;
}

int CAtmoRegistryConfig::RegistryValueExists(HKEY mykey, char *path, char *valueName) {
	HKEY keyHandle;
	int temp = 0;
    DWORD size1;
    DWORD valueType;

	if( RegOpenKeyEx(mykey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
		if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, NULL, &size1) == ERROR_SUCCESS) {
		   temp = 1;
		}
	    RegCloseKey(keyHandle);	
	}
    return temp;
}

void CAtmoRegistryConfig::WriteRegistryInt(HKEY mykey, char *path, char *valueName, int value) {
    HKEY keyHandle;

	if( RegCreateKeyEx(mykey, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &keyHandle, NULL)  == ERROR_SUCCESS) {
		RegSetValueEx(keyHandle,valueName,0,REG_DWORD,(LPBYTE)&value,sizeof(int));
	    RegCloseKey(keyHandle);	
	}

}

void CAtmoRegistryConfig::WriteRegistryString(HKEY mykey, char *path, char *valueName, char *value) {
    HKEY keyHandle;

	
	if( RegCreateKeyEx(mykey, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &keyHandle, NULL)  == ERROR_SUCCESS) {
		RegSetValueEx(keyHandle,valueName,0,REG_SZ,(LPBYTE)value,(DWORD)(strlen(value)+1));
	    RegCloseKey(keyHandle);	
	}
}

void CAtmoRegistryConfig::WriteRegistryStringList(HKEY mykey, char *path, char *valueName, char *default_value) {
	DWORD valueType, size;
	HKEY keyHandle;
	std::vector<std::string> target;
	std::string vals;
	int len=1;
	LONG ConfigurationStringSize = 512;
	CHAR *ConfigurationString = new CHAR[ConfigurationStringSize];
	memset(ConfigurationString,'\0',512);

	for (int i=0; i<profiles.size();i++){
		len += profiles[i].length()+1;
		strcat(ConfigurationString, (char*)profiles[i].data());
		strcat(ConfigurationString, (char*)("$"));
		}
	// Format the string correctly.
	int ConfigStringLen = strlen(ConfigurationString);
	for(int x = 0; x < ConfigStringLen; ++x)
	{
	if(ConfigurationString[x] == '$')
	ConfigurationString[x] = '\0';
	}

	if( RegCreateKeyEx(mykey, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &keyHandle, NULL) == ERROR_SUCCESS){
		RegSetValueEx(keyHandle,valueName,0,REG_MULTI_SZ,reinterpret_cast<const BYTE*>(&ConfigurationString[0]), ConfigStringLen);
		RegCloseKey(keyHandle);	
	  }
	delete [] ConfigurationString;

}

void CAtmoRegistryConfig::WriteRegistryDouble(HKEY mykey, char *path, char *valueName, double value) {
    HKEY keyHandle;
	if( RegCreateKeyEx(mykey, path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &keyHandle, NULL)  == ERROR_SUCCESS) {
		RegSetValueEx(keyHandle,valueName,0,REG_BINARY,(LPBYTE)&value,sizeof(double));
	    RegCloseKey(keyHandle);	
	}
}

int CAtmoRegistryConfig::ReadRegistryInt(HKEY mykey, char *path, char *valueName, int default_value) {
	HKEY keyHandle;
	int tempValue;
    DWORD size1;
    DWORD valueType;

	if( RegOpenKeyEx(mykey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
		if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, NULL, &size1) == ERROR_SUCCESS) {
		   if(valueType == REG_DWORD) {
			   if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, (LPBYTE)&tempValue, &size1) == ERROR_SUCCESS) {
                  default_value = tempValue;
			   };
		   }
		}
	    RegCloseKey(keyHandle);	
	}
    return default_value;
}

char * CAtmoRegistryConfig::ReadRegistryString(HKEY mykey, char *path, char *valueName, char *default_value) {
	HKEY keyHandle;
	char *tempValue = NULL;
    DWORD size1;
    DWORD valueType;

	if( RegOpenKeyEx(mykey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
		if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, NULL, &size1) == ERROR_SUCCESS) {
		   if(valueType == REG_SZ) {
			   // free
			   tempValue = (char *)malloc(size1+1); // +1 für NullByte`?
			   if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, (LPBYTE)tempValue, &size1) == ERROR_SUCCESS) {
                  default_value = tempValue;
			   };
		   }
		}
	    RegCloseKey(keyHandle);	
	}
	if(tempValue == NULL)  {
		// wenn tempValue nicht aus registry gelesen wurde dafür sorgen das ein neuer String mit der Kopie von DefaultValue
		// geliefert wird - das macht das Handling des Rückgabewertes der Funktion einfacher - immer schön mit free freigeben!
		default_value = strdup(default_value);
	}

    return default_value;
}

void CAtmoRegistryConfig::ReadRegistryStringList(HKEY mykey, char *path, char *valueName, char *default_value) {
	DWORD valueType, size;
	HKEY keyHandle;
	std::vector<std::string> target;

	if( RegOpenKeyEx(mykey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS){
		if ( RegQueryValueEx(keyHandle,valueName,NULL, &valueType, NULL, &size ) == ERROR_SUCCESS ){
			if ( valueType == REG_MULTI_SZ ){
			  
			  std::vector<char> temp(size);
			  profiles.clear();

			  if ( size>0 && RegQueryValueEx(keyHandle, valueName, NULL, &valueType, reinterpret_cast<LPBYTE>(&temp[0]), &size ) == ERROR_SUCCESS )
			  {
			  
			  size_t index = 0;
			  size_t len = strlen( &temp[0] );
			  int size2 =temp.size();
			  while (  index < size2-1 )
			  {
				target.push_back(&temp[index]);
				index += len + 1;
				if ( index < size2-1 ) len = strlen( &temp[index] );
			  }
			  //profiles.clear();
			  for (int i=0; i<target.size();i++) profiles.push_back (target[i]);
			}
		}
	  }
	  RegCloseKey(keyHandle);
	}
	if(target.size() == 0)  {
		// wenn tempValue nicht aus registry gelesen wurde dafür sorgen das ein neuer String mit der Kopie von DefaultValue
		// geliefert wird - das macht das Handling des Rückgabewertes der Funktion einfacher - immer schön mit free freigeben!
		default_value = strdup(default_value);
	}

    return;
}



double CAtmoRegistryConfig::ReadRegistryDouble(HKEY mykey, char *path, char *valueName, double default_value) {
	HKEY keyHandle;
	double tempValue;
    DWORD size1;
    DWORD valueType;

	if( RegOpenKeyEx(mykey, path, 0, KEY_READ, &keyHandle) == ERROR_SUCCESS) {
		if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, NULL, &size1) == ERROR_SUCCESS) {
		   if((valueType == REG_BINARY) && (size1 == sizeof(double))) {
			   if(RegQueryValueEx( keyHandle, valueName, NULL, &valueType, (LPBYTE)&tempValue, &size1) == ERROR_SUCCESS) {
                  default_value = tempValue;
			   };
		   }
		}
	    RegCloseKey(keyHandle);	
	}
    return default_value;
}

int CAtmoRegistryConfig::CheckByteValue(int value) 
{
	if(value < 0) value = 0;
	if(value > 255) value = 255;
	return value;
}

int CAtmoRegistryConfig::Check8BitValue(int value) 
{
	if(value < 0) value = 0;
	if(value > 256) value = 256;
	return value;
}


