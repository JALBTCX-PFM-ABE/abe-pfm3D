
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/




#ifndef VERSION

#define     VERSION     "PFM Software - pfm3D V2.73 - 10/20/17"

#endif

/*! <pre>


    Version 1.0
    Jan C. Depner
    09/10/08

    First working version.


    Version 1.01
    Jan C. Depner
    10/09/08

    Changed rotation code so that control is in both the Y and ZX directions.


    Version 1.02
    Jan C. Depner
    10/20/08

    Moved getNearestPoint.cpp into nvMapGL.cpp


    Version 2.00
    Jan C. Depner
    10/22/08

    Using quaternions to handle rotations (don't ask).  Quaternion code from Superliminal Software:
    http://www.superliminal.com/sources/sources.htm
    Apparently this is by Melinda Green but it is an "Implementation of a simple C++ quaternion
    class called "Squat". Popularized by a seminal paper by Ken Shoemake, a quaternion represents a
    rotation about an axis.  Squats can be concatenated together via the * and *= operators and
    converted back and forth between transformation matrices. Implementation also includes a wonderful
    3D vector macro library by Don Hatch."  Which it didn't ;-)  I had to go find the vec.h code
    online.  Made a couple of minor modifications to squat.cpp. 


    Version 2.01
    Jan C. Depner
    10/28/08

    More intuitive handling of the Y rotations (Z in our world).


    Version 2.02
    Jan C. Depner
    11/21/08

    Using setTargetPoints instead of drawSphere to display targets.  This will allow us in future to edit the
    targets.  Made controls match pfmEdit3D.


    Version 2.03
    Jan C. Depner
    11/26/08

    Fix rotation bug when near flat view.


    Version 2.04
    Jan C. Depner
    12/08/08

    Flush map after turning targets off.


    Version 2.10
    Jan C. Depner
    01/23/09

    Changed to XOR for movable objects because redrawing the point cloud every time made the response too
    slow when you were viewing a lot of points.  I saved the original, non-XOR version in nvMapGL.cpp.noXOR.


    Version 2.20
    Jan C. Depner
    02/11/09

    Added subsampled layer for rotation and zooming.  Changed wheel zoom to be continuous until stopped.
    Added ability to edit from this window (actually tells pfmView to do a polygon edit).  Added coordination
    between pfmView, pfmEdit3D, and pfm3D.  When pfmEdit3D is started pfm3D will unload it's display lists
    and memory unless you move the cursor back into pfm3D in which case it will reload until you leave again.
    Lists and memory will be reloaded upon exit from pfmEdit3D.  Fixed problems with rubberband polygons.


    Version 2.21
    Jan C. Depner
    02/12/09

    Added draw scale option to prefs.  It's not really a scale, just a box at the moment.  Also, added ability
    to change background color.


    Version 2.22
    Jan C. Depner
    02/18/09

    Numerous hacks to try to fix the Windoze version of nvMapGL.cpp.  Added the extended help feature.


    Version 2.23
    Jan C. Depner
    02/22/09

    Hopefully handling the movable objects (XORed) correctly now.  This should make the Windoze version
    a bit nicer.


    Version 2.24
    Jan C. Depner
    02/24/09

    Finally figured it out - I have to handle all of the GL_BACK and GL_FRONT buffer swapping manually.  If
    I let it auto swap it gets out of control.


    Version 2.25
    Jan C. Depner
    03/24/09

    Added GUI control help.  Stop Mouse wheel zoom on reversal of direction.


    Version 2.26
    Jan C. Depner
    03/25/09

    Cleaned up the mouse wheel zoom.  Fixed the flicker at the end of a rotate or zoom operation.


    Version 2.30
    Jan C. Depner
    04/02/09

    Replaced support for NAVO standard target (XML) files with support for Binary Feature Data (BFD) files.


    Version 2.31
    Jan C. Depner
    04/14/09

    Added selectable feature marker size option.


    Version 2.32
    Jan C. Depner
    04/23/09

    Changed the acknowledgments help to include Qt and a couple of others.


    Version 2.33
    Jan C. Depner
    04/27/09

    Replaced QColorDialog::getRgba with QColorDialog::getColor.


    Version 2.34
    Jan C. Depner
    05/04/09

    Use paintEvent for "expose" events in nvMapGL.cpp.  This wasn't a problem under compiz but shows up
    under normal window managers.


    Version 2.40
    Jan C. Depner
    05/21/09

    Fixed the geotiff overlay problem caused by Z exaggeration.  Also, removed GeoTIFF (GDAL) specific code from
    nvMapGL.cpp and moved nvMapGL.cpp and .hpp to utility library.


    Version 2.41
    Jan C. Depner
    06/24/09

    Changed the BFD names to avoid collision with GNU Binary File Descriptor library.


    Version 2.42
    Jan C. Depner
    07/28/09

    Changed use of _sleep to Sleep on MinGW (Windows).


    Version 2.43
    Jan C. Depner
    09/11/09

    Fixed getColor calls so that cancel works.  Never forget!


    Version 2.44
    Jan C. Depner
    12/09/09

    Fixed VirtualBox close bug.


    Version 2.50
    Jan C. Depner
    01/25/10

    Added ability to diplay the On-The-Fly (OTF) grid from pfmView.  BTW - New Orleans Saints,
    2009 NFC Champions... Who Dat!!!


    Version 2.51
    Jan C. Depner
    04/29/10

    Fixed posfix and fixpos calls to use numeric constants instead of strings for type.  BTW - New Orleans Saints,
    2009 NFL Champions... Who Dat!!!


    Version 2.52
    Jan C. Depner
    08/17/10

    Replaced our kludgy old UTM transform with calls to the PROJ 4 library functions.  All hail the PROJ 4 developers!


    Version 2.53
    Jan C. Depner
    09/17/10

    Fixed bug displaying feature info.


    Version 2.54
    Jan C. Depner
    11/05/10

    Minor mods for auto scale changes to nvMapGL.cpp in nvutility library.


    Version 2.55
    Jan C. Depner
    01/06/11

    Correct problem with the way I was instantiating the main widget.  This caused an intermittent error on Windows7/XP.


    Version 2.56
    Jan C. Depner
    01/15/11

    Added an exaggeration scrollbar to the left side of the window.


    Version 2.57
    Jan C. Depner
    04/15/11

    Fixed the geoTIFF reading by switching to using GDAL instead of Qt.  Hopefully Qt will get fixed eventually.


    Version 2.58
    Jan C. Depner
    06/30/11

    Added menu to allow displaying of all, unverified, verified, or no features (as opposed to just on/off).


    Version 2.59
    Jan C. Depner
    11/30/11

    Converted .xpm icons to .png icons.


    Version 2.60
    Jan C. Depner
    01/05/12

    Fixed killer bug by moving the show () call and forcing event processing afterwards.  Why?
    Who the hell knows.


    Version 2.61
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 2.62
    Jan C. Depner (PFM Software)
    02/28/14

    Reverted to using Qt to read geoTIFFs.  It quit working in Qt 4.7.2 but is working again.  It's
    much faster than using GDAL and it's probably a lot more bulletproof.


    Version 2.63
    Jan C. Depner (PFM Software)
    03/19/14

    - Changed the pointer to the ABE user guide HTML file from PFM/Area_Based_Editor.html to PFM_ABE_User_Guide.html.
    - Straightened up the Open Source acknowledgments.


    Version 2.64
    Jan C. Depner (PFM Software)
    05/27/14

    - Added the new LGPL licensed GSF library to the acknowledgments.


    Version 2.65
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 2.66
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 2.67
    Jan C. Depner (PFM Software)
    02/16/15

    - To give better feedback to shelling programs in the case of errors I've added the program name to all
      output to stderr.


    Version 2.70
    Jan C. Depner (PFM Software)
    08/15/15

    - Now reads the globalABE.ini file to get Z orientation so the "box scale" will be drawn with the proper
      numbers (depth or elevation).


    Version 2.71
    Jan C. Depner (PFM Software)
    08/20/15

    - Fixed my own cleverness ;-)  Now the mouse scroll wheel zoom works like you'd expect it to.


    Version 2.72
    Jan C. Depner (PFM Software)
    08/08/16

    - Now gets font from globalABE.ini as set in Preferences by pfmView.
    - To avoid possible confusion, removed translation setup from QSettings in env_in_out.cpp.


    Version 2.73
    Jan C. Depner (PFM Software)
    10/20/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      pfm3D_xx.ts file that can be run through Qt's "linguist" to translate to another language.

</pre>*/
