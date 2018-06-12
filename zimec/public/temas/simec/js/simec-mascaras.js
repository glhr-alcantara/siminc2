$(document).ready(function(){

	$('.datemask').mask("99/99/9999");
    $('div.date :disabled').closest('.date').datepicker('remove');

	$(".moeda").inputmask("currency", {
        radixPoint: ",",
        groupSeparator: ".",
        digits: 2,
        autoGroup: true,
        prefix: ''
    });

    $('body').on('keyup', '.valor', function(){
        $(this).val(mascaraglobal('###.###.###.###.###,##', $(this).val()));
    });

    $('body').on('keyup', '.moedaInteiro', function(){
        $(this).val(mascaraglobal('###.###.###.###.###', $(this).val()));
    });

});