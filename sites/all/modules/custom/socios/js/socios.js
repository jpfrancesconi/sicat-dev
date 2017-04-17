(function ($) {
  Drupal.behaviors.socios = {
    attach: function (context, settings) {
		$(document).one('ready', function(event){
			//obtenemos el valor
            var tipo_socio = $("#edit-tipo-socio").val();
            var forma_pago = $("#edit-forma-pago").val();
            $('#edit-forma-pago').prop('disabled', true);
            if(tipo_socio != 2 && forma_pago != 1)
            {
                $(".form-item.form-type-textfield.form-item-lugar-cobro").css("display","none");
                $(".form-item.form-type-select.form-item-personal").css("display","none");
            }
            setTimeout(function() {
               $('#edit-forma-pago').prop('disabled', false);
            }, 6000);
		});
    }
  };
}(jQuery));


