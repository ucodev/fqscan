<!DOCTYPE html>
<?php

/*
 *	fqscan - RF Signal Scanner and Plotter - Web Interface
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: index.php
 *
 *	Authors:
 *		- Pedro A. Hortas (pah@ucodev.org)
 *
 *	Last Update: 2014/04/15
 * 
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *	
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 */


include('core/fqpanel.php');

$panel = new fqpanel();

?>
<html>
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="cache-control" content="max-age=0" />
    <meta http-equiv="cache-control" content="no-cache" />
    <meta http-equiv="expires" content="0" />
    <meta http-equiv="expires" content="Tue, 01 Jan 1980 1:00:00 GMT" />
    <meta http-equiv="pragma" content="no-cache" />
    <title><?=$panel->config->name?> <?=$panel->config->version?></title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <meta name="author" content="<?=$panel->config->author_name?> - <?=$panel->config->author_email?>" />
    <meta name="description" content="<?=$panel->config->name?> <?=$panel->config->version?> (Home: <?=$panel->config->url?>) - Control Panel" />
    <link rel="stylesheet" href="css/main.css" type="text/css" />
    <script type="text/javascript" src="js/jquery-1.11.0.min.js"></script>
    <script type="text/javascript" src="js/fullscreen.js"></script>
    <script type="text/javascript" src="js/fqscan.js"></script>
  </head>
  <body id="body">
    <div id="header" class="control">
      <table id="header_table" class="control_table">
        <tr>
          <td>
            <fieldset id="panel_chart">
              <legend>Chart</legend>
              <table class="control_table_left">
                <tr>
                  <td>
                    <span>&nbsp;</span><br />
                    <a id="chart_button_bands" onClick="button_press(this.id);" href="javascript:void(0);" class="button_on">Bands</a>
                  </td>
                  <td>
                    <span>&nbsp;</span><br />
                    <a id="chart_button_limits" onClick="button_press(this.id);" href="javascript:void(0);" class="button_on">Limits</a>
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>&nbsp;</span><br />
                    <a id="chart_button_max" onClick="button_press(this.id);" href="javascript:void(0);" class="button_on">Max</a>
                  </td>
                  <td>
                    <span>&nbsp;</span><br />
                    <a id="chart_button_cur" onClick="button_press(this.id);" href="javascript:void(0);" class="button_on">Cur</a>
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>&nbsp;</span><br />
                    <a id="chart_button_x_coord" onClick="button_press(this.id);" href="javascript:void(0);" class="button_on">X Coord</a>
                  </td>
                  <td>
                    <span>&nbsp;</span><br />
                    <a id="chart_button_y_coord" onClick="button_press(this.id);" href="javascript:void(0);" class="button_on">Y Coord</a>
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>&nbsp;</span><br />
                    <a id="chart_button_x_grid" onClick="button_press(this.id);" href="javascript:void(0);" class="button_on">X Grid</a>
                  </td>
                  <td>
                    <span>&nbsp;</span><br />
                    <a id="chart_button_y_grid" onClick="button_press(this.id);" href="javascript:void(0);" class="button_on">Y Grid</a>
                  </td>
                </tr>
              </table>
            </fieldset>
          </td>
          <td>
            <fieldset id="panel_parameters">
              <legend>Parameters</legend>
              <table class="control_table">
                <tr>
                  <td>
                    <span>Freq. Range:</span><br />
                    <input id="params_freq_range_start" style="width: 58px" type="text" name="params_freq_range_start" value="90"/>-<input id="params_freq_range_end" style="width: 58px" type="text" name="params_freq_range_end" value="900"/>
                  </td>
                  <td>
                    <span>Coarse BW:</span><br />
                    <input id="params_coarse_bw" type="text" name="params_coarse_bw" value="0.200" />
                  </td>
                  <td>
                    <span>Threshold:</span><br />
                    <input id="params_threshold" type="text" name="params_threshold" value="2.00" />
                  </td>
                  <td>
                    <span>Freq. Corr.:</span><br />
                    <input id="params_freq_corr" type="text" name="params_freq_corr" value="0" />
                  </td>
                  <td>
                    <span>Freq. Shift:</span><br />
                    <input id="params_freq_shift" type="text" name="params_freq_shift" value="0.000" />
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>Average Count:</span><br />
                    <input id="params_average_count" type="text" name="params_average_count" value="50" />
                  </td>
                  <td>
                    <span>Fine BW:</span><br />
                    <input id="params_fine_bw" type="text" name="params_fine_bw" value="0.025" />
                  </td>
                  <td>
                    <span>FFT Width:</span><br />
                    <input id="params_fft_width" type="text" name="params_fft_width" value="1000" />
                  </td>
                  <td>
                    <span>Gain:</span><br />
                    <input id="params_gain" type="text" name="params_gain" value="10.00" />
                  </td>
                  <td>
                    <span>Filter:</span><br />
                    <select id="params_filter" name="params_filter">
                      <?php foreach ($panel->filters as $filter): ?>
                        <option <?php if ($filter['def'] == '1') { echo('selected="selected"'); } ?> value="<?=strtolower($filter['filter'])?>"><?=$filter['filter']?></option>
                      <?php endforeach; ?>
                    </select>
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>Sample Rate:</span><br />
                    <input id="params_sample_rate" type="text" name="params_sample_rate" value="2.00" />
                  </td>
                  <td>
                    <span>Spread:</span><br />
                    <input id="params_spread" type="text" name="params_spread" value="0.050" />
                  </td>
                  <td>
                    <span>Step:</span><br />
                    <input id="params_step" type="text" name="params_step" value="2.000" />
                  </td>
                  <td>
                    <span>IF Gain:</span><br />
                    <input id="params_if_gain" type="text" name="params_if_gain" value="20.00" />
                  </td>
                  <td>
                    <span>P. Time:</span><br />
                    <input id="params_p_time" type="text" name="params_p_time" value="0" />
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>Filter #1:</span><br />
                    <input id="params_filter_a1" type="text" name="params_filter_a1" value="-" />
                  </td>
                  <td>
                    <span>Filter #2:</span><br />
                    <input id="params_filter_a2" type="text" name="params_filter_a2" value="-" />
                  </td>
                  <td>
                    <span>Filter #3:</span><br />
                    <input id="params_filter_a3" type="text" name="params_filter_a3" value="-" />
                  </td>
                  <td>
                    <span>Filter #4:</span><br />
                    <input id="params_filter_a4" type="text" name="params_filter_a4" value="-" />
                  </td>
                  <td>
                    <span>Filter #5:</span><br />
                    <input id="params_filter_a5" type="text" name="params_filter_a5" value="-" />
                  </td>
                </tr>
              </table>
            </fieldset>
          </td>
          <td>
            <fieldset id="panel_operations">
              <legend>Operations</legend>
              <table class="control_table_right">
                <tr>
                  <td>
                    <span>&nbsp;</span><br/>
                    <a id="opers_button_basic" onClick="button_press_opers_basic();" href="javascript:void(0);" class="button_off">Basic</a>
                  </td>
                  <td>
                    <span>&nbsp;</span><br/>
                    <a id="opers_button_advanced" onClick="button_press_opers_advanced();" href="javascript:void(0);" class="button_on">Advanced</a>
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>&nbsp;</span><br/>
                    <a id="opers_button_clear" onClick="button_press_opers_clear();" href="javascript:void(0);" class="button_off">Clear</a>
                  </td>
                  <td>
                    <span>&nbsp;</span><br/>
                    <a id="opers_button_export" onClick="button_press_opers_export();" href="javascript:void(0);" class="button_off">Export</a>
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>&nbsp;</span><br/>
                    <a id="opers_button_reset" onClick="button_press_opers_reset();" href="javascript:void(0);" class="button_off">Reset</a>
                  </td>
                  <td>
                    <span>&nbsp;</span><br/>
                    <a id="opers_button_update" onClick="button_press_opers_update();" href="javascript:void(0);" class="button_off">Update</a>
                  </td>
                </tr>
                <tr>
                  <td>
                    <span>&nbsp;</span><br/>
                    <a id="opers_button_start" onClick="button_press_opers_start();" href="javascript:void(0);" class="button_off">Start</a>
                  </td>
                  <td>
                    <span>&nbsp;</span><br/>
                    <a id="opers_button_stop" onClick="button_press_opers_stop(this.id);" href="javascript:void(0);" class="button_on">Stop</a>
                  </td>
                </tr>
              </table>
            </fieldset>
          </td>
        </tr>
      </table>
    </div>
    <hr />
    <div id="results" class="results">
      <div id="current_freq_dbm" style="position: absolute; left: -10000px; top: -10000px;"></div>
      <img id="current_img" onmousemove="interface_img_coords(event)" class="current_img" src="images/current.png" alt="Current Scan Data" />
      <script type="text/javascript">
        setInterval(interface_refresh_data, 100);
      </script>
    </div>
    <hr />
    <div id="footer" class="chart_params">
      <table id="footer_table">
        <tr>
          <td>
            <fieldset id="panel_status">
              <legend>Status</legend>
              <table style="height: 32px;">
                <tr>
                  <td>Running:</td><td><span id="running">No</span> |</td>
                  <td>Completed Scans:</td><td><span id="nr_scans">0</span></td>
                </tr>
              </table>
            </fieldset>
          <td>
            <fieldset id="panel_resolution">
              <legend>Resolution</legend>
              <table>
                <tr>
                  <td><input id="res_chart_width" type="text" name="res_chart_width" value="1200" /> x <input id="res_chart_height" type="text" name="res_chart_height" value="400" /></td>
                </tr>
              </table>
            </fieldset>
          </td>
          <td>
            <fieldset id="panel_interface">
              <legend>Interface</legend>
              <table>
                <tr>
                  <td><a id="interface_button_header" onClick="button_press_interface_header();" href="javascript:void(0);" class="button_on">Header</a></td>
                  <td><a id="interface_button_footer" onClick="button_press_interface_footer();" href="javascript:void(0);" class="button_on">Footer</a></td>
                  <td><a id="interface_button_full" onClick="button_press_interface_full();" href="javascript:void(0);" class="button_off">Full</a></td>
                </tr>
              </table>
            </fieldset>
          </td>
        </tr>
      </table>
    </div>
    <input id="params_band_info" name="params_band_info" type="hidden" value="generic" />
    <input id="xo" type="hidden" value="0" />
    <input id="xoo" type="hidden" value="0" />
    <input id="yo" type="hidden" value="0" />
    <input id="yoo" type="hidden" value="0" />
  </body>
</html>

