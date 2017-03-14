<?php
/**
* Consulta los socios
* @author Giudici Francisco
* @param 
* @return array - socios 
*/
function get_socios($apellidos=NULL, $nombres=NULL, $nro_socio=NULL,$estado=NULL, $tipo_socio=NULL, $categoria_socio=NULL)
{
	$limit = 50;
	$select = db_select('io_socio', 'soc');
	$select->join('io_persona', 'per', 'soc.persona_id = per.id_persona');
	$select->join('io_estado_socio', 'est', 'soc.estado_socio_id = est.id_estado_socio');	
	$select->leftJoin('io_categoria_socio', 'cate', 'soc.categoria_socio_id = cate.id_categoria_socio');
	$select->leftJoin('io_tipo_socio', 'tip', 'soc.tipo_socio_id = tip.id_tipo_socio');
	$select->leftJoin('io_grupo_familiar_socio', 'gfs', 'soc.id_socio = gfs.socio_id');
	$select->leftJoin('io_grupo_familiar', 'gf', 'gfs.grupo_familiar_id = gf.id_grupo_familiar');
	$select->fields('soc', array('id_socio', 'secuencia'));
	$select->fields('per', array('apellidos', 'nombres', 'tipo_doc', 'nro_doc'));		
	$select->addField('est', 'nombre', 'estado');	
	$select->addField('tip', 'descripcion', 'tipo_socio');
	$select->addField('tip', 'id_tipo_socio', 'id_tipo_socio');
	//$select->addField('gf', 'nro_socio_titular', 'nro_socio_titular');
	
	$select->addField('cate', 'nombre', 'categoria_socio');
	
	//si los parametros tienen valor se agregan las condiciones
	if(!is_null($apellidos) && isset($apellidos))
		$select->condition('per.apellidos', '%'.db_like($apellidos).'%', 'LIKE');
	if(!is_null($nombres) && isset($nombres))
		$select->condition('per.nombres', '%'.db_like($nombres).'%', 'LIKE');
	if(!is_null($nro_socio) && isset($nro_socio))
		$select->condition('soc.nro_socio', '%'.db_like($nro_socio).'%', 'LIKE');
	if(!is_null($estado)&& isset($estado))
		$select->condition('est.id_estado_socio', $estado, '=');
	if(!is_null($tipo_socio)&& isset($tipo_socio))
		$select->condition('tip.id_tipo_socio', $tipo_socio, '=');
	if(!is_null($categoria_socio)&& isset($categoria_socio))
		$select->condition('cate.id_categoria_socio', $categoria_socio, '=');
	$select->addExpression('IF(soc.nro_socio IS NULL, gf.nro_socio_titular , soc.nro_socio)', 'nro_socio');
	$select->orderBy('nro_socio', 'asc');
	$select->orderBy('soc.secuencia', 'asc');
	//$select->limit($limit);
	# ejecutamos la consulta
	$socios = $select->execute();

	
	//drupal_set_message('<pre>'.print_r($socios, true).'</pre>');
	return $socios;	
}

function get_socio_por_id($id_socio)
{
	$socio = db_query('SELECT soc.id_socio,
	soc.nro_socio,
	soc.secuencia,	
	date_format(soc.fecha_ingreso, \'%d/%m/%Y\') as fecha_ingreso,
	soc.forma_pago_id,
	soc.tipo_socio_id,
	soc.estado_socio_id,
	soc.categoria_socio_id,
	per.tipo_doc, 
	per.nro_doc,
	per.apellidos,
	per.nombres,
	YEAR(CURDATE())-YEAR(per.fecha_nac) + IF(DATE_FORMAT(CURDATE(),\'%m-%d\') > DATE_FORMAT(per.fecha_nac,\'%m-%d\'), 0, -1) AS edad,
	date_format(per.fecha_nac, \'%d/%m/%Y\') as fecha_nac,
	per.sexo,
	per.direccion,            
	per.telefono_fijo,            
	per.celular_1,                        
	per.comp_cel_1,                       
	per.celular_2,                       
	per.comp_cel_2,            
	per.email,                        
	per.fallecido,
	est.nombre as estado,
	tip.descripcion as tipo_socio,
	cat.nombre as categoria,
	loc.ID_LOCALIDAD as id_localidad,
	prov.ID_PROVINCIA as id_provincia,
	per.id_persona,
	per.uri_foto
	FROM io_socio soc 
	LEFT JOIN io_persona per on per.id_persona = soc.persona_id
	LEFT JOIN io_estado_socio est on est.id_estado_socio = soc.estado_socio_id
	LEFT JOIN io_tipo_socio tip on soc.tipo_socio_id = tip.id_tipo_socio
	LEFT JOIN io_categoria_socio cat on soc.categoria_socio_id = cat.id_categoria_socio 
	LEFT JOIN io_localidad loc on loc.ID_LOCALIDAD = per.localidad_id 
	LEFT JOIN io_provincia prov on prov.ID_PROVINCIA = loc.PROVINCIA_ID
	WHERE soc.id_socio = :id_socio', array(':id_socio' => $id_socio ))->fetchObject();
	
	//drupal_set_message('<pre>'.print_r($socio, true).'</pre>');
	return $socio;	
}

function get_socio_por_nro_socio($nro_socio)
{
	$socio = db_query('SELECT soc.id_socio,
	soc.nro_socio,
	soc.secuencia,	
	date_format(soc.fecha_ingreso, \'%d/%m/%Y\') as fecha_ingreso,
	soc.forma_pago_id,
	soc.tipo_socio_id,
	soc.estado_socio_id,
	soc.categoria_socio_id,
	per.tipo_doc, 
	per.nro_doc,
	per.apellidos,
	per.nombres,
	YEAR(CURDATE())-YEAR(per.fecha_nac) + IF(DATE_FORMAT(CURDATE(),\'%m-%d\') > DATE_FORMAT(per.fecha_nac,\'%m-%d\'), 0, -1) AS edad,
	date_format(per.fecha_nac, \'%d/%m/%Y\') as fecha_nac,
	per.sexo,
	per.direccion,            
	per.telefono_fijo,            
	per.celular_1,                        
	per.comp_cel_1,                       
	per.celular_2,                       
	per.comp_cel_2,            
	per.email,                        
	per.fallecido,
	est.nombre as estado,
	tip.descripcion as tipo_socio,
	cat.nombre as categoria,
	loc.ID_LOCALIDAD as id_localidad,
	prov.ID_PROVINCIA as id_provincia,
	per.id_persona,
	per.uri_foto
	FROM io_socio soc 
	LEFT JOIN io_persona per on per.id_persona = soc.persona_id
	LEFT JOIN io_estado_socio est on est.id_estado_socio = soc.estado_socio_id
	LEFT JOIN io_tipo_socio tip on soc.tipo_socio_id = tip.id_tipo_socio
	LEFT JOIN io_categoria_socio cat on soc.categoria_socio_id = cat.id_categoria_socio 
	LEFT JOIN io_localidad loc on loc.ID_LOCALIDAD = per.localidad_id 
	LEFT JOIN io_provincia prov on prov.ID_PROVINCIA = loc.PROVINCIA_ID
	WHERE soc.nro_socio = :nro_socio', array(':nro_socio' => $nro_socio ))->fetchObject();
	
	//drupal_set_message('<pre>'.print_r($socio, true).'</pre>');
	return $socio;	
}

function get_socios_grupo_familiar($id_socio)
{
	//buscamos el id del grupo familiar del cual es titular
	$id_grupo_familiar = get_id_grupo_familiar($id_socio);

	$grupo_familiar_socios = db_query('SELECT per.nombres, per.apellidos, YEAR(CURDATE())-YEAR(per.fecha_nac) + IF(DATE_FORMAT(CURDATE(),\'%m-%d\') > DATE_FORMAT(per.fecha_nac,\'%m-%d\'), 0, -1) AS edad, soc.*, gf.*,gfs.parentezco, tgf.nombre as tipo_grupo_familiar, tgf.cant_int_desde, tgf.cant_int_hasta
						FROM io_socio soc 
						INNER JOIN io_persona per ON per.id_persona = soc.persona_id
						INNER JOIN io_grupo_familiar_socio gfs ON soc.id_socio = gfs.socio_id
						INNER JOIN io_grupo_familiar gf ON gf.id_grupo_familiar = gfs.grupo_familiar_id
						INNER JOIN io_tipo_grupo_familiar tgf ON gf.tipo_grupo_familiar_id = tgf.id_tipo_grupo_familiar
						WHERE gf.id_grupo_familiar = :id_grupo_familiar', array(':id_grupo_familiar' => $id_grupo_familiar ))->fetchAll();
	
	//drupal_set_message('<pre>'.print_r($grupo_familiar_socios, true).'</pre>');
	return $grupo_familiar_socios;	
}

function get_socio_grupo_familiar($id_socio)
{
	//buscamos el id del grupo familiar del cual es titular
	$id_grupo_familiar = get_id_grupo_familiar($id_socio);

	$grupo_familiar_socio = db_query('SELECT per.nombres, per.apellidos, YEAR(CURDATE())-YEAR(per.fecha_nac) + IF(DATE_FORMAT(CURDATE(),\'%m-%d\') > DATE_FORMAT(per.fecha_nac,\'%m-%d\'), 0, -1) AS edad, soc.*, gf.*,gfs.parentezco, tgf.nombre as tipo_grupo_familiar, tgf.cant_int_desde, tgf.cant_int_hasta
						FROM io_socio soc 
						INNER JOIN io_persona per ON per.id_persona = soc.persona_id
						INNER JOIN io_grupo_familiar_socio gfs ON soc.id_socio = gfs.socio_id
						INNER JOIN io_grupo_familiar gf ON gf.id_grupo_familiar = gfs.grupo_familiar_id
						INNER JOIN io_tipo_grupo_familiar tgf ON gf.tipo_grupo_familiar_id = tgf.id_tipo_grupo_familiar
						WHERE gf.id_grupo_familiar = :id_grupo_familiar AND soc.id_socio = :id_socio', array(':id_grupo_familiar' => $id_grupo_familiar, ':id_socio' => $id_socio ))->fetchObject();
	
	//drupal_set_message('<pre>'.print_r($grupo_familiar_socios, true).'</pre>');
	return $grupo_familiar_socio;	
}


//obtiene todos los tipos de socio para mostrarlos en un componente select
function get_tipos_socio_select($agregar=NULL)
{
	$result = db_query('select ts.id_tipo_socio, ts.descripcion
											from io_tipo_socio as ts
											')->fetchAll();

	//drupal_set_message('<pre>'.print_r($consorcio, true).'</pre>');
	if(empty($result[0]))
		$tipos_socio = null;
	else
	{
		$tipos_socio = array();  
		$row = array(); 

		if(!is_null($agregar))
			$tipos_socio[0]=$agregar;

		foreach($result as $record){
			$row = (array)$record;
			$tipos_socio[$row['id_tipo_socio']] = $row['descripcion'];
		}
	}

	return $tipos_socio;
}

//obtiene todos los estados de socio para mostrarlos en un componente select
function get_estados_socio_select($agregar=NULL)
{
	$result = db_query('select es.id_estado_socio, es.nombre
						from io_estado_socio as es
						where es.activo = 1
											')->fetchAll();

	//drupal_set_message('<pre>'.print_r($consorcio, true).'</pre>');
	if(empty($result[0]))
		$estados = null;
	else
	{
		$estados = array();  
		$row = array(); 

		if(!is_null($agregar))
			$estados[0]=$agregar;

		foreach($result as $record){
			$row = (array)$record;
			$estados[$row['id_estado_socio']] = $row['nombre'];
		}
	}

	return $estados;
}

//obtiene todos las categorias de socio para mostrarlos en un componente select
function get_categorias_socio_select($agregar=NULL)
{
	$result = db_query('select cat.id_categoria_socio, cat.nombre
						from io_categoria_socio as cat
						where cat.activo = 1
											')->fetchAll();

	//drupal_set_message('<pre>'.print_r($consorcio, true).'</pre>');
	if(empty($result[0]))
		$categorias = null;
	else
	{
		$categorias = array();  
		$row = array(); 

		if(!is_null($agregar))
			$categorias[0]=$agregar;

		foreach($result as $record){
			$row = (array)$record;
			$categorias[$row['id_categoria_socio']] = $row['nombre'];
		}
	}

	return $categorias;
}

//obtiene todos el personal vinculado al club como cobradores
function get_cobradores_select()
{
	$result = db_query('select pl.id_personal, per.nombres
						from io_personal as pl
						inner join io_persona as per on per.id_persona = pl.persona_id
						where per.id_persona != 3')->fetchAll();

	//drupal_set_message('<pre>'.print_r($consorcio, true).'</pre>');
	if(empty($result[0]))
		$cobradores = null;
	else
	{
		$cobradores = array();  
		$row = array(); 

		foreach($result as $record){
			$row = (array)$record;
			$cobradores[$row['id_personal']] = $row['nombres'];
		}
	}

	return $cobradores;
}

//obtiene todos las formas de pago de un socio para mostrarlos en un componente select
function get_formas_pago_select()
{
	$formas_pago = array(1=>'COBRADOR', 2=>'DEBITO AUTOMÁTICO', 3=>'MOSTRADOR');

	return $formas_pago;
}

//obtiene todas las relaciones de parentezco
function get_parentezco_select()
{
	$parentezco = array('NINGUNO'=>'NINGUNO','HIJA/O'=>'HIJA/O', 'HERMANA/O'=>'HERMANA/O', 'ESPOSA'=>'ESPOSA/O');

	return $parentezco;
}

function get_id_grupo_familiar($id_socio)
{
	//buscamos el id del grupo familiar del cual es titular
	
	$grupo_familiar = db_query('SELECT gfs.grupo_familiar_id
						FROM io_grupo_familiar_socio gfs
						WHERE gfs.socio_id = :id_socio', array(':id_socio' => $id_socio ))->fetchObject();

	//drupal_set_message('<pre>'.print_r($grupo_familiar, true).'</pre>');

	return $grupo_familiar->grupo_familiar_id;
}


//obtiene el  siguiente numero de socio libre
function get_next_num_socio()
{
	$num_socio = db_query('select MAX(soc.nro_socio)+1 as siguiente
						from io_socio as soc')->fetchObject();

	//drupal_set_message('<pre>'.print_r($consorcio, true).'</pre>');

	return $num_socio->siguiente;
}

//obtiene el siguiente numero de secuencia
function get_next_num_secuencia($id_socio)
{
	//drupal_set_message('<pre>'.print_r($id_socio, true).'</pre>');
	//buscamos el id del grupo familiar del cual es titular
	$id_grupo_familiar = get_id_grupo_familiar($id_socio);
	
	$num_secuencia = db_query('SELECT gfs.grupo_familiar_id ,MAX( soc.secuencia ) +1 AS siguiente_sec
							FROM io_grupo_familiar_socio gfs
							INNER JOIN io_socio soc ON soc.id_socio = gfs.socio_id
							WHERE grupo_familiar_id =:id_grupo_familiar', array(':id_grupo_familiar' => $id_grupo_familiar ))->fetchObject();

	//drupal_set_message('<pre>'.print_r($num_secuencia, true).'</pre>');

	return $num_secuencia;
}

//obtiene los tipos de grupos familiares
function get_tipos_grupo_familiar_select()
{
	$result = db_query('select id_tipo_grupo_familiar, nombre
						from io_tipo_grupo_familiar as tgf')->fetchAll();

	//drupal_set_message('<pre>'.print_r($consorcio, true).'</pre>');
	if(empty($result[0]))
		$tipos_gf = null;
	else
	{
		$tipos_gf = array();  
		$row = array(); 

		foreach($result as $record){
			$row = (array)$record;
			$tipos_gf[$row['id_tipo_grupo_familiar']] = $row['nombre'];
		}
	}

	return $tipos_gf;
}

/**
 * autocomplete helper
 * $string = string for search
 */
function _titular_autocomplete($string) {
  if(!is_numeric($string))
  	$string = strtoupper($string);

  $matches = array();
  $query = db_select('io_persona', 'per');
  $query->join('io_socio', 'soc', 'soc.persona_id = per.id_persona');
  $query->fields('per', array('apellidos'));
  $query->fields('soc', array('id_socio'));
  $query->condition('soc.tipo_socio_id', '3','=');
  $or = db_or()->condition('per.apellidos', '%' . db_like($string) . '%', 'LIKE')->condition('soc.nro_socio', '%' . db_like($string) . '%', 'LIKE');
  $query->condition($or);
  $result = $query->execute();

  // save the query to matches
  foreach ($result as $row) {
    $matches[$row->apellidos.'-'.$row->id_socio] = check_plain($row->apellidos);
  }

  // Return the result to the form in json
  drupal_json_output($matches);
}


/**
 * autocomplete helper
 * $string = string for search
 */
function titular_autocomplete() {
  $query = db_select('io_persona', 'per');
  $query->join('io_socio', 'soc', 'soc.persona_id = per.id_persona');
  $query->fields('per', array('apellidos', 'nombres'));
  $query->fields('soc', array('id_socio', 'nro_socio'));
  $query->condition('soc.tipo_socio_id', '3','=');
  $query->orderBy('per.apellidos', 'ASC');
  $result = $query->execute();

  //drupal_set_message('<pre>'.print_r($result, true).'</pre>');
	if(empty($result))
		$titulares = null;
	else
	{
		$titulares = array();  
		$row = array();
		
		$i = 0;  

		foreach($result as $record){
		  $row = (array)$record;
		  $titulares[$row['id_socio']] = $row['apellidos'].' '.$row['nombres'].' ['.$row['nro_socio'].']';
		  $i++;
		}
	}

	return $titulares;
}

//obtiene el grupo familiar del socio
function get_grupo_familiar($id_socio)
{	
	$grupo_familiar = db_query('SELECT gf.* 
								FROM io_grupo_familiar_socio gfs
								INNER JOIN io_socio soc ON soc.id_socio = gfs.socio_id
								INNER JOIN io_grupo_familiar gf ON gf.id_grupo_familiar = gfs.grupo_familiar_id
								WHERE soc.id_socio =:id_socio', array(':id_socio' => $id_socio ))->fetchObject();

	//drupal_set_message('<pre>'.print_r($grupo_familiar, true).'</pre>');

	return $grupo_familiar;
}

//obtiene la cuenta corriente del socio
function get_cta_cte($id_socio)
{
	//buscamos el id del grupo familiar del cual es titular
	$cta_cte_socio = db_query('SELECT date_format(cci.fecha_emision, \'%d/%m/%Y\') as fecha_emision, cci.mes, cci.anio, con.nombre as concepto, cci.valor, IF(cci.pagado = 1, \'SI\', \'NO\') AS pagado, date_format(cci.fecha_pago, \'%d/%m/%Y\') as fecha_pago, IF(cci.anulado = 1, \'SI\', \'NO\') AS anulado 
		FROM io_cta_cte cc
		INNER JOIN io_cta_cte_item cci ON cc.id_cta_cte = cci.cta_cte_id
		INNER JOIN io_socio soc ON soc.id_socio = cc.socio_id
		INNER JOIN io_concepto con ON con.id_concepto = cci.concepto_id
		WHERE socio_id = :id_socio
		ORDER BY cci.fecha_emision DESC', array(':id_socio' => $id_socio ))->fetchAll();
	
	//drupal_set_message('<pre>'.print_r($grupo_familiar_socios, true).'</pre>');
	return $cta_cte_socio;	
}

//valida si la categoria pasada como parametro corresponde con la edad pasada como parametro
function validar_categoria($edad, $id_categoria)
{
	$categoria = db_query('SELECT * 
							FROM io_categoria_socio cat							
							WHERE cat.id_categoria_socio =:id_categoria', array(':id_categoria' => $id_categoria ))->fetchObject();
	//drupal_set_message('<pre>'.print_r($categoria, true).'</pre>');
	if(is_null($id_categoria))
	{
		return true;
	}
	else
	{
		if($edad >= $categoria->edad_desde && $edad <= $categoria->edad_hasta)
		{
			return false;
		}
		else
		{
			return  true;
		}
	}	
}

//valida si la categoria pasada como parametro corresponde con la edad pasada como parametro
function validar_tipo_grupo_familiar($grupo_familiar)
{	
	//cantidad de integrantes mayores a 6 años
	$mas_de_seis = 0;
	for ($i=0; $i < count($grupo_familiar) ; $i++) { 
		if($grupo_familiar[$i]->edad >= 6)
			$mas_de_seis++;
	}

	if($mas_de_seis > $grupo_familiar[0]->cant_int_hasta || $mas_de_seis < $grupo_familiar[0]->cant_int_desde)
	{
		return true;
	}
	else
	{
		return  false;
	}
}

//retorna un socio por tipo y nro de doc
function get_socio_por_doc($tipo_doc, $nro_doc)
{
	$socio = db_query('SELECT soc.*, per.* 
						FROM io_socio soc
						INNER JOIN io_persona per ON per.id_persona = soc.persona_id			
						WHERE per.tipo_doc =:tipo_doc AND per.nro_doc = :nro_doc', array(':tipo_doc' => $tipo_doc, ':nro_doc' => $nro_doc))->fetchObject();

	//drupal_set_message('<pre>'.print_r($grupo_familiar, true).'</pre>');

	return $socio;
}

//obtiene las categorias 
function get_categorias_socio()
{
	//buscamos el id del grupo familiar del cual es titular
	$categorias = db_query('SELECT cat.*
		FROM io_categoria_socio cat')->fetchAll();
	
	//drupal_set_message('<pre>'.print_r($categorias, true).'</pre>');
	return $categorias;	
}

//obtiene una categoria de socio por su id
function get_categoria_por_id($id_categoria_socio)
{
	$categoria = db_query('SELECT cat.*
						FROM io_categoria_socio cat
						WHERE cat.id_categoria_socio =:id_categoria_socio', array(':id_categoria_socio' => $id_categoria_socio))->fetchObject();

	//drupal_set_message('<pre>'.print_r($grupo_familiar, true).'</pre>');

	return $categoria;
}

//obtiene los estados
function get_estados_socio()
{
	//buscamos el id del grupo familiar del cual es titular
	$estados = db_query('SELECT est.*
		FROM io_estado_socio est')->fetchAll();
	
	//drupal_set_message('<pre>'.print_r($categorias, true).'</pre>');
	return $estados;	
}

//obtiene un estado de socio por su id
function get_estado_por_id($id_estado_socio)
{
	$estado = db_query('SELECT est.*
						FROM io_estado_socio est
						WHERE est.id_estado_socio =:id_estado_socio', array(':id_estado_socio' => $id_estado_socio))->fetchObject();

	//drupal_set_message('<pre>'.print_r($grupo_familiar, true).'</pre>');

	return $estado;
}

//obtiene los tipos de grupo familiar
function get_get_tipos_grupo_familiar()
{
	//buscamos el id del grupo familiar del cual es titular
	$tipos_grupo_familiar = db_query('SELECT tgf.*
						FROM io_tipo_grupo_familiar tgf')->fetchAll();
	
	//drupal_set_message('<pre>'.print_r($categorias, true).'</pre>');
	return $tipos_grupo_familiar;	
}

//obtiene un tipo de grupo familiar por su id
function get_tipo_grupo_familiar_por_id($id_tipo_grupo_familiar)
{
	$tipo_grupo_familiar = db_query('SELECT tgf.*
						FROM io_tipo_grupo_familiar tgf
						WHERE tgf.id_tipo_grupo_familiar =:id_tipo_grupo_familiar', array(':id_tipo_grupo_familiar' => $id_tipo_grupo_familiar))->fetchObject();

	//drupal_set_message('<pre>'.print_r($grupo_familiar, true).'</pre>');

	return $tipo_grupo_familiar;
}

//obtiene un pago en particular por codigo de barra
function get_pago_por_cod_barras($cod_barras)
{

	$long_cod_barras = strlen($cod_barras);
	//conformacion de codigo de barras 
	//02310120175.447
	$mes = substr($cod_barras, 0, 2);
	$fecha_emision = substr($cod_barras, 2, 8);
	$fecha_dia = substr($fecha_emision, 0, 2);
	$fecha_mes = substr($fecha_emision, 2, 2);
	$fecha_anio = substr($fecha_emision, 4, 4);
	$fecha_emision = $fecha_anio.'-'.$fecha_mes.'-'.$fecha_dia;
	$num_socio = substr($cod_barras, 10, $long_cod_barras-10);
	$num_socio = str_replace('.', '', $num_socio);

	$pago = db_query('SELECT soc.nro_socio, cci.id_cta_cte_item, date_format(cci.fecha_emision, \'%d/%m/%Y\') as fecha_emision, cci.mes, cci.anio, con.nombre as concepto, cci.valor, IF(cci.pagado = 1, \'SI\', \'NO\') AS pagado, date_format(cci.fecha_pago, \'%d/%m/%Y\') as fecha_pago, IF(cci.anulado = 1, \'SI\', \'NO\') AS anulado
					FROM io_cta_cte_item cci
					INNER JOIN io_cta_cte cc on cci.cta_cte_id = cc.id_cta_cte
					INNER JOIN io_socio soc on cc.socio_id = soc.id_socio
					INNER JOIN io_concepto con on con.id_concepto = cci.concepto_id
					WHERE soc.nro_socio = :num_socio
					AND cci.fecha_emision = :fecha_emision
					AND cci.mes = :mes', array(':num_socio' => $num_socio, ':fecha_emision' => $fecha_emision, ':mes' => $mes))->fetchObject();

	//drupal_set_message('<pre>'.print_r($pago, true).'</pre>');

	return $pago;
}

function get_persona_por_id($id_persona)
{
//drupal_set_message('<pre>'.$id_grupo.'</pre>');
	$persona = db_query('SELECT *
						FROM io_persona per
						WHERE per.id_persona = :id_persona'
						,array(':id_persona' => $id_persona))->fetchObject();
		
	if(isset($persona->id_persona))
		return $persona;
	else
		return null;
}


function get_uri_foto_persona($id_persona)
{
//drupal_set_message('<pre>'.$id_grupo.'</pre>');
	$persona = db_query('SELECT *
						FROM io_persona per
						WHERE per.id_persona = :id_persona'
						,array(':id_persona' => $id_persona))->fetchObject();
		
	if(isset($persona->uri_foto))
		return $persona->uri_foto;
	else
		return null;
}



?>