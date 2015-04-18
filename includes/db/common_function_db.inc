<?php

/* * ********************************************************************

 * ********************************************************************* */

//
//	General database functions common for all modules.
//
//-------------------------------------------------------------------
// Multilevel transaction control.

function translateToWords($number) 
{
    $max_size = pow(10,12);
    if (!$number) return "zero";
    if (is_int($number) && $number < abs($max_size)) 
    {            
        switch ($number) 
        {
             case $number < 0:
                $prefix = "negative";
                $suffix = translateToWords(-1*$number);
                $string = $prefix . " " . $suffix;
                break;
            case 1:
                $string = "one";
                break;
            case 2:
                $string = "two";
                break;
            case 3:
                $string = "three";
                break;
            case 4: 
                $string = "four";
                break;
            case 5:
                $string = "five";
                break;
            case 6:
                $string = "six";
                break;
            case 7:
                $string = "seven";
                break;
            case 8:
                $string = "eight";
                break;
            case 9:
                $string = "nine";
                break;                
            case 10:
                $string = "ten";
                break;            
            case 11:
                $string = "eleven";
                break;            
            case 12:
                $string = "twelve";
                break;            
            case 13:
                $string = "thirteen";
                break;            
           case 15:
                $string = "fifteen";
                break;            
            case $number < 20:
                $string = translateToWords($number%10);
                if ($number == 18)
                {
                $suffix = "een";
                } else 
                {
                $suffix = "teen";
                }
                $string .= $suffix;
                break;            
            case 20:
                $string = "twenty";
                break;            
            case 30:
                $string = "thirty";
                break;            
            case 40:
                $string = "forty";
                break;            
            case 50:
                $string = "fifty";
                break;            
            case 60:
                $string = "sixty";
                break;            
            case 70:
                $string = "seventy";
                break;            
            case 80:
                $string = "eighty";
                break;            
            case 90:
                $string = "ninety";
                break;                
            case $number < 100:
                $prefix = translateToWords($number-$number%10);
                $suffix = translateToWords($number%10);
                $string = $prefix . "-" . $suffix;
                break;
           
            case $number < pow(10,3):                    
                $prefix = translateToWords(intval(floor($number/pow(10,2)))) . " hundred";
                if ($number%pow(10,2)) $suffix = " and " . translateToWords($number%pow(10,2));
                $string = $prefix . $suffix;
                break;
            case $number < pow(10,5):
                $prefix = translateToWords(intval(floor($number/pow(10,3)))) . " thousand";
                if ($number%pow(10,3)) $suffix = translateToWords($number%pow(10,3));
                $string = $prefix . " " . $suffix;
                break;
            case $number < pow(10,7):
                $prefix = translateToWords(intval(floor($number/pow(10,5)))) . " lakh";
                if ($number%pow(10,5)) $suffix = translateToWords($number%pow(10,5));
                $string = $prefix . " " . $suffix;
                break;                    
            case $number < pow(10,12):
                $prefix = translateToWords(intval(floor($number/pow(10,7)))) . " crore";
                if ($number%pow(10,7)) $suffix = translateToWords($number%pow(10,7));
                $string = $prefix . " " . $suffix;    
                break;                
        }
    } else
    {
        echo "ERROR with - $number<br/> Number must be an integer between -" . number_format($max_size, 0, ".", ",") . " and " . number_format($max_size, 0, ".", ",") . " exclussive.";
    }
    return $string;    
}

function role_wise_access_setup(){
    
    $role_locations=db_fetch(db_query("SELECT sr.roll_cate,sr.default_loc,loc.location_name,sr.other_loc 
        FROM ".TB_PREF."security_roles sr
        left JOIN ".TB_PREF."locations loc ON loc.loc_code=sr.default_loc  
        WHERE sr.id=".  db_escape($_SESSION["wa_current_user"]->role_id)));
    
    $_SESSION["wa_current_user"]->default_location['loc_code']=$role_locations['default_loc'];
    $_SESSION["wa_current_user"]->default_location['location_name']=$role_locations['location_name'];
    
   
    //-------------------------role locations-----------------------------
    if($role_locations['other_loc']!='all'){
        $sc=explode(',',$role_locations['other_loc']);
//        implode("','", $sc);
//       display_error(print_r($sc,true));
    $query=db_query("SELECT * FROM ".TB_PREF."locations 
        WHERE loc_code in('". implode("','", $sc)."')");
//     display_error( implode('","', $sc));
    $other_locations=array();
    while($other_loc=db_fetch($query)){
       
        $other_locations[$other_loc['loc_code']]=$other_loc['location_name'];
        
    }
    $_SESSION["wa_current_user"]->other_locations=$other_locations;
    }
    else{
        $_SESSION["wa_current_user"]->other_locations='all';
    }
    //----------------------------role category-----------------------------
        if($role_locations['roll_cate']!='all'){
    $_SESSION["wa_current_user"]->role_categories=$role_locations['roll_cate'];
    $query=db_query("SELECT * FROM ".TB_PREF."stock_category 
        WHERE category_id in(".$role_locations['roll_cate'].")");
    $role_categories=array();
    while($role_cate=db_fetch($query)){
        
        $role_categories[$role_cate['category_id']]=$role_cate['description'];
        
        
    }
    
     $_SESSION["wa_current_user"]->role_categories_name=$role_categories;
    }
    else{
        $_SESSION["wa_current_user"]->role_categories='all';
    }
//     display_error($_SESSION["wa_current_user"]->role_categories);
        }
function common_str_requsition_order(&$sp_sr_obj) {
//    display_error(print_r($sp_sr_obj,true));

   // display_error($sp_sr_obj->order);exit;
    begin_transaction();

    /* Insert to print_requisition_master  record */
    if (isset($sp_sr_obj->dyeing_order))
        $sp_sr_obj->order = $sp_sr_obj->dyeing_order;
    elseif (isset($sp_sr_obj->knit_order))
        $sp_sr_obj->order = $sp_sr_obj->knit_order;
    elseif (isset($sp_sr_obj->print_order))
        $sp_sr_obj->order = $sp_sr_obj->print_order;
    elseif (isset($sp_sr_obj->wash_order))
        $sp_sr_obj->order = $sp_sr_obj->wash_order;
    elseif (isset($sp_sr_obj->ebd_order))
        $sp_sr_obj->order = $sp_sr_obj->ebd_order;
    elseif (isset($sp_sr_obj->cut_order))
        $sp_sr_obj->order = $sp_sr_obj->cut_order;
    elseif (isset($sp_sr_obj->swi_order))
        $sp_sr_obj->order = $sp_sr_obj->swi_order;



    if (isset($sp_sr_obj->from_loc))
        $sp_sr_obj->frm_location = $sp_sr_obj->from_loc;
    if (isset($sp_sr_obj->to_loc))
        $sp_sr_obj->rcv_location = $sp_sr_obj->to_loc;

   
    $sql = "INSERT INTO " . TB_PREF . "str_requisition_master (trans_no, fr_location, to_location, order_no, po_no, order_item_no, mod_name, order_date, 
     	requir_date,order_trans, comments) VALUES(";
    $sql .= db_escape($sp_sr_obj->tr_no) . "," .
            db_escape($sp_sr_obj->frm_location) . "," .
            db_escape($sp_sr_obj->rcv_location) . "," .
            db_escape($sp_sr_obj->order) . "," .
            db_escape($sp_sr_obj->po_no) . "," .
            db_escape($sp_sr_obj->order_item_no) . "," .    //item_order
            db_escape($sp_sr_obj->mod_name) . "," .
            db_escape(date2sql($sp_sr_obj->order_date)) . ", " .
            db_escape(date2sql($sp_sr_obj->requ_date)) . ", " .
            db_escape($sp_sr_obj->order_trans) . ", " .
            db_escape($sp_sr_obj->comments) . ")";

    db_query($sql, "The str requisition record could not be inserted");


    /* Insert the print_requisition_details records */
    foreach ($sp_sr_obj->line_items as $line_no => $print_line) {
        $sql1 = "INSERT INTO " . TB_PREF . "str_requisition_details (trans_no, item_code, percent, quantity) VALUES (";
        $sql1 .= db_escape($sp_sr_obj->tr_no) . ", " .
                db_escape($print_line->stock_id) . ", " .
                db_escape($print_line->percent) . ", " .
                db_escape($print_line->quantity) . ") ";

        db_query($sql1, "One of the str requisition  order detail records could not be inserted");
    }

    commit_transaction();
    return $sp_sr_obj->tr_no;
}

function update_common_sr_order(&$sp_sr_obj) {
    // display_error(print_r($sp_sr_obj,true));
    begin_transaction();


    if (isset($sp_sr_obj->from_loc))
        $sp_sr_obj->frm_location = $sp_sr_obj->from_loc;
    if (isset($sp_sr_obj->to_loc))
        $sp_sr_obj->rcv_location = $sp_sr_obj->to_loc;

    $sql = "UPDATE " . TB_PREF . "str_requisition_master SET 
                fr_location=" . db_escape($sp_sr_obj->frm_location) . ",
                to_location=" . db_escape($sp_sr_obj->rcv_location) . ", 
		mod_name=" . db_escape($sp_sr_obj->mod_name) . ",
                order_date=" . db_escape(date2sql($sp_sr_obj->order_date)) . ",
                requir_date=" . db_escape(date2sql($sp_sr_obj->requ_date)) . ",
                comments=" . db_escape($sp_sr_obj->comments) . "
                WHERE trans_no = " . db_escape($sp_sr_obj->trans_id);



//     $sql = "INSERT INTO ".TB_PREF."str_requisition_master (trans_no, fr_location,to_location, mod_name, order_date, 
//     	requir_date, comments) VALUES(";
//     $sql .= db_escape($sp_sr_obj->tr_no) . "," .
//         db_escape($sp_sr_obj->frm_location) . "," .
//         db_escape($sp_sr_obj->rcv_location) . "," .
//         db_escape($sp_sr_obj->mod_name) . "," .
//         db_escape(date2sql( $sp_sr_obj->order_date)) . ", " .
//         db_escape(date2sql($sp_sr_obj->requ_date)) . ", " .
//         db_escape($sp_sr_obj->comments) . ")";

    db_query($sql, "The str requisition record could not be inserted");
    // display_error($sql);


    $sqlz = "DELETE FROM " . TB_PREF . "str_requisition_details WHERE trans_no=" . db_escape($sp_sr_obj->trans_id);
    db_query($sqlz, "cannot delete BOM Items");

    //  display_error($sqlz);

    /* Insert the print_requisition_details records */
    foreach ($sp_sr_obj->line_items as $line_no => $print_line) {
        $sql1 = "INSERT INTO " . TB_PREF . "str_requisition_details (trans_no, item_code, quantity) VALUES (";
        $sql1 .= db_escape($sp_sr_obj->trans_id) . ", " .
                db_escape($print_line->stock_id) . ", " .
                db_escape($print_line->quantity) . ") ";

        db_query($sql1, "One of the str requisition  order detail records could not be inserted");
        // display_error($sql1);
    }

    commit_transaction();
    return $sp_sr_obj->trans_id;
}

//
function approve_common_sr(&$sp_sr_obj) {
    
    begin_transaction();

    $sql = "UPDATE " . TB_PREF . "str_requisition_master SET 
                status = ".  db_escape(1).",
                app_by=" . db_escape($sp_sr_obj->username) . ", 
		app_date=" . db_escape(date2sql(Today())) . ",                              
                chalan_no=" . db_escape($sp_sr_obj->chalan_no) . ",
                app_note=" . db_escape($sp_sr_obj->comments) . "
                WHERE trans_no = " . db_escape($sp_sr_obj->trans_id);
    db_query($sql, "The str requisition record could not be Approved");
     // status=" . db_escape(1) . ",
    
    /* Insert the approval_requisition_details records */
    foreach ($sp_sr_obj->line_items as $line_no => $print_line) {
        $sql1 = "INSERT INTO " . TB_PREF . "str_requisition_app_details (trans_no, lot_number, brand, item_code, quantity, or_qty, available_qty) VALUES (";
        $sql1 .= db_escape($sp_sr_obj->trans_id) . ", " .
                db_escape($print_line->lot) . ", " .
                db_escape($print_line->brand) . ", " .
                db_escape($print_line->stock_id) . ", " .
                db_escape($print_line->quantity) . ", " .
                db_escape($print_line->orginal_quantity) . ", " .
                db_escape($print_line->avail_qty) . ") ";

        db_query($sql1, "One of the str requisition  order detail records could not be inserted");
        
        $sql = "UPDATE " . TB_PREF . "str_requisition_details SET 
                status=" . db_escape(1) . "
                WHERE trans_no = " . db_escape($sp_sr_obj->trans_id)." AND item_code=".db_escape($print_line->stock_id);
         db_query($sql, "The str requisition record could not be Approved");
        
    }

    commit_transaction();
    return $sp_sr_obj->trans_id;
}

function get_common_str_requis_info_print($frm_loc=FALSE, $specfic_location=FALSE, $view=FALSE) {

//    $sql = "SELECT sprm.trans_no,loc.location_name,sprm.order_no,mp.po_head, sprm.order_date,sprm.requir_date, sprm.status 
//        FROM " . TB_PREF . "str_requisition_master sprm 
//        LEFT JOIN  " . TB_PREF . "locations as loc ON loc.loc_code= sprm.to_location 
//        LEFT JOIN " . TB_PREF . "mf_po as mp ON sprm.po_no=mp.id ";
    
    $sql = "SELECT sprm.trans_no,loc.location_name,mpc.category_head,mp.po_head, sprm.order_date,sprm.requir_date, sprm.status 
        FROM " . TB_PREF . "str_requisition_master sprm 
        LEFT JOIN  " . TB_PREF . "locations as loc ON loc.loc_code= sprm.to_location 
        LEFT JOIN " . TB_PREF . "mf_po as mp ON sprm.po_no=mp.id 
        LEFT JOIN " . TB_PREF . "mf_bom as mb ON sprm.order_trans=mb.trans_no
        LEFT JOIN " . TB_PREF . "mf_pro_category as mpc ON mb.cat_id=mpc.id ";

    if ($view)
        $sql .= " where sprm.po_no > 0";
//    else
//        $sql .= " where sprm.status <> 1";

    if ($frm_loc)
        $sql .=" and sprm.fr_location=" . db_escape($frm_loc);

    if ($specfic_location) {
        $values = "(" . implode(',', $specfic_location) . ")";
        //display_error($values);
        $sql .= " and sprm.fr_location NOT IN " . $values;

//$sql .=" and sprm.fr_location in (". implode( ',',db_escape($specfic_location)).")";
    }
    $sql .=" order by sprm.id desc ";
    $result = db_query($sql,"Could'n find data !");
    return $result;
}




function get_common_str_requis_info($frm_loc=FALSE, $specfic_location=FALSE, $view=FALSE) {

//    $sql = "SELECT sprm.trans_no,loc.location_name,sprm.order_no,mp.po_head, sprm.order_date,sprm.requir_date, sprm.status 
//        FROM " . TB_PREF . "str_requisition_master sprm 
//        LEFT JOIN  " . TB_PREF . "locations as loc ON loc.loc_code= sprm.to_location 
//        LEFT JOIN " . TB_PREF . "mf_po as mp ON sprm.po_no=mp.id ";
    
    $sql = "SELECT sprm.trans_no,loc.location_name,mpc.category_head,mp.po_head, sprm.order_date,sprm.requir_date, sprm.status,sprm.order_no 
        FROM " . TB_PREF . "str_requisition_master sprm 
        LEFT JOIN  " . TB_PREF . "locations as loc ON loc.loc_code= sprm.to_location 
        LEFT JOIN " . TB_PREF . "mf_po as mp ON sprm.po_no=mp.id 
        LEFT JOIN " . TB_PREF . "mf_bom as mb ON sprm.order_trans=mb.trans_no
        LEFT JOIN " . TB_PREF . "mf_pro_category as mpc ON mb.cat_id=mpc.id ";

    if ($view)
        $sql .= " where sprm.po_no > 0";
//    else
//        $sql .= " where sprm.status <> 1";

    if ($frm_loc)
        $sql .=" and sprm.fr_location=" . db_escape($frm_loc);

    if ($specfic_location) {
        $values = "(" . implode(',', $specfic_location) . ")";
        //display_error($values);
        $sql .= " and sprm.fr_location NOT IN " . $values;

//$sql .=" and sprm.fr_location in (". implode( ',',db_escape($specfic_location)).")";
    }
    $sql .=" order by sprm.id desc ";

    return $sql;
}

function get_common_str_requis_information($frm_loc=FALSE, $specfic_location=FALSE, $view=FALSE) {

    $sql = "SELECT sprm.trans_no,loc.location_name, CONCAT(st.style_head, ' - ', mp.po_head, ' - ', cat.category_head) as bomname, sprm.order_no,mp.po_head, sprm.order_date,sprm.requir_date, sprm.status 
        FROM " . TB_PREF . "str_requisition_master sprm 
        LEFT JOIN  " . TB_PREF . "locations as loc ON loc.loc_code= sprm.to_location 
        JOIN " . TB_PREF . "mf_bom as bom ON sprm.po_no=bom.po_id
        JOIN " . TB_PREF . "mf_po as mp ON sprm.po_no=mp.id
        LEFT JOIN " . TB_PREF . "mf_style st ON st.id = bom.style_id
        LEFT JOIN " . TB_PREF . "mf_pro_category cat ON cat.id = bom.cat_id";

    if ($view)
        $sql .= " where sprm.po_no > 0";
    else
        $sql .= " where sprm.status <> 1";

    if ($frm_loc)
        $sql .=" and sprm.fr_location=" . db_escape($frm_loc);

    if ($specfic_location) {
        $values = "(" . implode(',', $specfic_location) . ")";
        //display_error($values);
        $sql .= " and sprm.fr_location NOT IN " . $values;

//$sql .=" and sprm.fr_location in (". implode( ',',db_escape($specfic_location)).")";
    }
    if (isset($_POST['bom_id']) && $_POST['bom_id'] != '')
        $sql .=" AND bom.id= " . db_escape($_POST['bom_id']);

    if (isset($_POST['order_id']) && $_POST['order_id'] != '')
        $sql .=" AND  sprm.order_no = " . db_escape($_POST['order_id']);

    if (isset($_POST['RefreshInquiry'])) {
        if (isset($_POST['CuttingOrAfterDate']) && $_POST['CuttingOrAfterDate'] != '')
            $sql .=" AND  sprm.order_date >= " . db_escape(date2sql($_POST['CuttingOrAfterDate']));

        if (isset($_POST['CuttingOrToDate']) && $_POST['CuttingOrToDate'] != '')
            $sql .=" AND  sprm.order_date <= " . db_escape(date2sql($_POST['CuttingOrToDate']));

        if (isset($_POST['CuttingReqAfterDate']) && $_POST['CuttingReqAfterDate'] != '')
            $sql .=" AND  sprm.requir_date >= " . db_escape(date2sql($_POST['CuttingReqAfterDate']));

        if (isset($_POST['CuttingReqToDate']) && $_POST['CuttingReqToDate'] != '')
            $sql .=" AND  sprm.requir_date <= " . db_escape(date2sql($_POST['CuttingReqToDate']));
    }

    $sql .=" order by sprm.id desc ";

//    display_error($sql);
    return $sql;
}

function approve_common_requ_order($tr_no, $user, $app_dt, $status, $approve_note) {
    $app_date = date2sql($app_dt);

    $sql = "UPDATE " . TB_PREF . "str_requisition_master SET 
                app_by=" . db_escape($user) . ",
                app_date=" . db_escape($app_date) . ", 
		status=" . db_escape($status) . ",
                app_note=" . db_escape($approve_note) . "
                WHERE trans_no = " . db_escape($tr_no);

    begin_transaction();
    db_query($sql, "The Order could not be updated");
    commit_transaction();

    return true;
}

function get_common_approve_order($mod_name) {
   //display_error($mod_name);
    $sql = "SELECT prm.trans_no,loc.location_name,loct.location_name as to_loc,
            prm.order_no,mp.po_head,prm.order_date,prm.requir_date, 
            prm.status,prm.order_item_no from " . TB_PREF . "str_requisition_master prm
            LEFT  JOIN  " . TB_PREF . "locations as loc on loc.loc_code= prm.fr_location 
            LEFT  JOIN  " . TB_PREF . "locations as loct on loct.loc_code=prm.to_location
            LEFT JOIN " . TB_PREF . "mf_po as mp ON mp.id=prm.po_no
            where prm.status = 1 ";
    if ($mod_name)
        $sql .=" and prm.mod_name=" . db_escape($mod_name);

    $sql .=" order by prm.id desc ";

    return $sql;
}

function get_common_approve_order_on_date($mod_name,$fd = null, $td = null) {
   //display_error($mod_name);
    $sql = "SELECT prm.trans_no,loc.location_name,loct.location_name as to_loc,
            prm.order_no,mp.po_head,prm.order_date,prm.requir_date, 
            prm.status,prm.order_item_no from " . TB_PREF . "str_requisition_master prm
            LEFT  JOIN  " . TB_PREF . "locations as loc on loc.loc_code= prm.fr_location 
            LEFT  JOIN  " . TB_PREF . "locations as loct on loct.loc_code=prm.to_location
            LEFT JOIN " . TB_PREF . "mf_po as mp ON mp.id=prm.po_no
            where prm.status = 1 ";
    if ($mod_name)
        $sql .=" and prm.mod_name=" . db_escape($mod_name);
    if(!empty($fd) && !empty($td)){
       $sql .=  " AND prm.order_date >= ". db_escape($fd) ." AND prm.order_date <= " . db_escape($td);
    }
    $sql.=" order by prm.id desc ";

    return $sql;
}

function get_common_approve_order_infor($mod_name) {

    $sql = "SELECT prm.trans_no,loc.location_name,loct.location_name as to_loc,CONCAT(st.style_head, ' - ', mp.po_head, ' - ', cat.category_head) as bomname,
            prm.order_no,mp.po_head,prm.order_date,prm.requir_date, 
            prm.status from " . TB_PREF . "str_requisition_master prm
            LEFT JOIN  " . TB_PREF . "locations as loc on loc.loc_code= prm.fr_location 
            LEFT JOIN  " . TB_PREF . "locations as loct on loct.loc_code=prm.to_location
                 JOIN " . TB_PREF . "mf_bom as bom ON prm.po_no=bom.po_id
            LEFT JOIN " . TB_PREF . "mf_po as mp ON mp.id=prm.po_no
            LEFT JOIN " . TB_PREF . "mf_style st ON st.id = bom.style_id
            LEFT JOIN " . TB_PREF . "mf_pro_category cat ON cat.id = bom.cat_id
            where prm.status = 1 ";
    if ($mod_name)
        $sql .=" and prm.mod_name=" . db_escape($mod_name);

    if (isset($_POST['bom_id']) && $_POST['bom_id'] != '')
        $sql .=" AND bom.id= " . db_escape($_POST['bom_id']);

    if (isset($_POST['order_id']) && $_POST['order_id'] != '')
        $sql .=" AND  prm.order_no = " . db_escape($_POST['order_id']);

    if (isset($_POST['RefreshInquiry'])) {
        if (isset($_POST['CuttingOrAfterDate']) && $_POST['CuttingOrAfterDate'] != '')
            $sql .=" AND  prm.order_date >= " . db_escape(date2sql($_POST['CuttingOrAfterDate']));

        if (isset($_POST['CuttingOrToDate']) && $_POST['CuttingOrToDate'] != '')
            $sql .=" AND  prm.order_date <= " . db_escape(date2sql($_POST['CuttingOrToDate']));

        if (isset($_POST['CuttingReqAfterDate']) && $_POST['CuttingReqAfterDate'] != '')
            $sql .=" AND  prm.requir_date >= " . db_escape(date2sql($_POST['CuttingReqAfterDate']));

        if (isset($_POST['CuttingReqToDate']) && $_POST['CuttingReqToDate'] != '')
            $sql .=" AND  prm.requir_date <= " . db_escape(date2sql($_POST['CuttingReqToDate']));
    }
    $sql .=" order by prm.id desc ";

    return $sql;
}

function receive_order_quantity($trans_no) {

    $sql = "SELECT sum(quantity) as re_qty,sum(rec_qty) as rec_qty  FROM " . TB_PREF . "str_requisition_details             
           WHERE trans_no = " . db_escape($trans_no);
    $query = db_query($sql, "data could not be retreived");
    $result = db_fetch($query);

    return $result;
}

function receive_order_quantity_app($trans_no) {

    $sql = "SELECT sum(quantity) as re_qty,sum(rec_qty) as rec_qty  FROM " . TB_PREF . "str_requisition_app_details             
           WHERE trans_no = " . db_escape($trans_no);
    $query = db_query($sql, "data could not be retreived");
    $result = db_fetch($query);

    return $result;
}

function receive_order_app_quantity($trans_no) {

    $sql = "SELECT sum(quantity) as re_qty,sum(rec_qty) as rec_qty  FROM " . TB_PREF . "str_requisition_app_details             
           WHERE trans_no = " . db_escape($trans_no);
    $query = db_query($sql, "data could not be retreived");
    $result = db_fetch($query);

    return $result;
}


function get_common_order_details_item($trans_id = null) {
    //display_error($trans_id);
    $sql = "SELECT * from " . TB_PREF . "str_requisition_details WHERE trans_no=" . db_escape($trans_id);
    return db_query($sql, "Can't get Schedule Info");
}

function get_app_order_details_item($trans_id = null) {
//    display_error($trans_id);
    $sql = "SELECT * from " . TB_PREF . "str_requisition_app_details WHERE trans_no=" . db_escape($trans_id);

    return db_query($sql, "Can't get Schedule Info");
}

function get_common_order_details_group_item($trans_id = null, $loc) {
    $sql = "SELECT srd.*, SUM(sm.qty) AS sqty from " . TB_PREF . "str_requisition_details srd
            left join " . TB_PREF . "stock_moves  as sm on sm.stock_id=srd.item_code
            WHERE srd.trans_no=" . db_escape($trans_id) . "AND sm.loc_code=" . db_escape($loc) . " group by sm.stock_id";

    return db_query($sql, "Can't get Schedule Info");

    //display_error($sql);
}

function get_app_common_order_details_group_item($trans_id = null, $loc) {

//     $sql = "SELECT srd.*,SUM(sm.qty) AS qty from " . TB_PREF . "str_requisition_app_details srd
//            left join " . TB_PREF . "stock_moves as sm on sm.stock_id=srd.item_code
//            WHERE srd.trans_no=" . db_escape($trans_id) . "AND sm.loc_code=" . db_escape($loc) . " group by sm.stock_id";   
//    
    $sql = "SELECT srd.* from " . TB_PREF . "str_requisition_app_details srd
            WHERE srd.trans_no=" . db_escape($trans_id);  
    
    return db_query($sql, "Can't get Schedule Info");
}

function get_common_data_order_details_by_id($id) {
    $sql = "SELECT *  FROM " . TB_PREF . "str_requisition_details WHERE id=" . db_escape($id);

    $result = db_query($sql, "could not get details");
    return $result;
}

function get_common_data_app_order_details_by_id($id) {
    $sql = "SELECT *  FROM " . TB_PREF . "str_requisition_app_details WHERE id=" . db_escape($id);

    $result = db_query($sql, "could not get details");
    return $result;
}

function get_common_order_ms_item($tr_no) {


    $sql = "SELECT * FROM " . TB_PREF . "str_requisition_master WHERE trans_no = " . db_escape($tr_no);

    return db_query($sql, "The order cannot be retrieved");
}

function update_store_requisition($selected_id, $re_qty) {

    $sql = "UPDATE " . TB_PREF . "str_requisition_details SET rec_qty=rec_qty +" . db_escape($re_qty) . " 
                WHERE id = " . db_escape($selected_id);

    db_query($sql, "The  Order could not be updated");


    $sql1 = "SELECT prm.*, prd.* FROM " . TB_PREF . "str_requisition_master prm  
            left join " . TB_PREF . "str_requisition_details prd  on prd.trans_no=prm.trans_no
            where prd.id=" . db_escape($selected_id);

    $all_items = db_query($sql1, "Can't get  data");

    return db_fetch($all_items);
}

function update_approval_store_requisition($selected_id, $re_qty) {

    $sql = "UPDATE " . TB_PREF . "str_requisition_app_details SET rec_qty=rec_qty +" . db_escape($re_qty) . " 
                WHERE id = " . db_escape($selected_id);

    db_query($sql, "The  Order could not be updated");


    $sql1 = "SELECT prm.*, prd.* FROM " . TB_PREF . "str_requisition_master prm  
            left join " . TB_PREF . "str_requisition_app_details prd  on prd.trans_no=prm.trans_no
            where prd.id=" . db_escape($selected_id);

    $all_items = db_query($sql1, "Can't get  data");

    return db_fetch($all_items);
}

function add_stock_transfer_common($Items, $location_from, $location_to, $date_, $type, $reference, $memo_,$order_id=null,$lot_no=null,$supplier_id=null) {

    global $Refs;
    
   
    begin_transaction();
    $args = func_get_args();
    $args = (object) array_combine(array('Items', 'location_from', 'location_to',
                'date_', 'type', 'reference', 'memo_'), $args);
    $args->trans_no = 0;
    hook_db_prewrite($args, ST_LOCTRANSFER);

    $transfer_id = get_next_trans_no(ST_LOCTRANSFER);



    add_stock_transfer_item($transfer_id, $Items['stock_id'], $location_from, $location_to, $date_, $type, $reference, $Items['quantity'], null, $memo_,$order_id,$lot_no,$supplier_id);



    add_comments(ST_LOCTRANSFER, $transfer_id, $date_, $memo_);

    $Refs->save(ST_LOCTRANSFER, $transfer_id, $reference);
    add_audit_trail(ST_LOCTRANSFER, $transfer_id, $date_);

    $args->trans_no = $transfer_id;
    hook_db_postwrite($args, ST_LOCTRANSFER);

    commit_transaction();

//	return $transfer_id;
}

/* ------------------------------------------stock adjustment-------------------------------------------- */

function add_stock_adjustment_common($items, $act_qty, $location, $date_, $type, $increase, $reference, $memo_, $order_id,$batch_no) {
    global $Refs;

    begin_transaction();
    $args = func_get_args();
    $args = (object) array_combine(array('items', 'location', 'date_', 'type', 'increase',
                'reference', 'memo_'), $args);
    $args->trans_no = 0;
    hook_db_prewrite($args, ST_INVADJUST);

    $adj_id = get_next_trans_no(ST_INVADJUST);
    if (!$increase)
        $act_qty = -$act_qty;

    add_stock_adjustment_item($adj_id, $items, $location, $date_, $type, $reference, $act_qty, null, $memo_, $order_id,$batch_no);

    add_comments(ST_INVADJUST, $adj_id, $date_, $memo_);

    $Refs->save(ST_INVADJUST, $adj_id, $reference);
    add_audit_trail(ST_INVADJUST, $adj_id, $date_);

    $args->trans_no = $adj_id;
    hook_db_postwrite($args, ST_INVADJUST);
    commit_transaction();
}

function insert_data($table, $data) {
    $fields = implode(",", array_keys($data));
    $values = implode(",", array_values($data));

    $sql = "INSERT INTO " . TB_PREF . $table . " (" . $fields . ")
		VALUES (" . $values . ")";
    //display_error($sql);
    db_query($sql, "could not add");

    return true;
}

function update_data($table, $data, $condition) {
    $cols = array();
    foreach ($data as $key => $val) {
        $cols[] = "$key = $val";
    }
    $where = array();
    $num = count($condition);
    foreach ($condition as $conkey => $conval) {
        $where[] = "$conkey = $conval ";
        if ($num > 1) {
            $where[] .="AND ";
            $num = $num - 1;
        }
    }
    $sql = "UPDATE " . TB_PREF . $table . " SET " . implode(', ', $cols) . " WHERE " . implode('', $where);
    //display_error($sql);
    db_query($sql, "could not update");
    if (db_num_affected_rows() > 0)
        return true;
    else
        return false;
}
function data_delete($table, $condition) {

    begin_transaction();
    $where = array();
    $num = count($condition);
    foreach ($condition as $conkey => $conval) {
        $where[] = "$conkey = '$conval' ";
        if ($num > 1) {
            $where[] .="AND ";
            $num = $num - 1;
        }
    }
    $sql = "DELETE FROM " . TB_PREF . $table . "  WHERE " . implode('', $where);
    db_query($sql, "cannot delete");
    commit_transaction();
}

function data_retrieve_condition($table, $field=false, $condition=false, $order=false) {
    //$condition=associative array $key=col name, $value= value
    //$field= array(field1,field2);
    $field_item = $field ? implode(',', $field) : ' * ';   // output =>>>    start_time , end_time

    $where = array();
    $num = count($condition);                             // output = 1
    foreach ($condition as $conkey => $conval) {
        $where[] = "$conkey = '$conval' ";               //  e_allowance=1
        if ($num > 1) {
            $where[] .="AND ";                           //if  multiple  condition passed
            $num = $num - 1;
        }
    }

    $sql = "SELECT " . $field_item . " FROM " . TB_PREF . $table;   //select start_time,end_time from hcm_worker_shift

    if ($condition)
        $sql .= " WHERE " . implode('', $where);                    //where  e_allowance=1

    if ($order)
        $sql .= " ORDER BY " . $order;
//     display_error($sql);
    return db_query($sql, "could not get Data");
}

function data_retrieve($table, $field, $item_id, $orderBy=false) {

    //$orderBy="field_name desc";

    $sql = "SELECT * FROM " . TB_PREF . $table . " WHERE " . $field . "=" . db_escape($item_id) . " ";
    if ($orderBy)
        $sql .=$orderBy;
    $sql .=" LIMIT 1";
//    display_error($sql);
    $result = db_query($sql, "could not get Data");

    return db_fetch($result);
}

function get_sql_for_table($table, $fields, $condition=false, $orderby=false) {
    //$join=array('jointable'=>'maintable.id=jointable.id');
    /* --------------------------info----------------------- */
    //$fields=array('table_field1','table_field2');
    //$condition=array('table_field1'=>$value1,'table_field2'=>$value2);
    //$orderby=array('table_field','asc');
    $field_item = implode(',', $fields);
    $sql = "SELECT " . $field_item . " FROM " . TB_PREF . $table;

//      if($join){
//          foreach($join as $join_table => $joining){
//          "LEFT JOIN ".TB_PREF.$join_table." on ".$joining." " ;
//      }
//      
//      }

    if ($condition) {
        $where = array();

        $num = count($condition);
        foreach ($condition as $conkey => $conval) {
            $where[] = "$conkey = $conval ";
            if ($num > 1) {
                $where[] .="AND ";
                $num = $num - 1;
            }
        }

        $sql .= " WHERE " . implode('', $where);
    }

    if ($orderby)
        $sql .= " order by " . $orderby[0] . " " . $orderby[1];
    //display_error($sql);
    return $sql;
}

function get_sql_for_table2($table, $fields, $condition=false, $orderby=false, $fd=  null, $td = null) {

    $field_item = implode(',', $fields);
    $sql = "SELECT " . $field_item . " FROM " . TB_PREF . $table;

    if ($condition) {
        $where = array();

        $num = count($condition);
        foreach ($condition as $conkey => $conval) {
            $where[] = "$conkey = $conval ";
            if ($num > 1) {
                $where[] .="AND ";
                $num = $num - 1;
            }
        }

        $sql .= " WHERE " . implode('', $where);
    }
    
    if(!empty($fd) && !empty($td)){
        $sql .=  " WHERE entry_dt >= ". db_escape($fd) ." AND entry_dt <= " . db_escape($td);
    }

    if ($orderby)
        $sql .= " order by " . $orderby[0] . " " . $orderby[1];
    
    return $sql;
}

function get_sql_for_table_fol($table, $fields, $condition=false, $orderby=false, $fd=  null, $td = null) {

    $field_item = implode(',', $fields);
    $sql = "SELECT " . $field_item . " FROM " . TB_PREF . $table;

    if ($condition) {
        $where = array();

        $num = count($condition);
        foreach ($condition as $conkey => $conval) {
            $where[] = "$conkey = $conval ";
            if ($num > 1) {
                $where[] .="AND ";
                $num = $num - 1;
            }
        }

        $sql .= " WHERE " . implode('', $where);
    }
    
    if(!empty($fd) && !empty($td)){
        $sql .=  " WHERE followup_date >= ". db_escape($fd) ." AND followup_date <= " . db_escape($td);
    }

    if ($orderby)
        $sql .= " order by " . $orderby[0] . " " . $orderby[1];
    
    return $sql;
}

function get_sql_for_table_inv($table, $fields, $condition=false, $orderby=false, $fd=  null, $td = null) {

    $field_item = implode(',', $fields);
    $sql = "SELECT " . $field_item . " FROM " . TB_PREF . $table;

    if ($condition) {
        $where = array();

        $num = count($condition);
        foreach ($condition as $conkey => $conval) {
            $where[] = "$conkey = $conval ";
            if ($num > 1) {
                $where[] .="AND ";
                $num = $num - 1;
            }
        }

        $sql .= " WHERE " . implode('', $where);
    }
    
    if(!empty($fd) && !empty($td)){
        $sql .=  " AND req_date >= ". db_escape($fd) ." AND req_date <= " . db_escape($td);
    }

    if ($orderby)
        $sql .= " order by " . $orderby[0] . " " . $orderby[1];
    
    return $sql;
}


function get_common_location_name($loc_code) {

    $sql = "SELECT *  FROM " . TB_PREF . "locations WHERE loc_code=" . db_escape($loc_code);

    $result = db_query($sql, "could not get location");
    return db_fetch($result);
}

function get_common_machine_planning($tr_no) {


    $sql = "SELECT dm.*,dms.machine_name FROM " . TB_PREF . "dyeing_machine_planning dm
            LEFT  JOIN  " . TB_PREF . "dye_machine_setup as dms on dms.id= dm.machine_no     
            WHERE machine_no = " . db_escape($tr_no);

    return db_query($sql, "The order cannot be retrieved");
}

function get_common_machine_planning_details_item($tr_no) {

    $sql = "SELECT dmp.*,dc.color_name from " . TB_PREF . "dyeing_machine_planning dmp
            LEFT  JOIN  " . TB_PREF . "dyeing_color as dc on dc.id= dmp.color 
            WHERE dmp.machine_no=" . db_escape($tr_no);

    return db_query($sql, "Can't get Schedule Info");
}

function update_roll_item($id, $qty) {
    begin_transaction();
    $sql = "UPDATE " . TB_PREF . "dyeing_roll_details SET 
                roll_qty=" . db_escape($qty) . "   
                WHERE id=" . db_escape($id);

    db_query($sql, "The Items record could not be Update");


    commit_transaction();
    return true;
}

//***********  common function for create manufact.   item    *******************//


function create_manufact_common_item($item_code, $item_name, $item_category) 
{

    $tax_type = 1;   // 1 for Regular

    $sql1 = "SELECT stock_id FROM " . TB_PREF . "stock_master WHERE stock_id = " . db_escape($item_code);
    $result1 = db_query($sql1, "Could not get details");
   
    if (db_num_rows($result1) > 0) {
        // return 0;
    } else {

        begin_transaction();
        
        $sales_acc = $_SESSION['SysPrefs']->prefs['default_inv_sales_act'];
        $cogs_acc = $_SESSION['SysPrefs']->prefs['default_cogs_act'];
        $inv_acc = $_SESSION['SysPrefs']->prefs['default_inventory_act'];
        $adj_acc = $_SESSION['SysPrefs']->prefs['default_adj_act'];
        $assem_acc = $_SESSION['SysPrefs']->prefs['default_assembly_act'];

        $sql2 = "INSERT INTO " . TB_PREF . "stock_master (stock_id, description, long_description, category_id,tax_type_id,
            mb_flag,sales_account,cogs_account,inventory_account,adjustment_account,assembly_account )
        VALUES (" . db_escape($item_code) . ", " . db_escape($item_name) . ", " . db_escape($item_name) . ",
        " . db_escape($item_category) . ", " . db_escape($tax_type) . ", " . db_escape('M') .  ", " . db_escape($sales_acc) . ", " 
          . db_escape($cogs_acc) . ", " . db_escape($inv_acc) . ", " . db_escape($adj_acc) . ", " . db_escape($assem_acc) .")";

        db_query($sql2, "The item could not be added");


        $sql3 = "INSERT INTO " . TB_PREF . "loc_stock (loc_code, stock_id)
        SELECT " . TB_PREF . "locations.loc_code, " . db_escape($item_code)
                . " FROM " . TB_PREF . "locations";

        db_query($sql3, "The item locstock could not be added");

        // quantity=1 and  is_foreign=0 is default

        $sql4 = "INSERT INTO " . TB_PREF . "item_codes (item_code, stock_id, description, category_id,quantity, is_foreign ) 
        VALUES( " . db_escape($item_code) . ","
                . db_escape($item_code) . ","
                . db_escape($item_name) . ","
                . db_escape($item_category) . ","
                . db_escape(1) . ","
                . db_escape(0) . ")";

        db_query($sql4, " item code could not be added");
       
        commit_transaction();

    }
}


function user_activity($table_name,$action_id,$action_type,$interface_name=null,$action_field=null){

//$action_type=1 or 2 1=Update and 2=Delete
//$action_field=array('table_field1','table_field2')
    global $db_connections;
    $sql = "INSERT INTO " . TB_PREF . "user_activity (table_name, action_id, 
        action_type, user,action_field,ip_address,interface_name) 
    VALUES( " . db_escape($db_connections[$_SESSION["wa_current_user"]->company]['tbpref'].$table_name) . ","
            . db_escape($action_id) . ","
            . db_escape($action_type) . ","
            . db_escape($_SESSION['wa_current_user']->user) . ","
            . db_escape(implode(',', $action_field)) . ","
            . db_escape($_SESSION['IPaddress']) . ","
            . db_escape($interface_name) . ")";

    db_query($sql, " User activity could not be added");
       
}