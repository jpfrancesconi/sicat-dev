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
?>