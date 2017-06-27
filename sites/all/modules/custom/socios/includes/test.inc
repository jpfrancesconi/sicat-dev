<?php
//este formulario se utiliza solo para pruebas.
//La idea es tener un fumalario donde tirar mensajes y ejecutar llamadas a la base y ver que retorna y demas

function test_form($form, &$form_state)
{
  get_next_num_secuencia(24);
  return $form;
}

function test_form_validate($form, &$form_state)
{

}

function test_form_submit($form, &$form_state)
{

}
?>
