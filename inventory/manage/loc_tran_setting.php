<?php
/**********************************************************************
    Copyright (C) codeself, LLC.
	Released under the terms of the GNU General Public License, GPL, 
	as published by the Free Software Foundation, either version 3 
	of the License, or (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the License here <http://www.gnu.org/licenses/gpl-3.0.html>.
***********************************************************************/
$page_security = 'SA_UOM';
$path_to_root = "../..";
include($path_to_root . "/includes/session.inc");

page(_($help_context = "Units of Measure"));

include_once($path_to_root . "/includes/ui.inc");

include_once($path_to_root . "/inventory/includes/db/items_units_db.inc");

simple_page_mode(false);
//----------------------------------------------------------------------------------

if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;

	if (strlen($_POST['abbr']) == 0)
	{
		$input_error = 1;
		display_error(_("The unit of measure code cannot be empty."));
		set_focus('abbr');
	}
	if (strlen(db_escape($_POST['abbr']))>(20+2))
	{
		$input_error = 1;
		display_error(_("The unit of measure code is too long."));
		set_focus('abbr');
	}
	if (strlen($_POST['description']) == 0)
	{
		$input_error = 1;
		display_error(_("The unit of measure description cannot be empty."));
		set_focus('description');
	}

	if ($input_error !=1) {
    	write_item_unit($selected_id, $_POST['abbr'], $_POST['description'], $_POST['decimals'] );
		if($selected_id != '')
			display_notification(_('Selected unit has been updated'));
		else
			display_notification(_('New unit has been added'));
		$Mode = 'RESET';
	}
}

//----------------------------------------------------------------------------------

if ($Mode == 'Delete')
{

	// PREVENT DELETES IF DEPENDENT RECORDS IN 'stock_master'

	if (item_unit_used($selected_id))
	{
		display_error(_("Cannot delete this unit of measure because items have been created using this unit."));

	}
	else
	{
		delete_item_unit($selected_id);
		display_notification(_('Selected unit has been deleted'));
	}
	$Mode = 'RESET';
}

if ($Mode == 'RESET')
{
	$selected_id = '';
	$sav = get_post('show_inactive');
	unset($_POST);
	$_POST['show_inactive'] = $sav;
}

//----------------------------------------------------------------------------------

$result = get_all_location_set();

start_form();
start_table(TABLESTYLE, "width=40%");
$th = array(_('Location Type'), _('Location'),"", "");
inactive_control_column($th);

table_header($th);
$k = 0; //row colour counter

while ($myrow = db_fetch($result))
{
    

	alt_table_row_color($k);

	
        $t=db_fetch(data_retrieve_condition('loc_tran_settings', array('type'), array('id'=>$myrow["type"])));
        label_cell($t["type"]);
        $loc=db_fetch(data_retrieve_condition('locations', array('location_name'), array('loc_code'=>$myrow["location"])));
	label_cell($loc["location_name"]);
	
//	inactive_control_cell($id, $myrow["inactive"], 'item_units', 'abbr');
 	edit_button_cell("Edit".$id, _("Edit"));
 	delete_button_cell("Delete".$id, _("Delete"));
	end_row();
}

inactive_control_row($th);
end_table(1);

//----------------------------------------------------------------------------------

start_table(TABLESTYLE2);

if ($selected_id != '') 
{
 	if ($Mode == 'Edit') {
		//editing an existing item category

		$myrow = get_wastageSettingLoc($selected_id);

		$_POST['wastageLocation'] = $myrow["location"];
		
	}
	hidden('selected_id', $myrow["abbr"]);
}

locations_list_row(_("Wastage Location:"), 'wastageLocation', null);

end_table(1);

submit_add_or_update_center($selected_id == '', '', 'both');

end_form();

end_page();

?>
