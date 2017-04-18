<?php
function get_provincias_select($id_pais=1)
{
	$result = db_query('select prov.ID_PROVINCIA, prov.NOMBRE
											from io_provincia as prov
											where prov.PAIS_ID = :id_pais
											order by prov.NOMBRE asc
											', array(':id_pais'=>$id_pais))->fetchAll();

	//drupal_set_message('<pre>'.print_r($consorcio, true).'</pre>');
  if(empty($result[0]))
	$provincias = null;
	else
	{
		$provincias = array();  
    $row = array(); 
       
    foreach($result as $record){
       $row = (array)$record;
       $provincias[$row['ID_PROVINCIA']] = $row['NOMBRE'];
    }
	}

	return $provincias;

}


function get_localidades_select($id_provincia)
{
	$result = db_query('select loc.ID_LOCALIDAD, loc.NOMBRE
											from io_localidad as loc
											where loc.PROVINCIA_ID = :id_provincia
											order by loc.NOMBRE asc
											', array(':id_provincia'=>$id_provincia))->fetchAll();

	//drupal_set_message('<pre>'.print_r($id_provincia, true).'</pre>');
  if(empty($result[0]))
	$localidades = null;
	else
	{
		$localidades = array();  
    	$row = array(); 
       
	    foreach($result as $record){
	       $row = (array)$record;
	       $localidades[$row['ID_LOCALIDAD']] = $row['NOMBRE'];
	    }
	}

	return $localidades;

}

function get_roles_usuario_logueado()
{
	$result = db_query('SELECT R.rid
	FROM role R
	INNER JOIN users_roles U_R ON R.RID = U_R.RID
	INNER JOIN users U ON U.UID = U_R.UID
	WHERE U.UID = :uid', array(':uid'=>$GLOBALS['user']->uid))->fetchAll();
	
	if(empty($result[0]))
		$roles = array();
	else
	{
		$roles = array();  
    $row = array(); 
    $i = 0;

    foreach($result as $record){
     	$row = (array)$record;
		  $roles[$i] = $row['rid'];
		  $i++;
    }
	}
	//drupal_set_message('<pre>'.print_r($roles, true).'</pre>');
	return $roles;
}

function es_usr_secretaria_control($roles)
{
	if(buscar_array(9, $roles))
		return true;
	else
		return false;
}
?>