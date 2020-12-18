
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



#include "pfm3D.hpp"

void set_defaults (MISC *misc, OPTIONS *options, uint8_t restore)
{
  //  Set Defaults so that if keys don't for envin the parms are defined.

  if (!restore)
    {
      strcpy (misc->GeoTIFF_name, "");
      misc->GeoTIFF_init = NVFalse;
      misc->GeoTIFF_open = NVFalse;
      misc->drawing_canceled = NVFalse;
      misc->drawing = NVFalse;
      misc->display_GeoTIFF = NVFalse;
      misc->feature = NULL;
      misc->prev_mbr.min_x = 999.0;
      misc->nearest_pfm = 0;
      misc->color_by_attribute = 0;
      memset (&misc->bfd_header, 0, sizeof (BFDATA_HEADER));


      //  Tooltip text for the buttons that have editable accelerators

      misc->buttonText[0] = pfm3D::tr("Redraw");
      misc->buttonText[1] = pfm3D::tr("Redraw Coverage Map");
      misc->buttonText[2] = pfm3D::tr("Toggle contour drawing");
      misc->buttonText[3] = pfm3D::tr("Display average edited depth/elevation surface");
      misc->buttonText[4] = pfm3D::tr("Display minimum edited depth/elevation surface");
      misc->buttonText[5] = pfm3D::tr("Display maximum edited depth/elevation surface");
      misc->buttonText[6] = pfm3D::tr("Display average unedited depth/elevation surface");
      misc->buttonText[7] = pfm3D::tr("Display minimum unedited depth/elevation surface");
      misc->buttonText[8] = pfm3D::tr("Display maximum unedited depth/elevation surface");
      misc->buttonText[9] = pfm3D::tr("Toggle GeoTIFF display");

      misc->width = 990;
      misc->height = 964;
      misc->window_x = 0;
      misc->window_y = 0;

      misc->html_help_tag = "#5.2.3_The_3D_PFM_viewer_-_pfm3D";

      misc->bfd_open = NVFalse;
    }


  options->position_form = 0;
  options->display_feature = 0;
  options->display_children = NVFalse;
  options->display_feature_info = NVFalse;
  options->z_factor = 1.0;
  options->z_offset = 0.0;
  options->zoom_percent = 5;
  options->exaggeration = 3.0;
  options->background_color = QColor (96, 96, 96);
  options->highlight_color = QColor (255, 0, 0, 192);
  options->tracker_color = QColor (255, 255, 255, 255);
  options->edit_color = QColor (255, 255, 255, 255);
  options->scale_color = QColor (255, 255, 255, 255);
  options->feature_color = QColor (255, 255, 255, 192);
  options->feature_info_color = QColor (255, 255, 255);
  options->font = QApplication::font ();
  options->feature_size = 0.003;
  options->highlight = 0;
  options->highlight_percent = 0.1;
  options->edit_mode = RECT_EDIT_AREA_3D;
  options->draw_scale = NVTrue;
  options->z_orientation = 1.0;


  options->buttonAccel[0] = "Shift+r";
  options->buttonAccel[1] = "Shift+m";
  options->buttonAccel[2] = "Shift+c";
  options->buttonAccel[3] = "F3";
  options->buttonAccel[4] = "F4";
  options->buttonAccel[5] = "F5";
  options->buttonAccel[6] = "F6";
  options->buttonAccel[7] = "F7";
  options->buttonAccel[8] = "F8";
  options->buttonAccel[9] = "F9";
}
