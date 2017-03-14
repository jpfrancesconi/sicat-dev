(function ($) {
  Drupal.behaviors.talleresModule = {
    attach: function (context, settings) {
      // Code to be run on page load, and
      // on ajax load added here
       /*$('#dt').dataTable( {
            "language": {
                "url": "https://cdn.datatables.net/plug-ins/1.10.12/i18n/Spanish.json"
            }
        } );*/
    	if ( $.fn.dataTable.isDataTable('#dt') ) {
    		
		} else {
    		table = $('#dt').DataTable({
	            "language": {
	                "url": "https://cdn.datatables.net/plug-ins/1.10.12/i18n/Spanish.json"
	            }
        	});
		}
    }
  };
}(jQuery));