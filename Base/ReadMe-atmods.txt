AtmoLight Direct Show Filter

use "reg_filter.cmd" to install the filter - in the Software you are
useing choose the DirectShow Filter "AtmoLight" - which should fit
into the filter queue shortly before the renderer.

Currently there is no property page to configure the filter - just
click the file "atmods_settings.reg" to create some options in the
registry under

HKEY_LOCAL_MACHINE\Software\AtmoWinX\dshow\

bzw. bei 64-Bit-Windows unter

HKEY_LOCAL_MACHINE\Software\Wow6432Node\AtmoWinX\dshow\



[string] exe  -- here you can write the complete filename 
                 of the atmowin exe you want to launch 
           (by default the filter tries to load AtmoWinA.exe in same 
            folder where the filter DLL is located.)

[dword] log_level -- defines log file mode 
                         0 = no logging
                         1 = write textlogfile
                         2 = write textlogfile and every 25th Image to a bitmap

[string] log_file -- complete path of a textfile to which the log is written
                     by default the file <DLL folder>\atmo_ds.log will be used.

[string] log_path -- defines a path where in log_level=2 the images will be placed
                     by default the <DLL folder> is used. (its bad I know)





