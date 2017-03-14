<?php
/**
* Permite Gestionar los estados de un socio
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que gestionar socios
*/
function estado_socio_form($form, &$form_state)
{
	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	$form['datos_socio'] = array(
		'#title' => t('Cambiar Estado del socio'),
  		'#type' => 'fieldset', 
	);

	if(isset($_GET['id']))
	{
		$id_socio_seleccionado = $_GET['id'];
		$form['id_socio_seleccionado'] = array(
			'#type'=>'hidden',
			'#value'=>$id_socio_seleccionado,
		);
	}

	//los posibles estados que puede seleccionar son
	//ACTIVO Y ACTIVO CON LICENCIA
	$opciones_estados = array(1=>'ACTIVO', 2=>'ACTIVO CON LICENCIA');
	$form['datos_socio']['estado_seleccionado'] = array(
		'#title' => t('Estado'),
  		'#type' => 'select', 
  		'#options' => $opciones_estados,
  		'#description'=>t('Seleccione el Nuevo Estado para el socio')
	);

	$form['datos_socio']['guardar'] = array(
		'#value' => t('GUARDAR'),
  		'#type' => 'submit', 
	);
	
	return $form;
}

function estado_socio_form_submit($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form_state['values']['listado_socios'], true).'</pre>');
	$id_socio_seleccionado = $form_state['values']['id_socio_seleccionado'];
	$id_estado_seleccionado = $form_state['values']['estado_seleccionado'];
	$transaction = db_transaction();
 	try {
		//creamos el grupo y obtenemos el id
		db_update('io_socio')
		->fields(array('estado_socio_id'=>$id_estado_seleccionado,	      		      
		))
		->condition('id_socio', $id_socio_seleccionado, '=')
		->execute();	
		//commit
		unset($transaction);
		drupal_set_message("Se modifico correctamente el estado del socio");
	}
	catch (Exception $e) {
		$transaction->rollback();
		watchdog_exception('estado_socio', $e);
		drupal_set_message('<pre>'. print_r("Error en la transaccion".$e, TRUE) .'</pre>');
	}
}
?>