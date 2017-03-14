<?php 
//Modulo BASES: Este modulo concentra todas las funcionalidades relacionadas a la gestion y administraión de los datos correspondientes a los diferentes programas que se ejecutan desde la SES

//include 'includes/registrar_fecha_cheque.inc';

function socios_init(){ 
  //drupal_add_js(drupal_get_path('module', 'generico') . '/js/generico.js');

} 

function socios_permission(){   

 
  return array('access page manager' =>  array(
      'title' => t('Access Page Manager'),
    'description' => t('Permission for accessing the page manager module.'),
    ),
  );
}

/**
* Implements hook_help()
*/

function socios_help($path, $arg){
  switch ($path) {
    case 'gest_fin/manage':
      // Help text for the simple page registered for this path.
      return t('This section displays the list of Pages.');
  }
}

/**
* Implements hook_menu()
*/

function socios_menu()
{
  /*------ ITEMS DE MENU PARA GESTION FINANCIERA -------*/
  /*$items['socios/gestionar_socios'] = array(
    'title' => 'SOCIOS',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('alta_asignacion_form'),
    'access callback' => 'user_is_logged_in',    
    'expanded' => TRUE,
  );*/
    
  return $items;
}
?>
