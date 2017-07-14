(function ($) {
  Drupal.behaviors.socios = {
    attach: function (context, settings) {
		$(document).ready(function(event){
            $('body').keydown(function(e) {
                    if (e.keyCode === 13) {
                            e.preventDefault();
                            $("#ingreso-socio-datos-socio-form").submit();
                        }
                });
            });
        }
    }
}(jQuery));
