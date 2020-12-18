
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


void overlayFlag (nvMapGL *map, OPTIONS *options, MISC *misc)
{
  double              wlon, elon, slat, nlat;
  float               feature_half_size;
  QColor              tmp;
  QString             feature_search_string = QString (misc->abe_share->feature_search_string);


  uint8_t checkFeature (MISC *misc, QString feature_search_string, int32_t ftr, uint8_t *highlight, QString *feature_info);



  slat = misc->abe_share->viewer_displayed_area.min_y;
  nlat = misc->abe_share->viewer_displayed_area.max_y;
  wlon = misc->abe_share->viewer_displayed_area.min_x;
  elon = misc->abe_share->viewer_displayed_area.max_x;


  map->clearFeaturePoints ();


  //  Display features.

  if (options->display_feature)
    {
      feature_half_size = options->feature_size / 2.0;

      for (uint32_t i = 0 ; i < misc->bfd_header.number_of_records ; i++)
        {
          if (misc->feature[i].confidence_level || misc->abe_share->layer_type == AVERAGE_DEPTH || 
              misc->abe_share->layer_type == MIN_DEPTH || misc->abe_share->layer_type == MAX_DEPTH)
            {
              if (misc->feature[i].longitude >= wlon && misc->feature[i].longitude <= elon &&
                  misc->feature[i].latitude >= slat && misc->feature[i].latitude <= nlat)
                {
                  //  Check for the type of feature display

                  if (options->display_feature == 1 || (options->display_feature == 2 && misc->feature[i].confidence_level != 5) ||
                      (options->display_feature == 3 && misc->feature[i].confidence_level == 5))
                    {
                      //  Check the feature for the feature search string and highlighting.

                      QString feature_info;
                      uint8_t highlight;
                      if (checkFeature (misc, feature_search_string, i, &highlight, &feature_info))
                        {
                          if (options->display_children || !misc->feature[i].parent_record)
                            {
                              tmp = options->feature_color;


                              //  Go through here if we want to display feature info or we want to
                              //  highlight using a feature text search.

                              if (options->display_feature_info || highlight)
                                {
                                  tmp = options->feature_info_color;
                                  if (options->display_feature_info && !feature_info.isEmpty ()) 
                                    map->drawText (feature_info, misc->feature[i].longitude, misc->feature[i].latitude, -misc->feature[i].depth, 
                                                   0.0, 10, tmp, NVFalse);


                                  if (highlight)
                                    {
                                      tmp = options->highlight_color;
                                    }
                                  else
                                    {
                                      tmp = options->feature_color;
                                    }
                                }

                              if (misc->feature[i].parent_record)
                                {
                                  map->setFeaturePoints (misc->feature[i].longitude, misc->feature[i].latitude, -misc->feature[i].depth,
                                                         tmp, feature_half_size, 7, 7, NVFalse);
                                }
                              else
                                {
                                  map->setFeaturePoints (misc->feature[i].longitude, misc->feature[i].latitude, -misc->feature[i].depth,
                                                         tmp, options->feature_size, 7, 7, NVFalse);
                                }
                            }
                        }
                    }
                }
            }

          if (qApp->hasPendingEvents ())
            {
              qApp->processEvents();
              if (misc->drawing_canceled) break;
            }
        }

      map->setFeaturePoints (0.0, 0.0, 0.0, Qt::black, 0.00, 0, 0, NVTrue);
    }
  else
    {
      map->flush ();
    }
}
