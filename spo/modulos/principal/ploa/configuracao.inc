<?php
/**
 * Arquivo de controle da configura��o da PLOA.
 *
 * Permite ao usu�rio selecionar uma configura��o anterior da PLOA e a copiar para o
 * per�odo atual.
 *
 * $Id: configuracao.inc 98916 2015-06-22 12:37:10Z maykelbraz $
 *
 * @filesource
 */

$fm = new Simec_Helper_FlashMessage('spo/ploa/config');

if (isset($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    $srvPrf = new Proporc_Service_Periodoreferencia();
    $srvPrf->setDados($_REQUEST['configuracao']? $_REQUEST['configuracao']: array());
    try {
        switch ($requisicao) {
            case 'visualizarConfigPeriodo':
                $srvPrf->visualizarConfig();
                die();
            default:
                die();
//                ver($requisicao, d);
        }
    } catch(Exception $e) {
        $fm->addMensagem('N�o foi poss�vel executar sua requisi��o.', Simec_Helper_FlashMessage::ERRO);
    }

    header("Location: {$_SERVER['REQUEST_URI']}");
    die();
}

// -- fluxo normal da app
$prfref = new Proporc_Model_Periodoreferencia();
$prfref->carregarAtual();

/**
 * Cabe�alho padr�o do sistema.
 * @uses cabecalho.inc
 */
require_once(APPRAIZ . 'includes/cabecalho.inc');
?>
<style type="text/css">
.label-td{text-align:right;font-weight:bold}
</style>
<div class="col-md-12">
    <ol class="breadcrumb">
        <li>
            <a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisdsc']; ?></a>
        </li>
        <li class="active">Configura��o da PLOA</li>
    </ol>
<?
$panel = <<<HTML
<table class="table table-bordered">
    <tbody>
        <tr>
            <td class="label-td" style="width:25%">Exerc�cio:</td>
            <td colspan="2">{$_SESSION['exercicio']}</td>
        </tr>
        <tr>
            <td class="label-td">Per�odo:</td>
            <td>{$prfref}</td>
            <td style="width:5%">
                <button class="btn btn-success btn-visualizar-principal"><span class="glyphicon glyphicon-eye-open"></span></button>
            </td>
        </tr>
    </tbody>
</table>
HTML;
bootstrapPanel('Informa��es do per�odo', $panel);
echo $fm;

$sql = $prfref->queryTodosPeriodosCombo();
$config = array(
    'flabel' => 'Per�odo de origem',
    'titulo' => 'Selecione um per�odo de refer�ncia de origem'
);
$form = new Simec_View_Form('configuracao');
$form->addInputCombo('prfidorigem', $sql, null, 'prfidorigem', $config)
    ->addHidden('prfiddestino', $prfref->prfid, 'prfiddestino')
    ->addBotoes(array('visualizar', 'copiar'))->render();

// -- popup de adi��o de novos itens
bootstrapPopup(
    'Configura��o do per�odo',
    'mdl-config',
    '',
    array('fechar'),
    array('tamanho' => 'lg')
);
?>
</div>
<script type="text/javascript">
function visualizarConfigPeriodo(prfid){
    var data = {
        'configuracao[prfid]': prfid,
        requisicao: 'visualizarConfigPeriodo'
    };

    $.get(window.location.href, data, function(html){
        $('#mdl-config .modal-body').empty().html(html);
        $('#mdl-config').modal();
    }, 'html');
}

$(function(){
    $('.btn-visualizar').click(function(){
        var prfid = $('#prfidorigem').val();
        if (!prfid) {
            bootbox.alert('Selecione um per�odo de refer�ncia para visualiza��o.');
            return false;
        }
        visualizarConfigPeriodo(prfid);
    });

    $('.btn-visualizar-principal').click(function(){
        var prfid = $('#prfiddestino').val();
        if (!prfid) {
            bootbox.alert('N�o h� nenhum per�odo de refer�ncia aberto.');
            return false;
        }
        visualizarConfigPeriodo(prfid);
    });
});
</script>