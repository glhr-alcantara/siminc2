<script>
    $(document).ready(function(){
        $('#executar').on('click',function(){
            if($('#periodo').val() == '' || $('#periodo').val() == null || $('#periodo').val() == undefined){
                bootbox.alert('Voc� deve selecionar um per�odo.');
                $('#periodo').parent().parent().addClass('has-error');
                return false;
            }
            bootbox.confirm('Voc� realmente deseja apagar os dados do snapshot para o per�do '+ $('#periodo option[value='+$('#periodo').val()+']').text() +' e inserir novamente?',function(re){
                if(re){
                    $('#formAcompanhamentoAcao').submit();
                }
            });
        });
        $('#voltar').on('click',function(){
            window.location = $(this).attr('data-url');
        });
    });
</script>
<?php
$form = new Simec_View_Form('formAcompanhamentoAcao', Simec_View_Form::POST);
$form->setRequisicao('cargaAcao');
$valor = <<<HTML
    <div> Baixe com a data de <b>HOJE</b> os dados abaixo, para o per�odo desejado.</div>
HTML;
$form->addInputTextarea('previa', $valor, 'previa', $limite, array('somentetexto' => true));
$query = <<<DML
    SELECT prfid AS codigo, prftitulo AS descricao FROM acomporc.periodoreferencia WHERE prftipo = 'A' AND prsano = '{$_SESSION['exercicio']}'
DML;
$form->addInputCombo('prfid', $query, $valor, 'periodo', array('flabel' => 'Per�odo'));
$form->addBotao('avancado',array('id' => 'voltar', 'label' => 'Voltar', 'class' => 'btn-warning', 'extra' => 'data-url="spo.php?modulo=inicio&acao=C"', 'span' => 'glyphicon glyphicon-arrow-left'));
$form->addBotao('novo',array('id' => 'executar', 'label' => 'Executar'));
$form->render();