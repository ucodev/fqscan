/*
 *	fqscan - RF Signal Scanner and Plotter - Web Interface
 *	Copyright (C) 2014  Pedro A. Hortas (pah@ucodev.org)
 * 
 *	File: fqscan.js
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


var pool;

serialize_interface_state = function() {
	return	"chart_button_bands="      + button_state('chart_button_bands')       + "&" +
		"chart_button_limits="     + button_state('chart_button_limits')      + "&" +
		"chart_button_max="        + button_state('chart_button_max')         + "&" +
		"chart_button_cur="        + button_state('chart_button_cur')         + "&" +
		"chart_button_x_coord="    + button_state('chart_button_x_coord')     + "&" +
		"chart_button_y_coord="    + button_state('chart_button_y_coord')     + "&" +
		"chart_button_x_grid="     + button_state('chart_button_x_grid')      + "&" +
		"chart_button_y_grid="     + button_state('chart_button_y_grid')      + "&" +
		"params_freq_range_start=" + jQuery('#params_freq_range_start').val() + "&" +
		"params_freq_range_end="   + jQuery('#params_freq_range_end').val()   + "&" +
		"params_coarse_bw="        + jQuery('#params_coarse_bw').val()        + "&" +
		"params_threshold="        + jQuery('#params_threshold').val()        + "&" +
		"params_freq_corr="        + jQuery('#params_freq_corr').val()        + "&" +
		"params_freq_shift="       + jQuery('#params_freq_shift').val()       + "&" +
		"params_average_count="    + jQuery('#params_average_count').val()    + "&" +
		"params_fine_bw="          + jQuery('#params_fine_bw').val()          + "&" +
		"params_fft_width="        + jQuery('#params_fft_width').val()        + "&" +
		"params_gain="             + jQuery('#params_gain').val()             + "&" +
		"params_filter="           + jQuery('#params_filter').val()           + "&" +
		"params_sample_rate="      + jQuery('#params_sample_rate').val()      + "&" +
		"params_spread="           + jQuery('#params_spread').val()           + "&" +
		"params_step="             + jQuery('#params_step').val()             + "&" +
		"params_if_gain="          + jQuery('#params_if_gain').val()          + "&" +
		"params_p_time="           + jQuery('#params_p_time').val()           + "&" +
		"params_filter_a1="        + jQuery('#params_filter_a1').val()        + "&" +
		"params_filter_a2="        + jQuery('#params_filter_a2').val()        + "&" +
		"params_filter_a3="        + jQuery('#params_filter_a3').val()        + "&" +
		"params_filter_a4="        + jQuery('#params_filter_a4').val()        + "&" +
		"params_filter_a5="        + jQuery('#params_filter_a5').val()        + "&" +
		"params_band_info="        + jQuery('#params_band_info').val()        + "&" +
		"opers_button_basic="      + button_state('opers_button_basic')       + "&" +
		"opers_button_advanced="   + button_state('opers_button_advanced')    + "&" +
		"opers_button_clear="      + button_state('opers_button_clear')       + "&" +
		"opers_button_export="     + button_state('opers_button_export')      + "&" +
		"opers_button_reset="      + button_state('opers_button_reset')       + "&" +
		"opers_button_update="     + button_state('opers_button_update')      + "&" +
		"opers_button_start="      + button_state('opers_button_start')       + "&" +
		"opers_button_stop="       + button_state('opers_button_stop')        + "&" +
		"res_chart_width="         + jQuery('#res_chart_width').val()         + "&" +
		"res_chart_height="        + jQuery('#res_chart_height').val()        + "&" +
		"interface_button_header=" + button_state('interface_button_header')  + "&" +
		"interface_button_footer=" + button_state('interface_button_footer')  + "&" +
		"interface_button_full="   + button_state('interface_button_full');
}
	
data_commit = function() {
	jQuery.ajax({
		type:	 "POST",
		url:	 "process.php",
		data:	 serialize_interface_state() + "&op=commit",
		success: function(data) {
			jQuery('#nr_scans').html(data);
		},
		error:   function(xhr, ajaxOptions, thrownError) {
			alert('Error: ' + xhr.responseText);
		}
	});
}

data_retrieve = function() {
	var pdata;

	jQuery.ajax({
		type:	 "POST",
		url:	 "process.php",
		data:	 "op=retrieve",
		success: function(data) {
			pool = JSON.parse(data)
		},
		error:   function(xhr, ajaxOptions, thrownError) {
			alert('Error: ' + xhr.responseText);
		}
	});

	return pool;
}

button_state = function(id) {
	return jQuery('#' + id).attr('class').split('_')[1];
}

button_toggle = function(id) {
	jQuery('#' + id).attr('class', 'button_' + (button_state(id) == 'on' ? 'off' : 'on'));
}

button_press = function(id) {
	button_toggle(id);

	data_commit();
}

button_press_interface_header = function() {
	if (button_state('interface_button_header') == 'on')
		jQuery('#header').slideUp(600);
	else
		jQuery('#header').slideDown(600);

	button_toggle('interface_button_header');

}

button_press_interface_footer = function() {
	if (button_state('interface_button_footer') == 'on')
		jQuery('#footer').fadeOut(600);
	else
		jQuery('#footer').fadeIn(600);

	button_toggle('interface_button_footer');
}

button_press_interface_full = function() {
	if (fullScreenApi.supportsFullScreen) {
		if (fullScreenApi.isFullScreen()) {
			if (button_state('interface_button_full') == 'on')
				button_toggle('interface_button_full');

			fullScreenApi.cancelFullScreen(document.getElementById('body'));

			/* Remove any previously added styles when switched to full screen */
			jQuery('#body').removeAttr('style');
		} else {
			if (button_state('interface_button_full') == 'off')
				button_toggle('interface_button_full');

			fullScreenApi.requestFullScreen(document.getElementById('body'));

			/* Grant that <hr /> fills the full width of <body> */
			jQuery('#body').attr('style', 'width: 100%;');
		}
	}
}

button_press_opers_basic = function() {
	button_toggle('opers_button_basic');
	button_toggle('opers_button_advanced');
	data_commit();
}

button_press_opers_advanced = function() {
	button_toggle('opers_button_basic');
	button_toggle('opers_button_advanced');
	data_commit();
}

button_press_opers_clear = function() {
	button_toggle('opers_button_clear');

	if (button_state('opers_button_stop') == 'off')
		button_press_opers_stop();

	data_commit();

	button_toggle('opers_button_clear');
}

button_press_opers_export = function() {
	button_toggle('opers_button_export');

	data_commit();
	/* TODO */

	button_toggle('opers_button_export');
}

button_press_opers_reset = function() {
	button_toggle('opers_button_reset');

	jQuery('#params_freq_range_start').val('90');
	jQuery('#params_freq_range_end').val('900');
	jQuery('#params_coarse_bw').val('0.200');
	jQuery('#params_threshold').val('2.00');
	jQuery('#params_freq_corr').val('0');
	jQuery('#params_freq_shift').val('0.000');
	jQuery('#params_average_count').val('50');
	jQuery('#params_fine_bw').val('0.025');
	jQuery('#params_fft_width').val('1000');
	jQuery('#params_gain').val('10.00');
	jQuery('#params_filter').val('dolph-chebyshev');
	jQuery('#params_sample_rate').val('2.00');
	jQuery('#params_spread').val('0.050');
	jQuery('#params_step').val('2.000');
	jQuery('#params_if_gain').val('20.00');
	jQuery('#params_p_time').val('0');
	jQuery('#params_filter_a1').val('-');
	jQuery('#params_filter_a2').val('-');
	jQuery('#params_filter_a3').val('-');
	jQuery('#params_filter_a4').val('-');
	jQuery('#params_filter_a5').val('-');
	jQuery('#params_band_info').val('generic');
	jQuery('#res_chart_width').val('1200');
	jQuery('#res_chart_height').val('400');

	data_commit();

	button_toggle('opers_button_reset');

}

button_press_opers_update = function() {
	button_toggle('opers_button_update');

	data_commit();
	/* TODO */

	button_toggle('opers_button_update');
}

button_press_opers_start = function() {
	button_toggle('opers_button_stop');
	button_toggle('opers_button_start');
	data_commit();

	if (button_state('opers_button_start') == 'on')
		jQuery('#running').html('Yes');
}

button_press_opers_stop = function() {
	button_toggle('opers_button_stop');
	button_toggle('opers_button_start');
	data_commit();

	if (button_state('opers_button_stop') == 'on')
		jQuery('#running').html('No');
}

image_refresh_current = function() {
	jQuery("#current_img").attr("src", "images/current.png?" + Math.random());
}

interface_refresh_data = function() {
	pdata = data_retrieve();

	jQuery('#nr_scans').html(pdata['nr_scans']);
	jQuery('#xo').val(pdata['xo']);
	jQuery('#xoo').val(pdata['xoo']);
	jQuery('#yo').val(pdata['yo']);
	jQuery('#yoo').val(pdata['yoo']);

	image_refresh_current();
}

interface_img_coords = function(event) {
	img_pos_left = document.getElementById("current_img").offsetLeft;
	img_pos_top = document.getElementById("current_img").offsetTop;
	img_width = document.getElementById("current_img").offsetWidth;
	img_height = document.getElementById("current_img").offsetHeight;
	chart_orig_x_border = 100;
	chart_orig_y_border = 100;
	chart_orig_width = parseInt(jQuery('#res_chart_width').val());
	chart_orig_height = parseInt(jQuery('#res_chart_height').val());
	img_x_border = chart_orig_x_border * img_width / (chart_orig_width + (chart_orig_x_border * 2));
	img_y_border = chart_orig_y_border * img_height / (chart_orig_height + (chart_orig_y_border * 2));
	chart_width = img_width - (img_x_border * 2);
	chart_height = img_height - (img_y_border * 2);
	xo = parseFloat(jQuery('#xo').val());
	xoo = parseFloat(jQuery('#xoo').val());
	yo = parseFloat(jQuery('#yo').val());
	yoo = parseFloat(jQuery('#yoo').val());
	delta_x = (xoo - xo);
	delta_y = (yoo - yo);
	ratio_x = delta_x / chart_width;
	ratio_y = delta_y / chart_height;
	x_pos = event.offsetX ? event.offsetX : (event.pageX - img_pos_left);
	y_pos = event.offsetY ? event.offsetY : (event.pageY - img_pos_top);
	x_val = Math.round((xo + ((parseInt(x_pos) - img_x_border) * ratio_x)) * 1000) / 1000;
	y_val = Math.round(-((yo + ((parseInt(y_pos) - img_y_border) * ratio_y)) - (yo + yoo)) * 1000) / 1000;

	if ((x_pos < img_x_border) || (x_pos > (chart_width + img_x_border))) {
		jQuery('#current_freq_dbm').css("position", "absolute");
		jQuery('#current_freq_dbm').css("left", "-10000");
		jQuery('#current_freq_dbm').css("top", "-10000");
		jQuery('#current_freq_dbm').hide();
		return;
	}

	if ((y_pos < img_y_border) || (y_pos > (chart_height + img_y_border))) {
		jQuery('#current_freq_dbm').css("position", "absolute");
		jQuery('#current_freq_dbm').css("left", "-10000");
		jQuery('#current_freq_dbm').css("top", "-10000");
		jQuery('#current_freq_dbm').hide();
		return;
	}
   
	jQuery('#current_freq_dbm').html(x_val + " MHz, " + y_val + " dBm");
	jQuery('#current_freq_dbm').css("position", "absolute");
	jQuery('#current_freq_dbm').css("left", img_pos_left + x_pos + 15);
	jQuery('#current_freq_dbm').css("top", img_pos_top + y_pos - 15);

	jQuery('#current_freq_dbm').show();
}

