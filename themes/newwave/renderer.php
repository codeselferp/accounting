<?php
	class renderer
	{
		function wa_header()
		{
			page(_("Main Menu"), false, true);
		}

		function wa_footer()
		{
			end_page(false, true);
		}

		function menu_header($title, $no_menu, $is_index)
		{
			global $path_to_root, $applications, $help_base_url, $db_connections;
			$local_path_to_root = $path_to_root;
			global $leftmenu_save, $app_title, $version;

			// Build screen header
			$leftmenu_save = "";
			$sel_app = $_SESSION['sel_app'];
			echo "<div id='maincontainer'> \n";

			echo "<div id='topsection'> \n";
			echo "  <div class='innertube'> \n";
			echo "    <h1>" . $app_title . " " . $version . "</h1>\n";
			echo "  </div>\n";
			echo "  <div id='topinfo'>" . $db_connections[$_SESSION["wa_current_user"]->company]["name"] . " | " . $_SERVER['SERVER_NAME'] . "</div>\n";
			echo "  <div id='iconlink'>";
	   		// Logout on main window only
	   		if (!$no_menu) {
        		echo "    <a href='$local_path_to_root/access/logout.php?'><img src='$local_path_to_root/themes/newwave/images/system-shutdown.png' title='"._("Logout")."' /></a>";
     		}
  			// Popup help
     		if ($help_base_url != null) {
			  echo "<a target = '_blank' onclick=" .'"'."javascript:openWindow(this.href,this.target); return false;".'" '. "href='". help_url($title, $sel_app)."'><img src='$local_path_to_root/themes/newwave/images/help-browser.png' title='"._("Help")."' /></a></div>\n";
	   		}
     		echo "  </div>\n";

     		if (!$no_menu)
     		{
				$leftmenu_save .= "<div id='leftcolumn'>\n";
       			$leftmenu_save .= "  <div id='ddblueblockmenu'>\n";
				$leftmenu_save .= "    <div class='menutitle'>" . $_SESSION["wa_current_user"]->name . "</div>\n";
				$leftmenu_save .= "    <ul>\n";
				$leftmenu_save .= "      <li><a href='$local_path_to_root/admin/display_prefs.php?'>"._("Preferences")."</a></li>\n";
				$leftmenu_save .= "      <li><a href='$local_path_to_root/admin/change_current_user_password.php?selected_id=".$_SESSION["wa_current_user"]->username."'>"._("Change password")."</a></li>\n";
				$leftmenu_save .= "      <li><a href='$local_path_to_root/access/logout.php?'>"._("Logout")."</a></li>\n";
				$leftmenu_save .= "    </ul>\n";

				$leftmenu_save .= "    <div class='menutitle'>Applications</div>\n";
				$leftmenu_save .= "    <ul>\n";

		   		foreach($applications as $app => $name)
       			{
					$leftmenu_save .= "      <li>\n";
					$leftmenu_save .= "        <a ".($sel_app == $app ? "class='current' " : "").
			     	"href='$local_path_to_root/index.php?application=".$app.
				    SID ."'>" .$name . "</a>\n";
					if ($sel_app == $app)
				  	{
            			$curr_app_name = $name;
            			$curr_app_link = $app;
          			}
					$leftmenu_save .= "      </li>\n";
				}
				$leftmenu_save .= "    </ul>\n";
				$leftmenu_save .= "  </div>\n";
				$leftmenu_save .= "</div>\n";
			}

			echo "\n<div id='contentwrapper'>\n";
			if ($title && !$no_menu) {
				echo "  <div id='contentcolumn'>\n";
				echo "    <div class='innertube'>\n";
				echo (user_hints() ? "<span id='hints' style='float:right;'></span>" : "");
				echo "      <p class='breadcrumb'>\n";
				echo "        <a href='$local_path_to_root/index.php?application=".$curr_app_link. SID ."'>" . $curr_app_name . "</a>\n";
				if ($title && !$no_menu && !$is_index)
					echo "        <a href='#'>" . $title . "</a>\n";
				echo "      </p>\n";
     		}
		}

		function menu_footer($no_menu, $is_index)
		{
			global $leftmenu_save;

			if (!$no_menu)
			{
				echo "    </div>\n";
				echo "  </div>\n";
			}
			echo "</div>\n";
			echo $leftmenu_save;
			echo "</div>\n";
/*
				if (isset($_SESSION['wa_current_user']))
					echo "<td class=bottomBarCell>" . Today() . " | " . Now() . "</td>\n";
				echo "<td align='center' class='footer'><a target='_blank' href='$power_url'><font color='#ffffff'>$app_title $version - " . _("Theme:") . " " . user_theme() . "</font></a></td>\n";
				echo "<td align='center' class='footer'><a target='_blank' href='$power_url'><font color='#ffff00'>$power_by</font></a></td>\n";
*/
		}

		function display_applications(&$waapp)
		{

			$selected_app = &$waapp->get_selected_application();

			foreach ($selected_app->modules as $module)
			{
				echo "      <div class='shiftcontainer'>\n";
				echo "        <div class='shadowcontainer'>\n";
				echo "          <div class='innerdiv'>\n";
				echo "            <b>" . str_replace(' ','&nbsp;',$module->name) . "</b><br />\n";
				echo "            <div class='buttonwrapper'>\n";

				foreach ($module->lappfunctions as $appfunction)
				{
					$this->renderButtonsForAppFunctions($appfunction);
				}

				foreach ($module->rappfunctions as $appfunction)
				{
					$this->renderButtonsForAppFunctions($appfunction);
				}

				echo "            </div>\n";
				echo "          </div>\n";
				echo "        </div>\n";
				echo "      </div>\n";
				echo "      <br />\n";
			}
		}

		/* private, no good for php4 */ function renderButtonsForAppFunctions($appfunction)
		{
			if ($_SESSION["wa_current_user"]->can_access_page($appfunction->access)) {
          		if ($appfunction->label != "") {
            		echo "              <a class='boldbuttons' href='$appfunction->link'><span>" . str_replace(' ','&nbsp;',$appfunction->label) . "</span></a>\n";
				}
          	}
		}
	}

?>