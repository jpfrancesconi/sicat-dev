<?php
/**
* Permite Gestionar las categorias de los socios
*
* @author Giuici Francisco
* @version 1.0
* @package sicat
* @subpackage socios
*/


/**
* Formulario que gestionar socios
*/
function gestionar_categorias_form($form, &$form_state)
{

	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	//buscamos las categorias de los socios
	$categorias = get_categorias_socio();

	$header_categorias = array(    
		'nombre'=>t('NOMBRE'),
		'importe_cuota'=>t('IMPORTE CUOTA'),
	    'incluye_cuota_social'=>t('INCLUYE CUOTA SOCIAL'),
	    'edad_desde'=>t('EDAD DESDE'), 
	    'edad_hasta'=>t('EDAD HASTA.'), 
	    'observaciones'=>t('OBSERVACIONES'),
	    'activo'=>t('ACTIVA'),
    );
	

	if(empty($categorias))
			$categorias = array();
	//se genera un array fila_categoria que tendra como elementos a cada fila generada por categoria
	$fila_categoria =array();
	$filas_categoria =array();

	$i = 0;
	//recorremos todos los socios recuperados y generamos el array de opciones
	//drupal_set_message('<pre>'.print_r(array_values($categorias), true).'</pre>');
	if(!empty($categorias))
		for($i=0; $i < count($categorias);$i++) 
		{
			$categoria = $categorias[$i];
			//drupal_set_message('<pre>'.print_r($socio->id_socio, true).'</pre>');
			$id_categoria_socio = $categoria->id_categoria_socio;
			$fila_categoria =  
			  array(
			    'nombre'=> $categoria->nombre,
			    'importe_cuota'=> $categoria->importe_cuota,			   
			    'incluye_cuota_social'=>($categoria->incluye_cuota_social == 0 ? '<span style="color:red"><strong>NO</strong></span>' : '<span style="color:green"><strong>SI</strong></span>'),
			    'edad_desde'=> $categoria->edad_desde,
			    'edad_hasta'=> $categoria->edad_hasta,
			    'observaciones'=> $categoria->observaciones,
			    'activo'=>($categoria->activo == 0 ? '<span style="color:red"><strong>NO</strong></span>' : '<span style="color:green"><strong>SI</strong></span>'),
			    'id_categoria_socio'=> array(
			    	'data'=> array( 
			      		'#type' => 'hidden',               
		      			'#name' => '',               
			      		'#value'=> $id_categoria_socio,
			      	), 
		    	),     
			  ); 
			$filas_categorias[$id_categoria_socio] = $fila_categoria;
		}      
	
	$form['categorias']['gestion_categorias'] = array( 
		'#type'=>'fieldset',
		'#title'=>t('CATEGORIAS DE SOCIOS'),
	);
	
	$form['categorias']['gestion_categorias']['nueva_categoria'] = array( 
		'#type'=>'submit',
		'#name'=>'nueva_categoria',
		'#value'=>'NUEVA CATEGORÍA',
		'#submit'=>array('alta_categoria_form_submit'),
	);

	$form['categorias']['gestion_categorias']['listado_categorias'] = array(
		'#type' => 'tableselect',
		'#header' => $header_categorias,
		'#options' => $filas_categorias,
		'#multiple'=>FALSE,
		'#empty' => 'No existen categorias.',
		'#prefix'=>'<div id="listado-categorias">',
		'#suffix'=>'</div>',
		'#description'=>t('Seleccione categoría.')
	);

	$form['categorias']['gestion_categorias']['ver_categoria'] = array( 
		'#type'=>'submit',
		'#name'=>'ver_categoria',
		'#value'=>'VER',
		'#validate'=>array('ver_categoria_form_valdiate'),
		'#submit'=>array('ver_categoria_form_submit'),
	);

	$form['categorias']['gestion_categorias']['editar_categoria'] = array( 
		'#type'=>'submit',
		'#name'=>'editar_categoria',
		'#value'=>'EDITAR',
		'#validate'=>array('editar_categoria_form_valdiate'),
		'#submit'=>array('editar_categoria_form_submit'),
	);
	
	return $form;
}


function ver_categoria_form_valdiate($form, &$form_state)
{
	
	if(empty($form_state['values']['listado_categorias']))
	{
		form_set_error('listado_categorias', 'Debe seleccionar una categoría de socio para realizar la accion sobre la misma');
	}


}

function ver_categoria_form_submit($form, &$form_state)
{
	//drupal_set_message('<pre>'.print_r($form_state['values'],true).'</pre>');
	$id_categoria_socio = $form_state['values']['listado_categorias'];
	$form_state['redirect'] = array('socios/categoria',array('query' => array('id'=>$id_categoria_socio, 'edit'=>0)));


}

function editar_categoria_form_valdiate($form, &$form_state)
{
	if(empty($form_state['values']['listado_categorias']))
	{
		form_set_error('listado_categorias', 'Debe seleccionar una categoría de socio para realizar la accion sobre la misma');
	}
	


}

function editar_categoria_form_submit($form, &$form_state)
{
	
	//drupal_set_message('<pre>'.print_r($form_state['values'],true).'</pre>');
	$id_categoria_socio = $form_state['values']['listado_categorias'];
	$form_state['redirect'] = array('socios/categoria',array('query' => array('id'=>$id_categoria_socio, 'edit'=>1)));


}

function alta_categoria_form_submit($form, &$form_state)
{
	$form_state['redirect'] = array('socios/categoria');
}
?>