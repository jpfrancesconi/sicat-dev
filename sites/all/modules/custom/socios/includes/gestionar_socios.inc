<?php
/**
* Permite Gestionar los socios del club
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que gestionar socios
*/
function gestionar_socios_form($form, &$form_state)
{
	$formulario = 'gestionar_socios';

	$form['formulario'] = array(
		'#type'=>'hidden',
		'#value'=>$formulario,
	);

	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);
  	//recuperamos los parametros
	if(isset($_SESSION[$formulario]['params']) && !empty($_SESSION[$formulario]['params']))
	{

		$filtro = false;
		$collapsed = true;
		$params = $_SESSION[$formulario]['params'];
		//drupal_set_message('<pre>'.print_r($params, true).'</pre>');
		//buscamos los socios
		$socios = get_socios($params['apellidos'], $params['nombres'], $params['nro_socio'], $params['estado'], $params['tipo_socio'], $params['categoria_socio'], $params['tipo_doc'], $params['nro_doc']);

	}
	else
	{
		$collapsed = false;
		$socios = array();
	}


	$form['gestion_socios']['filtrar'] = array(
		'#title' => t('FILTRAR SOCIOS'),
		'#type' => 'fieldset',
		'#collapsible'=>true,
		'#collapsed'=>$collapsed,
	);

	$form['gestion_socios']['filtrar']['filtro_nro_socio'] = array(
		'#title' => t('NUMERO DE SOCIO'),
	    '#type' => 'textfield',
	    '#default_value'=>(!empty($params['nro_socio']) ? $params['nro_socio'] : ''),
	    '#disabled'=>false,
	);

	$form['gestion_socios']['filtrar']['filtro_tipo_doc'] = array(
		'#type'=>'select',
		'#title'=>'TIPO DE DOCUMENTO',
		'#options' => array('DNI'=>'DNI','LC'=>'LC','LE'=>'LE','DNI EXT.'=>'DNI EXT.'),
    '#default_value'=>(isset($tipo_doc) ? $tipo_doc : null)
	);

	$form['gestion_socios']['filtrar']['filtro_nro_doc'] = array(
		'#type'=>'textfield',
		'#title'=>'NUMERO DE DOCUMENTO',
    '#default_value'=>(isset($nro_doc) ? $nro_doc : null)
	);

	$form['gestion_socios']['filtrar']['filtro_nombres'] = array(
		'#title' => t('NOMBRE'),
	    '#type' => 'textfield',
	    '#default_value'=>(!empty($params['nombres']) ? $params['nombres'] : ''),
	    '#disabled'=>false,
	);

	$form['gestion_socios']['filtrar']['filtro_apellidos'] = array(
		'#title' => t('APELLIDO'),
	    '#type' => 'textfield',
	    '#default_value'=>(!empty($params['apellidos']) ? $params['apellidos'] : ''),
	    '#disabled'=>false,
	);

	$estados_socio = get_estados_socio_select('--TODOS--');
	$form['gestion_socios']['filtrar']['filtro_estado'] = array(
		'#title' => t('ESTADO'),
	    '#type' => 'select',
	    '#options'=>$estados_socio,
	    '#disabled'=>false,
	    '#default_value'=>(!empty($params['estado']) ? $params['estado'] : NULL),
	);

	$categorias_socio = get_categorias_socio_select('--TODOS--');
	$form['gestion_socios']['filtrar']['filtro_categoria_socio'] = array(
		'#title' => t('CATEGORIA'),
	    '#type' => 'select',
	    '#options'=>$categorias_socio,
	    '#disabled'=>false,
	    '#default_value'=>(!empty($params['categoria_socio']) ? $params['categoria_socio'] : NULL),
	);

	$tipos_socio = get_tipos_socio_select('--TODOS--');
	$form['gestion_socios']['filtrar']['filtro_tipo_socio'] = array(
		'#title' => t('TIPO DE SOCIO'),
	    '#type' => 'select',
	    '#options'=>$tipos_socio,
	    '#disabled'=>false,
	    '#default_value'=>(!empty($params['tipo_socio']) ? $params['tipo_socio'] : NULL),
	);

	$form['gestion_socios']['filtrar']['buscar'] = array(
		'#value' => t('BUSCAR'),
    '#type' => 'submit',
	);

	$form['gestion_socios']['filtrar']['nuevo_socio'] = array(
		'#value' => t('NEVO SOCIO'),
    '#type' => 'submit',
    '#limit_validation_errors'=> array(),
		'#submit'=> array('nuevo_socio_submit'),
	);

	$header_socios = array(
		'nro_socio'=>t('NRO. SOCIO'),
		'apellidos'=>t('APELLIDO'),
    'nombres'=>t('NOMBRE'),
    'tipo_doc'=>t('TIPO DOC.'),
    'nro_doc'=>t('NUMERO DOC.'),
    'fecha_ingreso'=>t('FECHA DE INGRESO'),
    'estado'=>t('ESTADO'),
    'categoria_socio'=>t('CAT. O TIPO GRUPO FAMILIAR'),
    'tipo_socio'=>t('TIPO DE SOCIO'),
		'cobrador'=>t('COBRADOR'),
  );


	if(empty($socios))
			$socios = array();
	//se genera un array filas_socios que tendra como elementos a cada fila generada por socio
	$filas_socios =array();
	$socios_gf = null;
	$i = 0;
	//recorremos todos los socios recuperados y generamos el array de opciones

	if(!empty($socios))
		while ($socio = $socios->fetchObject())
		{
			$id_socio = $socio->id_socio;
			//ver si el socio es miembro de familia y buscar el numero de socio del titular
			/*if($socio->id_tipo_socio == 2)
			{
				$grupo_familiar = get_grupo_familiar($socio->id_socio);
				$socio->nro_socio = $grupo_familiar->nro_socio_titular;
				if(isset($grupo_familiar->nombre_tipo_grupo_familiar))
					$socio->categoria_socio = $grupo_familiar->nombre_tipo_grupo_familiar;
			}
			else if($socio->id_tipo_socio == 3)
			{
				$grupo_familiar = get_grupo_familiar($socio->id_socio);
				if(isset($grupo_familiar->nombre_tipo_grupo_familiar))
					$socio->categoria_socio = $grupo_familiar->nombre_tipo_grupo_familiar;
				//si es titular de familia y se busco por nro de socio me debe traer
				if((isset($params['nro_socio']) && !is_null($params['nro_socio'])))
				{

				}
				//drupal_set_message('<pre>'.print_r($grupo_familiar,true).'</pre>');
				//$socios_gf = get_socios_gf($socio->id_grupo_familiar);
				//drupal_set_message('<pre>'.print_r($socios_gf->fetchAll(),true).'</pre>');
			}*/

			$fila_socio =
			array(
				'nro_socio'=> $socio->nro_socio.'/'.$socio->secuencia,
				'apellidos'=> $socio->apellidos,
				'nombres'=> $socio->nombres,
				'tipo_doc'=> $socio->tipo_doc,
				'nro_doc'=> $socio->nro_doc,
				'fecha_ingreso'=> $socio->fecha_ingreso,
				'estado'=> $socio->estado,
				'categoria_socio'=> $socio->categoria_socio,
				'tipo_socio'=> $socio->tipo_socio,
				'cobrador'=> $socio->apellido_personal,
				//l('Ver', 'microcreditos/ver_observacion_carpeta', array('query' => array('id' => $carpeta['id_carpeta_personal']))),
				'id_socio'=> array(
					'data'=> array(
				  		'#type' => 'hidden',
							'#name' => '',
				  		'#value'=> $id_socio,
				  	),
				),
			);

			$filas_socios[$id_socio] = $fila_socio;
			$i++;

			if(!is_null($socios_gf))
			{//recorremos los miembros de familia y los agregamos

			}
		}

	$form['socios']['gestion_socios'] = array(
		'#type'=>'fieldset',
		'#title'=>t('SOCIOS'),
		'#collapsible'=> TRUE,
	);

	$form['socios']['gestion_socios']['listado_socios'] = array(
		'#type' => 'tableselect',
		'#header' => $header_socios,
		'#options' => $filas_socios,
		'#multiple'=>FALSE,
		'#empty' => 'No existen socios.',
		'#prefix'=>'<div id="listado-socios">',
		'#suffix'=>'</div>',
		'#description'=>t('Seleccione socio.')
	);
//drupal_set_message('<pre>'.print_r($form['socios']['gestion_socios']['listado_socios'], true).'</pre>');
	/*$form['socios']['gestion_socios']['paginador'] = array(
		'#markup'=>theme('pager'),
	);*/

	$form['socios']['gestion_socios']['ver_socio'] = array(
		'#type'=>'submit',
		'#name'=>'ver_socio',
		'#value'=>'VER SOCIO',
		'#validate'=>array('ver_socio_form_valdiate'),
		'#submit'=>array('ver_socio_form_submit'),
	);

	$form['salir'] = array(
		'#type'=>'submit',
		'#value'=> t('SALIR'),
	);

	return $form;
}

function nuevo_socio_submit($form, &$form_state)
{
	$form_state['redirect'] = array('socios/socio');
}

function gestionar_socios_form_validate($form, &$form_state)
{
	if(empty($form_state['values']['filtro_apellidos'])
	&& empty($form_state['values']['filtro_nombres'])
	&& empty($form_state['values']['filtro_nro_socio'])
	&& empty($form_state['values']['filtro_estado'])
	&& empty($form_state['values']['filtro_tipo_socio'])
	&& empty($form_state['values']['filtro_categoria_socio'])
	&& empty($form_state['values']['filtro_nro_doc']))
		form_set_error('filtrar', 'Debe ingresar algún filtro de busqueda');
}

function gestionar_socios_form_submit($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form, true).'</pre>');

	$uid = $GLOBALS['user']->uid;
	$formulario = $form_state['values']['formulario'];
    try {
    	//creamos un arrray dentro del SESSION para que sea recuperado dentro de params
    	$_SESSION[$formulario]['params']['apellidos'] = (!empty($form_state['values']['filtro_apellidos']) ? strtoupper($form_state['values']['filtro_apellidos']) : NULL);
    	$_SESSION[$formulario]['params']['nombres'] = (!empty($form_state['values']['filtro_nombres']) ? strtoupper($form_state['values']['filtro_nombres']) : NULL);
    	$_SESSION[$formulario]['params']['nro_socio'] = (!empty($form_state['values']['filtro_nro_socio']) ? $form_state['values']['filtro_nro_socio'] : NULL);
			$_SESSION[$formulario]['params']['tipo_doc'] = (!empty($form_state['values']['filtro_tipo_doc']) ? $form_state['values']['filtro_tipo_doc'] : NULL);
			$_SESSION[$formulario]['params']['nro_doc'] = (!empty($form_state['values']['filtro_nro_doc']) ? $form_state['values']['filtro_nro_doc'] : NULL);
    	$_SESSION[$formulario]['params']['estado'] = (($form_state['values']['filtro_estado'] != 0) ? $form_state['values']['filtro_estado'] : NULL);
    	$_SESSION[$formulario]['params']['tipo_socio'] = (($form_state['values']['filtro_tipo_socio'] != 0) ? $form_state['values']['filtro_tipo_socio'] : NULL);
    	$_SESSION[$formulario]['params']['categoria_socio'] = (($form_state['values']['filtro_categoria_socio'] != 0) ? $form_state['values']['filtro_categoria_socio'] : NULL);

   	$form_state['redirect'] = array('socios/gestionar_socios');
  	}
 	catch (Exception $e) {
  		watchdog_exception('my_type', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}
}

function ver_socio_form_valdiate($form, &$form_state)
{
	if(empty($form_state['values']['listado_socios']))
	{
		form_set_error('listado_socios', 'Debe seleccionar un socio para realizar la accion sobre el mismo');
	}
}

function ver_socio_form_submit($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form_state['values']['listado_socios'], true).'</pre>');

	$uid = $GLOBALS['user']->uid;
	$formulario = $form_state['values']['formulario'];
    try {
    	//creamos un arrray dentro del SESSION para que sea recuperado dentro de params
    	$id_socio_seleccionado = (!empty($form_state['values']['listado_socios']) ? $form_state['values']['listado_socios'] : NULL);

    	/*$_SESSION[$formulario]['params']['apellidos'] = (!empty($form_state['values']['filtro_apellidos']) ? strtoupper($form_state['values']['filtro_apellidos']) : NULL);
    	$_SESSION[$formulario]['params']['nombres'] = (!empty($form_state['values']['filtro_nombres']) ? strtoupper($form_state['values']['filtro_nombres']) : NULL);
    	$_SESSION[$formulario]['params']['nro_socio'] = (!empty($form_state['values']['filtro_nro_socio']) ? $form_state['values']['filtro_nro_socio'] : NULL);
    	$_SESSION[$formulario]['params']['estado'] = (($form_state['values']['filtro_estado'] != 0) ? $form_state['values']['filtro_estado'] : NULL);*/
    	unset($_SESSION[$formulario]['params']);
    	$form_state['redirect'] = array('socios/datos_socio',array('query' => array('id'=>$id_socio_seleccionado)));


  	}
 	catch (Exception $e) {
  		watchdog_exception('my_type', $e);
  		drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
	}
}


function ajax_radio_tabla_panel_socios_callback($form, $form_state)
{
	return $form['socios']['socio'];
}

function ajax_prueba_callback($form, $form_state)
{
	return $form['gestion_socios']['filtrar']['nombre_p'];
}


?>
