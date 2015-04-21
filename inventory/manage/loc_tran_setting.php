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

$js = "";
if ($use_popup_windows)
	$js .= get_js_open_window(800, 500);
page(_($help_context = "Location Transfer Settings"),FALSE,FALSE,"",$js);

include_once($path_to_root . "/includes/ui.inc");
include_once($path_to_root . "/inventory/includes/inventory_db.inc");
include_once($path_to_root . "/inventory/includes/db/items_units_db.inc");

simple_page_mode(false);
//----------------------------------------------------------------------------------

if ($Mode=='ADD_ITEM' || $Mode=='UPDATE_ITEM') 
{

	//initialise no input errors assumed initially before we test
	$input_error = 0;

	if($Mode=='ADD_ITEM' ){
        if ($_POST['type']== 0)
	{
		$input_error = 1;
		display_error(_("Must Be a Valid Type"));
		set_focus('type');
	}
         if($_POST['type']){
	$count_loc=  db_num_rows(db_query("select * from ".TB_PREF."loc_tran_settings where type=".$_POST['type']." AND dimension=".  db_escape($_POST['dimension_id'])));
        
        if($count_loc>0){
        
            $input_error = 1;
		display_error(_("Type already Exist "));
		set_focus('type');
        }
        }
        
	if ($_POST['loc_code'])
	{
            $count=  db_num_rows(db_query("select * from ".TB_PREF."loc_tran_settings where loc_code=".db_escape($_POST['loc_code'])." AND dimension=".$_POST['dimension_id']));
		if($count>0){
             $input_error = 1;
		display_error(_("Location already used by another type."));
		set_focus('description');
                }
	}
        if ((strlen(db_escape($_POST['loc_code'])) > 7) || $_POST['loc_code']=='') //check length after conversion
	{
		$input_error = 1;
		display_error( _("The location code must be three characters or less long (including converted special chars)."));
		set_focus('loc_code');
	} 
        }
        if($_POST['dimension_id']==0)
        {
            $input_error = 1;
            display_error("Dimension can not be empty, please select a dimension");
            set_focus('dimension');
        }
     
//        if ($_POST['loc_code']== '')
//	{
//		$input_error = 1;
//		display_error(_("Must Be a Valid Location"));
//		set_focus('location');
//	}
        }
       
        if($_POST['type']==RETURN_LOCATION ||$_POST['type']==WASTAGE || $_POST['type']==SOLD){
        	$_POST['loc_code'] = strtoupper($_POST['loc_code']);

	
	if (strlen($_POST['location_name']) == 0) 
	{
		$input_error = 1;
		display_error( _("The location name must be entered."));		
		set_focus('location_name');
	}
        
        
       
	if ($input_error !=1) {
 
                if($Mode=='UPDATE_ITEM'){
                    $selected_id=$_POST['loc_code'];
               
                 update_item_location($selected_id, $_POST['location_name'], $_POST['delivery_address'],
    			$_POST['phone'], $_POST['phone2'], $_POST['fax'], $_POST['email'], $_POST['contact'],$_POST['dimension_id']);	
			display_notification(_('Selected location has been updated'));
                }  else {
                 tran_add_item_location($_POST['loc_code'], $_POST['location_name'], $_POST['delivery_address'], 
    		 	$_POST['phone'], $_POST['phone2'], $_POST['fax'], $_POST['email'], $_POST['contact'],$_POST['dimension_id']);
			display_notification(_('New location has been added'));
                }
                  
         
             write_loc_tran_settings($selected_id, $_POST['loc_code'], $_POST['type'],$_POST['dimension_id'] );
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

$result = get_all_location_set($_POST['dim']);

start_form();

start_table();
dimensions_list_row("Dimension", 'dim', null," Select", null, null, null, TRUE);
end_table(1);
start_table(TABLESTYLE, "width=40%");
$th = array(_('Location Type'), _('Location'),_("Dimension"),"");
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
	$dim=db_fetch(data_retrieve_condition('dimensions', array('name'), array('id'=>$myrow["dimension"])));
	label_cell($dim["name"]);
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

		$myrow = get_wastageSettingLoc($selected_id);


		$_POST['type'] = $myrow["type"];
         
                   $selected_id=  $myrow["location"];
                    $myrow = get_item_location($selected_id);

		$_POST['loc_code'] = $myrow["loc_code"];
		$_POST['location_name']  = $myrow["location_name"];
		$_POST['delivery_address'] = $myrow["delivery_address"];
		$_POST['contact'] = $myrow["contact"];
		$_POST['phone'] = $myrow["phone"];
		$_POST['phone2'] = $myrow["phone2"];
		$_POST['fax'] = $myrow["fax"];
		$_POST['email'] = $myrow["email"];
                $_POST['dimension'] = $myrow["dimension"];
            
                
	}
	hidden('selected_id', $myrow["id"]);
}
$id=  find_submit('Edit');

if($id==1){

    label_row("Type:", $loc_trans_type[$_POST['type']]);
}else{
free_combo_list_cells('Type :', 'type', $_POST['type'], $loc_trans_type,array('select_submit'=>TRUE));
}
//dimensions_list_row("Dimension:", "dimension_id", $_POST['dimension_id'],null,null,null,null,true);


//$Ajax->activate('_page_body');
//if($_POST['type']==MAIN_REPAIRED){

//$query=array(array('loc_code','location_name',"select loc_code,location_name from ".TB_PREF."locations where inactive=0 AND type=0 AND dimension=".$_POST['dimension_id']));
//
//combo_list_row("Location", 'location', $_POST['location'],"Select a Location",false,$query);
//
//}elseif($_POST['type']==RETURN_LOCATION ||$_POST['type']==WASTAGE || $_POST['type']==SOLD){
  
    

    
if($id!=1){
    
text_row("Location code:", "loc_code",$_POST['loc_code']);
}else{
    label_row("Location Code:", $_POST['loc_code'])  ;
}
text_row_ex(_("Location Name:"), 'location_name', 50, 50);
text_row_ex(_("Contact for deliveries:"), 'contact', 30, 30);

textarea_row(_("Address:"), 'delivery_address', null, 35, 5);	

text_row_ex(_("Telephone No:"), 'phone', 32, 30);
text_row_ex(_("Secondary Phone Number:"), 'phone2', 32, 30);
text_row_ex(_("Facsimile No:"), 'fax', 32, 30);
email_row_ex(_("E-mail:"), 'email', 30);

//}else{
//
//$query=array(array('loc_code','location_name',"select loc_code,location_name from ".TB_PREF."locations where inactive=0 AND type=1 AND dimension=".$_POST['dimension_id']));
//
//combo_list_row("Location", 'location', $_POST['location'],"Select a Location",false,$query);
//
//}

end_table(1);

submit_add_or_update_center($selected_id == '', '', 'both');

end_form();

end_page();

?>
