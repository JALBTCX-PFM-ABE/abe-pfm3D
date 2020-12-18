
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

void paint_otf_surface (pfm3D *parent, MISC *misc, nvMapGL *map, uint8_t reset)
{
  static NV_F64_XYMBR prev_mbr = {-999.0, -999.0, -999.0, -999.0};
  static double prev_min_z = CHRTRNULL, prev_max_z = -CHRTRNULL;
  static int32_t prev_display_GeoTIFF = 0;
  float **data = NULL;
  NV_F64_XYMBC mbc;


  QImage *geotiff (NV_F64_XYMBR mbr, char *geotiff_file, NV_F64_XYMBR *geotiff_mbr);
  void geotiff_clear ();


  //  Make an MBC from the MBR and the data.

  mbc.min_x = misc->abe_share->viewer_displayed_area.min_x;
  mbc.min_y = misc->abe_share->viewer_displayed_area.min_y;
  mbc.max_x = misc->abe_share->viewer_displayed_area.max_x;
  mbc.max_y = misc->abe_share->viewer_displayed_area.max_y;


  //  Flip the min and max.

  mbc.max_z = -misc->abe_share->otf_min_z;
  mbc.min_z = -misc->abe_share->otf_max_z;


  //  Have to set the bounds prior to doing anything

  if (prev_mbr.min_x != misc->abe_share->viewer_displayed_area.min_x || prev_mbr.min_y != misc->abe_share->viewer_displayed_area.min_y ||
      prev_mbr.max_x != misc->abe_share->viewer_displayed_area.max_x || prev_mbr.max_y != misc->abe_share->viewer_displayed_area.max_y ||
      mbc.min_z != prev_min_z || mbc.max_z != prev_max_z)
    {
      map->setBounds (mbc);

      if (reset)
        {
          //  Get the map center so we can reset the view.

          map->getMapCenter (&misc->map_center_x, &misc->map_center_y, &misc->map_center_z);


          map->setMapCenter (misc->map_center_x, misc->map_center_y, misc->map_center_z);

          map->resetPOV ();
        }
    }


  //  If we want to display a GeoTIFF we must load the texture, unless it (or the displayed area) hasn't changed.

  if (misc->display_GeoTIFF && ((prev_mbr.min_x != misc->abe_share->viewer_displayed_area.min_x ||
                                 prev_mbr.min_y != misc->abe_share->viewer_displayed_area.min_y ||
                                 prev_mbr.max_x != misc->abe_share->viewer_displayed_area.max_x ||
                                 prev_mbr.max_y != misc->abe_share->viewer_displayed_area.max_y ||
                                 mbc.min_z != prev_min_z || mbc.max_z != prev_max_z ||
                                 misc->display_GeoTIFF != prev_display_GeoTIFF) || misc->GeoTIFF_init))
    {
      NV_F64_XYMBR geotiff_mbr;
      QImage *subImage = geotiff (misc->abe_share->viewer_displayed_area, misc->GeoTIFF_name, &geotiff_mbr);
      if (subImage != NULL)
        {
          map->setGeotiffTexture (subImage, geotiff_mbr, misc->display_GeoTIFF);
          geotiff_clear ();
          misc->GeoTIFF_init = NVFalse;
        }
      prev_display_GeoTIFF = misc->display_GeoTIFF;
    }
  prev_mbr = misc->displayed_area = misc->abe_share->viewer_displayed_area;
  prev_min_z = mbc.min_z;
  prev_max_z = mbc.max_z;


  //  Clear ALL of the data layers prior to loading.

  for (int32_t pfm = 0 ; pfm < MAX_ABE_PFMS ; pfm++) map->clearDataLayer (pfm);


  //  Now we load the otf grid from shared memory.

  QString skey;
  skey.sprintf ("%d_abe_otf_grid", misc->abe_share->ppid);

  misc->otfShare = new QSharedMemory (skey);

  if (!misc->otfShare->attach (QSharedMemory::ReadWrite))
    {
      QMessageBox::critical (parent, "pfm3D", pfm3D::tr ("Cannot attach the OTF grid shared memory - %1").arg (skey));
      return;
    }


  misc->otf_grid = (OTF_GRID_RECORD *) misc->otfShare->data ();


  int32_t layer_type = 0;
  switch (misc->abe_share->layer_type)
    {
    case AVERAGE_FILTERED_DEPTH:
    case AVERAGE_DEPTH:
      layer_type = 0;
      break;

    case MIN_FILTERED_DEPTH:
    case MIN_DEPTH:
      layer_type = -1;
      break;

    case MAX_FILTERED_DEPTH:
    case MAX_DEPTH:
      layer_type = 1;
      break;
    }


  //  I hate allocating the memory for this again but we have to flip the sign for the Z values.  On the 
  //  bright side, we free it up after we give it to OpenGL.

  data = (float **) malloc (sizeof (float *) * misc->abe_share->otf_height);
  if (data == NULL) 
    {
      fprintf (stderr, "%s %s %s %d - data - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
      parent->clean_exit (-1);
    }


  for (int32_t i = 0 ; i < misc->abe_share->otf_height ; i++)
    {
      data[i] = (float *) malloc (sizeof (float) * misc->abe_share->otf_width);
      if (data[i] == NULL) 
        {
          fprintf (stderr, "%s %s %s %d - data[i] - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
          parent->clean_exit (-1);
        }

      if (layer_type < 0)
        {
          for (int32_t j = 0 ; j < misc->abe_share->otf_width ; j++) data[i][j] = -misc->otf_grid[i * misc->abe_share->otf_width + j].min;
        }
      else
        {
          for (int32_t j = 0 ; j < misc->abe_share->otf_width ; j++) data[i][j] = -misc->otf_grid[i * misc->abe_share->otf_width + j].max;
        }
    }


  //  Let go of the shared OTF memory.

  misc->otfShare->detach ();

  map->setDataLayer (0, data, NULL, 0, 0, misc->abe_share->otf_height, misc->abe_share->otf_width, misc->abe_share->otf_y_bin_size,
                     misc->abe_share->otf_x_bin_size, -misc->abe_share->otf_null_value, misc->displayed_area, layer_type);

  for (int32_t i = 0 ; i < misc->abe_share->otf_height ; i++) free (data[i]);

  free (data);
}
