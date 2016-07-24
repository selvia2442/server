<!doctype html>
<html>
<head>
<meta charset="utf-8">
<style type="text/css">


<style type="text/css">
a:visited{color:#cc00cc; text-decoration:none; }
a:hover{color:#0000ff; text-decoration:none; }
a:active{color:#ff0000; text-decoration:none; }
a:link{color:#000000; text-decoration:none; }

#nav {
margin: 0px auto;
padding: 10px;
border:2px solid #999;
font-style:bold;	
line-height:auto;
margin-bottom: 1px;
padding-left: 10px;
}

h1{color:#003;}
h2{color:#00F;
font-size:1.2em;}
fieldset{
	width:90%;
	height:100%:
	border-color:#999;
	
	color:#666;
	background:#FEFEFE;
}
div{
	width:90%;
	height:80%;
	background:#CF3;
	border:thin solid #006 margin:7px 7px;
	
	padding:10px;
	color:#003;
	font-family:Verdana, Geneva, sans-serif;
	border-bottom:dashed 1px;
	
}



#a{
font-family:바탕체;

color:#390;
}
label{width:200px;
      color:#006;
	  font-size:1.3em;
	  }
</style>
</head>
<body>
<?php
include 'mysql_selvia.php';

$sqlrec   = "select * from t_user  ";

$info     = mysql_query($sqlrec) or die("쿼리실패 : ".mysql_error());

$user_idx        = array();
$user_id         = array();
$user_name       = array();
$win_count       = array();
$lose_count      = array();
$create_date     = array();
$app_start_time  = array();
$last_play_time  = array();

for($i=1; $val=mysql_fetch_array($info); $i++ )
{ 
	$user_idx[$i]       =$val['user_idx'];
	$user_id[$i]       =$val['user_id'];
	$user_name[$i]     =$val['user_name'];
	$user_img[$i]     =$val['user_img'];
	$win_count[$i]     =$val['win_count'];
	$lose_count[$i]    =$val['lose_count'];
	$create_date[$i]   =$val['create_date'];
	$app_start_time[$i]=$val['app_start_time'];
	$app_end_time[$i]  =$val['app_end_time'];
	$last_play_time[$i]=$val['last_play_time'];
}
?> 

 <fieldset>
  <legend> <h2>회원리스트</h2></legend>
  <center>
    <h1><a href='user_input.php'>이용자 추가하기</a></h1></center>
<div id="nav">
<table width="100%" border="1" cellspacing="1" cellpadding="2" align="center">
  <tr>
    <td>NO</td>
    <td>이용자아이디</td>
    <td>이용자이름</td>
    <td>맞힌갯수</td>
    <td>틀린갯수</td>
    <td>생성일자</td>
    <td>게임시작시간</td>
    <td>게임종료시간</td>
    <td>최종게임시간</td>
    <td>상세보기</td>
  </tr>
  <?php
  for($no=1; $no < $i; $no++ )
  {
		echo"<tr>";
		echo"   <td>$no</td>";
		echo"   <td>$user_id[$no]</td>";
		echo"   <td>$user_name[$no]</td>";
		echo"   <td>$win_count[$no]</td>";
		echo"   <td>$lose_count[$no]</td>";
		echo"   <td>$create_date[$no]</td>";
		echo"  <td>$app_start_time[$no]</td>";
		echo"   <td>$app_end_time[$no]</td>";
		echo"   <td>$last_play_time[$no]</td>";
		echo"   <td><a href='user_view.php?user_idx=$user_idx[$no]'><h2>수정</h2></td>";
		echo" </tr>";
  }
  
  ?>
 
  
</table></center>
</div>

</fieldset>


  </center>
 </form>
</body>
</html>
