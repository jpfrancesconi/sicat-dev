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

             if(res == 0)
             {
               $("#texto-msj-resultado").css({"background":"rgba(205, 10, 10, 0.11)", "border":"1px solid #cd0a0a"});
               mensaje='SE GENERÓ LA LIQUIDACION PARA EL PERIODO MES: '+paramMes+' AÑO:'+paramAnio+' CORRECTAMENTE!.';
             }
             else if(res == 1)
             {
               $("#texto-msj-resultado").css({"background":"rgba(205, 10, 10, 0.11)", "border":"1px solid #cd0a0a"});
               mensaje='LA LIQUIDACION PARA EL PERIODO MES: '+paramMes+' AÑO:'+paramAnio+' YA EXISTE.';
             }
             else if(res == 2)
             {
               $("#texto-msj-resultado").css({"background":"rgba(205, 10, 10, 0.11)", "border":"1px solid #cd0a0a"});
               mensaje='LA LIQUIDACION PARA EL PERIODO MES: '+paramMes+' AÑO:'+paramAnio+' NO SE HA GENERADO YA QUE NO EXISTE LIQUIDACIÓN GENERADA PRA EL PERIODO ANTERIOR.';
             }
             else
             {
               mensaje='SE GENERÓ LA LIQUIDACION PARA EL PERIODO MES: '+paramMes+' AÑO:'+paramAnio+' CORRECTAMENTE!.';
             }
               //mensaje='OCURRIÓ UN ERROR LA LIQUIDACION PARA EL PERIODO MES: '+paramMes+' AÑO:'+paramAnio+' NO SE HA GENERADO.';
             $("#msj-resultado").css("display", "block");
             $("#texto-msj-resultado span").text(mensaje);
           }
         });
       $("#wait").css("display", "block");
      });

    }
  };

}(jQuery));
