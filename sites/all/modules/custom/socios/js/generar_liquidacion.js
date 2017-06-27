(function ($) {

  Drupal.behaviors.socios = {
    attach: function (context, settings) {

      // CSS Selector for the button which will trigger the AJAX call
      $('.btn-generar').click(function () {
        //recuperamos los valores de mes y anio
        var paramMes = document.getElementById("paramMes").value;
        var paramAnio = document.getElementById("paramAnio").value;
          $.ajax({
           url: Drupal.settings.custom.generar_liquidacion, // This is the AjAX URL set by the custom Module
           method: "GET",
           data: { mes : paramMes, anio: paramAnio }, // Set the number of Li items requested
           dataType: "html",          // Type of the content we're expecting in the response
           success: function(res) {
             $("#wait").css("display", "none");
             var mensaje = '';
             alert(res);
             if(res == 0)
               mensaje='SE GENERÓ LA LIQUIDACION PARA EL PERIOD\nMES: '+paramMes+'AÑO:'+paramAnio+'\nCORRECTAMENTE!.';
             else if(res == 1)
               mensaje='LA LIQUIDACION PARA EL PERIODO\nMES: '+paramMes+'\nAÑO:'+paramAnio+'\nYA EXISTE.';
             else
               mensaje='OCURRIÓ UN ERROR LA LIQUIDACION PARA EL PERIODO\nMES: '+paramMes+'\nAÑO:'+paramAnio+'\nNO SE HA GENERADO.';
             $("#msj-resultado").css("display", "block");
             $("#texto-msj-resultado span").text(mensaje);
           }
         });
       $("#wait").css("display", "block");
      });

    }
  };

}(jQuery));
