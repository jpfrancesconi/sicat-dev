/*(function ($) {
  Drupal.behaviors.socios = {
    attach: function (context, settings) {
		$(document).ready(function(event){
			$.ajax({
            type: 'POST',
            url: 'ajax.php',
            data: 'id=testdata',
            dataType: 'json',
            cache: false,
            success: function(result) {
                $('#content1').html(result[0]);
            },
        	});
		});
    }
  };
}(jQuery));*/


