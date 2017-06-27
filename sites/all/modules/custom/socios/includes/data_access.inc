<?php
/**
* Consulta los socios
* @author Giudici Francisco
* @param
* @return array - socios
*/
function get_socios($apellidos=NULL, $nombres=NULL, $nro_socio=NULL,$estado=NULL, $tipo_socio=NULL, $categoria_socio=NULL, $tipo_doc=NULL, $nro_doc=NULL)
{
	$apellidos = mb_strtoupper($apellidos, 'UTF-8');
	$nombres = mb_strtoupper($nombres, 'UTF-8');

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
	//$select->addField('gf', 'id_grupo_familiar', 'id_grupo_familiar');
	//$select->addField('gf', 'nro_socio_titular', 'nro_socio_titular');

	$select->addField('cate', 'nombre', 'categoria_socio');

	//si los parametros tienen valor se agregan las condiciones
	if(!is_null($apellidos) && isset($apellidos))
		$select->condition('per.apellidos', '%'.db_like($apellidos).'%', 'LIKE');
	if(!is_null($nombres) && isset($nombres))
		$select->condition('per.nombres', '%'.db_like($nombres).'%', 'LIKE');
	if(!is_null($tipo_doc)&& isset($tipo_doc))
		$select->condition('per.tipo_doc', $tipo_doc, '=');
	if(!is_null($nro_doc)&& isset($nro_doc))
			$select->condition('per.nro_doc', $nro_doc, '=');
	if(!is_null($nro_socio) && isset($nro_socio))
	{
		//definimos una condicion or
		$or_nro_socio = db_or();
		//al filatrar por numero de socio no solo tenemos que ver si el nro de socio pertence a un socio titular o unico (campo nro_socio) sino tambien si no es de un miembro de familia (nro_adherente)
		$or_nro_socio->condition('soc.nro_socio', '%'.db_like($nro_socio).'%', 'LIKE');
		$or_nro_socio->condition('soc.nro_adherente', '%'.db_like($nro_socio).'%', 'LIKE');
		//una vez que armamos las condiciones or la cargamos dentro de la consulta
		$select->condition($or_nro_socio);
	}

	if(!is_null($estado)&& isset($estado))
		$select->condition('est.id_estado_socio', $estado, '=');
	if(!is_null($tipo_socio)&& isset($tipo_socio))
		$select->condition('tip.id_tipo_socio', $tipo_socio, '=');
	if(!is_null($categoria_socio)&& isset($categoria_socio))
		$select->condition('cate.id_categoria_socio', $categoria_socio, '=');
	$select->addExpression('IF(tip.id_tipo_socio = 2, soc.nro_adherente , soc.nro_socio)', 'nro_socio');
	$select->addExpression('date_format(soc.fecha_ingreso, \'%d/%m/%Y\')','fecha_ingreso');
	$select->orderBy('nro_socio', 'asc');
	$select->orderBy('soc.secuencia', 'asc');
	//$select->limit($limit);
	# ejecutamos la consulta
	$socios = $select->execute();

	//drupal_set_message('<pre>'.print_r($SELECT, true).'</pre>');
	return $socios;
}

//funcion que busca los socios que pertenecen a un grupo familiar
function get_socios_gf($id_grupo_familiar)
{
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
	$select->condition('gf.id_grupo_familiar', $id_grupo_familiar, '=');
	$select->addExpression('IF(tip.id_tipo_socio = 2, soc.nro_adherente , soc.nro_socio)', 'nro_socio');
	$select->addExpression('date_format(soc.fecha_ingreso, \'%d/%m/%Y\')','fecha_ingreso');
	$select->orderBy('nro_socio', 'asc');
	$select->orderBy('soc.secuencia', 'asc');
	//$select->limit($limit);
	# ejecutamos la consulta
	$socios = $select->execute();

	//drupal_set_message('<pre>'.print_r($SELECT, true).'</pre>');
	return $socios;
}

function get_socio_por_id($id_socio)
{
	$socio = db_query('SELECT soc.id_socio,
	IF(soc.tipo_socio_id = 2, soc.nro_adherente, soc.nro_socio) as nro_socio,
	soc.secuencia,
	date_format(soc.fecha_ingreso, \'%d/%m/%Y\') as fecha_ingreso,
	soc.forma_pago_id,
	soc.cbu,
	soc.vencimiento_dia,
	soc.vencimiento_mes,
	soc.tipo_socio_id,
	soc.estado_socio_id,
	soc.categoria_socio_id,
	soc.lugar_cobro,
	soc.personal_id,
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
	per.uri_foto,
	fp.nombre as forma_pago_nombre
	FROM io_socio soc
	LEFT JOIN io_persona per on per.id_persona = soc.persona_id
	LEFT JOIN io_estado_socio est on est.id_estado_socio = soc.estado_socio_id
	LEFT JOIN io_tipo_socio tip on soc.tipo_socio_id = tip.id_tipo_socio
	LEFT JOIN io_categoria_socio cat on soc.categoria_socio_id = cat.id_categoria_socio
	LEFT JOIN io_forma_pago fp on soc.forma_pago_id = fp.id_forma_pago
	LEFT JOIN io_localidad loc on loc.ID_LOCALIDAD = per.localidad_id
	LEFT JOIN io_provincia prov on prov.ID_PROVINCIA = loc.PROVINCIA_ID
	WHERE soc.id_socio = :id_socio', array(':id_socio' => $id_socio ))->fetchObject();

	//drupal_set_message('<pre>'.print_r($socio, true).'</pre>');
	return $socio;
}

function get_socio_por_nro_socio($nro_socio)
{
	$socio = db_query('SELECT soc.id_socio,
	IF(soc.tipo_socio_id = 2, soc.nro_adherente, soc.nro_socio) as nro_socio,
	soc.secuencia,
	date_format(soc.fecha_ingreso, \'%d/%m/%Y\') as fecha_ingreso,
	soc.forma_pago_id,
	soc.cbu,
	soc.vencimiento_dia,
	soc.vencimiento_mes,
	soc.tipo_socio_id,
	soc.estado_socio_id,
	soc.categoria_socio_id,
	soc.lugar_cobro,
	soc.personal_id,
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
	per.uri_foto,
	fp.nombre as forma_pago_nombre
	FROM io_socio soc
	LEFT JOIN io_persona per on per.id_persona = soc.persona_id
	LEFT JOIN io_estado_socio est on est.id_estado_socio = soc.estado_socio_id
	LEFT JOIN io_tipo_socio tip on soc.tipo_socio_id = tip.id_tipo_socio
	LEFT JOIN io_categoria_socio cat on soc.categoria_socio_id = cat.id_categoria_socio
	LEFT JOIN io_forma_pago fp on soc.forma_pago_id = fp.id_forma_pago
	LEFT JOIN io_localidad loc on loc.ID_LOCALIDAD = per.localidad_id
	LEFT JOIN io_provincia prov on prov.ID_PROVINCIA = loc.PROVINCIA_ID
	WHERE (soc.nro_socio = :nro_socio or soc.nro_adherente = :nro_socio)
	ORDER BY soc.secuencia ASC', array(':nro_socio' => $nro_socio ))->fetchObject();

	//drupal_set_message('<pre>'.print_r($socio, true).'</pre>');
	return $socio;
}

function get_socio_por_tipo_nro_doc($tipo_doc, $nro_doc)
{
	$socio = db_query('SELECT soc.id_socio,
	IF(soc.tipo_socio_id = 2, soc.nro_adherente, soc.nro_socio) as nro_socio,
	soc.secuencia,
	date_format(soc.fecha_ingreso, \'%d/%m/%Y\') as fecha_ingreso,
	soc.forma_pago_id,
	soc.cbu,
	soc.vencimiento_dia,
	soc.vencimiento_mes,
	soc.tipo_socio_id,
	soc.estado_socio_id,
	soc.categoria_socio_id,
	soc.lugar_cobro,
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
	per.uri_foto,
	fp.nombre as forma_pago_nombre
	FROM io_socio soc
	LEFT JOIN io_persona per on per.id_persona = soc.persona_id
	LEFT JOIN io_estado_socio est on est.id_estado_socio = soc.estado_socio_id
	LEFT JOIN io_tipo_socio tip on soc.tipo_socio_id = tip.id_tipo_socio
	LEFT JOIN io_categoria_socio cat on soc.categoria_socio_id = cat.id_categoria_socio
	LEFT JOIN io_forma_pago fp on soc.forma_pago_id = fp.id_forma_pago
	LEFT JOIN io_localidad loc on loc.ID_LOCALIDAD = per.localidad_id
	LEFT JOIN io_provincia prov on prov.ID_PROVINCIA = loc.PROVINCIA_ID
	WHERE per.tipo_doc = :tipo_doc
	AND per.nro_doc = :nro_doc', array(':tipo_doc' => $tipo_doc, ':nro_doc'=>$nro_doc ))->fetchObject();

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

	//drupal_set_message('<pre>'.print_r($grupo_familiar_socio, true).'</pre>');
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
						where per.id_persona != 3
						and pl.activo = 1')->fetchAll();

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

//obtiene todos el personal vinculado al club como cobradores y tambien agrega al club talleres
function get_cobradores_club_select($seleccionar=false)
{
	$result = db_query('select pl.id_personal, per.nombres
						from io_personal as pl
						inner join io_persona as per on per.id_persona = pl.persona_id
						where pl.activo = 1
						')->fetchAll();

	//drupal_set_message('<pre>'.print_r($consorcio, true).'</pre>');
	if(empty($result[0]))
		$cobradores = null;
	else
	{
		$cobradores = array();
		$row = array();

		if($seleccionar)
			array_push($cobradores, '- Seleccionar -');

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
	$parentezco = array('NINGUNO'=>'NINGUNO','HIJA/O'=>'HIJA/O', 'HERMANA/O'=>'HERMANA/O', 'ESPOSA'=>'ESPOSA/O', 'MADRE'=>'MADRE', 'PADRE'=>'PADRE', 'PRIMA/O'=>'PRIMA/O', 'ABUELA/O'=>'ABUELA/O', 'SOBRINA/O'=>'SOBRINA/O', 'CONYUGE/O'=>'CONYUGE/O', 'SUEGRA/O'=>'SUEGRA/O', 'CUÑADA/O'=>'CUÑADA/O', 'OTRO'=>'OTRO');

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

	$num_secuencia = db_query('SELECT gfs.grupo_familiar_id ,MAX( soc.secuencia ) +1 AS siguiente_sec, gf.nro_socio_titular
							FROM io_grupo_familiar_socio gfs
							INNER JOIN io_socio soc ON soc.id_socio = gfs.socio_id
							INNER JOIN io_grupo_familiar gf ON gf.id_grupo_familiar = gfs.grupo_familiar_id
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
	$grupo_familiar = db_query('SELECT gf.*, gft.nombre as nombre_tipo_grupo_familiar
								FROM io_grupo_familiar_socio gfs
								INNER JOIN io_socio soc ON soc.id_socio = gfs.socio_id
								INNER JOIN io_grupo_familiar gf ON gf.id_grupo_familiar = gfs.grupo_familiar_id
								INNER JOIN io_tipo_grupo_familiar gft ON gft.id_tipo_grupo_familiar = gf.tipo_grupo_familiar_id
								WHERE soc.id_socio =:id_socio', array(':id_socio' => $id_socio ))->fetchObject();

   //drupal_set_message('<pre>'.print_r($grupo_familiar, true).'</pre>');

	return $grupo_familiar;
}

//obtiene la cuenta corriente del socio
function get_estado_cta_cte($id_socio)
{
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
		//buscamos el tipo de estado para ver si no es de tipo baja
		$tipo_estado = get_tipo_estado($grupo_familiar[$i]->estado_socio_id);
		//si tipo_estado vale 1 es porque esta en baja el socio de ser asi no computa para validar
		if($tipo_estado == 0)
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

//obtiene un personal en particualar
function get_personal_por_id($id_personal)
{
	$personal = db_query('SELECT personal.*, per.*
						FROM io_personal personal
						INNER JOIN io_persona per on per.id_persona = personal.persona_id
						WHERE personal.id_personal=:id_personal', array(':id_personal' => $id_personal))->fetchObject();

	//drupal_set_message('<pre>'.print_r($personal, true).'</pre>');

	return $personal;
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
	//drupal_set_message('MES:'.$mes);
	$fecha_emision = substr($cod_barras, 2, 8);
	///drupal_set_message('FECHA EMISION:'.$fecha_emision);
	$fecha_dia = substr($fecha_emision, 0, 2);
	$fecha_mes = substr($fecha_emision, 2, 2);
	$fecha_anio = substr($fecha_emision, 4, 4);
	$fecha_emision = $fecha_anio.'-'.$fecha_mes.'-'.$fecha_dia;
	//drupal_set_message('FECHA EMISION 2:'.$fecha_emision);
	$num_socio = substr($cod_barras, 10, $long_cod_barras-10);
	//drupal_set_message('NUMERO SOCIO :'.$num_socio);
	$num_socio = str_replace('.', '', $num_socio);
	//drupal_set_message('NUMERO SOCIO 2:'.$num_socio);

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

//obtiene un pago en particular por un socio particular para un mes particular
function get_pago_por_socio($num_socio, $mes)
{
	$pago = db_query('SELECT soc.nro_socio, cci.id_cta_cte_item, date_format(cci.fecha_emision, \'%d/%m/%Y\') as fecha_emision, cci.mes, cci.anio, con.nombre as concepto, cci.valor, IF(cci.pagado = 1, \'SI\', \'NO\') AS pagado, date_format(cci.fecha_pago, \'%d/%m/%Y\') as fecha_pago, IF(cci.anulado = 1, \'SI\', \'NO\') AS anulado
					FROM io_cta_cte_item cci
					INNER JOIN io_cta_cte cc on cci.cta_cte_id = cc.id_cta_cte
					INNER JOIN io_socio soc on cc.socio_id = soc.id_socio
					INNER JOIN io_concepto con on con.id_concepto = cci.concepto_id
					WHERE soc.nro_socio = :num_socio
					AND cci.mes = :mes', array(':num_socio' => $num_socio, ':mes' => $mes))->fetchObject();

	//drupal_set_message('<pre>'.print_r($pago, true).'</pre>');

	return $pago;
}

//obtiene un pago en particular por un socio particular para un mes particular
function get_pago_por_id_socio($id_socio, $mes=null, $anio=null, $pagado=null, $refinanciado=NULL, $anulado=NULL)
{
	$filtro_pagado = '';

	if(!is_null($mes))
		$filtro_mes = ' AND cci.mes = '.$mes;
	else
		$filtro_mes = '';
	if(!is_null($anio))
		$filtro_anio = ' AND cci.anio = '.$anio;
	else
		$filtro_anio = '';
	if(!is_null($pagado))
		$filtro_pagado = $filtro_pagado.' AND cci.pagado = '.$pagado;
	if(!is_null($anulado))
		$filtro_pagado = $filtro_pagado.' AND cci.anulado = '.$anulado;
	if(!is_null($refinanciado))
		$filtro_pagado =$filtro_pagado.' AND cci.refinanciado = '.$refinanciado;

	$pago = db_query('SELECT soc.nro_socio, cci.id_cta_cte_item, date_format(cci.fecha_emision, \'%d/%m/%Y\') as fecha_emision, cci.mes, cci.anio,con.id_concepto, con.nombre as concepto, cci.valor, IF(cci.pagado = 1, \'SI\', \'NO\') AS pagado, date_format(cci.fecha_pago, \'%d/%m/%Y\') as fecha_pago, IF(cci.anulado = 1, \'SI\', \'NO\') AS anulado
					FROM io_cta_cte_item cci
					INNER JOIN io_cta_cte cc on cci.cta_cte_id = cc.id_cta_cte
					INNER JOIN io_socio soc on cc.socio_id = soc.id_socio
					INNER JOIN io_concepto con on con.id_concepto = cci.concepto_id
					WHERE soc.id_socio = :id_socio'.$filtro_mes.$filtro_anio.$filtro_pagado, array(':id_socio' => $id_socio))->fetchAll();

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

//Replica los datos de pago en todos los socios de un grupo familiar
function replicar_pago_grupo_familiar($id_socio, $forma_pago, $cbu, $vencimiento_1, $vencimiento_2, $personal, $lugar_cobro)
{
	$uid = $GLOBALS['user']->uid;
	//buscamos el grupo familiar del socio por el id_socio recibido como parametro
	$socios_grupo_familiar = get_socios_grupo_familiar($id_socio);

	//recorremos cada socio obtenido y le pegamos los datos de pago del titular
	foreach($socios_grupo_familiar as $socio)
	{
		$id_socio_fam = $socio->id_socio;
		//actualizamos los datos de cada socio
		db_update('io_socio')
			->fields(array(
				'forma_pago_id'=>(isset($socio_titular_pago) ? $socio_titular_pago->forma_pago_id : $forma_pago),
				'cbu'=>(isset($socio_titular_pago) ? $socio_titular_pago->cbu : $cbu),
				'vencimiento_dia'=>(isset($socio_titular_pago) ? $socio_titular_pago->vencimiento_dia : $vencimiento_1),
				'vencimiento_mes'=>(isset($socio_titular_pago) ? $socio_titular_pago->vencimiento_mes : $vencimiento_2),
				'lugar_cobro'=>$lugar_cobro,
				'personal_id'=>$personal,
				'usuario_mod'=>$uid,
			))
			->condition('id_socio',$id_socio_fam,'=')
			->execute();
	}
}

//funcion que retorna el tipo de estado pasado como parametro
//si el tipo_baja es 1 es un estado de tipo BAJA
function get_tipo_estado($id_estado)
{
	$estado = db_query('SELECT *
						FROM io_estado_socio est
						WHERE est.id_estado_socio = :id_estado'
						,array(':id_estado' => $id_estado))->fetchObject();

	if(isset($estado->tipo_baja))
		return $estado->tipo_baja;
	else
		return null;
}

function replicar_baja_a_grupo_familiar($id_socio, $id_estado_socio)
{
	$uid = $GLOBALS['user']->uid;
	//buscar los socios del grupo familiar
	$socios = get_socios_grupo_familiar($id_socio);
	//recorremos los socios del grupo familiar
	foreach ($socios as $socio) {
		db_update('io_socio')
		->fields(array(
		'estado_socio_id'=>$id_estado_socio,
		'usuario_mod'=>$uid,
		))
		->condition('id_socio',$socio->id_socio,'=')
		->execute();
	}
}

function consultar_deuda($id_socio, $tipo_socio_id)
{
	//si el socio es titular de familia buscaremos la deuda para el tipo de grupo familiar del socio
	if($tipo_socio_id == 3)
	{
		//buscamos el tipo de grupo familiar primero
		//$grupo_familiar = get_grupo_familiar($id_socio);

		$deuda = db_query('select cci.*, con.nombre, tgf.importe as valor_actual
							from io_socio soc
							inner join io_cta_cte cc on cc.socio_id = soc.id_socio
							inner join io_cta_cte_item cci on cci.cta_cte_id = cc.id_cta_cte
							inner join io_concepto con on cci.concepto_id = con.id_concepto
							inner join io_grupo_familiar_socio gfs on gfs.socio_id = soc.id_socio
							inner join io_grupo_familiar gf on gf.id_grupo_familiar = gfs.grupo_familiar_id
							inner join io_tipo_grupo_familiar tgf on tgf.id_tipo_grupo_familiar = gf.tipo_grupo_familiar_id
							where cci.pagado = 0
							and cci.anulado = 0
							and cci.refinanciado = 0
							and soc.id_socio =:id_socio',array(':id_socio' => $id_socio))->fetchAll();

	}
	else if($tipo_socio_id == 1)
	{//si el socio es socio unico buscamos el valor de la deuda por su categoria del socio
		$deuda = db_query('select cci.*, con.nombre, cate.importe_cuota as valor_actual
							from io_socio soc
							inner join io_cta_cte cc on cc.socio_id = soc.id_socio
							inner join io_cta_cte_item cci on cci.cta_cte_id = cc.id_cta_cte
							inner join io_concepto con on cci.concepto_id = con.id_concepto
							inner join io_categoria_socio cate on soc.categoria_socio_id = cate.id_categoria_socio
							where cci.pagado = 0
							and cci.anulado = 0
							and cci.refinanciado = 0
							and soc.id_socio =:id_socio',array(':id_socio' => $id_socio))->fetchAll();
	}
	else
	{
		$deuda = null;
	}

	if(isset($deuda) && !empty($deuda)){
		//drupal_set_message('<pre>'.print_r($deuda, true).'</pre>');
		return $deuda;

	}
	else {
		return null;
	}
}

function importe_cuota_gf($tipo_grupo_familiar)
{
	$tipo_grupo_familiar_socio = db_query('SELECT *
						FROM io_tipo_grupo_familiar tgf
						WHERE tgf.id_tipo_grupo_familiar = :tipo_grupo_familiar'
						,array(':tipo_grupo_familiar' => $tipo_grupo_familiar))->fetchObject();

	if(isset($tipo_grupo_familiar_socio->importe))
		return $tipo_grupo_familiar_socio->importe;
	else
		return 0;
}

function importe_cuota_su($categoria)
{
	$categoria_socio = db_query('SELECT *
						FROM io_categoria_socio cat
						WHERE cat.id_categoria_socio = :categoria'
						,array(':categoria' => $categoria))->fetchObject();
		//drupal_set_message('<pre>'.print_r($categoria_socio, true).'</pre>');
	if(isset($categoria_socio->importe_cuota))
		return $categoria_socio->importe_cuota;
	else
		return 0;
}

function get_cta_cte_por_id_socio($id_socio)
{
	$cta_cte_socio = db_query('SELECT *
						FROM io_cta_cte cc
						WHERE cc.socio_id = :id_socio'
						,array(':id_socio' => $id_socio))->fetchObject();
		//drupal_set_message('<pre>'.print_r($categoria_socio, true).'</pre>');
	if(isset($cta_cte_socio->id_cta_cte))
		return $cta_cte_socio;
	else
		return null;
}

//genera una cuota para la cuenta corriente del socio
function generar_cuota($id_socio, $concepto_id, $mes, $anio)
{
	$uid = $GLOBALS['user']->uid;
	//buscamos el socio
	$socio = get_socio_por_id($id_socio);

	//buscamos la cuenta corriente del socio
	$cta_cte_socio = get_cta_cte_por_id_socio($id_socio);

	//el importe se obtiene dependiendo del tipo de socio
	if($socio->tipo_socio_id == 1)
	{//si es socio unico buscamos el importe segun su categoria
		$importe = importe_cuota_su($socio->categoria_socio_id);
	}
	else if($socio->tipo_socio_id == 3)
	{//si es titular de familia se debe buscar el tipo de grupo familiar
		$grupo_familiar = get_grupo_familiar($socio->id_socio);
		$tipo_grupo_familiar = $grupo_familiar->tipo_grupo_familiar_id;
		$importe = importe_cuota_gf($tipo_grupo_familiar);
	}
	else
	{//cualquier otro caso se retorna falso ya que no se debera generar nada
		return false;
	}

	$cobrador_id = null;
	//buscamos la forma de pago del socio
	if($socio->forma_pago_id == 1 || $socio->forma_pago_id == 3)
	{
		$cobrador_id = $socio->personal_id;
	}

	$fecha_emision = date('Y').'-'.date('m').'-'.date('d');

	$id_item_cta_cte = db_insert('io_cta_cte_item')
			->fields(array(
				'fecha_emision'=>$fecha_emision,
				'cta_cte_id'=>$cta_cte_socio->id_cta_cte,
				'mes'=>$mes,
				'anio'=>$anio,
				'concepto_id'=>$concepto_id,
				'valor'=>$importe,
				'cobrador_id'=>$cobrador_id,
				'usuario_mod'=>$uid,
				'usuario_alta'=>$uid,
			))
			->execute();

	return true;
}

//permite obtener los datos de un socio para generar un plan de pago
function get_socios_deudor($nro_socio, $apellidos, $nombres, $tipo_doc, $nro_doc)
{
	//Armamos el filtro dependiendo si viene el nro de socio o el apellido y el nombres
	$filtro = '';
	if(!is_null($nro_socio))
	{
		$filtro = 'AND soc.nro_socio = '.$nro_socio;
	}
	else if(!is_null($apellidos))
	{
		//pasar a mayusculas el valor de $apellidos para la busqueda
		$apellidos = strtoupper($apellidos);
		$filtro = 'AND per.apellidos LIKE \'%'.$apellidos.'%\'';
	}
	else if(!is_null($nombres))
	{
		//pasar a mayusculas el valor de $nombres para la busqueda
		$nombres = strtoupper($nombres);
		$filtro = 'AND per.nombres LIKE \'%'.$nombres.'%\'';
	}
	else if(!is_null($tipo_doc))
	{
		$filtro = 'AND per.tipo_doc LIKE \'%'.$tipo_doc.'%\'';
	}
	else if(!is_null($nro_doc))
	{
		$filtro = 'AND per.nro_doc LIKE \'%'.$nro_doc.'%\'';
	}

	$socios_deuda = db_query('
	SELECT soc.id_socio,
	soc.nro_socio,
	soc.secuencia,
	soc.tipo_socio_id,
	per.apellidos,
	per.nombres,
	per.tipo_doc,
	per.nro_doc,
	ts.descripcion as tipo_socio,
	cs.nombre as categoria,
	tgf.nombre as tipo_grupo_familiar,
	es.nombre as estado,
	es.tipo_baja as estado_tipo_baja,
	count(cci.id_cta_cte_item) as cant_cuotas_adeudadas
	FROM io_socio soc
	INNER JOIN io_persona per on per.id_persona = soc.persona_id
	INNER JOIN io_cta_cte cc on cc.socio_id = soc.id_socio
	INNER JOIN io_cta_cte_item cci on cci.cta_cte_id = cc.id_cta_cte
	INNER JOIN io_tipo_socio ts on ts.id_tipo_socio = soc.tipo_socio_id
	INNER JOIN io_estado_socio es on es.id_estado_socio =  soc.estado_socio_id
	LEFT JOIN io_categoria_socio cs on cs.id_categoria_socio = soc.categoria_socio_id
	LEFT JOIN io_grupo_familiar_socio gfs on gfs.socio_id = soc.id_socio
	LEFT JOIN io_grupo_familiar gf on gf.id_grupo_familiar = gfs.grupo_familiar_id
	LEFT JOIN io_tipo_grupo_familiar tgf on tgf.id_tipo_grupo_familiar = gf.tipo_grupo_familiar_id
	WHERE (soc.tipo_socio_id = 1 or soc.tipo_socio_id = 3)
	AND cci.pagado = 0 AND cci.anulado = 0 AND cci.refinanciado = 0 '.
	$filtro.'
	 GROUP BY soc.id_socio,
	soc.nro_socio,
	per.apellidos,
	per.nombres,
	ts.descripcion,
	cs.nombre,
	tgf.nombre,
	es.nombre,
	es.tipo_baja
	ORDER BY count(cci.id_cta_cte_item) desc');

	return $socios_deuda;
}

//recuepera los datos del socio y los datos del item pasado por parametro
function get_datos_socio_pago_por_item_cc($id_item_cta_cte)
{
	$datos_socio_pago = db_query('
	SELECT icc.id_cta_cte_item,
	date_format(icc.fecha_emision, \'%d-%m-%Y\') as fecha_emision,
	icc.mes,
	icc.anio,
	icc.concepto_id,
	icc.valor,
	icc.pagado,
	conc.nombre as concepto,
	soc.id_socio,
	soc.nro_socio,
	soc.tipo_socio_id,
	per.apellidos,
	per.nombres,
	per.direccion,
	IF(soc.tipo_socio_id = 1, cs.nombre, tgf.nombre) as cate_tgf,
	ts.descripcion as tipo_socio
	FROM io_cta_cte_item icc
	INNER JOIN io_cta_cte cc on cc.id_cta_cte = icc.cta_cte_id
	INNER JOIN io_concepto conc on conc.id_concepto = icc.concepto_id
	INNER JOIN io_socio soc on soc.id_socio = cc.socio_id
	INNER JOIN io_persona per on per.id_persona = soc.persona_id
	INNER JOIN io_tipo_socio ts on ts.id_tipo_socio = soc.tipo_socio_id
	LEFT JOIN io_categoria_socio cs on cs.id_categoria_socio = soc.categoria_socio_id
	LEFT JOIN io_grupo_familiar_socio gfs on gfs.socio_id = soc.id_socio
	LEFT JOIN io_grupo_familiar gf on gf.id_grupo_familiar = gfs.grupo_familiar_id
	LEFT JOIN io_tipo_grupo_familiar tgf on tgf.id_tipo_grupo_familiar = gf.tipo_grupo_familiar_id
	WHERE icc.id_cta_cte_item = :id_cta_cte_item',array(':id_cta_cte_item' => $id_item_cta_cte))->fetchObject();

	return $datos_socio_pago;
}

//recuepera los datos del socio y los datos del item pasado por parametro
function get_items_cta_cte_por_recibo($id_recibo)
{
	$datos_socio_pago = db_query('
	SELECT rs.id_recibo,
	icc.id_cta_cte_item,
	date_format(icc.fecha_emision, \'%d-%m-%Y\') as fecha_emision,
	icc.mes,
	icc.anio,
	icc.concepto_id,
	icc.valor,
	icc.pagado,
	conc.nombre as concepto,
	soc.id_socio,
	soc.nro_socio,
	soc.tipo_socio_id,
	per.apellidos,
	per.nombres,
	per.direccion,
	IF(soc.tipo_socio_id = 1, cs.nombre, tgf.nombre) as cate_tgf,
	ts.descripcion as tipo_socio
	FROM io_cta_cte_item icc
	INNER JOIN io_cta_cte cc on cc.id_cta_cte = icc.cta_cte_id
	INNER JOIN io_concepto conc on conc.id_concepto = icc.concepto_id
	INNER JOIN io_socio soc on soc.id_socio = cc.socio_id
	INNER JOIN io_persona per on per.id_persona = soc.persona_id
	INNER JOIN io_tipo_socio ts on ts.id_tipo_socio = soc.tipo_socio_id
	INNER JOIN io_recibo_item rs_i on rs_i.cta_cte_item_id = icc.id_cta_cte_item
	INNER JOIN io_recibo rs on rs.id_recibo = rs_i.recibo_id
	LEFT JOIN io_categoria_socio cs on cs.id_categoria_socio = soc.categoria_socio_id
	LEFT JOIN io_grupo_familiar_socio gfs on gfs.socio_id = soc.id_socio
	LEFT JOIN io_grupo_familiar gf on gf.id_grupo_familiar = gfs.grupo_familiar_id
	LEFT JOIN io_tipo_grupo_familiar tgf on tgf.id_tipo_grupo_familiar = gf.tipo_grupo_familiar_id
	WHERE rs.id_recibo = :id_recibo',array(':id_recibo' => $id_recibo))->fetchAll();
	//drupal_set_message('<pre>'.print_r($datos_socio_pago, true).'</pre>');
	return $datos_socio_pago;
}

//genera la liquidacion correspondiente a un mes y a un año
function generar_liquidacion_mensual($mes, $anio)
{
	//primero debemos validar que no exista una liquidacion generada para este mismo periodo
	if(existe_liquidacion($mes, $anio))
	{
		//si existe retornaremos un valor 1 que indicara que ya existe
		$resultado_liquidacion = 1;
	}
	else {
		//llamamos al procedimiento almacenado que genera la liquidacion de todo el mes
		$resultado_liquidacion = db_query('CALL PRC_GENERAR_MOVIMIENTOS(:anio,:mes)',array(':anio' => $anio, ':mes' => $mes))->fetchObject();
		//drupal_set_message('<pre>'.print_r($resultado_liquidacion, true).'</pre>');
	}

	return $resultado_liquidacion;
}

function existe_liquidacion($mes, $anio)
{
	//revisamos en la tabla movimiento si existe la liquidacion genera para ese mes y año
	$movimiento = db_query('SELECT count(*) AS cant_movimientos FROM io_movimiento WHERE anio = :anio AND mes = :mes',array(':anio' => $anio, ':mes' => $mes))->fetchObject();

	//si la cantidad de movimientos retornada por la consulta es mayor a 0 significa que existe ya un movimiento para ese mes y año
	if($movimiento->cant_movimientos > 0)
	{
		return true;
	}
	else {
		return false;
	}
}

function get_id_socio_por_doc($tipo_doc, $nro_doc)
{
	$socio = db_query('SELECT soc.id_socio
 										 FROM io_socio soc
	 									 INNER JOIN io_persona per ON per.id_persona = soc.persona_id
										 WHERE per.tipo_doc = :tipo_doc AND per.nro_doc = :nro_doc', array(':tipo_doc'=>$tipo_doc, ':nro_doc'=>$nro_doc))->fetchObject();

	if(isset($socio->id_socio))
	{
		return $socio->id_socio;
	}
	else {
		return null;
	}
}


function get_id_socio_por_nro_socio($nro_socio)
{
	$socio = db_query('SELECT soc.id_socio
 										 FROM io_socio soc
										 WHERE soc.tipo_doc = :tipo_doc AND per.nro_doc = :nro_doc', array(':tipo_doc'=>$tipo_doc, ':nro_doc'=>$nro_doc))->fetchObject();

	if(isset($socio->id_socio))
	{
		return $socio->id_socio;
	}
	else {
		return null;
	}
}
?>
