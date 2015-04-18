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

page(_($help_context = "Location Transfer Settings"));

include_once($path_to_root . "/includes/ui.inc");

include_once($path_to_root . "/inventory/includes/db/items_units_db.inc");

simple_page_mode(false);
//----------------------------------------------------------------------------------

if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;

	if ($_POST['type']== 0)
	{
		$input_error = 1;
		display_error(_("Must Be a Valid Type"));
		set_focus('abbr');
	}
        if ($_POST['location']== '')
	{
		$input_error = 1;
		display_error(_("Must Be a Valid Location"));
		set_focus('abbr');
	}
        if($_POST['type']){
	$count_loc=  db_num_rows(db_query("select * from ".TB_PREF."loc_tran_settings where type=".$_POST['type']));
        
        if($count_loc>0){
        
            $input_error = 1;
		display_error(_("Type already Exist "));
		set_focus('type');
        }
        }
	if ($_POST['location'])
	{
            $count=  db_num_rows(db_query("select * from ".TB_PREF."loc_tran_settings where loc_code=".db_escape($_POST['location'])));
		if($count>0){
             $input_error = 1;
		display_error(_("Location already used by another type."));
		set_focus('description');
                }
	}

	if ($input_error !=1) {
    	write_loc_tran_settings($selected_id, $_POST['location'], $_POST['type'] );
		if($selected_id != '')
			display_notification(_('Selected setup has been updated'));
		else
			display_notification(_('New setup for location has been added'));
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
$th = array(_('Location Type'), _('Location'),"");
//inactive_control_column($th);

table_header($th);
$k = 0; //row colour counter
Global $loc_trans_type;
while ($myrow = db_fetch($result))
{
    

	alt_table_row_color($k);

	$t=$myrow['type'];
//       $t=db_fetch(data_retrieve_condition('loc_tran_settings', array('type'), array('id'=>$myrow["type"])));
        label_cell($loc_trans_type[$t]);
        $loc=db_fetch(data_retrieve_condition('locations', array('location_name'), array('loc_code'=>$myrow["location"])));
	label_cell($loc["location_name"]);
	
//	inactive_control_cell($id, $myrow["inactive"], 'item_units', 'abbr');
 	edit_button_cell("Edit".$myrow['id'], _("Edit"));
// 	delete_button_cell("Delete".$id, _("Delete"));
	end_row();
}

//inactive_control_row($th);
end_table(1);

//----------------------------------------------------------------------------------

start_table(TABLESTYLE2);

if ($selected_id != '') 
{
    
 	if ($Mode == 'Edit') {
		//editing an existing item category
display_error($selected_id);
		$myrow = get_wastageSettingLoc($selected_id);

		$_POST['location'] = $myrow["location"];
		$_POST['type'] = $myrow["type"];
	}
	hidden('selected_id', $myrow["id"]);
}

free_combo_list_cells('Type :', 'type', $_POST['type'], $loc_trans_type,array('select_submit'=>TRUE));
$query=array(array('loc_code','location_name',"select loc_code,location_name from ".TB_PREF."locations where inactive=0"));
combo_list_row("Location", 'location', $_POST['location'],"Select a Location",false,$query);
//locations_list_row("Location :", 'loc_code', $_POST['loc_code'],true);

end_table(1);

submit_add_or_update_center($selected_id == '', '', 'both');

end_form();

end_page();

?>
