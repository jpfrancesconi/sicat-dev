<?php
/**
* Permite Buscar un socio para Generarle un plan de pago para un socio
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que busca un socio para generar un plan de pago
*/
function generar_plan_pago_buscar_form($form, &$form_state)
{
  //si esta seteado el parametro get ns quiere decir que se esta buscando por numero de socio
  if(!empty($_GET['ns']) && isset($_GET['ns']))
  {
    $nro_socio = $_GET['ns'];
    //buscamos el socio que corresponda con ese numero de socio
    $socios = get_socios_deudor($nro_socio, NULL, NULL);
  }//sino buscamos si los parametros apellidos y/o nombre esta seteados para buscar socios por sus apellidos y/o nombres
  else if((!empty($_GET['ap']) && isset($_GET['ap'])) || (!empty($_GET['nom']) && isset($_GET['nom'])))
  {
    $nombres = (isset($_GET['nom']) ? $_GET['nom'] : NULL);
    $apellidos = (isset($_GET['ap']) ? $_GET['ap'] : NULL);
    //buscamos el socio que corresponda con ese numero de socio
    $socios = get_socios_deudor(NULL, $apellidos,$nombres);
  }
  else {
    $socios = null;
  }

	//Buscamos el socios
  $form['buscar_socio'] = array(
    '#type'=>'fieldset',
    '#title'=>'BUSCAR SOCIO',
  );

  $form['buscar_socio']['nro_socio'] = array(
    '#type'=>'textfield',
    '#title'=>'NRO. DE SOCIO',
    '#default_value'=>(isset($nro_socio) ? $nro_socio : null)
  );

  $form['buscar_socio']['nombres'] = array(
    '#type'=>'textfield',
    '#title'=>'NOMBRES',
    '#default_value'=>(isset($nombres) ? $nombres : null)
  );

  $form['buscar_socio']['apellidos'] = array(
    '#type'=>'textfield',
    '#title'=>'APELLIDOS',
    '#default_value'=>(isset($apellidos) ? $apellidos : null)
  );

  $form['buscar_socio']['buscar'] = array(
    '#type'=>'submit',
    '#value'=>'BUSCAR'
  );

  //mastrar una tabla con el resultado de la busqueda del socios
  $form['socios_buscados'] = array(
    '#type'=>'fieldset',
    '#title'=>'LISTADO DE SOCIOS'
  );

  //definimos el encabezado de la tabla de resultado
  $header_socios = array(
    'nro_socio'=>'NRO. SOCIO',
    'apellidos'=>'APELLIDOS',
    'nombres'=>'NOMBRES',
    'estado'=>'ESTADO',
    'categoria_gf_socio'=>'CAT. O TIPO GRUPO FAMILIAR',
    'tipo_socio'=>'TIPO DE SOCIO',
    'cant_cuotas_deuda'=>'CANT. CUOTAS ADEUDADAS',
  );
  //drupal_set_message('<pre>'.print_r(count($socios->fetchAll()), true).'</pre>');
	//se genera un array filas_socios que tendra como elementos a cada fila generada por socio
	$filas_socios =array();

	$i = 0;
	//recorremos todos los socios recuperados y generamos el array de opciones
  if(!is_null($socios))
  	while ($socio = $socios->fetchObject())
  	{
      $id_socio = $socio->id_socio;
  		//ver si el socio es titular de familia en la categoria se pegara el tipo de grupo familiar
  		if($socio->tipo_socio_id == 3)
  			$socio->categoria = $socio->tipo_grupo_familiar;

      //si el estado es de tipo baja damos valor rojo a la variable $color_estado para poder por css pintar el texto del estado de rojo
      if($socio->estado_tipo_baja == 1)
          $color_estado = 'red';
      else
          $color_estado = 'green';


  		$fila_socio =
  		  array(
  		    'nro_socio'=> $socio->nro_socio.'/'.$socio->secuencia,
  		    'apellidos'=> $socio->apellidos,
  		    'nombres'=> $socio->nombres,
  		    'estado'=> '<span style="color:'.$color_estado.';"><strong>'.$socio->estado.'</strong></span>',
  		    'categoria_gf_socio'=> $socio->categoria,
  		    'tipo_socio'=> $socio->tipo_socio,
  		    'cant_cuotas_deuda'=> $socio->cant_cuotas_adeudadas,
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
    }

  $form['socios_buscados']['listado_socios'] = array(
		'#type' => 'tableselect',
		'#header' => $header_socios,
		'#options' => $filas_socios,
		'#multiple'=>FALSE,
		'#empty' => 'No existen socios con deuda.',
		'#prefix'=>'<div id="listado-socios-pp">',
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
		'#value'=>'GENERAR PLAN DE PAGO',
		'#validate'=>array('generar_plan_pago_valdiate'),
		'#submit'=>array('generar_plan_pago_submit'),
	);

  //$form['socios_buscados']['listado_socios'];

  return $form;
}

function generar_plan_pago_buscar_form_validate($form, &$form_state)
{
  //validamos que el numero de socios contenga solo numeros
  if(!empty($form_state['values']['nro_socio']) && !is_numeric($form_state['values']['nro_socio']))
  {
    form_set_error('nro_socio', 'EL NUMERO DE SOCIO ES INCORRECTO - Solo puede contener numeros.');
  }
  //validamos que el apellido no contenga numeros
  if(!empty($form_state['values']['apellidos']) && preg_match('/[0-9]/', $form_state['values']['apellidos'])){
      form_set_error('apellido', 'El campo APELLIDO es incorrecto - No puede contener numeros.');
  }
  //validamos que el nombre no contenga numeros
  if(!empty($form_state['values']['nombres']) && preg_match('/[0-9]/', $form_state['values']['nombres'])){
    form_set_error('nombres', 'El campo NOMBRE es incorrecto - No puede contener numeros.');
  }
}

function generar_plan_pago_buscar_form_submit($form, &$form_state)
{
  //drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');

  //si se ingreso en el form de busqueda un numero de socio solo buscamos por numero de socio
  if(!empty($form_state['values']['nro_socio']) && isset($form_state['values']['nro_socio']))
  {
    $nro_socio = $form_state['values']['nro_socio'];
    //redireccionamos a la misma pagina pero con el parametro ns correspondiente al numero de socio
    $form_state['redirect']=array('socios/plan_pago', array('query'=>array('ns'=>$nro_socio)));
  }
  else {
    //definimos un array de parametros
    $params = array('query'=>array());
    //si se busca por apellido, se ingreso un apellido
    if(!empty($form_state['values']['apellidos']) && isset($form_state['values']['apellidos']))
      $params['query']['ap'] = $form_state['values']['apellidos'];
    if(!empty($form_state['values']['nombres']) && isset($form_state['values']['nombres']))
      $params['query']['nom'] = $form_state['values']['nombres'];
    //redireccionamos a la misma pagina pero con el parametro ap y nom correspondiente al apellido y nombre
    $form_state['redirect']=array('socios/plan_pago', $params);
  }
}

function generar_plan_pago_valdiate($form, &$form_state)
{
  //validamos que se haya seleccionado un socio de la lista para generarle el plan de pago
  if(empty($form_state['values']['listado_socios']))
	{
		form_set_error('listado_socios', 'Debe seleccionar un socio para realizar la accion sobre el mismo');
	}
}

function generar_plan_pago_submit($form, &$form_state)
{
  try {
    //creamos un arrray dentro del SESSION para que sea recuperado dentro de params
    $id_socio_seleccionado = (!empty($form_state['values']['listado_socios']) ? $form_state['values']['listado_socios'] : NULL);

    //redireccionamos a la proxima pagina gestionar el plan de pago
    $form_state['redirect']=array('socios/plan_pago_socio', array('query'=>array('id_s'=>$id_socio_seleccionado)));
  }
  catch (Exception $e) {
  		watchdog_exception('generar_plan_pago', $e);
  		drupal_set_message("Error en la trasaccion - Comunicarse con el proveedor de sistemas.");
	}


}
?>
