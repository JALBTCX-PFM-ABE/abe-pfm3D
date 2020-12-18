
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

void paint_surface (pfm3D *parent, MISC *misc, nvMapGL *map, uint8_t reset)
{
  static NV_F64_XYMBR prev_mbr = {-999.0, -999.0, -999.0, -999.0};
  static double prev_min_z = CHRTRNULL, prev_max_z = -CHRTRNULL;
  static int32_t prev_display_GeoTIFF = 0;
  float **data;
  NV_F64_XYMBC mbc;


  void adjust_bounds (MISC *misc, int32_t pfm);
  QImage *geotiff (NV_F64_XYMBR mbr, char *geotiff_file, NV_F64_XYMBR *geotiff_mbr);
  void geotiff_clear ();



  //  Make an MBC from the MBR and the data.

  mbc.min_x = misc->abe_share->viewer_displayed_area.min_x;
  mbc.min_y = misc->abe_share->viewer_displayed_area.min_y;
  mbc.max_x = misc->abe_share->viewer_displayed_area.max_x;
  mbc.max_y = misc->abe_share->viewer_displayed_area.max_y;
  mbc.max_z = -999999999.0;
  mbc.min_z = 999999999.0;


  //  First we have to compute the minimum bounding cube for all of the available PFMs

  for (int32_t pfm = misc->abe_share->pfm_count - 1 ; pfm >= 0 ; pfm--)
    {
      //  Only if we want to display it.

      if (misc->abe_share->display_pfm[pfm])
        {
          if (misc->pfm_handle[pfm] >= 0) close_pfm_file (misc->pfm_handle[pfm]);

          if ((misc->pfm_handle[pfm] = open_existing_pfm_file (&misc->abe_share->open_args[pfm])) < 0)
            {
              QMessageBox::warning (parent, pfm3D::tr ("Open PFM Structure"),
                                    pfm3D::tr ("The file %1 is not a PFM handle or list file or there was an error reading the file.\n"
                                               "The error message returned was:\n\n%2").arg 
                                    (QDir::toNativeSeparators (QString (misc->abe_share->open_args[pfm].list_path))).arg (pfm_error_str (pfm_error)));
              parent->clean_exit (-1);
            }

          if (misc->abe_share->open_args[pfm].head.proj_data.projection)
            {
              QMessageBox::warning (parent, pfm3D::tr ("Open PFM Structure"), pfm3D::tr ("Sorry, pfm3D does not handle projected data."));
              close_pfm_file (misc->pfm_handle[pfm]);
              printf ("0\n");
              parent->clean_exit (-1);
            }


          //  Adjust bounds to nearest grid point (compute displayed_area).

          adjust_bounds (misc, pfm);


          //  If none of the area is in the displayed area the width and/or height will be 0 or negative.

          if (misc->displayed_area_width > 0 && misc->displayed_area_height > 0)
            {
              BIN_RECORD *bin;

              bin = (BIN_RECORD *) malloc (sizeof (BIN_RECORD) * misc->displayed_area_width);
              if (bin == NULL)
                {
                  fprintf (stderr, "%s %s %s %d - bin - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
                  parent->clean_exit (-1);
                }


              for (int32_t i = 0 ; i < misc->displayed_area_height ; i++)
                {
                  read_bin_row (misc->pfm_handle[pfm], misc->displayed_area_width, misc->displayed_area_row + i, misc->displayed_area_column, bin);

                  float z = -misc->abe_share->open_args[pfm].head.null_depth;

                  for (int32_t j = 0 ; j < misc->displayed_area_width ; j++)
                    {
                      switch (misc->abe_share->layer_type)
                        {
                        case AVERAGE_FILTERED_DEPTH:
                        default:
                          if (bin[j].validity & (PFM_DATA | PFM_INTERPOLATED)) z = -bin[j].avg_filtered_depth;
                          break;

                        case MIN_FILTERED_DEPTH:
                          if (bin[j].validity & PFM_DATA) z = -bin[j].min_filtered_depth;
                          break;

                        case MAX_FILTERED_DEPTH:
                          if (bin[j].validity & PFM_DATA) z = -bin[j].max_filtered_depth;
                          break;

                        case AVERAGE_DEPTH:
                          if (bin[j].num_soundings) z = -bin[j].avg_depth;
                          break;

                        case MIN_DEPTH:
                          if (bin[j].num_soundings) z = -bin[j].min_depth;
                          break;

                        case MAX_DEPTH:
                          if (bin[j].num_soundings) z = -bin[j].max_depth;
                          break;
                        }

                      if (z != -misc->abe_share->open_args[pfm].head.null_depth)
                        {
                          mbc.max_z = qMax (mbc.max_z, (double) z);
                          mbc.min_z = qMin (mbc.min_z, (double) z);
                        }
                    }
                }

              free (bin);
            }
        }
    }


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
  prev_mbr = misc->abe_share->viewer_displayed_area;
  prev_min_z = mbc.min_z;
  prev_max_z = mbc.max_z;


  //  Clear ALL of the data layers prior to loading.

  for (int32_t pfm = 0 ; pfm < MAX_ABE_PFMS ; pfm++) map->clearDataLayer (pfm);


  //  Now we load each of the PFMs (setDataLayer) using the minimum bounding cube as the limits.

  for (int32_t pfm = misc->abe_share->pfm_count - 1 ; pfm >= 0 ; pfm--)
    {
      //  Only if we want to display it.

      if (misc->abe_share->display_pfm[pfm])
        {
          //  Adjust bounds to nearest grid point (compute displayed_area).

          adjust_bounds (misc, pfm);


          //  Make sure that each PFM is actually in the desired area.  If it's not then either the width
          //  or the height will be 0 or negative.

          if (misc->displayed_area_height > 0 && misc->displayed_area_width > 0)
            {
              data = (float **) malloc (sizeof (float *) * misc->displayed_area_height);
              if (data == NULL) 
                {
                  fprintf (stderr, "%s %s %s %d - data - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
                  parent->clean_exit (-1);
                }


              BIN_RECORD *bin;

              bin = (BIN_RECORD *) malloc (sizeof (BIN_RECORD) * misc->displayed_area_width);
              if (bin == NULL)
                {
                  fprintf (stderr, "%s %s %s %d - bin - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
                  parent->clean_exit (-1);
                }


              for (int32_t i = 0 ; i < misc->displayed_area_height ; i++)
                {
                  data[i] = (float *) malloc (sizeof (float) * misc->displayed_area_width);
                  if (data[i] == NULL) 
                    {
                      fprintf (stderr, "%s %s %s %d - data[i] - %s\n", misc->progname, __FILE__, __FUNCTION__, __LINE__, strerror (errno));
                      parent->clean_exit (-1);
                    }


                  read_bin_row (misc->pfm_handle[pfm], misc->displayed_area_width, misc->displayed_area_row + i, misc->displayed_area_column, bin);

                  for (int32_t j = 0 ; j < misc->displayed_area_width ; j++)
                    {
                      data[i][j] = -misc->abe_share->open_args[pfm].head.null_depth;

                      switch (misc->abe_share->layer_type)
                        {
                        case AVERAGE_FILTERED_DEPTH:
                        default:
                          if (bin[j].validity & (PFM_DATA | PFM_INTERPOLATED)) data[i][j] = -bin[j].avg_filtered_depth;
                          break;

                        case MIN_FILTERED_DEPTH:
                          if (bin[j].validity & PFM_DATA) data[i][j] = -bin[j].min_filtered_depth;
                          break;

                        case MAX_FILTERED_DEPTH:
                          if (bin[j].validity & PFM_DATA) data[i][j] = -bin[j].max_filtered_depth;
                          break;

                        case AVERAGE_DEPTH:
                          if (bin[j].num_soundings) data[i][j] = -bin[j].avg_depth;
                          break;

                        case MIN_DEPTH:
                          if (bin[j].num_soundings) data[i][j] = -bin[j].min_depth;
                          break;
                          
                        case MAX_DEPTH:
                          if (bin[j].num_soundings) data[i][j] = -bin[j].max_depth;
                          break;
                        }
                    }
                }


              free (bin);


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

              map->setDataLayer (pfm, data, NULL, 0, 0, misc->displayed_area_height, misc->displayed_area_width, 
                                 misc->abe_share->open_args[pfm].head.y_bin_size_degrees, misc->abe_share->open_args[pfm].head.x_bin_size_degrees,
                                 -misc->abe_share->open_args[pfm].head.null_depth, misc->displayed_area, layer_type);

              for (int32_t i = 0 ; i < misc->displayed_area_height ; i++) free (data[i]);

              free (data);
            }
        }
    }
}
