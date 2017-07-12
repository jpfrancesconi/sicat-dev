(function ($) {
  Drupal.behaviors.socios = {
    attach: function (context, settings) {
      //las siguientes variables se utilizan en el formulario de alta y edicion del socio para mostrar  u ocultar campos de forma dinamica
      //guardamos en variables los campos que vamos a ir mostrando y ocultado
      var lugar_de_cobro = $(".form-item-lugar-cobro");
      var personal = $(".form-item-personal");
      var cbu = $(".form-item-cbu");
      var vencimiento_1 = $(".form-item-vencimiento-1");
      var vencimiento_2 = $(".form-item-vencimiento-2");
      var tarjeta_debito = $(".form-item-tarjeta-debito");
      var tarjeta_credito = $(".form-item-tarjeta-credito");

		$(document).one('ready', function(event){
           //obtenemos el valor
            var tipo_socio = $("#edit-tipo-socio").val();
            var forma_pago = $("#edit-forma-pago").val();

            set_campos_forma_pago_visibles(forma_pago, tipo_socio, lugar_de_cobro, personal, cbu, vencimiento_1, vencimiento_2, tarjeta_credito, tarjeta_debito);
            /*$('#edit-forma-pago').prop('disabled', true);
            setTimeout(function() {
               $('#edit-forma-pago').prop('disabled', false);
            }, 6000);*/
		});
    $("#edit-forma-pago").change(function(event){//evento que se dispara cuando se cambia el valor en el select forma de pago
      //obtenemos el valor
       var tipo_socio = $("#edit-tipo-socio").val();
       var forma_pago = $("#edit-forma-pago").val();

       set_campos_forma_pago_visibles(forma_pago, tipo_socio, lugar_de_cobro, personal, cbu, vencimiento_1, vencimiento_2, tarjeta_credito, tarjeta_debito);
    });
    $("#edit-tipo-socio").change(function(event){//evento que se dispara cuando se cambia el valor en el selct de tipo de socio
      //obtenemos el valor
       var tipo_socio = $("#edit-tipo-socio").val();
       var forma_pago = $("#edit-forma-pago").val();

       set_campos_forma_pago_visibles(forma_pago, tipo_socio, lugar_de_cobro, personal, cbu, vencimiento_1, vencimiento_2, tarjeta_credito, tarjeta_debito);
    });
    }
  };
}(jQuery));

function set_campos_forma_pago_visibles(forma_pago, tipo_socio, lugar_de_cobro, personal, cbu, vencimiento_1, vencimiento_2, tarjeta_debito, tarjeta_credito)
{
  if(tipo_socio != 2)
  {
    if(forma_pago == 1)
    {
      lugar_de_cobro.css("display","block");
      personal.css("display","block");
      cbu.css("display","none");
      vencimiento_1.css("display","none");
      vencimiento_2.css("display","none");
      tarjeta_debito.css("display","none");
      tarjeta_credito.css("display","none");
    }
    else if(forma_pago == 2)
    {
      lugar_de_cobro.css("display","none");
      personal.css("display","none");
      cbu.css("display","block");
      vencimiento_1.css("display","block");
      vencimiento_2.css("display","block");
      tarjeta_debito.css("display","none");
      tarjeta_credito.css("display","none");
    }
    else if(forma_pago == 3)
    {
      lugar_de_cobro.css("display","none");
      personal.css("display","none");
      cbu.css("display","none");
      vencimiento_1.css("display","none");
      vencimiento_2.css("display","none");
      tarjeta_debito.css("display","none");
      tarjeta_credito.css("display","none");
    }
    else if(forma_pago == 4)
    {
      lugar_de_cobro.css("display","none");
      personal.css("display","none");
      cbu.css("display","none");
      vencimiento_1.css("display","none");
      vencimiento_2.css("display","none");
      tarjeta_debito.css("display","none");
      tarjeta_credito.css("display","block");
    }
    else if(forma_pago == 5)
    {
      lugar_de_cobro.css("display","none");
      personal.css("display","none");
      cbu.css("display","none");
      vencimiento_1.css("display","none");
      vencimiento_2.css("display","none");
      tarjeta_debito.css("display","block");
      tarjeta_credito.css("display","none");
    }
  }
  else {
    lugar_de_cobro.css("display","none");
    personal.css("display","none");
    cbu.css("display","none");
    vencimiento_1.css("display","none");
    vencimiento_2.css("display","none");
    tarjeta_debito.css("display","none");
    tarjeta_credito.css("display","none");
  }
}
