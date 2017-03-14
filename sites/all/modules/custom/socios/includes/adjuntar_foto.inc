<?php
/**
* Permite adjuntar foto del socio
*
* @author Giudici Francisco/Juan Pablo Francesconi
* @version 1.0
* @package sicat
* @subpackage socios
*/

/**
* Formulario que genera cupones de pago de cuota
*/
function adjuntar_foto_form($form, &$form_state)
{
	$form['#attached']['css'] = array(
		drupal_get_path('module', 'socios') . '/css/socios.css',
	);

	$disabled = false;

	if(strpos($_SERVER['HTTP_REFERER'], 'datos_socio'))
	{
		$array_retorno = parse_url($_SERVER['HTTP_REFERER']);
		$_SESSION['retorno'] = 'socios/datos_socio';    
		$_SESSION['retorno_params'] =array('query' =>get_params_url($_SERVER['HTTP_REFERER']));
	}

	if((isset($_GET['id_p']) && !is_null($_GET['id_p']) && !empty($_GET['id_p'])))
	{
		$id_persona = $_GET['id_p'];
		$form['id_persona'] = array(
		      '#type' => 'hidden', 
		      '#value' => $_GET['id_p'],
		);

		//buscamos si la persona tiene ya una foto cargada
		$uri_foto = get_uri_foto_persona($id_persona);

		if(!is_null($uri_foto))
		$form['uri_foto'] = array(
		  '#type' => 'hidden', 
		  '#value' => $uri_foto,
		);
	}

	//FOTO DEL SOCIO
    $form['foto'] = array(
      '#type'=>'fieldset',
      '#title'=>t('FOTO'),
    );

    $form['foto']['foto_subir'] = array(
      '#type'=>'file',
      '#title'=>t('SELECCIONE UNA IMAGEN'),
      '#description'=>t('Tamaño Máximo de Archivo Permitido: <strong>2MB</strong> </br> Extenciones de archivos permitidas: <strong>PDF, JPEG, JPG, PNG</strong>'),
    );

    $form['foto']['foto_submit'] = array(
      '#type'=>'submit',
      '#value'=>t('SUBIR IMAGEN AL SERVIDOR'),
      '#limit_validation_errors'=>array(),
      '#submit'=>array(),
      '#ajax'=>array(
        'callback'=>'subir_foto_submit',  
        'wrapper'=>'foto-socio',
        'method'=>'replace',
        'effect'=>'fade',
      ),
    );  

    $form['foto']['elimiar_foto'] = array(
      '#type'=>'submit',
      '#value'=>t('ELIMINAR FOTO'),
      '#attributes' => array('onclick' => 'if(!confirm("Esta seguro que desea eliminar la foto?")){return false;}'),
    ); 

    if(is_null($uri_foto))
    {
      $form['foto']['imagen'] = array(
        '#prefix'=>'<div id="foto-socio">',
        '#suffix'=>'</div>',
      );
    }    
    else
    {
      $form['foto']['imagen']= array(
        '#prefix'=>'<div id="foto-socio">',
        '#markup'=>'<div id="mensaje-foto-socio-cargada">
                      <span>FOTO: </span>                      
                      <div class="foto-socio-inter" style="width:45%; max-width:600px;">
                        <a href='.$uri_foto.'><img src='.$uri_foto.'></img></a>  
                      </div>                   
                    </div>',
        '#suffix'=>'</div>',
      );
    }

    $form['foto']['volver'] = array( 
			'#type'=>'submit',
			'#name'=>'volver_foto',
			'#value'=>'VOLVER',
			'#limit_validation_errors'=> array(),
			'#submit'=> array('adjuntar_foto_form_salir'),
	);
	
	return $form;
}

function adjuntar_foto_form_salir($form, &$form_state)
{
  $retorno = array($_SESSION['retorno']);    
  unset($_SESSION['retorno']);
  if(isset($_SESSION['retorno_params']))
  {
    array_push($retorno, $_SESSION['retorno_params']);
    unset($_SESSION['retorno_params']);
  }
  //drupal_set_message('<pre>'.print_r($retorno, true).'</pre>');
  $form_state['redirect'] = $retorno;
}

function subir_foto_submit($form, &$form_state)
{   
  $uid = $GLOBALS['user']->uid;
  //obtenemos el id de la persona pasado como parametro
  $id_persona = $form_state['input']['id_persona'];

  try
  {
  //buscamos los datos de la persona por su id
  $persona = get_persona_por_id($id_persona);
 // error_log(print_r($form_state['values'], true));
  //drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');
  
  //definimos el nombre del directorio a donde lo guardaremos
  $destino = "public://fotos/".$id_persona;

  //si no existe lo creamos sino no
  if(!file_exists($destino))
  {
    mkdir($destino, 0777, true);
  }

  $file = file_save_upload('foto_subir', array('file_validate_extensions'=>array('jpg jpeg png pdf')), $destino, $replace=FILE_EXISTS_REPLACE);
  $fileinfo = pathinfo($file->filename);
 //drupal_set_message('<pre>'.print_r($fileinfo, true).'</pre>');
  $nombre_archivo = $id_persona.'_'.$persona->nro_doc.'_documento.'.$fileinfo['extension'];
    
  if($file)
  {
    $file->status=FILE_STATUS_PERMANENT;
    $file = file_move($file, $destino."/".$nombre_archivo, FILE_EXISTS_REPLACE);
    $res = file_save($file);
    
    //definimos el destino que usamos para poder mostrar luego el archivo subido
    $destino_mostrar = "../sites/default/files/fotos/".$id_persona."/".$nombre_archivo;

    $form['foto']['imagen']= array(
        '#prefix'=>'<div id="foto-socio">',
        '#markup'=>'<div id="mensaje-foto-socio-cargada">
                      <span>ARCHIVO SUBIDO CORRECTAMENTE</span>
                    	<a href='.$destino_mostrar.' target="_blank">VER ARCHIVO</a>                     
                    </div>',
        '#suffix'=>'</div>',
      );
    //<img src='.$destino_mostrar.'></img> <a href='.$destino_mostrar.'><img src='.$destino_mostrar.'></img></a> 
  }

  $_SESSION['uri_foto']=$destino_mostrar;

  //actualizamos en la tabla persona el uri de la imagen
  $transaction = db_transaction();
  try 
  { 

    db_update('io_persona')
      ->fields(array( 'uri_foto'=>$destino_mostrar, 
                      'usuario_mod'=>$uid
      ))
      ->condition('id_persona', $id_persona, '=')
      ->execute();

    unset($transaction); 
  }  
  catch (Exception $e) {
      $transaction->rollback();
      watchdog_exception('adjuntar_foto', $e);
      drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas a traves de un Mensaje a Soporte.".$e, TRUE) .'</pre>');
  }
  }  
  catch (Exception $e) {    
      watchdog_exception('adjuntar_foto', $e);
      drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas a traves de un Mensaje a Soporte.".$e, TRUE) .'</pre>');
  }

  //drupal_set_message('<pre>'.print_r($form_state['input'], true).'</pre>');

  return $form['foto']['imagen'];
  
}

function adjuntar_foto_form_valdiate($form, &$form_state)
{
	/*if(!is_numeric($form_state['values']['importe_cuota'])){
      form_set_error('importe_cuota', 'El campo IMPORTE CUOTA es incorrecto - Solo puede contener números');
    }*/
}

function adjuntar_foto_form_submit($form, &$form_state)
{
	$uid = $GLOBALS['user']->uid;
    //obtenemos el id de la persona pasado como parametro
    //drupal_set_message('<pre>'.print_r($form_state['values'], true).'</pre>');
    $id_persona = $form_state['input']['id_persona'];
    $redirect['id_p'] =$id_persona;

    if(!empty($form_state['input']['uri_foto']))
      $uri_foto =$form_state['input']['uri_foto'];
    else
      $uri_foto = $_SESSION['uri_foto'];
    $uri_foto = substr($uri_foto, 2);
    
    
    //definimos el nombre del directorio a donde lo guardaremos
    $archivo_eliminar = "/var/www/html/bases".$uri_foto;
    //si no existe lo creamos sino no
    file_unmanaged_delete($archivo_eliminar);
      

    //actualizamos en la tabla persona el uri de la imagen
    $transaction = db_transaction();
    try 
    { 

      db_update('io_persona')
        ->fields(array( 'uri_foto'=>null, 
                        'usuario_mod'=>$uid
        ))
        ->condition('id_persona', $id_persona, '=')
        ->execute();

      unset($transaction); 
    }  
    catch (Exception $e) {
        $transaction->rollback();
        watchdog_exception('my_type', $e);
        drupal_set_message('<pre>'. print_r("Error en la trasaccion - Comunicarse con el proveedor de sistemas.".$e, TRUE) .'</pre>');
    }

    //drupal_set_message('<pre>'.print_r($form_state['input'], true).'</pre>');
    unset($_SESSION['uri_foto']);
    $form_state['redirect'] = array('socios/adjuntar_foto',array('query' => $redirect));
}
?>