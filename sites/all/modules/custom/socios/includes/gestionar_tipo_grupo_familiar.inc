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
function gestionar_tipo_grupo_familiar_form($form, &$form_state)
{

	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	//buscamos las categorias de los socios
	$tipos_grupo_familiar = get_get_tipos_grupo_familiar();

	$header_tipos_grupo_familiar = array(    
		'nombre'=>t('NOMBRE'),
		'cant_int_desde'=>t('CANT. MÍNIMA INTEGRANTES'),
		'cant_int_hasta'=>t('CANT. MÁXIMA INTEGRANTES'),
	    'importe'=>t('IMPORTE'),	    
	    'activo'=>t('ACTIVO'), 	    
    );
	

	if(empty($tipos_grupo_familiar))
			$tipos_grupo_familiar = array();
	//se genera un array fila_categoria que tendra como elementos a cada fila generada por categoria
	$fila_tipos_grupo_familiar =array();
	$filas_tipos_grupo_familiar =array();

	$i = 0;
	//recorremos todos los socios recuperados y generamos el array de opciones
	//drupal_set_message('<pre>'.print_r(array_values($categorias), true).'</pre>');
	if(!empty($tipos_grupo_familiar))
		for($i=0; $i < count($tipos_grupo_familiar);$i++) 
		{
			$tipo_grupo_familiar = $tipos_grupo_familiar[$i];
			//drupal_set_message('<pre>'.print_r($socio->id_socio, true).'</pre>');
			$id_tipo_grupo_familiar = $tipo_grupo_familiar->id_tipo_grupo_familiar;
			$fila_tipos_grupo_familiar =  
			  array(
			    'nombre'=> $tipo_grupo_familiar->nombre,
			    'cant_int_desde'=> $tipo_grupo_familiar->cant_int_desde,			   
			    'cant_int_hasta'=>$tipo_grupo_familiar->cant_int_hasta,			    
			    'importe'=>number_format($tipo_grupo_familiar->importe, 2, ",", "."),
			    'activo'=>($tipo_grupo_familiar->activo == 0 ? '<span style="color:red"><strong>NO</strong></span>' : '<span style="color:green"><strong>SI</strong></span>'),
			    'id_tipo_grupo_familiar'=> array(
			    	'data'=> array( 
			      		'#type' => 'hidden',               
		      			'#name' => '',               
			      		'#value'=> $id_tipo_grupo_familiar,
			      	), 
		    	),     
			  ); 
			$filas_tipos_grupo_familiar[$id_tipo_grupo_familiar] = $fila_tipos_grupo_familiar;
		}      
	
	$form['tipo_grupo_familiar']['gestionar_tipo_grupo_familiar'] = array( 
		'#type'=>'fieldset',
		'#title'=>t('ESTADOS DE SOCIOS'),
	);
	
	$form['tipo_grupo_familiar']['gestionar_tipo_grupo_familiar']['nuevo_tipo_grupo_familiar'] = array( 
		'#type'=>'submit',
		'#name'=>'nuevo_tipo_grupo_familiar',
		'#value'=>'NUEVO TIPO DE GRUPO FAMILIAR',
		'#submit'=>array('alta_tipo_grupo_familiar_form_submit'),
	);

	$form['tipo_grupo_familiar']['gestionar_tipo_grupo_familiar']['listado_tipos_grupo_familiar'] = array(
		'#type' => 'tableselect',
		'#header' => $header_tipos_grupo_familiar,
		'#options' => $filas_tipos_grupo_familiar,
		'#multiple'=>FALSE,
		'#empty' => 'No existen tipos de grupo familiar.',
		'#prefix'=>'<div id="listado-tipos-grupo-familiar">',
		'#suffix'=>'</div>',
		'#description'=>t('Seleccione un tipo de grupo familiar.')
	);

	$form['tipo_grupo_familiar']['gestionar_tipo_grupo_familiar']['ver_tipos_grupo_familiar'] = array( 
		'#type'=>'submit',
		'#name'=>'ver_tipos_grupo_familiar',
		'#value'=>'VER',
		'#validate'=>array('ver_tipos_grupo_familiar_form_valdiate'),
		'#submit'=>array('ver_tipos_grupo_familiar_form_submit'),
	);

	$form['tipo_grupo_familiar']['gestionar_tipo_grupo_familiar']['editar_tipos_grupo_familiar'] = array( 
		'#type'=>'submit',
		'#name'=>'editar_tipos_grupo_familiar',
		'#value'=>'EDITAR',
		'#validate'=>array('editar_tipos_grupo_familiar_form_valdiate'),
		'#submit'=>array('editar_tipos_grupo_familiar_form_submit'),
	);
	
	return $form;
}


function ver_tipos_grupo_familiar_form_valdiate($form, &$form_state)
{
	
	if(empty($form_state['values']['listado_tipos_grupo_familiar']))
	{
		form_set_error('listado_tipos_grupo_familiar', 'Debe seleccionar una tipos de grupo familiar para realizar la accion sobre el mismo');
	}


}

function ver_tipos_grupo_familiar_form_submit($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form_state['values'],true).'</pre>');
	$id_tipo_grupo_familiar = $form_state['values']['listado_tipos_grupo_familiar'];
	$form_state['redirect'] = array('socios/tipo_grupo_familiar',array('query' => array('id'=>$id_tipo_grupo_familiar, 'edit'=>0)));


}

function editar_tipos_grupo_familiar_form_valdiate($form, &$form_state)
{
	if(empty($form_state['values']['listado_tipos_grupo_familiar']))
	{
		form_set_error('listado_tipos_grupo_familiar', 'Debe seleccionar una categoría de socio para realizar la accion sobre la misma');
	}
	


}

function editar_tipos_grupo_familiar_form_submit($form, &$form_state)
{
	
	//drupal_set_message('<pre>'.print_r($form_state['values'],true).'</pre>');
	$id_tipo_grupo_familiar = $form_state['values']['listado_tipos_grupo_familiar'];
	$form_state['redirect'] = array('socios/tipo_grupo_familiar',array('query' => array('id'=>$id_tipo_grupo_familiar, 'edit'=>1)));


}

function alta_tipo_grupo_familiar_form_submit($form, &$form_state)
{
	$form_state['redirect'] = array('socios/tipo_grupo_familiar');
}
?>