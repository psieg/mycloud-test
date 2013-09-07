<?php
define('MC_VERSION', "0.1.0");

define('MC_SYNCSTAT_READY',0);
define('MC_SYNCSTAT_RUNNING',1); // Not used on server

define('MC_FILESTAT_COMPLETE',0);
define('MC_FILESTAT_DELETED',1);
define('MC_FILESTAT_INCOMPLETE_UP',2);
define('MC_FILESTAT_INCOMPLETE_UP_ME',3); // Not used on server
define('MC_FILESTAT_INCOMPLETE_DOWN',4); // Not used on server

define('MC_FILTERT_MATCH_NAME',0);
define('MC_FILTERT_MATCH_EXTENSION',1);
define('MC_FILTERT_MATCH_FULLNAME',2);
define('MC_FILTERT_MATCH_PATH',3);
define('MC_FILTERT_REGEX_NAME',10);
define('MC_FILTERT_REGEX_EXTENSION',11);
define('MC_FILTERT_REGEX_FULLNAME',12);
define('MC_FILTERT_REGEX_PATH',13);

define('MC_SYNCID_NONE',-1);
define('MC_FILTERID_NONE',-1);
define('MC_FILEID_NONE',-1);
?>
