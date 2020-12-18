
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



#ifndef _PFM3D_DEF_H_
#define _PFM3D_DEF_H_


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <cerrno>
#include <cmath>


#include "nvutility.h"
#include "nvutility.hpp"


#include <gdal.h>
#include <gdal_priv.h>
#include <cpl_string.h>
#include <ogr_spatialref.h>
#include <gdalwarper.h>
#include <ogr_spatialref.h>


#include "pfm.h"
#include "pfm_extras.h"

#include "binaryFeatureData.h"

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif
#include <QSharedMemory>


#ifdef NVWIN3X
    #include "windows_getuid.h"

using namespace std;  // Windoze bullshit - God forbid they should follow a standard
#endif


//  Pointer interaction functions.

#define         RECT_EDIT_AREA              0
#define         POLY_EDIT_AREA              1
#define         RECT_EDIT_AREA_3D           2
#define         POLY_EDIT_AREA_3D           3
#define         ROTATE                      4
#define         ZOOM                        5
//#define         DRAG                        6


#define         H_NONE                      0
#define         H_ALL                       1
#define         H_CHECKED                   2
#define         H_01                        3
#define         H_02                        4
#define         H_03                        5
#define         H_04                        6
#define         H_05                        7
#define         H_INT                       8
#define         H_MULT                      9
#define         H_IHO_S                     10
#define         H_IHO_1                     11
#define         H_IHO_2                     12
#define         H_PERCENT                   13  //  Leave this as last highlight always


#define         NUMSHADES                   256
#define         NUMHUES                     128
#define         LINE_WIDTH                  2
#define         POLYGON_POINTS              NVMAP_POLYGON_POINTS
#define         HOTKEYS                     10


#define         MARKER_W                    15
#define         MARKER_H                    10


//  The OPTIONS structure contains all those variables that can be saved to the
//  users pfmView QSettings.


typedef struct
{
  int32_t     position_form;              //  Position format number
  QColor      background_color;           //  Color to be used for background
  QColor      highlight_color;            //  Color to be used for highlighting features
  QColor      tracker_color;              //  Color to be used for track cursor
  QColor      edit_color;                 //  Color to be used for edit rectangles and polygons
  QColor      scale_color;                //  Color to be used for the "scale"
  QColor      feature_color;              //  Color to be used for features
  QColor      feature_info_color;         //  Color to be used for feature information text display
  QFont       font;                       //  Font used for all ABE map GUI applications
  float       feature_size;               //  Feature size
  int32_t     zoom_percent;               //  Zoom in/out percentage
  double      exaggeration;               //  Z exaggeration
  int32_t     display_feature;            //  0 = no features, 1 = all features, 2 = unverified features, 3 = verified features,
  uint8_t     display_children;           //  Flag to show grouped feature children locations
  uint8_t     display_feature_info;       //  Flag to show feature info
  float       z_factor;                   //  Coversion factor for Z values. May be used to convert m to ft...
  float       z_offset;                   //  Offset value for Z values.
  int32_t     highlight;                  //  Type of data to highlight, -1 - all, 0 - none, 6 - checked,
                                          //  7 - multiple coverage, 5 - interpolated bins,
                                          //  1-4 - PFM_USER_01-04
  QString     buttonAccel[12];            //  Accelerator key sequences for some of the buttons
  int32_t     new_feature;                //  Stupid counter so I can advertise new features once and then move on
  float       highlight_percent;          //  Percentage of depth for highlight.
  int32_t     edit_mode;                  //  Saved "misc.function"
  uint8_t     draw_scale;                 //  Set this to draw the scale in the display.
  float       z_orientation;              /*  This is set to 1.0 to display numeric data to the user as depths or -1.0 to display them as elevations.
                                              This value is stored in a separate QSettings .ini file because it is used by a number of other programs
                                              (e.g. pfmView).  */
} OPTIONS;


//  General stuff.

typedef struct
{
  int32_t     function;                   //  Active edit function
  int32_t     save_function;              //  Save last edit function
  char        GeoTIFF_name[512];          //  GeoTIFF file name
  uint8_t     GeoTIFF_open;               //  Set if GeoTIFF file has been opened.
  uint8_t     GeoTIFF_init;               //  Set if GeoTIFF is new
  int32_t     display_GeoTIFF;            //  0 - no display, 1 - display as decal, -1 display modulated with depth color
  int32_t     color_by_attribute;         //  0 for normal colors, otherwise 1 for number, 2 for std, attribute + 3.
  uint8_t     drawing;                    //  set if we are drawing the surface
  uint8_t     drawing_canceled;           //  set to cancel drawing
  NV_F64_XYMBR displayed_area;            //  displayed area for the top level PFM (or PFM currently being drawn when actually drawing)
  NV_F64_XYMBR total_displayed_area;      //  displayed area for all of the displayed PFMs
  NV_F64_XYMBR prev_mbr;                  //  previously displayed area
  int32_t     displayed_area_width;       //  displayed area width in columns
  int32_t     displayed_area_height;      //  displayed area height in rows
  int32_t     displayed_area_column;      //  displayed area start column
  int32_t     displayed_area_row;         //  displayed area start row
  float       displayed_area_min;         //  min Z value for displayed area
  float       displayed_area_max;         //  max Z value for displayed area
  float       displayed_area_range;       //  Z range for displayed area
  float       displayed_area_attr_min;
  float       displayed_area_attr_max;
  float       displayed_area_attr_range;
  BFDATA_SHORT_FEATURE *feature;          //  Feature (target) data array
  int32_t     bfd_handle;                 //  BFD (target) file handle
  BFDATA_HEADER  bfd_header;              //  Header for the current BFD file
  uint8_t     bfd_open;                   //  Set if we have an open BFD file
  QColor      color_array[2][NUMHUES + 1][NUMSHADES];
                                          //  arrays of surface colors
  QString     buttonText[12];             //  Tooltip text for the buttons that have editable accelerators
  QToolButton *button[12];                //  Buttons that have editable accelerators
  int32_t     nearest_pfm;
  QColor      widgetBackgroundColor;      //  The normal widget background color.
  QString     html_help_tag;              //  Help tag for the currently running section of code.  This comes from
                                          //  the PFM_ABE html help documentation.
  QSharedMemory *abeShare;                //  ABE's shared memory pointer.
  QSharedMemory *abeRegister;             //  ABE's process register
  ABE_SHARE   *abe_share;                 //  Pointer to the ABE_SHARE structure in shared memory.
  ABE_REGISTER *abe_register;             //  Pointer to the ABE_REGISTER structure in shared memory.
  double      map_center_x;               //  Center of the map to be used for view reset.
  double      map_center_y;               //  Center of the map to be used for view reset.
  double      map_center_z;               //  Center of the map to be used for view reset.
  OTF_GRID_RECORD *otf_grid;              //  Pointer to on-the-fly grid (from pfmView).
  QSharedMemory *otfShare;                //  otf_grid shared memory pointer.
  int32_t     width;                      //  Main window width
  int32_t     height;                     //  Main window height
  int32_t     window_x;                   //  Main window x position
  int32_t     window_y;                   //  Main window y position
  char        progname[256];              /*  This is the program name.  It will be used in all output to stderr so that shelling programs
                                              will know what program printed out the error message.  */


  //  The following concern PFMs as layers.  There are a few things from ABE_SHARE that also need to be 
  //  addressed when playing with layers - open_args, display_pfm, and pfm_count.

  NV_F64_XYMBR total_mbr;                 //  MBR of all of the displayed PFMs
  char        attr_format[MAX_ABE_PFMS][NUM_ATTR][20]; //  Formats to use for displaying attributes
  int32_t     pfm_handle[MAX_ABE_PFMS];   //  PFM file handle
  int32_t     average_type[MAX_ABE_PFMS]; //  Type of average filtered surface, 0 - normal, 1 - minimum misp, 
                                          //  2 - average misp, 3 - maximum misp
  uint8_t     cube_attr_available[MAX_ABE_PFMS]; //  NVTrue is CUBE attributes are in the file
  int32_t     layer[MAX_ABE_PFMS];
} MISC;


#endif
