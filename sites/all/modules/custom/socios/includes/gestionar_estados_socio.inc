<?php
/**
* Permite Gestionar los estados del socio
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que gestionar los estados del socio
*/
function gestionar_estados_socio_form($form, &$form_state)
{

	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	//buscamos las categorias de los socios
	$estados = get_estados_socio();

	$header_estados = array(    
		'nombre'=>t('NOMBRE'),
		'observacion'=>t('OBSERVACION'),
		'cuota'=>t('PORCENTAJE CUOTA'),
	    'cant_cuota_adeuda'=>t('CANT. CUOTAS ADEUDA'),
	    'imprime_cupon'=>t('IMPRIME CUPÓN'), 	    
	    'activo'=>t('ACTIVO'), 	    
    );
	

	if(empty($estados))
			$estados = array();
	//se genera un array fila_categoria que tendra como elementos a cada fila generada por categoria
	$fila_estado =array();
	$filas_estado =array();

	$i = 0;
	//recorremos todos los socios recuperados y generamos el array de opciones
	//drupal_set_message('<pre>'.print_r(array_values($categorias), true).'</pre>');
	if(!empty($estados))
		for($i=0; $i < count($estados);$i++) 
		{
			$estado = $estados[$i];
			//drupal_set_message('<pre>'.print_r($socio->id_socio, true).'</pre>');
			$id_estado_socio = $estado->id_estado_socio;
			$fila_estado =  
			  array(
			    'nombre'=> $estado->nombre,
			    'observacion'=> $estado->observacion,			   
			    'cuota'=>$estado->cuota.'%',
			    'cant_cuota_adeuda'=>$estado->cant_cuota_adeuda,
			    'imprime_cupon'=> ($estado->imprime_cupon == 0 ? '<span style="color:red"><strong>NO</strong></span>' : '<span style="color:green"><strong>SI</strong></span>'),			    
			    'activo'=>($estado->activo == 0 ? '<span style="color:red"><strong>NO</strong></span>' : '<span style="color:green"><strong>SI</strong></span>'),
			    'id_estado_socio'=> array(
			    	'data'=> array( 
			      		'#type' => 'hidden',               
		      			'#name' => '',               
			      		'#value'=> $id_estado_socio,
			      	), 
		    	),     
			  ); 
			$filas_estado[$id_estado_socio] = $fila_estado;
		}      
	
	$form['estado']['gestionar_estados'] = array( 
		'#type'=>'fieldset',
		'#title'=>t('ESTADOS DE SOCIOS'),
	);
	
	$form['estado']['gestionar_estados']['nuevo_estado'] = array( 
		'#type'=>'submit',
		'#name'=>'nuevo_estado',
		'#value'=>'NUEVO ESTADO',
		'#submit'=>array('alta_estado_form_submit'),
	);

	$form['estado']['gestionar_estados']['listado_estados'] = array(
		'#type' => 'tableselect',
		'#header' => $header_estados,
		'#options' => $filas_estado,
		'#multiple'=>FALSE,
		'#empty' => 'No existen estados.',
		'#prefix'=>'<div id="listado-estados">',
		'#suffix'=>'</div>',
		'#description'=>t('Seleccione estado.')
	);

	$form['estado']['gestionar_estados']['ver_estado'] = array( 
		'#type'=>'submit',
		'#name'=>'ver_estado',
		'#value'=>'VER',
		'#validate'=>array('ver_estado_form_valdiate'),
		'#submit'=>array('ver_estado_form_submit'),
	);

	$form['estado']['gestionar_estados']['editar_estado'] = array( 
		'#type'=>'submit',
		'#name'=>'editar_estado',
		'#value'=>'EDITAR',
		'#validate'=>array('editar_estado_form_valdiate'),
		'#submit'=>array('editar_estado_form_submit'),
	);
	
	return $form;
}


function ver_estado_form_valdiate($form, &$form_state)
{
	
	if(empty($form_state['values']['listado_estados']))
	{
		form_set_error('listado_categorias', 'Debe seleccionar una categoría de socio para realizar la accion sobre la misma');
	}


}

function ver_estado_form_submit($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form_state['values'],true).'</pre>');
	$id_categoria_socio = $form_state['values']['listado_estados'];
	$form_state['redirect'] = array('socios/estados_socio',array('query' => array('id'=>$id_categoria_socio, 'edit'=>0)));


}

function editar_estado_form_valdiate($form, &$form_state)
{
	if(empty($form_state['values']['listado_estados']))
	{
		form_set_error('listado_estados', 'Debe seleccionar una categoría de socio para realizar la accion sobre la misma');
	}
	


}

function editar_estado_form_submit($form, &$form_state)
{
	
	//drupal_set_message('<pre>'.print_r($form_state['values'],true).'</pre>');
	$id_categoria_socio = $form_state['values']['listado_estados'];
	$form_state['redirect'] = array('socios/estados_socio',array('query' => array('id'=>$id_categoria_socio, 'edit'=>1)));


}

function alta_estado_form_submit($form, &$form_state)
{
	$form_state['redirect'] = array('socios/estados_socio');
}
?>