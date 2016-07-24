<?php

function tel_dash($tel)
{
    $tel = preg_replace("/[^0-9]/", "", $tel);    // 숫자 이외 제거
    if (substr($tel,0,2)=='02')
        return preg_replace("/([0-9]{2})([0-9]{3,4})([0-9]{4})$/", "\\1-\\2-\\3", $tel);
    else if (strlen($tel)=='8' && (substr($tel,0,2)=='15' || substr($tel,0,2)=='16' || substr($tel,0,2)=='18'))
        // 지능망 번호이면
        return preg_replace("/([0-9]{4})([0-9]{4})$/", "\\1-\\2", $tel);
    else
        return preg_replace("/([0-9]{3})([0-9]{3,4})([0-9]{4})$/", "\\1-\\2-\\3", $tel);
}


$file="/home/www/pcall/cfg/mysql.selvia"; 
$fp = fopen($file, "r");
$status = fread($fp, filesize($file));
fclose($fp);

$line1 = strtok($status, "\n"); 
$line2 = strtok("\n"); 
$line3 = strtok("\n"); 

$mysql_host;
$mysql_id;
$mysql_pw;

strtok($line1, "= \t"); $mysql_host=strtok("= \n\t");
strtok($line2, "= \t"); $mysql_id  =strtok("= \n\t");
strtok($line3, "= \t"); $mysql_pw  =strtok("= \n\t");

//$connect=mysql_connect($mysql_host,$mysql_id,$mysql_pw) or die("mysql_connect 실패!!");       
$connect=mysql_connect("localhost","selvia","tmfrlfl1") or die("mysql_connect 실패!!");       
mysql_select_db("quiz20") or die("mysql_select_db 실패!!");
?>
