<?php
/**
* Permite generar cupones de pago de cuota
*
* @author Giudici Francisco/Juan Pablo Francesconi
* @version 1.0
* @package sicat
* @subpackage socios
*/

include_once DRUPAL_ROOT.'/sites/all/libraries/fpdf/fpdf.php'; 

/**
* Formulario que genera cupones de pago de cuota
*/
function generar_cupones_form($form, &$form_state)
{
	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	$disabled = false;

	$form['parametros_cupones'] = array( 
		'#type'=>'fieldset',
		'#title'=>'PARAMETROS DE GENERACIÓN',
	);

	$form['parametros_cupones']['anio'] = array( 
		'#type'=>'numberfield',
		'#title'=>'AÑO',		
	);

	$form['parametros_cupones']['mes'] = array( 
		'#type'=>'numberfield',
		'#title'=>'MES',		
		'#min'=>1,
		'#max'=>12,
	);	

	$form['parametros_cupones']['generar'] = array( 
		'#type'=>'submit',
		'#name'=>'generar_cupones',
		'#value'=>'GENERAR',
	);
	
	//drupal_add_library('socios', 'sites/default/all/libraries/fpdf/fpdf.php');

	//dpm(drupal_get_library('system'));	
	return $form;
}



function generar_cupones_form_valdiate($form, &$form_state)
{
	/*if(!is_numeric($form_state['values']['importe_cuota'])){
      form_set_error('importe_cuota', 'El campo IMPORTE CUOTA es incorrecto - Solo puede contener números');
    }*/
}

function generar_cupones_form_submit($form, &$form_state){
	$uid = $GLOBALS['user']->uid;	

	libraries_load('fpdf');
	$pdf = new FPDF("P", "mm", "A4");
	$pdf->AddPage();
	$pdf->SetFont('Arial','B',12);
	$pdf->Cell(40,10,'CUPONES DE CUOTA FEBRERO 2016');

	// Obtenemos los parametros para el nombre del reporte
	$anio= $form_state['values']['anio'];
	$mes= $form_state['values']['mes'];
	$time = date("d-m-Y h:i:s");

	$pdf->Output("sites/default/files/cupones/"."$anio"."$mes"."_"."$time".".pdf", "F", true);
	
}
?>