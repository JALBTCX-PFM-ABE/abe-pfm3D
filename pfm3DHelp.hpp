
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



using namespace std;  // Windoze bullshit - God forbid they should follow a standard


QString geoText = 
  pfm3D::tr ("<img source=\":/icons/geo_off.png\"> This is a tristate button.  Click it to change the way GeoTIFFs "
             "are displayed.  The three states are:<br><br>"
             "<ul>"
             "<li><img source=\":/icons/geo_off.png\"> - Image will not be displayed</li>"
             "<li><img source=\":/icons/geo_decal.png\"> - Image will be draped over the surface as an opaque overlay</li>"
             "<li><img source=\":/icons/geo_modulate.png\"> - Image will be draped over the surface as a translucent "
             "overlay</li>"
             "</ul>");

QString prefsText = 
  pfm3D::tr ("<img source=\":/icons/prefs.png\"> Click this button to change program preferences.  Right now "
             "that's just the position format.  Maybe more later...");

QString quitText = 
  pfm3D::tr ("<img source=\":/icons/quit.png\"> Click this button to <b><em>exit</em></b> from the program.  "
             "You can also use the <b>Quit</b> entry in the <b>File</b> menu.");

QString geoOpenText = 
  pfm3D::tr ("<img source=\":/icons/geotiffopen.png\"> Click this button to open a GeoTIFF file to be draped over the "
             "surface.");

QString exagBarText = 
  pfm3D::tr ("This scroll bar controls/monitors the vertical exaggeration of the data.  Pressing the up and down "
             "arrow keys will change the exaggeration by 1.0.  Clicking the mouse while the cursor is in the trough "
             "will change the value by 5.0.  Dragging the slider will display the value in the <b>Exag:</b> field in "
             "the status area on the left but the exaggeration will not change until the slider is released.  The "
             "maximum value is 100.0 and the minimum value is 1.0.  If the exaggeration has been set to less than 1.0 "
             "due to large vertical data range the scroll bar will be inactive.  The exaggeration can also be changed "
             "by pressing the <b>Ctrl</b> key and then simultaneously pressing the <b>PageUp</b> or <b>PageDown</b> button.");

QString mapText = 
  pfm3D::tr ("This is the pfm3D program, a companion to the <b>pfmView</b> program.  "
             "It is used for viewing PFM surfaces in 3D.  It can also display GeoTIFF files as opaque or translucent "
             "overlays.<br><br>"
             "Point of view is controlled by holding down the <b>Ctrl</b> key and clicking and dragging with the "
             "left mouse button.  It can also be controlled by holding down the <b>Ctrl</b> key and using the "
             "left, right, up, and down arrow keys.  Zoom is controlled by holding down the <b>Ctrl</b> key and "
             "using the mouse wheel or holding down the <b>Ctrl</b> key, pressing the right mouse button, and moving "
             "the cursor up or down.  To reposition the center of the view just place the cursor at the desired "
             "location, hold down the <b>Ctrl</b> key, and click the middle mouse button.  Z exaggeration can be "
             "changed by pressing <b>Ctrl-Page Up</b> or <b>Ctrl-Page Down</b> or by editing "
             "the exaggeration value in the Preferences dialog <img source=\":/icons/prefs.png\">");


QString resetText = 
  pfm3D::tr ("<img source=\":/icons/camera.png\"> Click this button to return to the original view.");


QString editRectText = 
  pfm3D::tr ("<img source=\":/icons/edit_rect.png\"> Click this button to switch the default mode to rectangle "
             "edit.<br><br>"
             "After clicking the button the cursor will change to the edit rectangle cursor "
             "<img source=\":/icons/edit_rect_cursor.png\">.  Click the left mouse button to define a starting point "
             "for a rectangle.  Move the mouse to define the edit bounds.  Left click again to begin the edit "
             "operation.  To abort the operation click the middle mouse button.<br><br> "
             "<b>NOTE: This mode will remain active unless one of the other modes "
             "(like add feature <img source=\":/icons/addfeature.png\"> or polygon edit "
             "<img source=\":/icons/edit_poly.png\">) is selected.</b>");
QString editPolyText = 
  pfm3D::tr ("<img source=\":/icons/edit_poly.png\"> Click this button to switch the default mode to polygon "
             "edit.<br><br>"
             "After clicking the button the cursor will change to the edit polygon cursor "
             "<img source=\":/icons/edit_poly_cursor.png\">.  Click the left mouse button to define a starting point "
             "for a polygon.  Moving the mouse will draw a line.  Left click to define the next vertex of the polygon.  "
             "Double click to define the last vertex of the polygon and begin the edit operation.  "
             "To abort the operation click the middle mouse button.<br><br> "
             "<b>NOTE: This mode will remain active unless one of the other modes "
             "(like add feature <img source=\":/icons/addfeature.png\"> or rectangle edit "
             "<img source=\":/icons/edit_rect.png\">) is selected.</b>");

QString displayFeatureText = 
  pfm3D::tr ("<img source=\":/icons/display_all_feature.png\"> Click this button to select the feature display mode.  "
             "You can highlight features that have descriptions or remarks containing specific text strings by setting "
             "the feature search string in the preferences dialog <img source=\":/icons/prefs.png\">.");
QString displayChildrenText = 
  pfm3D::tr ("<img source=\":/icons/displayfeaturechildren.png\"> Click this button to display feature sub-records.  "
             "Feature sub-records are features that have been grouped under a master feature record.  Features "
             "can be grouped and un-grouped in the edit feature dialog.");
QString displayFeatureInfoText = 
  pfm3D::tr ("<img source=\":/icons/displayfeatureinfo.png\"> Click this button to write the description and "
             "remarks fields of features next to any displayed features.  This button is "
             "meaningless if <b>Flag Feature Data</b> is set to not display features "
             "<img source=\":/icons/display_no_feature.png\">.");
